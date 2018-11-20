#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"
require 'json'
require 'optparse'
require 'bowls'
require 'positions'
require 'repository'
require 'stat_rankings'
require 'utils'


class LeadersPrinter
  def initialize( single_team, single_season )
    @single_team   = single_team
    @single_season = single_season
  end

  def print_empty_indicator()
    puts "--"
  end

  def print( player, format, index, tied )
    value = player.get_sort_value

    name = player.name + player.get_class

    if tied
      prefix = ' - '
    else
      prefix = sprintf '%2d.', index + 1
    end

    if @single_team
      if @single_season
        context = ''
      else
        context = sprintf "S%02d", player.season
      end
    else
      if @single_season
        context = sprintf "%-15s", player.school
      else
        context = sprintf "S%02d %-15s", player.season, player.school
      end
    end

    printf "#{prefix} %-2s %-24s #{context}  #{format}\n", player.pos, name, value
  end

  def print_tie_message( summary, format, index )
    if @single_team
      if @single_season
        padding = ''
      else
        padding = '   '
      end
    else
      if @single_season
        padding = '               '
      else
        padding = '                   '
      end
    end

    printf "%2d.    %-20s      #{padding} #{format}\n", index + 1, "#{summary.count} Players Tied At", summary.value
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


@repository = Repository.new Utils.get_db "#{location}/../ncfo.db"

@options = {}

OptionParser.new do |opt|
  opt.on( '-s', '--season SEASON',   'Show best games for this season'    ) { |o| @options[ :season ] = o }
  opt.on( '-t', '--team   TEAM',     'Show best games for this team'      ) { |o| @options[ :team   ] = o }
end.parse!


def get_organization( organization_id )
  result = @repository.custom_read "select * from organizations_t where organization_id = #{organization_id}"
  result[0]
end


def get_conferences_by_org( organization )
  @repository.custom_read "select c.* from organization_conferences_t oc join conferences_t c on oc.conference_id = c.conference_id where oc.organization_id = #{organization[:organization_id]}"
end


def get_teams_by_conference( conference )
  @repository.custom_read "select t.* from conference_teams_t ct join teams_t t on ct.team_id = t.team_id where ct.conference_id = #{conference[:conference_id]}"
end


def get_players_by_team( team )
  @repository.custom_read "select distinct p.* from team_players_t tp join players_t p on tp.player_id = p.player_id where tp.team_id = #{team[:team_id]}"
end


def get_player_offense_stats_by_player_id( player_id, season )
  query = "select * from player_offense_stats_t where player_id = #{player_id} and bowl_game = #{Bowls::None}"

  if season
    query = "#{query} and season = #{season}"
  end

  @repository.custom_read query
end


def get_player_defense_stats_by_player_id( player_id, season )
  query = "select * from player_defense_stats_t where player_id = #{player_id} and bowl_game = #{Bowls::None}"

  if season
    query = "#{query} and season = #{season}"
  end

  @repository.custom_read query
end


def get_player_kicking_stats_by_player_id( player_id, season )
  query = "select * from player_kicking_stats_t where player_id = #{player_id} and bowl_game = #{Bowls::None}"

  if season
    query = "#{query} and season = #{season}"
  end

  @repository.custom_read query
end


def get_player_returns_stats_by_player_id( player_id, season )
  query = "select * from player_returns_stats_t where player_id = #{player_id} and bowl_game = #{Bowls::None}"

  if season
    query = "#{query} and season = #{season}"
  end

  @repository.custom_read query
end


def get_player_stats_by_player( player, season )
  player[:stats] = {}

  case player[:position]
  when Positions::Quarterback
    player[:stats][:offense] = get_player_offense_stats_by_player_id player[:player_id], season
  when Positions::Runningback
    player[:stats][:offense] = get_player_offense_stats_by_player_id player[:player_id], season
    player[:stats][:returns] = get_player_returns_stats_by_player_id player[:player_id], season
  when Positions::WideReceiver
    player[:stats][:offense] = get_player_offense_stats_by_player_id player[:player_id], season
    player[:stats][:returns] = get_player_returns_stats_by_player_id player[:player_id], season
  when Positions::TightEnd
    player[:stats][:offense] = get_player_offense_stats_by_player_id player[:player_id], season
    player[:stats][:returns] = get_player_returns_stats_by_player_id player[:player_id], season
  when Positions::DefensiveLine
    player[:stats][:defense] = get_player_defense_stats_by_player_id player[:player_id], season
  when Positions::Linebacker
    player[:stats][:defense] = get_player_defense_stats_by_player_id player[:player_id], season
  when Positions::Cornerback
    player[:stats][:defense] = get_player_defense_stats_by_player_id player[:player_id], season
  when Positions::Safety
    player[:stats][:defense] = get_player_defense_stats_by_player_id player[:player_id], season
  when Positions::Kicker
    player[:stats][:kicking] = get_player_kicking_stats_by_player_id player[:player_id], season
  when Positions::Punter
    player[:stats][:kicking] = get_player_kicking_stats_by_player_id player[:player_id], season
  end
end


def flatten_stats( player )
  players = []

  player[:stats].each do |category, list|
    list.each_with_index do |stats, idx|
      if players[idx].nil?
        plr = player.dup
        plr[:stats] = {}
      else
        plr = players[idx]
      end

      plr[:stats][category] = stats

      players[idx] = plr
    end
  end

  return players
end


chosen_team   = @options[:team]
chosen_season = @options[:season]

org = get_organization 1
org[:conferences] = get_conferences_by_org org

org[:conferences].each do |conference|
  conference[:teams] = get_teams_by_conference conference

  conference[:teams].each do |team|
    next if chosen_team and team[:location].downcase != chosen_team.downcase

    team[:players] = []
    players = get_players_by_team team

    players.each do |player|
      get_player_stats_by_player player, chosen_season

      player[:position] = Positions::string_value player[:position]

      p_list = flatten_stats player

      team[:players].concat p_list
    end
  end
end

printer  = LeadersPrinter.new !chosen_team.nil?, !chosen_season.nil?
filter   = LeadersFilter.new
compiler = LeadersCompiler.new org

sr = StatRankings.new printer, filter, compiler

sr.process_categories @player_categories
