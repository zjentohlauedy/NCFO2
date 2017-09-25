#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"
require 'json'
require 'bowls'
require 'positions'
require 'repository'
require 'stat_rankings'
require 'utils'


class LeadersPrinter
  def initialize( single_season, single_week )
    @single_season = single_season
    @single_week   = single_week
  end

  def print_empty_indicator()
    puts "--"
  end

  def print( player, format, index, tied )
    value = player.get_sort_key

    name = player.name + player.get_class

    if tied
      prefix = ' - '
    else
      prefix = sprintf '%2d.', index + 1
    end

    if @single_week
      context = ''
    elsif @single_season
      context = sprintf 'W%02d ', player.week
    else
      context = sprintf 'S%02d W%02d ', player.season, player.week
    end

    printf "#{prefix} %-2s %-24s #{context}%-15s #{format}\n", player.pos, name, player.school, value
  end

  def print_tie_message( summary, format, index )
    if @single_week
      padding = ''
    elsif @single_season
      padding = '    '
    else
      padding = '        '
    end

    printf "%2d.    %-35s      #{padding}#{format}\n", index + 1, "#{summary.count} Players Tied At", summary.value
  end
end

class LeadersFilter
  def apply( players, filter_stat )
    if filter_stat.nil?; return players; end

    filtered_players = players.select { |p| (p.send filter_stat) > 0 }

    case filter_stat
    when :att;   return filtered_players.select { |p| (p.send filter_stat) > 5 }
    when :rec;   return filtered_players.select { |p| (p.send filter_stat) > 3 }
    when :ret;   return filtered_players.select { |p| (p.send filter_stat) > 3 }
    when :fga;   return filtered_players.select { |p| (p.send filter_stat) > 3 }
    when :punts; return filtered_players.select { |p| (p.send filter_stat) > 3 }
    end

    return filtered_players
  end
end


@repository = Repository.new Utils.get_db "#{location}/../ncfo.db"


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


def set_query_conditions( parameters, player_id, season, week )
  parameters[:player_id] =        player_id
  clause                 = 'where player_id = :player_id'

  unless season.nil?
    parameters[:season]  =       season
    clause              += ' and season = :season'
  end

  unless week.nil?
    parameters[:week]  =       week
    clause            += ' and week = :week'
  end

  return clause
end


def get_player_offense_stats_by_player_id( player_id, season, week )
  parameters = {}
  query      = "select * from player_game_offense_stats_t #{set_query_conditions parameters, player_id, season, week}"

  @repository.custom_read query, parameters
end


def get_player_defense_stats_by_player_id( player_id, season, week )
  parameters = {}
  query      = "select * from player_game_defense_stats_t #{set_query_conditions parameters, player_id, season, week}"

  @repository.custom_read query, parameters
end


def get_player_kicking_stats_by_player_id( player_id, season, week )
  parameters = {}
  query      = "select * from player_game_kicking_stats_t #{set_query_conditions parameters, player_id, season, week}"

  @repository.custom_read query, parameters
end


def get_player_returns_stats_by_player_id( player_id, season, week )
  parameters = {}
  query      = "select * from player_game_returns_stats_t #{set_query_conditions parameters, player_id, season, week}"

  @repository.custom_read query, parameters
end


def get_player_stats_by_player( player, season, week )
  player[:stats] = {}

  case player[:position]
  when Positions::Quarterback
    player[:stats][:offense] = get_player_offense_stats_by_player_id player[:player_id], season, week
  when Positions::Runningback
    player[:stats][:offense] = get_player_offense_stats_by_player_id player[:player_id], season, week
    player[:stats][:returns] = get_player_returns_stats_by_player_id player[:player_id], season, week
  when Positions::WideReceiver
    player[:stats][:offense] = get_player_offense_stats_by_player_id player[:player_id], season, week
    player[:stats][:returns] = get_player_returns_stats_by_player_id player[:player_id], season, week
  when Positions::TightEnd
    player[:stats][:offense] = get_player_offense_stats_by_player_id player[:player_id], season, week
    player[:stats][:returns] = get_player_returns_stats_by_player_id player[:player_id], season, week
  when Positions::DefensiveLine
    player[:stats][:defense] = get_player_defense_stats_by_player_id player[:player_id], season, week
  when Positions::Linebacker
    player[:stats][:defense] = get_player_defense_stats_by_player_id player[:player_id], season, week
  when Positions::Cornerback
    player[:stats][:defense] = get_player_defense_stats_by_player_id player[:player_id], season, week
  when Positions::Safety
    player[:stats][:defense] = get_player_defense_stats_by_player_id player[:player_id], season, week
  when Positions::Kicker
    player[:stats][:kicking] = get_player_kicking_stats_by_player_id player[:player_id], season, week
  when Positions::Punter
    player[:stats][:kicking] = get_player_kicking_stats_by_player_id player[:player_id], season, week
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


season = ARGV[0]
week   = ARGV[1]

org = get_organization 1
org[:conferences] = get_conferences_by_org org

org[:conferences].each do |conference|
  conference[:teams] = get_teams_by_conference conference

  conference[:teams].each do |team|
    team[:players] = []
    players = get_players_by_team team

    players.each do |player|
      get_player_stats_by_player player, season, week

      player[:position] = Positions::string_value player[:position]

      p_list = flatten_stats player

      team[:players].concat p_list
    end
  end
end

lp = LeadersPrinter.new !season.nil?, !week.nil?

sr = StatRankings.new lp, LeadersFilter.new, org

sr.process_categories @player_categories
