#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"
require 'json'
require 'ProgRunner'
require 'stat_rankings'


class LeadersPrinter
  def print_empty_indicator()
    puts "--"
  end

  def print( player, format, index, tied )
    value = player.get_sort_value

    name = player.name + player.get_class

    if tied; then printf " -  ";
    else          printf "%2d. ", index + 1;
    end

    printf "%-2s %-24s %-15s #{format}\n", player.pos, name, player.school, value
  end

  def print_tie_message( summary, format, index )
    printf "%2d.    %-35s      #{format}\n", index + 1, "#{summary.count} Players Tied At", summary.value
  end
end

class LeadersFilter
  def apply( players, filter_stat )
    if filter_stat.nil?; return players; end

    filtered_players = players.select { |p| (p.send filter_stat) > 0 }

    sum = filtered_players.reduce(0) { |sum, p| sum + (p.send filter_stat) }

    mean = sum.to_f / filtered_players.length

    variance = filtered_players.reduce(0) { |var, p| var + ((p.send filter_stat) - mean)**2 }
    variance /= (filtered_players.length - 1).to_f

    stddev = Math.sqrt variance

    return filtered_players.select { |p| (p.send filter_stat) > (mean - stddev) }
  end
end


@categories = {
  'quarterbacks'  => {  'class' => Passing,         'types' => ['QB'],
    'stats'       => [{ 'label' => "Passers",       'stat'  => :pass_score, 'filter' => nil, 'format' => '%d' }]},

  'runningbacks'  => {  'class' => Rushing,         'types' => ['RB'],
    'stats'       => [{ 'label' => "Rushers",       'stat'  => :rush_score, 'filter' => nil, 'format' => '%d' }]},

  'receivers'     => {  'class' => Receiving,       'types' => ['WR','TE'],
    'stats'       => [{ 'label' => "Receivers",     'stat'  => :recv_score, 'filter' => nil, 'format' => '%d' }]},

  'pass rushers'  => {  'class' => Sacks,           'types' => ['DL','LB'],
    'stats'       => [{ 'label' => "Pass Rushers",  'stat'  => :pr_score,   'filter' => nil, 'format' => '%d' }]},

  'coverage'      => {  'class' => Interceptions,   'types' => ['LB','CB', 'S'],
    'stats'       => [{ 'label' => "Pass Coverage", 'stat'  => :cvg_score,  'filter' => nil, 'format' => '%d' }]}
}



path = ARGV[0] || '.'

extract_data = ProgRunner.new "#{location}/../C", "extract_data"

extract_data.execute "#{path}/ncfo1.nes", "#{path}/ncfo2.nes"

org = JSON.parse extract_data.get_output, {:symbolize_names => true}

sr = StatRankings.new LeadersPrinter.new, LeadersFilter.new, org

sr.process_categories @categories

