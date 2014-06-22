require 'persistable'

class PlayerKickingRatings < Persistable
  attr_accessor :player_id, :kicking_ability, :avoid_kick_block

  @player_id
  @kicking_ability
  @avoid_kick_block

  def self.build id
    player_kicking_ratings = PlayerKickingRatings.new
    prng                   = Random.new

    player_kicking_ratings.player_id        = id
    player_kicking_ratings.kicking_ability  = prng.rand 16
    player_kicking_ratings.avoid_kick_block = prng.rand 16

    player_kicking_ratings
  end

  def initialize id = nil
    @TableName  = "Player_Kicking_Ratings_T"
    @FieldNames = %w(Player_Id Kicking_Ability Avoid_Kick_Block)
    @KeyFields  = %w(Player_Id)

    @player_id = id
  end

  def to_hash
    {
      player_id:        @player_id,
      kicking_ability:  @kicking_ability,
      avoid_kick_block: @avoid_kick_block
    }
  end

  def key_hash
    { player_id: @player_id }
  end

  def from_hash hash
    @player_id        = hash[ :player_id        ]
    @kicking_ability  = hash[ :kicking_ability  ]
    @avoid_kick_block = hash[ :avoid_kick_block ]
  end

end
