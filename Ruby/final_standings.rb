#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"
require 'FileParser'
require 'ScheduleParser'
require 'standings'


def print_team_record( team )
  printf "%-15s  %2d %2d %2d", team[:name], team[:record].overall.wins, team[:record].overall.losses, team[:record].overall.ties
end


path = ARGV[0] || '.'

filename = "#{path}/schedule.csv"

if ! File.exists? filename
  abort "File '#{filename}' not found.\n"
end

sp = ScheduleParser.new
fp = FileParser.new sp

fp.process_file filename

schedule = sp.schedule

standings = Standings.new schedule

conferences = standings.process

(0..3).each do |i|
  printf "%-15s   W  L  T    ", conferences[i][:name]
end

puts
puts "-                 -  -  -    -                 -  -  -    -                 -  -  -    -                 -  -  -"

(0..5).each do |i|
  print_team_record conferences[0][:standings][i]
  printf "    "
  print_team_record conferences[1][:standings][i]
  printf "    "
  print_team_record conferences[2][:standings][i]
  printf "    "
  print_team_record conferences[3][:standings][i]
  printf "    "
  puts
end

puts
puts

(4..7).each do |i|
  printf "%-15s   W  L  T    ", conferences[i][:name]
end

puts
puts "-                 -  -  -    -                 -  -  -    -                 -  -  -    -                 -  -  -"

(0..5).each do |i|
  print_team_record conferences[4][:standings][i]
  printf "    "
  print_team_record conferences[5][:standings][i]
  printf "    "
  print_team_record conferences[6][:standings][i]
  printf "    "
  print_team_record conferences[7][:standings][i]
  printf "    "
  puts
end
