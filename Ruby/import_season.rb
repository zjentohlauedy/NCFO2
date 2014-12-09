#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"
require 'ProgRunner'
require 'FileParser'
require 'ScheduleParser'
require 'TeamRecords'
require 'repository'
require 'organization'
require 'conference_stats'
require 'team_stats'
require 'bowls'
require 'utils'

@bowls = {
  'Cotton Bowl'       => Bowls::CottonBowl,
  'Orange Bowl'       => Bowls::OrangeBowl,
  'Rose Bowl'         => Bowls::RoseBowl,
  'Sugar Bowl'        => Bowls::SugarBowl,
  'Fiesta Bowl'       => Bowls::FiestaBowl,
  'Liberty Bowl'      => Bowls::LibertyBowl,
  'NCFO Championship' => Bowls::NCFOChampionship
}

@teams = {
  # Atlantic
  'Delaware'       => { id:  1, conference: 1 },
  'Maryland'       => { id:  2, conference: 1 },
  'New Jersey'     => { id:  3, conference: 1 },
  'Pennsylvania'   => { id:  4, conference: 1 },
  'Virginia'       => { id:  5, conference: 1 },
  'West Virginia'  => { id:  6, conference: 1 },
  # Great Lake
  'Illinois'       => { id:  7, conference: 2 },
  'Michigan'       => { id:  8, conference: 2 },
  'Minnesota'      => { id:  9, conference: 2 },
  'New York'       => { id: 10, conference: 2 },
  'Ohio'           => { id: 11, conference: 2 },
  'Wisconsin'      => { id: 12, conference: 2 },
  # Midwest
  'Arkansas'       => { id: 13, conference: 3 },
  'Indiana'        => { id: 14, conference: 3 },
  'Kansas'         => { id: 15, conference: 3 },
  'Kentucky'       => { id: 16, conference: 3 },
  'Missouri'       => { id: 17, conference: 3 },
  'Tennessee'      => { id: 18, conference: 3 },
  # New England
  'Connecticut'    => { id: 19, conference: 4 },
  'Maine'          => { id: 20, conference: 4 },
  'Massachusetts'  => { id: 21, conference: 4 },
  'New Hampshire'  => { id: 22, conference: 4 },
  'Rhode Island'   => { id: 23, conference: 4 },
  'Vermont'        => { id: 24, conference: 4 },
  # North
  'Iowa'           => { id: 25, conference: 5 },
  'Montana'        => { id: 26, conference: 5 },
  'Nebraska'       => { id: 27, conference: 5 },
  'North Dakota'   => { id: 28, conference: 5 },
  'South Dakota'   => { id: 29, conference: 5 },
  'Wyoming'        => { id: 30, conference: 5 },
  # Pacific
  'California'     => { id: 31, conference: 6 },
  'Idaho'          => { id: 32, conference: 6 },
  'Nevada'         => { id: 33, conference: 6 },
  'Oregon'         => { id: 34, conference: 6 },
  'Utah'           => { id: 35, conference: 6 },
  'Washington'     => { id: 36, conference: 6 },
  # Southeast
  'Alabama'        => { id: 37, conference: 7 },
  'Florida'        => { id: 38, conference: 7 },
  'Georgia'        => { id: 39, conference: 7 },
  'Mississippi'    => { id: 40, conference: 7 },
  'North Carolina' => { id: 41, conference: 7 },
  'South Carolina' => { id: 42, conference: 7 },
  # Southwest
  'Arizona'        => { id: 43, conference: 8 },
  'Colorado'       => { id: 44, conference: 8 },
  'Louisiana'      => { id: 45, conference: 8 },
  'New Mexico'     => { id: 46, conference: 8 },
  'Oklahoma'       => { id: 47, conference: 8 },
  'Texas'          => { id: 48, conference: 8 }
}

def add_game( team_records, game )
  return if !game.played

  # road team
  if team_records.has_key? game.road_team
    team_record = team_records.fetch game.road_team
  else
    team_record = TeamRecords.new
  end

  team_record.update game, false
  team_records.store game.road_team, team_record

  # home team
  if team_records.has_key? game.home_team
    team_record = team_records.fetch game.home_team
  else
    team_record = TeamRecords.new
  end

  team_record.update game, true
  team_records.store game.home_team, team_record
end

def import_bowl_game( location, path, game_name, bowl )
  game = game_name.gsub ' ', '_'

  import = ProgRunner.new "#{location}/../C", "import_bowl_game"

  puts "Importing #{game_name}..."

  import.execute "#{location}/../ncfo.db", "#{path}/playoffs.nes", "#{path}/#{game}.nst", bowl

  if import.success?
    puts "#{game_name} Imported successfully!"
  else
    puts "Error Importing #{game_name}:"
    puts import.get_output
    exit
  end
end

def import_aa_game( location, path, game_name )
  game = game_name.gsub ' ', '_'

  import = ProgRunner.new "#{location}/../C", "import_aa_game"

  puts "Importing #{game_name}..."

  import.execute "#{location}/../ncfo.db", "#{path}/all_americans.nes", "#{path}/#{game}.nst"

  if import.success?
    puts "#{game_name} Imported successfully!"
  else
    puts "Error Importing #{game_name}:"
    puts import.get_output
    raise 'Import Exception'
  end
end

path   = ARGV[0] || '.'

import_season = ProgRunner.new "#{location}/../C", "import_season"

puts "Importing Regular Season..."

import_season.execute "#{location}/../ncfo.db", "#{path}/ncfo1.nes", "#{path}/ncfo1.nst", "#{path}/ncfo2.nes", "#{path}/ncfo2.nst"

if import_season.success?
  puts "Regular season imported successfully!"
else
  puts "Error importing regular season:"
  puts import_season.get_output
  raise 'Import Exception'
end

sp = ScheduleParser.new
fp = FileParser.new sp

fp.process_file "#{path}/schedule.csv"

schedule = sp.schedule

records  = {}
playoffs = {}

schedule.days.each do |day|
  day.games.each do |game|
    if day.day <= 10
      add_game records, game
    end
  end
end

repo = Repository.new Utils::get_db "#{location}/../ncfo.db"

org = Organization.new 1

repo.read org

puts "Updating team stats records..."

repo.start_transaction

begin
  records.each do |team, team_record|
    team_stats = TeamStats.new @teams[team][:id], org.season, Bowls::None
    repo.read team_stats

    team_stats.home_wins   = team_record.home.wins
    team_stats.home_losses = team_record.home.losses
    team_stats.home_ties   = team_record.home.ties

    team_stats.road_wins   = team_record.road.wins
    team_stats.road_losses = team_record.road.losses
    team_stats.road_ties   = team_record.road.ties

    repo.update team_stats
  end

  repo.end_transaction

  puts "Team stats records update successfully!"
rescue Exception => e
  repo.cancel_transaction
  raise e
end

(schedule.days[10].games + schedule.days[11].games + schedule.days[12].games).each do |game|
  import_bowl_game location, path, game.name, @bowls[game.name]

  puts "Creating conference stats records..."

  repo.start_transaction

  begin
    conf_stats = ConferenceStats.new @teams[game.road_team][:conference], org.season, @bowls[game.name]

    conf_stats.wins           = (game.road_score > game.home_score) ? 1 : 0
    conf_stats.losses         = (game.road_score < game.home_score) ? 1 : 0
    conf_stats.home_wins      = 0
    conf_stats.home_losses    = 0
    conf_stats.road_wins      = 0
    conf_stats.road_losses    = 0
    conf_stats.points_scored  = game.road_score
    conf_stats.points_allowed = game.home_score

    repo.create conf_stats

    conf_stats = ConferenceStats.new @teams[game.home_team][:conference], org.season, @bowls[game.name]

    conf_stats.wins           = (game.home_score > game.road_score) ? 1 : 0
    conf_stats.losses         = (game.home_score < game.road_score) ? 1 : 0
    conf_stats.home_wins      = 0
    conf_stats.home_losses    = 0
    conf_stats.road_wins      = 0
    conf_stats.road_losses    = 0
    conf_stats.points_scored  = game.home_score
    conf_stats.points_allowed = game.road_score

    repo.create conf_stats

    repo.end_transaction

    puts "Conference stats records created successfully!"
  rescue Exception => e
    repo.cancel_transaction
    raise e
  end
end

schedule.days[13].games.each do |game|
  import_aa_game location, path, game.name
end

puts "Done."
