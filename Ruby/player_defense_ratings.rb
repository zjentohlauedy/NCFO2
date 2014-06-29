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

  def self.generate id
    player_defense_ratings = PlayerDefenseRatings.new

    # value      6  13   19    25     31     38     44     50     56    63    69   75  81  88  94  100
    # pct      .02 .04   .9     6     10     18     20     20     15     7     3   .1 .04 .02 .01 .005
    weights = [ 20, 40, 900, 6000, 10000, 18000, 20000, 20000, 15000, 7000, 3000, 100, 40, 20, 10,   5 ]

    player_defense_ratings.player_id     = id
    player_defense_ratings.interceptions = Utils::roll weights
    player_defense_ratings.quickness     = Utils::roll weights

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
