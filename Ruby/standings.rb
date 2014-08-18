#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"
require 'ProgRunner'

path = ARGV[0] || '.'

standings = ProgRunner.new "#{location}/../C", "standings"

standings.execute "#{path}/ncfo1.nes", "#{path}/ncfo1.nst", "#{path}/ncfo2.nes", "#{path}/ncfo2.nst"

puts standings.get_output
