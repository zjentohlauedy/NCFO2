#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"
require 'json'
require 'bowls'
require 'positions'
require 'repository'
require 'top_teams'
require 'utils'


@repository = Repository.new Utils.get_db "#{location}/../ncfo.db"


def get_organization( organization_id )
  result = @repository.custom_read "select * from organizations_t where organization_id = #{organization_id}"
  result[0]
end


def get_conferences_by_org( organization )
  @repository.custom_read "select c.* from organization_conferences_t oc join conferences_t c on oc.conference_id = c.conference_id where oc.organization_id = #{organization[:organization_id]}"
end


def get_teams_by_conference( conference )
  @repository.custom_read "select t.*, ts.season from conference_teams_t ct join teams_t t on ct.team_id = t.team_id join team_stats_t ts on ts.team_id = t.team_id where ct.conference_id = #{conference[:conference_id]} and ts.bowl_game = 0"
end

def get_team_stats_by_team_id_and_season( team_id, season )
  result = @repository.custom_read "select * from team_stats_t where team_id = #{team_id} and season = #{season} and bowl_game = 0"
  result[0]
end

def get_team_offense_stats_by_team_id_and_season( team_id, season )
  result = @repository.custom_read "select * from team_offense_stats_t where team_id = #{team_id} and season = #{season} and bowl_game = 0"
  result[0]
end

def get_team_defense_stats_by_team_id_and_season( team_id, season )
  result = @repository.custom_read "select * from team_defense_stats_t where team_id = #{team_id} and season = #{season} and bowl_game = 0"
  result[0]
end

def get_team_kicking_stats_by_team_id_and_season( team_id, season )
  result = @repository.custom_read "select * from team_kicking_stats_t where team_id = #{team_id} and season = #{season} and bowl_game = 0"
  result[0]
end


org = get_organization 1
org[:conferences] = get_conferences_by_org org

org[:conferences].each do |conference|
  conference[:teams] = get_teams_by_conference conference

  conference[:teams].each do |team|
    team[:stats]         = get_team_stats_by_team_id_and_season         team[:team_id], team[:season]
    team[:offense_stats] = get_team_offense_stats_by_team_id_and_season team[:team_id], team[:season]
    team[:defense_stats] = get_team_defense_stats_by_team_id_and_season team[:team_id], team[:season]
    team[:kicking_stats] = get_team_kicking_stats_by_team_id_and_season team[:team_id], team[:season]
  end
end

sr = StatRankings.new org, :absolute

sr.process_categories @categories
