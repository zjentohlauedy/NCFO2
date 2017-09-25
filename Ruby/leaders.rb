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
    value = player.get_sort_key

    name = player.name + player.get_class

    if tied
      printf " -  %-2s %-24s %-15s #{format}\n", player.pos, name, player.school, value
    else
      printf "%2d. %-2s %-24s %-15s #{format}\n", index + 1, player.pos, name, player.school, value
    end
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


path = ARGV[0] || '.'

extract_data = ProgRunner.new "#{location}/../C", "extract_data"

extract_data.execute "#{path}/ncfo1.nes", "#{path}/ncfo1.nst", "#{path}/ncfo2.nes", "#{path}/ncfo2.nst"

org = JSON.parse extract_data.get_output, {:symbolize_names => true}

sr = StatRankings.new LeadersPrinter.new, LeadersFilter.new, org

sr.process_categories @player_categories
