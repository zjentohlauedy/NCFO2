#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"
require 'json'
require 'ProgRunner'
require 'top_players'


@categories = {
  'quarterbacks'  => {  'class' => Passing,         'types' => ['QB'],
    'stats'       => [{ 'label' => "Passers",       'stat'  => :pass_score }]},

  'runningbacks'  => {  'class' => Rushing,         'types' => ['RB'],
    'stats'       => [{ 'label' => "Rushers",       'stat'  => :rush_score }]},

  'receivers'     => {  'class' => Receiving,       'types' => ['WR','TE'],
    'stats'       => [{ 'label' => "Receivers",     'stat'  => :recv_score }]},

  'pass rushers'  => {  'class' => Sacks,           'types' => ['DL','LB'],
    'stats'       => [{ 'label' => "Pass Rushers",  'stat'  => :pr_score   }]},

  'coverage'      => {  'class' => Interceptions,   'types' => ['LB','CB', 'S'],
    'stats'       => [{ 'label' => "Pass Coverage", 'stat'  => :cvg_score  }]}
}



path = ARGV[0] || '.'

extract_data = ProgRunner.new "#{location}/../C", "extract_data"

extract_data.execute "#{path}/ncfo1.nes", "#{path}/ncfo2.nes"

org = JSON.parse extract_data.get_output, {:symbolize_names => true}

sr = StatRankings.new org

sr.process_categories @categories

