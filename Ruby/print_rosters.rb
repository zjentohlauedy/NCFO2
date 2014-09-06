#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"
require 'ProgRunner'

path  = ARGV[0] || '.'
style = ARGV[1] || 'S'

print_rosters = ProgRunner.new "#{location}/../C", "print_rosters"

if File.file?("#{path}/ncfo1.nst") and File.file?("#{path}/ncfo2.nst")
  print_rosters.execute "#{path}/ncfo1.nes", "#{path}/ncfo1.nst", "#{path}/ncfo2.nes", "#{path}/ncfo2.nst", style
else
  print_rosters.execute "#{path}/ncfo1.nes", "#{path}/ncfo2.nes", style
end

puts print_rosters.get_output
