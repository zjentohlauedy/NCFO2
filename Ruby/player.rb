require 'persistable'

class Player < Persistable
  attr_accessor :player_id, :first_name, :last_name, :face, :position, :number, :freshman_season, :maturity
  attr_accessor :accolades, :ratings, :offense_ratings, :defense_ratings, :quarterback_ratings, :kicking_ratings, :offense_stats, :defense_stats, :kicking_stats, :returns_stats

  @player_id
  @first_name
  @last_name
  @face
  @position
  @number
  @freshman_season
  @maturity

  @accolades
  @ratings
  @quarterback_ratings
  @offense_ratings
  @defense_ratings
  @kicking_ratings
  @offense_stats
  @defense_stats
  @kicking_stats
  @returns_stats

  def self.build id
    player = Player.new
    prng   = Random.new

    player.player_id       = id
    player.first_name      = "Firstname#{id}"
    player.last_name       = "Lastname#{id}"
    player.face            = prng.rand 256
    player.position        = prng.rand  11
    player.number          = prng.rand 100
    player.freshman_season = prng.rand  10
    player.maturity        = prng.rand   4

    player
  end

  def initialize id = nil
    @TableName  = "Players_T"
    @FieldNames = %w(Player_Id First_Name Last_Name Face Position Number Freshman_Season Maturity)
    @KeyFields  = %w(Player_Id)

    @player_id = id
  end

  def to_hash
    {
      player_id:       @player_id,
      first_name:      @first_name,
      last_name:       @last_name,
      face:            @face,
      position:        @position,
      number:          @number,
      freshman_season: @freshman_season,
      maturity:        @maturity
    }
  end

  def key_hash
    { player_id: @player_id }
  end

  def from_hash hash
    @player_id       = hash[ :player_id       ]
    @first_name      = hash[ :first_name      ]
    @last_name       = hash[ :last_name       ]
    @face            = hash[ :face            ]
    @position        = hash[ :position        ]
    @number          = hash[ :number          ]
    @freshman_season = hash[ :freshman_season ]
    @maturity        = hash[ :maturity        ]
  end

end
