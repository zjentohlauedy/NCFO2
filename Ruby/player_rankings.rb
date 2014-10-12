#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"
require 'json'
require 'ProgRunner'



class Scores
  include Comparable

  def initialize
    @direction = :descending
    @sort_key = nil
  end

  def <=>( other )
    if @direction == :ascending
      get_sort_key.to_f <=> other.get_sort_key.to_f
    else
      other.get_sort_key.to_f <=> get_sort_key.to_f
    end
  end

  def set_direction( direction )
    @direction = direction
  end

  def set_sort_key( key )
    @sort_key = key
  end

  def get_sort_key
    send @sort_key
  end

  def to_s
    sprintf "%-2s %-20s %-15s", @pos, @name, @school
  end

end

class Passers < Scores
  attr_reader :score

  def initialize( team, player )
    super()

    @school = team[:location]
    @pos    = player[:position]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"

    @score = player[:score]

    case team[:offensive_preference]
    when 0; @score -=  5
    when 1; @score -= 25
    when 2; @score +=  5
    when 3; @score += 25
    end
  end

end

class Rushers < Scores
  attr_reader :score

  def initialize( team, player )
    super()

    @school = team[:location]
    @pos    = player[:position]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"

    @score = player[:score]

    case team[:offensive_preference]
    when 0; @score +=  5
    when 1; @score += 25
    when 2; @score -=  5
    when 3; @score -= 25
    end

    if team[:offensive_formation] == 0 and not player[:feature_back]
      @score /= 2
    end
  end

end

class Receivers < Scores
  attr_reader :score

  def initialize( team, player )
    super()

    @school = team[:location]
    @pos    = player[:position]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"

    @score = player[:score]

    case team[:offensive_preference]
    when 0; @score -=  5
    when 1; @score -= 25
    when 2; @score +=  5
    when 3; @score += 25
    end
  end

end

class PassRushers < Scores
  attr_reader :score

  def initialize( team, player )
    super()

    @school = team[:location]
    @pos    = player[:position]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"

    @score = player[:score]
  end

end

class Interceptors < Scores
  attr_reader :score

  def initialize( team, player )
    super()

    @school = team[:location]
    @pos    = player[:position]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"

    @score = player[:score]
  end

end

class Kickers < Scores
  attr_reader :score

  def initialize( team, player )
    super()

    @school = team[:location]
    @pos    = player[:position]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"

    @score = player[:score]
  end

end

class Punters < Scores
  attr_reader :score

  def initialize( team, player )
    super()

    @school = team[:location]
    @pos    = player[:position]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"

    @score = player[:score]
  end

end


class PlayerRankings

  def initialize( org )
    @org        = org
    @players    = Array.new
  end

  def process_categories( categories )
    categories.each do |key, value|
      compile_scores value.fetch( 'class' ), value.fetch( 'types' )

      print "#{value.fetch 'label'}\n"
      print_top_players value['score']
      print "\n"
    end
  end

  def compile_scores( object, types )
    @players = Array.new

    @org[:conferences].each do |conference|
      conference[:teams].each do |team|
        team[:players].each do |player|
          next unless player[:starter]

          if types.include? player[:position]
            @players.push( object.new team, player )
          end
        end
      end
    end
  end

  def print_top_players( score, count=15 )
    @players.each do |player|
      player.set_sort_key score
    end

    players = @players.sort

    i = 0

    while i < [count, players.length].min
      print "#{players[i].to_s}\n"
      i += 1
    end
  end

end


@categories = {
  'passers'       => {  'class' => Passers,      'label' => 'Passers',       'score' => :score, 'types' => ['QB']          },
  'rushers'       => {  'class' => Rushers,      'label' => 'Rushers',       'score' => :score, 'types' => ['RB']          },
  'receivers'     => {  'class' => Receivers,    'label' => 'Receivers',     'score' => :score, 'types' => ['WR','TE']     },
  'pass_rushers'  => {  'class' => PassRushers,  'label' => 'Pass Rushers',  'score' => :score, 'types' => ['DL','LB']     },
  'interceptors'  => {  'class' => Interceptors, 'label' => 'Interceptions', 'score' => :score, 'types' => ['LB','CB','S'] },
  'kickers'       => {  'class' => Kickers,      'label' => 'Kickers',       'score' => :score, 'types' => ['K']           },
  'punters'       => {  'class' => Punters,      'label' => 'Punters',       'score' => :score, 'types' => ['P']           }
}




path = ARGV[0] || '.'

extract_data = ProgRunner.new "#{location}/../C", "extract_data"

extract_data.execute "#{path}/ncfo1.nes", "#{path}/ncfo2.nes"

org = JSON.parse extract_data.get_output, {:symbolize_names => true}

sr = PlayerRankings.new org

sr.process_categories @categories
