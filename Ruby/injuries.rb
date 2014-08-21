#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"
require 'ProgRunner'

path = ARGV[0] || '.'

injuries = ProgRunner.new "#{location}/../C", "injuries"

injuries.execute "#{path}/ncfo1.nes", "#{path}/ncfo1.nst", "#{path}/ncfo2.nes", "#{path}/ncfo2.nst"

puts injuries.get_output
