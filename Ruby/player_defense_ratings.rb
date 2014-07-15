require 'persistable'

class PlayerDefenseRatings < Persistable
  attr_accessor :player_id, :interceptions, :quickness

  @player_id
  @interceptions
  @quickness

  def self.build id
    player_defense_ratings = PlayerDefenseRatings.new
    prng                   = Random.new

    player_defense_ratings.player_id     = id
    player_defense_ratings.interceptions = prng.rand 16
    player_defense_ratings.quickness     = prng.rand 16

    player_defense_ratings
  end

  def self.generate id, position = nil
    player_defense_ratings = PlayerDefenseRatings.new


    interceptions_weights = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
    quickness_weights     = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]

    case position
      #            index         0    1     2      3      4      5      6      7      8      9     10     11    12   13   14   15
      #            value         6   13    19     25     31     38     44     50     56     63     69     75    81   88   94  100
    when nil
      #            pct         .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      interceptions_weights = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      quickness_weights     = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]

    when Positions::DefensiveLine
      #            pct         .01   .1     2     10     23     25     16     12      8      3      1    .05   .01 .004 .002 .001
      interceptions_weights = [ 10, 100, 2000, 10000, 23000, 25000, 16000, 12000,  8000,  3000,  1000,    50,   10,   4,   2,   1 ]
      #            pct         .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      quickness_weights     = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]

    when Positions::Linebacker
      #            pct         .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      interceptions_weights = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      #            pct         .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      quickness_weights     = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]

    when Positions::Cornerback
      #            pct        .001  .01    .1     .2     .5      3      8     13     18     20     20     12     5   .2  .05  .01
      interceptions_weights = [  1,  10,  100,   200,   500,  3000,  8000, 13000, 18000, 20000, 20000, 12000, 5000, 200,  50,  10 ]
      #            pct         .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      quickness_weights     = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]

    when Positions::Safety
      #            pct        .001  .01    .1     .2     .5      3      8     13     18     20     20     12     5   .2  .05  .01
      interceptions_weights = [  1,  10,  100,   200,   500,  3000,  8000, 13000, 18000, 20000, 20000, 12000, 5000, 200,  50,  10 ]
      #            pct         .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      quickness_weights     = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]

    end

    player_defense_ratings.player_id     = id
    player_defense_ratings.interceptions = Utils::roll interceptions_weights
    player_defense_ratings.quickness     = Utils::roll quickness_weights

    player_defense_ratings
  end

  def initialize id = nil
    @TableName  = "Player_Defense_Ratings_T"
    @FieldNames = %w(Player_Id Interceptions Quickness)
    @KeyFields  = %w(Player_Id)

    @player_id = id
  end

  def to_hash
    {
      player_id:     @player_id,
      interceptions: @interceptions,
      quickness:     @quickness
    }
  end

  def key_hash
    { player_id: @player_id }
  end

  def from_hash hash
    @player_id     = hash[ :player_id     ]
    @interceptions = hash[ :interceptions ]
    @quickness     = hash[ :quickness     ]
  end

end
