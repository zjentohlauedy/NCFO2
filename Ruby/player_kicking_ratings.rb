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

  def self.generate id
    player_kicking_ratings = PlayerKickingRatings.new

    # value      6  13   19    25     31     38     44     50     56    63    69   75  81  88  94  100
    # pct      .02 .04   .9     6     10     18     20     20     15     7     3   .1 .04 .02 .01 .005
    weights = [ 20, 40, 900, 6000, 10000, 18000, 20000, 20000, 15000, 7000, 3000, 100, 40, 20, 10,   5 ]

    player_kicking_ratings.player_id        = id
    player_kicking_ratings.kicking_ability  = Utils::roll weights
    player_kicking_ratings.avoid_kick_block = Utils::roll weights

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
