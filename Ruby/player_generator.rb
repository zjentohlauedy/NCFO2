require 'player'
require 'player_ratings'
require 'player_quarterback_ratings'
require 'player_offense_ratings'
require 'player_defense_ratings'
require 'player_kicking_ratings'

class PlayerGenerator

  def initialize( name_manager )
    @name_manager = name_manager

    @OffensePositions = [ Positions::Runningback,   Positions::WideReceiver, Positions::TightEnd    ]
    @DefensePositions = [ Positions::DefensiveLine, Positions::Linebacker,   Positions::Cornerback, Positions::Safety ]
    @KickingPositions = [ Positions::Kicker,        Positions::Punter        ]
  end

  def generate_player id, position, number, season
    prng   = Random.new
    player = Player.new id

    face = prng.rand 166

    name = @name_manager.get_name

    player.position        = position
    player.number          = number
    player.freshman_season = season
    player.first_name      = name[:first]
    player.last_name       = name[:last ]
    player.face            = (face > 81) ? face + 46 : face
    player.maturity        = prng.rand( 4 ) + 1
    player.ratings         = PlayerRatings.generate id, position

    if position == Positions::Quarterback
      player.quarterback_ratings = PlayerQuarterbackRatings.generate id
    elsif @OffensePositions.include? position
      player.offense_ratings = PlayerOffenseRatings.generate id, position
    elsif @DefensePositions.include? position
      player.defense_ratings = PlayerDefenseRatings.generate id, position
    elsif @KickingPositions.include? position
      player.kicking_ratings = PlayerKickingRatings.generate id
    end

    player
  end

  def choose_number position, players
    number = get_number position

    while ! is_number_unique? number, players
      number = get_number position
    end

    number
  end

  private
  def get_number position
    prng = Random.new

    case position
    when Positions::Quarterback;   return prng.rand( 19 ) +  1
    when Positions::Runningback;   return prng.rand( 10 ) + 30
    when Positions::WideReceiver;  return prng.rand( 10 ) + 80
    when Positions::TightEnd;      return prng.rand( 10 ) + 80
    when Positions::OffensiveLine; return prng.rand( 20 ) + 60
    when Positions::DefensiveLine; return prng.rand( 10 ) + 90
    when Positions::Linebacker;    return prng.rand( 10 ) + 50
    when Positions::Cornerback;    return prng.rand( 10 ) + 20
    when Positions::Safety;        return prng.rand( 10 ) + 40
    when Positions::Kicker;        return prng.rand( 19 ) +  1
    when Positions::Punter;        return prng.rand( 19 ) +  1
    end
  end

  def is_number_unique? number, players
    players.each do |player|
      if player.player.number == number
        return false
      end
    end

    return true
  end

end
