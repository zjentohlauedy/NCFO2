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
