#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"
require 'ProgRunner'

path = ARGV[0] || '.'

injuries = ProgRunner.new "#{location}/../C", "injuries"

if File.file?("#{path}/playoffs.nes") and File.file?("#{path}/playoffs.nst")
  injuries.execute "#{path}/playoffs.nes", "#{path}/playoffs.nst"
else
  injuries.execute "#{path}/ncfo1.nes", "#{path}/ncfo1.nst", "#{path}/ncfo2.nes", "#{path}/ncfo2.nst"
end

puts injuries.get_output
