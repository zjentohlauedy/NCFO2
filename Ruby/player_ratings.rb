require 'persistable'

class PlayerRatings < Persistable
  attr_accessor :player_id, :run_speed, :rush_power, :max_speed, :hit_power

  @player_id
  @run_speed
  @rush_power
  @max_speed
  @hit_power

  def self.build id
    player_ratings = PlayerRatings.new
    prng           = Random.new

    player_ratings.player_id  = id
    player_ratings.run_speed  = prng.rand 16
    player_ratings.rush_power = prng.rand 16
    player_ratings.max_speed  = prng.rand 16
    player_ratings.hit_power  = prng.rand 16

    player_ratings
  end

  def initialize id = nil
    @TableName  = "Player_Ratings_T"
    @FieldNames = %w(Player_Id Run_Speed Rush_Power Max_Speed Hit_Power)
    @KeyFields  = %w(Player_Id)

    @player_id = id
  end

  def to_hash
    {
      player_id:  @player_id,
      run_speed:  @run_speed,
      rush_power: @rush_power,
      max_speed:  @max_speed,
      hit_power:  @hit_power
    }
  end

  def key_hash
    { player_id: @player_id }
  end

  def from_hash hash
    @player_id  = hash[ :player_id  ]
    @run_speed  = hash[ :run_speed  ]
    @rush_power = hash[ :rush_power ]
    @max_speed  = hash[ :max_speed  ]
    @hit_power  = hash[ :hit_power  ]
  end

end
