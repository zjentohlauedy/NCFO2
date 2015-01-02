#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"
require 'json'
require 'FileParser'
require 'ProgRunner'
require 'ScheduleParser'
require 'top_players'
require 'utils'


def get_bowl_game_data( cwd, path, game_name )
  game = game_name.gsub ' ', '_'

  extract_data = ProgRunner.new "#{cwd}/../C", "extract_bowl_game_data"

  extract_data.execute "#{path}/playoffs.nes", "#{path}/#{game}.nst"

  unless extract_data.success?
    puts "ERROR: #{extract_data.get_output}"

    return nil
  end

  return JSON.parse extract_data.get_output, {:symbolize_names => true}
end


path = ARGV[0] || '.'


sp = ScheduleParser.new
fp = FileParser.new sp

fp.process_file "#{path}/schedule.csv"

schedule = sp.schedule

week = nil
schedule.days.each do |day|
  break if ! day.completed
  week = day
end

exit if week.nil?
exit if week.day <= 10
exit if week.day >  13


teams = []
week.games.each do |game|
  next if game.name.nil?

  data = get_bowl_game_data location, path, game.name

  next if data.nil?

  teams.push data[:road_team]
  teams.push data[:home_team]
end

org = {conferences: [{ teams: teams }]}

sr = StatRankings.new org

sr.process_categories @categories
