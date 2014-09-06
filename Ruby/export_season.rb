#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"
require 'ProgRunner'

path  = ARGV[0] || '.'

export_season = ProgRunner.new "#{location}/../C", "export_season"

export_season.execute "#{location}/../ncfo1_template.nes", "#{location}/../ncfo2_template.nes", "#{location}/../ncfo.db", path

puts export_season.get_output
