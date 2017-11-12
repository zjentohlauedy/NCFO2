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

  def print( team, format, index, tied )
    value = team.get_sort_value

    if tied; then printf " -  ";
    else          printf "%2d. ", index + 1;
    end

    printf "%-15s %-15s #{format}\n", team.school, team.name, value
  end

  def print_tie_message( summary, format, index )
    printf "%2d. %-30s  #{format}\n", index + 1, "#{summary.count} Teams Tied At", summary.value
  end
end

class LeadersFilter
  def apply( list, filter_stat )
    return list
  end
end

class LeadersCompiler
  def initialize( org )
    @org = org
  end

  def compile_stats( list, target_class, types )
    @org[:conferences].each do |conference|
      conference[:teams].each do |team|
        list.push( target_class.new team )
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

sr.process_categories @team_categories
