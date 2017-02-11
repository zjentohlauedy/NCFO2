#!/usr/bin/env ruby
#
# From the season directiory run:
# find W* -name "*.txt" | xargs ../Ruby/ydsall.rb | ../Ruby/calc_yards_allowed.rb <season number>
#
# Example, for season 4 the command would be:
# S04 $ find W* -name "*.txt" | xargs ../Ruby/ydsall.rb | ../Ruby/calc_yards_allowed.rb 4
#

location = File.dirname __FILE__

$: << "#{location}"
require 'json'
require 'sqlite3'

@db = SQLite3::Database.new "#{location}/../ncfo.db"

@db.results_as_hash  = true
@db.type_translation = true

season = ARGV[0]
teams = {}

STDIN::each_line do |line|
  data = JSON.parse line

  data.each do |entry|
    name = entry['name']
    bowl = entry['bowl']

    if teams[name].nil?
      teams[name] = {}
      teams[name][bowl] = entry['yards_allowed'].to_i
    elsif teams[name][bowl].nil?
      teams[name][bowl] = entry['yards_allowed'].to_i
    else
      teams[name][bowl] += entry['yards_allowed'].to_i
    end
  end
end

teams.each do |team, bowls|
  team_id = (@db.execute "select team_id from teams_t where abbreviation = '#{team}'")[0]['Team_Id']

  bowls.each do |bowl, yards_allowed|
    @db.execute "update team_defense_stats_t set yards_allowed = #{yards_allowed} where team_id = #{team_id} and season = #{season} and bowl_game = #{bowl}"
  end
end
