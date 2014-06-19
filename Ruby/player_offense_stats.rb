require 'persistable'

class PlayerOffenseStats < Persistable
  attr_accessor :player_id, :season, :bowl_game, :pass_attempts, :completions, :interceptions, :pass_yards, :pass_touchdowns, :rush_attempts, :rush_yards, :rush_touchdowns, :receptions, :receiving_yards, :receiving_touchdowns

  @player_id
  @season
  @bowl_game
  @pass_attempts
  @completions
  @interceptions
  @pass_yards
  @pass_touchdowns
  @rush_attempts
  @rush_yards
  @rush_touchdowns
  @receptions
  @receiving_yards
  @receiving_touchdowns

  def initialize id = nil, season = nil, bowl_game = nil
    @TableName  = "Player_Offense_Stats_T"
    @FieldNames = %w(Player_Id Season Bowl_Game Pass_Attempts Completions Interceptions Pass_Yards Pass_Touchdowns Rush_Attempts Rush_Yards Rush_Touchdowns Receptions Receiving_Yards Receiving_Touchdowns)
    @KeyFields  = %w(Player_Id Season Bowl_Game)

    @player_id = id
    @season    = season
    @bowl_game = bowl_game
  end

  def build_select_by_player
    custom_select @TableName, @FieldNames, %w(Player_Id)
  end

  def to_hash
    {
      player_id:            @player_id,
      season:               @season,
      bowl_game:            @bowl_game,
      pass_attempts:        @pass_attempts,
      completions:          @completions,
      interceptions:        @interceptions,
      pass_yards:           @pass_yards,
      pass_touchdowns:      @pass_touchdowns,
      rush_attempts:        @rush_attempts,
      rush_yards:           @rush_yards,
      rush_touchdowns:      @rush_touchdowns,
      receptions:           @receptions,
      receiving_yards:      @receiving_yards,
      receiving_touchdowns: @receiving_touchdowns
    }
  end

  def key_hash
    { player_id: @player_id, season: @season, bowl_game: @bowl_game }
  end

  def from_hash hash
    @player_id            = hash[ :player_id            ]
    @season               = hash[ :season               ]
    @bowl_game            = hash[ :bowl_game            ]
    @pass_attempts        = hash[ :pass_attempts        ]
    @completions          = hash[ :completions          ]
    @interceptions        = hash[ :interceptions        ]
    @pass_yards           = hash[ :pass_yards           ]
    @pass_touchdowns      = hash[ :pass_touchdowns      ]
    @rush_attempts        = hash[ :rush_attempts        ]
    @rush_yards           = hash[ :rush_yards           ]
    @rush_touchdowns      = hash[ :rush_touchdowns      ]
    @receptions           = hash[ :receptions           ]
    @receiving_yards      = hash[ :receiving_yards      ]
    @receiving_touchdowns = hash[ :receiving_touchdowns ]
  end

end
