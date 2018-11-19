#!/usr/bin/env ruby
#
# Reads filled in schedule.csv file and details of
# the next unplayed days games.
location = File.dirname __FILE__

$: << "#{location}"
require 'FileParser'
require 'ScheduleParser'
require 'TeamRecords'
require 'team_rankings'


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

def get_player_status( injuries )
  case injuries
  when 1; then return 'Probable'
  when 2; then return 'Questionable'
  when 3; then return 'Doubtful'
  end

  return 'Unknown'
end

def print_team( team, records, rankings )
  record = records[team]

  unless rankings[team].nil?
    printf "\e[1m%d\e[0m ", rankings[team]
  end

  printf "\e[1;37m%s\e[0m (%d-%d", team, record.overall.wins, record.overall.losses

  if record.overall.ties > 0
    printf "-%d)", record.overall.ties
  else
    printf ")"
  end
end

def print_injuries(team_name, teams)
  team = teams[team_name]

  team[:players].each do |player|
    display_name = sprintf "%s, %s", player[:last_name], player[:first_name]
    status = get_player_status player[:injuries]

    if player[:injuries] > 0
      printf "%-2s %2d %-20s %-15s  %s\n", player[:position], player[:number], display_name, team[:location], status
    end
  end
end


path = ARGV[0] || '.'

if ! Dir.exists? path
  abort "File '#{path}' not found.\n"
end


team_rankings = TeamRankings.new location

team_rankings.load_teams path
team_rankings.process_schedule "#{path}/schedule.csv"

rankings = team_rankings.get_ranked_teams


teams = {}

extract_data = ProgRunner.new "#{location}/../C", "extract_data"

if File.file?("#{path}/ncfo1.nst") and File.file?("#{path}/ncfo2.nst")
  extract_data.execute "#{path}/ncfo1.nes", "#{path}/ncfo1.nst", "#{path}/ncfo2.nes", "#{path}/ncfo2.nst"
else
  extract_data.execute "#{path}/ncfo1.nes", "#{path}/ncfo2.nes"
end

org = JSON.parse extract_data.get_output, {:symbolize_names => true}

org[:conferences].each do |conf|
  conf[:teams].each do |team|
    location = team[:location].split.map(&:capitalize).join(' ')

    location.gsub! 'N.', 'North'
    location.gsub! 'S.', 'South'

    teams[location] = team
  end
end



sp = ScheduleParser.new
fp = FileParser.new sp

fp.process_file "#{path}/schedule.csv"

schedule = sp.schedule

reg_season   = Hash.new
playoffs     = Hash.new
allstar      = Hash.new
gameday      = nil


schedule.days.each do |day|
  day.games.each do |game|
    if !game.played and gameday.nil?
      gameday = day
    end

    if day.day <= 10
      add_game reg_season, game
    end
  end
end


gameday.games.each do |game|
  road_record = reg_season[game.road_team]
  home_record = reg_season[game.home_team]

  print_team game.road_team, reg_season, rankings
  printf " at "
  print_team game.home_team, reg_season, rankings
  printf "\n"

  print_injuries game.road_team, teams
  print_injuries game.home_team, teams

  printf "\n"
end
