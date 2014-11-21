#!/usr/bin/env ruby
#
# Reads filled in schedule.csv file and calculates
# Overall and Head-To-Head records as well as
# scoring totals.
location = File.dirname __FILE__

$: << "#{location}"
require 'FileParser'
require 'ScheduleParser'
require 'TeamRecords'


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


if ARGV.length != 1
  abort "Usage: #{__FILE__} <filename>\n"
end

filename = ARGV[0]

if ! File.exists? filename
  abort "File '#{filename}' not found.\n"
end

sp = ScheduleParser.new
fp = FileParser.new sp

fp.process_file filename

schedule = sp.schedule

reg_season   = Hash.new
playoffs     = Hash.new
allstar      = Hash.new


schedule.days.each do |day|
  day.games.each do |game|
    if day.day <= 10
      add_game reg_season, game
    elsif day.day <= 13
      add_game playoffs, game
    else
      add_game allstar, game
    end
  end
end

puts "Regular Season:\n\n"

reg_season.sort.each do |team, team_record|
  printf "%-15s ", team
  team_record.print_records
end

puts "Playoffs:\n\n"

playoffs.sort.each do |team, team_record|
  printf "%-15s ", team
  team_record.print_records
end

puts "All American Bowl:\n\n"

allstar.sort.each do |team, team_record|
  printf "%-15s ", team
  team_record.print_records
end
