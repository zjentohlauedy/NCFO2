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
  def print_empty_indicator()
    puts "--"
  end

  def print( player, format, index, tied )
    value = player.get_sort_value

    name = player.name + player.get_class

    if tied; then printf " -  ";
    else          printf "%2d. ", index + 1;
    end

    printf "%-2s %-24s %d %-15s #{format}\n", player.pos, name, player.seasons, player.school, value
  end

  def print_tie_message( summary, format, index )
    printf "%2d.    %-35s        #{format}\n", index + 1, "#{summary.count} Players Tied At", summary.value
  end
end

class LeadersFilter
  def apply( players, filter_stat )
    if filter_stat.nil?; return players; end

    return players.select { |p| (p.send filter_stat) >= (p.get_filter_threshold * 3) }
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


def get_player_offense_stats_by_player_id( player_id )
  result = @repository.custom_read "select player_id, count(1) seasons, sum(pass_attempts) pass_attempts, sum(completions) completions, sum(interceptions) interceptions, sum(pass_yards) pass_yards, sum(pass_touchdowns) pass_touchdowns, sum(rush_attempts) rush_attempts, sum(rush_yards) rush_yards, sum(rush_touchdowns) rush_touchdowns, sum(receptions) receptions, sum(receiving_yards) receiving_yards, sum(receiving_touchdowns) receiving_touchdowns from player_offense_stats_t where player_id = #{player_id} and bowl_game = #{Bowls::None} group by player_id"
  result[0]
end


def get_player_defense_stats_by_player_id( player_id )
  result = @repository.custom_read "select player_id, count(1) seasons, sum(sacks) sacks, sum(interceptions) interceptions, sum(return_yards) return_yards, sum(return_touchdowns) return_touchdowns from player_defense_stats_t where player_id = #{player_id} and bowl_game = #{Bowls::None} group by player_id"
  result[0]
end


def get_player_kicking_stats_by_player_id( player_id )
  result = @repository.custom_read "select player_id, count(1) seasons, sum(extra_point_attempts) extra_point_attempts, sum(extra_points_made) extra_points_made, sum(field_goal_attempts) field_goal_attempts, sum(field_goals_made) field_goals_made, sum(punts) punts, sum(punt_yards) punt_yards from player_kicking_stats_t where player_id = #{player_id} and bowl_game = #{Bowls::None} group by player_id"
  result[0]
end


def get_player_returns_stats_by_player_id( player_id )
  result = @repository.custom_read "select player_id, count(1) seasons, sum(kick_returns) kick_returns, sum(kick_return_yards) kick_return_yards, sum(kick_return_touchdowns) kick_return_touchdowns, sum(punt_returns) punt_returns, sum(punt_return_yards) punt_return_yards, sum(punt_return_touchdowns) punt_return_touchdowns from player_returns_stats_t where player_id = #{player_id} and bowl_game = #{Bowls::None} group by player_id"
  result[0]
end


def get_player_stats_by_player( player )
  player[:stats] = {}

  case player[:position]
  when Positions::Quarterback
    player[:stats][:offense] = get_player_offense_stats_by_player_id player[:player_id]
  when Positions::Runningback
    player[:stats][:offense] = get_player_offense_stats_by_player_id player[:player_id]
    player[:stats][:returns] = get_player_returns_stats_by_player_id player[:player_id]
  when Positions::WideReceiver
    player[:stats][:offense] = get_player_offense_stats_by_player_id player[:player_id]
    player[:stats][:returns] = get_player_returns_stats_by_player_id player[:player_id]
  when Positions::TightEnd
    player[:stats][:offense] = get_player_offense_stats_by_player_id player[:player_id]
    player[:stats][:returns] = get_player_returns_stats_by_player_id player[:player_id]
  when Positions::DefensiveLine
    player[:stats][:defense] = get_player_defense_stats_by_player_id player[:player_id]
  when Positions::Linebacker
    player[:stats][:defense] = get_player_defense_stats_by_player_id player[:player_id]
  when Positions::Cornerback
    player[:stats][:defense] = get_player_defense_stats_by_player_id player[:player_id]
  when Positions::Safety
    player[:stats][:defense] = get_player_defense_stats_by_player_id player[:player_id]
  when Positions::Kicker
    player[:stats][:kicking] = get_player_kicking_stats_by_player_id player[:player_id]
  when Positions::Punter
    player[:stats][:kicking] = get_player_kicking_stats_by_player_id player[:player_id]
  end
end


if ARGV.length > 0
  chosen_team = ARGV[0]
end


org = get_organization 1
org[:conferences] = get_conferences_by_org org

org[:conferences].each do |conference|
  conference[:teams] = get_teams_by_conference conference

  conference[:teams].each do |team|
    next if chosen_team and team[:location].downcase != chosen_team.downcase

    team[:players] = get_players_by_team team

    team[:players].each do |player|
      get_player_stats_by_player player

      player[:position] = Positions::string_value player[:position]
    end

    team[:players].reject! { |player|
      %w{QB}        .include? player[:position] and player[:stats][:offense].nil? or
      %w{RB TE WR}  .include? player[:position] and player[:stats][:offense].nil? and player[:stats][:returns].nil? or
      %w{DL LB CB S}.include? player[:position] and player[:stats][:defense].nil? or
      %w{K P}       .include? player[:position] and player[:stats][:kicking].nil?
    }
  end
end

sr = StatRankings.new LeadersPrinter.new, LeadersFilter.new, org

sr.process_categories @player_categories
