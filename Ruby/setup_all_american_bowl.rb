#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"
require 'ProgRunner'

player_list_file = ARGV[0] || abort( "Must supply a player list filename." )
path             = ARGV[1] || '.'

copy_aa = ProgRunner.new "#{location}/../C", "copy_all_americans"

copy_aa.execute path, "#{location}/..", path, player_list_file

if copy_aa.success?
  puts "All Americans rom and save state created."
else
  puts "An error occurred:"
  puts copy_aa.get_output
end
