require 'persistable'

class TeamGameKickingStats < Persistable
  attr_accessor :team_id, :season, :week, :game, :extra_point_attempts, :extra_points_made, :field_goal_attempts, :field_goals_made, :punts, :punt_yards, :kick_returns, :kick_return_yards, :kick_return_touchdowns, :punt_returns, :punt_return_yards, :punt_return_touchdowns

  @team_id
  @season
  @week
  @game
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

  def self.build id, season, week, game
    team_game_kicking_stats = TeamGameKickingStats.new
    prng                    = Random.new

    team_game_kicking_stats.team_id                = id
    team_game_kicking_stats.season                 = season
    team_game_kicking_stats.week                   = week
    team_game_kicking_stats.game                   = game
    team_game_kicking_stats.extra_point_attempts   = prng.rand   75
    team_game_kicking_stats.extra_points_made      = prng.rand   75
    team_game_kicking_stats.field_goal_attempts    = prng.rand   30
    team_game_kicking_stats.field_goals_made       = prng.rand   30
    team_game_kicking_stats.punts                  = prng.rand   20
    team_game_kicking_stats.punt_yards             = prng.rand  500
    team_game_kicking_stats.kick_returns           = prng.rand  100
    team_game_kicking_stats.kick_return_yards      = prng.rand 1000
    team_game_kicking_stats.kick_return_touchdowns = prng.rand   10
    team_game_kicking_stats.punt_returns           = prng.rand   20
    team_game_kicking_stats.punt_return_yards      = prng.rand  200
    team_game_kicking_stats.punt_return_touchdowns = prng.rand    5

    team_game_kicking_stats
  end

  def initialize id = nil, season = nil, week = nil, game = nil
    @TableName  = "Team_Game_Kicking_Stats_T"
    @FieldNames = %w(Team_Id Season Week Game Extra_Point_Attempts Extra_Points_Made Field_Goal_Attempts Field_Goals_Made Punts Punt_Yards Kick_Returns Kick_Return_Yards Kick_Return_Touchdowns Punt_Returns Punt_Return_Yards Punt_Return_Touchdowns)
    @KeyFields  = %w(Team_Id Season Week Game)

    @team_id = id
    @season  = season
    @week    = week
    @game    = game
  end

  def build_select_by_team
    custom_select @TableName, @FieldNames, %w(Team_Id)
  end

  def to_hash
    {
      team_id:                @team_id,
      season:                 @season,
      week:                   @week,
      game:                   @game,
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
    { team_id: @team_id, season: @season, week: @week, game: @game }
  end

  def from_hash hash
    @team_id                = hash[ :team_id                ]
    @season                 = hash[ :season                 ]
    @week                   = hash[ :week                   ]
    @game                   = hash[ :game                   ]
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
