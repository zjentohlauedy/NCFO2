#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"
require 'json'
require 'FileParser'
require 'ScheduleParser'
require 'ProgRunner'


class TeamRankings

  class TeamRating
    attr_reader :name
    attr_accessor :rating

    def initialize( name, rating )
      @name = name
      @rating = rating
    end

    def <=>( other )
      return other.rating <=> @rating
    end

    def calc_new_rating( opponent, scored, allowed )
      new_rating = @rating + (scored - allowed)

      if scored > allowed
        new_rating += 10 + (opponent.rating / 4.0).to_i
      else
        penalty = (@rating > opponent.rating) ? ((@rating - opponent.rating) / 4).to_i : 0
        new_rating -= (50 + penalty)
      end

      return new_rating
    end

  end

  def initialize( location = '.' )
    @ratings = Hash.new
    @teams   = Array.new
    @location = location
  end

  def calc_team_score( players, offensive_formation )
    scores = {}

    players.each do |player|
      if scores[player[:position]].nil?
        scores[player[:position]] = []
      end

      scores[player[:position]].push player[:score]
    end

    scores['QB'].nil? or scores['QB'].sort!
    scores['RB'].nil? or scores['RB'].sort!
    scores['WR'].nil? or scores['WR'].sort!
    scores['TE'].nil? or scores['TE'].sort!

    total_score = 0

    if offensive_formation == 0
      total_score += scores['QB'][1]
      total_score += scores['RB'][2] + scores['RB'][3]
      total_score += scores['WR'][2] + scores['WR'][3]
      total_score += scores['TE'][1]
    elsif offensive_formation == 1
      total_score += scores['QB'][1]
      total_score += scores['RB'][3]
      total_score += scores['WR'][2] + scores['WR'][3] + scores['WR'][4] + scores['WR'][5]
    else
      total_score += scores['QB'][1]
      total_score += scores['RB'][3]
      total_score += scores['WR'][1] + scores['WR'][2] + scores['WR'][3]
      total_score += scores['TE'][1]
    end

    total_score += scores['OL'].reduce { |sum, score| sum + score } / scores['OL'].length
    total_score += scores['DL'].reduce { |sum, score| sum + score } / scores['DL'].length
    total_score += scores['LB'].reduce { |sum, score| sum + score } / scores['LB'].length

    total_score += scores['CB'].concat( scores['S'] ).reduce { |sum, score| sum + score } / (scores['CB'].length + scores['S'].length)
  end

  def load_teams( path )
    extract_data = ProgRunner.new "#{@location}/../C", "extract_data"

    if File.file?("#{path}/ncfo1.nst") and File.file?("#{path}/ncfo2.nst")
      extract_data.execute "#{path}/ncfo1.nes", "#{path}/ncfo1.nst", "#{path}/ncfo2.nes", "#{path}/ncfo2.nst"
    else
      extract_data.execute "#{path}/ncfo1.nes", "#{path}/ncfo2.nes"
    end

    org = JSON.parse extract_data.get_output, {:symbolize_names => true}

    org[:conferences].each do |conf|
      conf[:teams].each do |team|

        team_score = calc_team_score team[:players], team[:offensive_formation]
        location   = team[:location].split.map(&:capitalize).join(' ')

        location.gsub! 'N.', 'North'
        location.gsub! 'S.', 'South'

        @teams.push TeamRating.new( location, team_score )
      end
    end

    rank_teams
  end

  def rank_teams
    @teams.sort!

    i = 0
    @teams.each do |team|
      team.rating = 500 - (i * 10)
      @ratings.store team.name, team
      i += 1
    end
  end

  def process_schedule( filename )
    sp = ScheduleParser.new
    fp = FileParser.new sp

    fp.process_file filename

    sp.schedule.days.each do |day|
      next if ! day.completed

      day.games.each do |game|
        update_ratings game
      end
    end
  end

  def update_ratings( game )
    home = @ratings.fetch game.home_team
    road = @ratings.fetch game.road_team

    new_home_rating = home.calc_new_rating road, game.home_score, game.road_score
    new_road_rating = road.calc_new_rating home, game.road_score, game.home_score

    home.rating = new_home_rating
    road.rating = new_road_rating
  end

  def print_top_25
    teams = @ratings.values
    teams.sort!

    i = 0
    teams.each do |team|
      i += 1
      printf "%2d. %s\n", i, team.name
      break if i >= 25
    end
  end

end


path = ARGV[0] || '.'

if ! Dir.exists? path
  abort "File '#{path}' not found.\n"
end

rankings = TeamRankings.new location

rankings.load_teams path
rankings.process_schedule "#{path}/schedule.csv"
rankings.print_top_25
