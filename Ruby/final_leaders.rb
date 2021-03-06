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

    return players.select { |p| (p.send filter_stat) >= p.get_filter_threshold }
  end
end

class LeadersCompiler
  def initialize( org )
    @org = org
  end

  def compile_stats( list, target_class, types )
    @org[:conferences].each do |conference|
      conference[:teams].each do |team|
        next if team[:players].nil?

        team[:players].each do |player|
          if types.include? player[:position]
            list.push( target_class.new team[:location], player )
          end
        end
      end
    end
  end
end


path = ARGV[0] || '.'

extract_data = ProgRunner.new "#{location}/../C", "extract_data"

extract_data.execute "#{path}/ncfo1.nes", "#{path}/ncfo1.nst", "#{path}/ncfo2.nes", "#{path}/ncfo2.nst"

org = JSON.parse extract_data.get_output, {:symbolize_names => true}

printer  = LeadersPrinter.new
filter   = LeadersFilter.new
compiler = LeadersCompiler.new org

sr = StatRankings.new printer, filter, compiler

sr.process_categories @player_categories
