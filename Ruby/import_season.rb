#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"
require 'ProgRunner'
require 'FileParser'
require 'ScheduleParser'
require 'TeamRecords'
require 'repository'
require 'team_stats'
require 'bowls'
require 'utils'

@team_ids = {
          # Atlantic
          'Delaware'       =>  1,
          'Maryland'       =>  2,
          'New Jersey'     =>  3,
          'Pennsylvania'   =>  4,
          'Virginia'       =>  5,
          'West Virginia'  =>  6,
          # Great Lake
          'Illinois'       =>  7,
          'Michigan'       =>  8,
          'Minnesota'      =>  9,
          'New York'       => 10,
          'Ohio'           => 11,
          'Wisconsin'      => 12,
          # Midwest
          'Arkansas'       => 13,
          'Indiana'        => 14,
          'Kansas'         => 15,
          'Kentucky'       => 16,
          'Missouri'       => 17,
          'Tennessee'      => 18,
          # New England
          'Connecticut'    => 19,
          'Maine'          => 20,
          'Massachusetts'  => 21,
          'New Hampshire'  => 22,
          'Rhode Island'   => 23,
          'Vermont'        => 24,
          # North
          'Iowa'           => 25,
          'Montana'        => 26,
          'Nebraska'       => 27,
          'North Dakota'   => 28,
          'South Dakota'   => 29,
          'Wyoming'        => 30,
          # Pacific
          'California'     => 31,
          'Idaho'          => 32,
          'Nevada'         => 33,
          'Oregon'         => 34,
          'Utah'           => 35,
          'Washington'     => 36,
          # Southeast
          'Alabama'        => 37,
          'Florida'        => 38,
          'Georgia'        => 39,
          'Mississippi'    => 40,
          'North Carolina' => 41,
          'South Carolina' => 42,
          # Southwest
          'Arizona'        => 43,
          'Colorado'       => 44,
          'Louisiana'      => 45,
          'New Mexico'     => 46,
          'Oklahoma'       => 47,
          'Texas'          => 48
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

season = ARGV[0] || abort( "Must supply a season." )
path   = ARGV[1] || '.'

import_season = ProgRunner.new "#{location}/../C", "import_season"

puts "Importing Regular Season..."

import_season.execute "#{location}/../ncfo.db", "#{path}/ncfo1.nes", "#{path}/ncfo1.nst", "#{path}/ncfo2.nes", "#{path}/ncfo2.nst", season

if import_season.success?
  puts "Regular Season Imported Successfully!"
else
  puts "Error Importing Regular Season:"
  puts import_season.get_output
  exit
end

sp = ScheduleParser.new
fp = FileParser.new sp

fp.process_file "#{path}/schedule.csv"

schedule = sp.schedule

records = {}

schedule.days.each do |day|
  day.games.each do |game|
    if day.day <= 10
      add_game records, game
    end
  end
end

repo = Repository.new Utils::get_db "#{location}/../ncfo.db"

puts "Updating team records..."

repo.start_transaction

begin
  records.each do |team, team_record|
    team_stats = TeamStats.new @team_ids[team], season, Bowls::None
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
  puts "Done."
rescue Exception => e
  repo.cancel_transaction
  puts "Error." + e.message
  exit
end
