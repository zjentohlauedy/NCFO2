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

  def self.generate id, position = nil
    player_offense_ratings = PlayerOffenseRatings.new

    case position
    #                index       0    1     2      3      4      5      6      7      8      9     10     11    12   13   14   15
    #                value       6   13    19     25     31     38     44     50     56     63     69     75    81   88   94  100
    when nil
      #              pct       .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      receiving_weights    = [  20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      #              pct       .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      ball_control_weights = [  20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]

    when Positions::Runningback
      #              pct       .01   .1     2     10     23     25     16     12      8      3      1    .05   .01 .004 .002 .001
      receiving_weights    = [  10, 100, 2000, 10000, 23000, 25000, 16000, 12000,  8000,  3000,  1000,    50,   10,   4,   2,   1 ]
      #              pct       .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      ball_control_weights = [  20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]

    when Positions::WideReceiver
      #              pct      .001  .01    .1     .2     .5      3      8     13     18     20     20     12     5   .2  .05  .01
      receiving_weights    = [   1,  10,  100,   200,   500,  3000,  8000, 13000, 18000, 20000, 20000, 12000, 5000, 200,  50,  10 ]
      #              pct       .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      ball_control_weights = [  20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]

    when Positions::TightEnd
      #              pct       .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      receiving_weights    = [  20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      #              pct       .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      ball_control_weights = [  20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]

    end

    player_offense_ratings.player_id    = id
    player_offense_ratings.ball_control = Utils::roll ball_control_weights
    player_offense_ratings.receiving    = Utils::roll receiving_weights

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
