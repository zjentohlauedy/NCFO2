#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"
require 'json'
require 'FileParser'
require 'ScheduleParser'
require 'ProgRunner'
require 'team_rankings'


path = ARGV[0] || '.'

if ! Dir.exists? path
  abort "File '#{path}' not found.\n"
end

rankings = TeamRankings.new location

rankings.load_teams path
rankings.process_schedule "#{path}/schedule.csv"
rankings.print_top_25


