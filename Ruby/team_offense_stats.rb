require 'persistable'

class TeamOffenseStats < Persistable
  attr_accessor :team_id, :season, :bowl_game, :pass_attempts, :completions, :interceptions, :pass_yards, :pass_touchdowns, :rush_attempts, :rush_yards, :rush_touchdowns

  @team_id
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

  def initialize id = nil, season = nil, bowl_game = nil
    @TableName  = "Team_Offense_Stats_T"
    @FieldNames = %w(Team_Id Season Bowl_Game Pass_Attempts Completions Interceptions Pass_Yards Pass_Touchdowns Rush_Attempts Rush_Yards Rush_Touchdowns)
    @KeyFields  = %w(Team_Id Season Bowl_Game)

    @team_id   = id
    @season    = season
    @bowl_game = bowl_game
  end

  def to_hash
    {
      team_id:         @team_id,
      season:          @season,
      bowl_game:       @bowl_game,
      pass_attempts:   @pass_attempts,
      completions:     @completions,
      interceptions:   @interceptions,
      pass_yards:      @pass_yards,
      pass_touchdowns: @pass_touchdowns,
      rush_attempts:   @rush_attempts,
      rush_yards:      @rush_yards,
      rush_touchdowns: @rush_touchdowns
    }
  end

  def key_hash
    { team_id: @team_id, season: @season, bowl_game: @bowl_game }
  end

  def from_hash hash
    @team_id         = hash[ :team_id         ]
    @season          = hash[ :season          ]
    @bowl_game       = hash[ :bowl_game       ]
    @pass_attempts   = hash[ :pass_attempts   ]
    @completions     = hash[ :completions     ]
    @interceptions   = hash[ :interceptions   ]
    @pass_yards      = hash[ :pass_yards      ]
    @pass_touchdowns = hash[ :pass_touchdowns ]
    @rush_attempts   = hash[ :rush_attempts   ]
    @rush_yards      = hash[ :rush_yards      ]
    @rush_touchdowns = hash[ :rush_touchdowns ]
  end

end