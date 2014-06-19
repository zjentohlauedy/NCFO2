require 'persistable'

class TeamKickingStats < Persistable
  attr_accessor :team_id, :season, :bowl_game, :extra_point_attempts, :extra_points_made, :field_goal_attempts, :field_goals_made, :punts, :punt_yards, :kick_returns, :kick_return_yards, :kick_return_touchdowns, :punt_returns, :punt_return_yards, :punt_return_touchdowns

  @team_id
  @season
  @bowl_game
  @extra_point_attempts
  @extra_points_made
  @field_goal_attempts
  @field_goals_made
  @punts
  @punt_yards
  @kick_returns
  @kick_return_yards
  @kick_return_touchdowns
  @punt_returns
  @punt_return_yards
  @punt_return_touchdowns

  def initialize id = nil, season = nil, bowl_game = nil
    @TableName  = "Team_Kicking_Stats_T"
    @FieldNames = %w(Team_Id Season Bowl_Game Extra_Point_Attempts Extra_Points_Made Field_Goal_Attempts Field_Goals_Made Punts Punt_Yards Kick_Returns Kick_Return_Yards Kick_Return_Touchdowns Punt_Returns Punt_Return_Yards Punt_Return_Touchdowns)
    @KeyFields  = %w(Team_Id Season Bowl_Game)

    @team_id   = id
    @season    = season
    @bowl_game = bowl_game
  end

  def build_select_by_team
    custom_select @TableName, @FieldNames, %w(Team_Id)
  end

  def to_hash
    {
      team_id:                @team_id,
      season:                 @season,
      bowl_game:              @bowl_game,
      extra_point_attempts:   @extra_point_attempts,
      extra_points_made:      @extra_points_made,
      field_goal_attempts:    @field_goal_attempts,
      field_goals_made:       @field_goals_made,
      punts:                  @punts,
      punt_yards:             @punt_yards,
      kick_returns:           @kick_returns,
      kick_return_yards:      @kick_return_yards,
      kick_return_touchdowns: @kick_return_touchdowns,
      punt_returns:           @punt_returns,
      punt_return_yards:      @punt_return_yards,
      punt_return_touchdowns: @punt_return_touchdowns
    }
  end

  def key_hash
    { team_id: @team_id, season: @season, bowl_game: @bowl_game }
  end

  def from_hash hash
    @team_id                = hash[ :team_id                ]
    @season                 = hash[ :season                 ]
    @bowl_game              = hash[ :bowl_game              ]
    @extra_point_attempts   = hash[ :extra_point_attempts   ]
    @extra_points_made      = hash[ :extra_points_made      ]
    @field_goal_attempts    = hash[ :field_goal_attempts    ]
    @field_goals_made       = hash[ :field_goals_made       ]
    @punts                  = hash[ :punts                  ]
    @punt_yards             = hash[ :punt_yards             ]
    @kick_returns           = hash[ :kick_returns           ]
    @kick_return_yards      = hash[ :kick_return_yards      ]
    @kick_return_touchdowns = hash[ :kick_return_touchdowns ]
    @punt_returns           = hash[ :punt_returns           ]
    @punt_return_yards      = hash[ :punt_return_yards      ]
    @punt_return_touchdowns = hash[ :punt_return_touchdowns ]
  end

end
