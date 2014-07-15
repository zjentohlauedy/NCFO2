require 'persistable'
require 'utils'

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

  def self.generate id, position = nil
    player_ratings = PlayerRatings.new

    case position
      #            index      0    1     2      3      4      5      6      7      8      9     10     11    12   13   14   15
      #            value      6   13    19     25     31     38     44     50     56     63     69     75    81   88   94  100
    when nil
      #           pct       .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      run_speed_weights  = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      rush_power_weights = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      max_speed_weights  = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      hit_power_weights  = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]

    when Positions::None
      #           pct       .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      run_speed_weights  = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      rush_power_weights = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      max_speed_weights  = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      hit_power_weights  = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]

    when Positions::Quarterback
      #           pct       .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      run_speed_weights  = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      rush_power_weights = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      #           pct       .01   .1     2     10     23     25     16     12      8      3      1    .05   .01 .004 .002 .001
      max_speed_weights  = [ 10, 100, 2000, 10000, 23000, 25000, 16000, 12000,  8000,  3000,  1000,    50,   10,   4,   2,   1 ]
      #           pct        .1    1    15     27     25     15     10      5      2     .5     .1    .02  .008 .002 .001 .001
      hit_power_weights  = [100,1000,15000, 27000, 25000, 15000, 10000,  5000,  2000,   500,   100,    20,    8,   2,   1,   1 ]

    when Positions::Runningback
      #           pct       .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      run_speed_weights  = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      rush_power_weights = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      #           pct      .005  .05    .2     .5      3      7     12     18     24     18     10      6     1   .1  .01 .005
      max_speed_weights  = [  5,  50,  200,   500,  3000,  7000, 12000, 18000, 24000, 18000, 10000,  6000, 1000, 100,  10,   5 ]
      #           pct       .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      hit_power_weights  = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]

    when Positions::WideReceiver
      #           pct       .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      run_speed_weights  = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      rush_power_weights = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      #           pct      .001  .01    .1     .2     .5      3      8     13     18     20     20     12     5   .2  .05  .01
      max_speed_weights  = [  1,  10,  100,   200,   500,  3000,  8000, 13000, 18000, 20000, 20000, 12000, 5000, 200,  50,  10 ]
      #           pct       .01   .1     2     10     23     25     16     12      8      3      1    .05   .01 .004 .002 .001
      hit_power_weights  = [ 10, 100, 2000, 10000, 23000, 25000, 16000, 12000,  8000,  3000,  1000,    50,   10,   4,   2,   1 ]

    when Positions::TightEnd
      #           pct       .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      run_speed_weights  = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      rush_power_weights = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      #           pct       .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      max_speed_weights  = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      #           pct      .005  .05    .2     .5      3      7     12     18     24     18     10      6     1   .1  .01 .005
      hit_power_weights  = [  5,  50,  200,   500,  3000,  7000, 12000, 18000, 24000, 18000, 10000,  6000, 1000, 100,  10,   5 ]

    when Positions::OffensiveLine
      #           pct       .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      run_speed_weights  = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      rush_power_weights = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      #           pct        .1    1    15     27     25     15     10      5      2     .5     .1    .02  .008 .002 .001 .001
      max_speed_weights  = [100,1000,15000, 27000, 25000, 15000, 10000,  5000,  2000,   500,   100,    20,    8,   2,   1,   1 ]
      #           pct      .001  .01    .1     .2     .5      3      8     13     18     20     20     12     5   .2  .05  .01
      hit_power_weights  = [  1,  10,  100,   200,   500,  3000,  8000, 13000, 18000, 20000, 20000, 12000, 5000, 200,  50,  10 ]

    when Positions::DefensiveLine
      #           pct       .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      run_speed_weights  = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      rush_power_weights = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      #           pct        .1    1    15     27     25     15     10      5      2     .5     .1    .02  .008 .002 .001 .001
      max_speed_weights  = [100,1000,15000, 27000, 25000, 15000, 10000,  5000,  2000,   500,   100,    20,    8,   2,   1,   1 ]
      #           pct      .001  .01    .1     .2     .5      3      8     13     18     20     20     12     5   .2  .05  .01
      hit_power_weights  = [  1,  10,  100,   200,   500,  3000,  8000, 13000, 18000, 20000, 20000, 12000, 5000, 200,  50,  10 ]

    when Positions::Linebacker
      #           pct       .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      run_speed_weights  = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      rush_power_weights = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      #           pct       .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      max_speed_weights  = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      #           pct      .005  .05    .2     .5      3      7     12     18     24     18     10      6     1   .1  .01 .005
      hit_power_weights  = [  5,  50,  200,   500,  3000,  7000, 12000, 18000, 24000, 18000, 10000,  6000, 1000, 100,  10,   5 ]

    when Positions::Cornerback
      #           pct       .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      run_speed_weights  = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      rush_power_weights = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      #           pct      .001  .01    .1     .2     .5      3      8     13     18     20     20     12     5   .2  .05  .01
      max_speed_weights  = [  1,  10,  100,   200,   500,  3000,  8000, 13000, 18000, 20000, 20000, 12000, 5000, 200,  50,  10 ]
      #           pct       .01   .1     2     10     23     25     16     12      8      3      1    .05   .01 .004 .002 .001
      hit_power_weights  = [ 10, 100, 2000, 10000, 23000, 25000, 16000, 12000,  8000,  3000,  1000,    50,   10,   4,   2,   1 ]

    when Positions::Safety
      #           pct       .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      run_speed_weights  = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      rush_power_weights = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      #           pct      .005  .05    .2     .5      3      7     12     18     24     18     10      6     1   .1  .01 .005
      max_speed_weights  = [  5,  50,  200,   500,  3000,  7000, 12000, 18000, 24000, 18000, 10000,  6000, 1000, 100,  10,   5 ]
      #           pct       .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      hit_power_weights  = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]

    when Positions::Kicker
      #           pct       .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      run_speed_weights  = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      rush_power_weights = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      #           pct       .01   .1     2     10     23     25     16     12      8      3      1    .05   .01 .004 .002 .001
      max_speed_weights  = [ 10, 100, 2000, 10000, 23000, 25000, 16000, 12000,  8000,  3000,  1000,    50,   10,   4,   2,   1 ]
      #           pct        .1    1    15     27     25     15     10      5      2     .5     .1    .02  .008 .002 .001 .001
      hit_power_weights  = [100,1000,15000, 27000, 25000, 15000, 10000,  5000,  2000,   500,   100,    20,    8,   2,   1,   1 ]

    when Positions::Punter
      #           pct       .02  .04    .9      6     10     18     20     20     15      7      3     .1   .04  .02  .01 .005
      run_speed_weights  = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      rush_power_weights = [ 20,  40,  900,  6000, 10000, 18000, 20000, 20000, 15000,  7000,  3000,   100,   40,  20,  10,   5 ]
      #           pct       .01   .1     2     10     23     25     16     12      8      3      1    .05   .01 .004 .002 .001
      max_speed_weights  = [ 10, 100, 2000, 10000, 23000, 25000, 16000, 12000,  8000,  3000,  1000,    50,   10,   4,   2,   1 ]
      #           pct        .1    1    15     27     25     15     10      5      2     .5     .1    .02  .008 .002 .001 .001
      hit_power_weights  = [100,1000,15000, 27000, 25000, 15000, 10000,  5000,  2000,   500,   100,    20,    8,   2,   1,   1 ]

    end

    player_ratings.player_id  = id
    player_ratings.run_speed  = Utils::roll run_speed_weights
    player_ratings.rush_power = Utils::roll rush_power_weights
    player_ratings.max_speed  = Utils::roll max_speed_weights
    player_ratings.hit_power  = Utils::roll hit_power_weights

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
