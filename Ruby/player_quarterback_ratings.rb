require 'persistable'

class PlayerQuarterbackRatings < Persistable
  attr_accessor :player_id, :pass_speed, :pass_control, :pass_accuracy, :avoid_pass_block

  @player_id
  @pass_speed
  @pass_control
  @pass_accuracy
  @avoid_pass_block

  def self.build id
    player_quarterback_ratings = PlayerQuarterbackRatings.new
    prng                       = Random.new

    player_quarterback_ratings.player_id        = id
    player_quarterback_ratings.pass_speed       = prng.rand 16
    player_quarterback_ratings.pass_control     = prng.rand 16
    player_quarterback_ratings.pass_accuracy    = prng.rand 16
    player_quarterback_ratings.avoid_pass_block = prng.rand 16

    player_quarterback_ratings
  end

  def initialize id = nil
    @TableName  = "Player_Quarterback_Ratings_T"
    @FieldNames = %w(Player_Id Pass_Speed Pass_Control Pass_Accuracy Avoid_Pass_Block)
    @KeyFields  = %w(Player_Id)

    @player_id = id
  end

  def to_hash
    {
      player_id:        @player_id,
      pass_speed:       @pass_speed,
      pass_control:     @pass_control,
      pass_accuracy:    @pass_accuracy,
      avoid_pass_block: @avoid_pass_block
    }
  end

  def key_hash
    { player_id: @player_id }
  end

  def from_hash hash
    @player_id        = hash[ :player_id        ]
    @pass_speed       = hash[ :pass_speed       ]
    @pass_control     = hash[ :pass_control     ]
    @pass_accuracy    = hash[ :pass_accuracy    ]
    @avoid_pass_block = hash[ :avoid_pass_block ]
  end

end
