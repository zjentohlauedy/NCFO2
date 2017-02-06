#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"
require 'ProgRunner'
require 'boxscore_parser'
require 'game_service'
require 'json'
require 'match'
require 'repository'
require 'utils'

@startdir = Dir::pwd
@boxscore_parser = BoxscoreParser.new
@repository = Repository.new Utils.get_db "#{location}/../ncfo.db"
@game_service = GameService.new @repository

@repository.start_transaction

# check if current directory is a season directory
unless @startdir.split('/')[-1].match %r(^S[0-9][0-9]$)
  puts 'Must run this script from a season directory!'
  exit
end

def parse_roms( location )
  extract_data = ProgRunner.new "#{location}/../C", "extract_data"

  extract_data.execute 'ncfo1.nes', 'ncfo2.nes'

  return JSON.parse extract_data.get_output, {:symbolize_names => true}
end

def find_team( org, teamloc)
  org[:conferences].each do |conference|
    conference[:teams].each do |team|
      if team[:location] == teamloc
        return team
      end
    end
  end

  return nil
end

# get list of week directories W01 - W10
week_dirs = Dir::glob 'W[0-9][0-9]'

week_dirs.select! { |dir| dir.match /W(0[0-9]|10)/ }

# iterate over week directories
week_dirs.sort.each do |dir|
  printf "Processing #{dir}"

  # parse roms (only roms not save states)
  org = parse_roms location

  # get a list of boxscore files in week
  Dir::chdir "#{@startdir}/#{dir}"
  boxscore_files = Dir::glob "#{dir}G[0-9][0-9].txt"

  # iterate over boxscore files
  boxscore_files.sort.each do |file|
    printf '.'

    # parse boxscore file
    boxscore = @boxscore_parser.parse file
    roadteam = nil
    hometeam = nil

    # merge boxscores with parsed rom output
    boxscore.keys.each do |teamloc|
      team = find_team org, teamloc

      if team.nil?
        raise "Could not find a team with location #{teamloc} from boxscore #{file} in #{@startdir}/#{dir}."
      end

      if boxscore[teamloc][:home]
        hometeam = team
      else
        roadteam = team
      end

      team[ :stats         ] = boxscore[teamloc][ :stats         ]
      team[ :offense_stats ] = boxscore[teamloc][ :offense_stats ]
      team[ :defense_stats ] = boxscore[teamloc][ :defense_stats ]
      team[ :kicking_stats ] = boxscore[teamloc][ :kicking_stats ]

      team[:players].each do |player|
        pbox = boxscore[teamloc][:players][ player[:number].to_s ]

        unless pbox.nil?
          pbox[:stats].each_key do |stat|
            player[:stats][stat] = pbox[:stats][stat]
          end
        end
      end
    end

    # save boxscores
    season = @startdir.split('/')[-1].gsub('S', '').to_i
    week   = file.match( /W([0-9]+)G[0-9]+\.txt/ )[1].to_i
    game   = file.match( /W[0-9]+G([0-9]+)\.txt/ )[1].to_i

    match = Match.new season, week, game
    match.road_team_id = roadteam[:team_id]
    match.home_team_id = hometeam[:team_id]

    @repository.create match

    @game_service.save season, week, game, roadteam, hometeam
  end

  puts 'done'

  Dir.chdir @startdir
end

puts 'Processing Complete.'

@repository.end_transaction

exit
