require 'persistable'

class PlayerOffenseRatings < Persistable
  attr_accessor :player_id, :ball_control, :receiving

  @player_id
  @ball_control
  @receiving

  def self.build id
    player_offense_ratings = PlayerOffenseRatings.new
    prng           = Random.new

    player_offense_ratings.player_id    = id
    player_offense_ratings.ball_control = prng.rand 16
    player_offense_ratings.receiving    = prng.rand 16

    player_offense_ratings
  end

  def self.generate id
    player_offense_ratings = PlayerOffenseRatings.new

    # value      6  13   19    25     31     38     44     50     56    63    69   75  81  88  94  100
    # pct      .02 .04   .9     6     10     18     20     20     15     7     3   .1 .04 .02 .01 .005
    weights = [ 20, 40, 900, 6000, 10000, 18000, 20000, 20000, 15000, 7000, 3000, 100, 40, 20, 10,   5 ]

    player_offense_ratings.player_id    = id
    player_offense_ratings.ball_control = Utils::roll weights
    player_offense_ratings.receiving    = Utils::roll weights

    player_offense_ratings
  end

  def initialize id = nil
    @TableName  = "Player_Offense_Ratings_T"
    @FieldNames = %w(Player_Id Ball_Control Receiving)
    @KeyFields  = %w(Player_Id)

    @player_id = id
  end

  def to_hash
    {
      player_id:    @player_id,
      ball_control: @ball_control,
      receiving:    @receiving
    }
  end

  def key_hash
    { player_id: @player_id }
  end

  def from_hash hash
    @player_id    = hash[ :player_id    ]
    @ball_control = hash[ :ball_control ]
    @receiving    = hash[ :receiving    ]
  end

end
