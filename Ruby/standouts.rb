#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"
require 'json'
require 'ProgRunner'
require 'top_players'
require 'utils'


path = ARGV[0] || '.'

extract_data = ProgRunner.new "#{location}/../C", "extract_data"

extract_data.execute "#{path}/ncfo1.nes", "#{path}/ncfo1.nst", "#{path}/ncfo2.nes", "#{path}/ncfo2.nst"

org = JSON.parse extract_data.get_output, {:symbolize_names => true}

if File.file?("#{path}/.ncfo1.nst.bak") and File.file?("#{path}/.ncfo2.nst.bak")
  extract_data.execute "#{path}/ncfo1.nes", "#{path}/.ncfo1.nst.bak", "#{path}/ncfo2.nes", "#{path}/.ncfo2.nst.bak"

  old_org = JSON.parse extract_data.get_output, {:symbolize_names => true}

  Utils::org_delta org, old_org
end

sr = StatRankings.new org

sr.process_categories @categories
