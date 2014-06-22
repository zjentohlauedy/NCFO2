require 'persistable'

class PlayerKickingStats < Persistable
  attr_accessor :player_id, :season, :bowl_game, :extra_point_attempts, :extra_points_made, :field_goal_attempts, :field_goals_made, :punts, :punt_yards

  @player_id
  @season
  @bowl_game
  @extra_point_attempts
  @extra_points_made
  @field_goal_attempts
  @field_goals_made
  @punts
  @punt_yards

  def self.build id, season, bowl_game
    player_kicking_stats = PlayerKickingStats.new
    prng                 = Random.new

    player_kicking_stats.player_id            = id
    player_kicking_stats.season               = season
    player_kicking_stats.bowl_game            = bowl_game
    player_kicking_stats.extra_point_attempts = prng.rand  20
    player_kicking_stats.extra_points_made    = prng.rand  20
    player_kicking_stats.field_goal_attempts  = prng.rand  20
    player_kicking_stats.field_goals_made     = prng.rand  20
    player_kicking_stats.punts                = prng.rand  50
    player_kicking_stats.punt_yards           = prng.rand 300

    player_kicking_stats
  end

  def initialize id = nil, season = nil, bowl_game = nil
    @TableName  = "Player_Kicking_Stats_T"
    @FieldNames = %w(Player_Id Season Bowl_Game Extra_Point_Attempts Extra_Points_Made Field_Goal_Attempts Field_Goals_Made Punts Punt_Yards)
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
      extra_point_attempts: @extra_point_attempts,
      extra_points_made:    @extra_points_made,
      field_goal_attempts:  @field_goal_attempts,
      field_goals_made:     @field_goals_made,
      punts:                @punts,
      punt_yards:           @punt_yards
    }
  end

  def key_hash
    { player_id: @player_id, season: @season, bowl_game: @bowl_game }
  end

  def from_hash hash
    @player_id            = hash[ :player_id            ]
    @season               = hash[ :season               ]
    @bowl_game            = hash[ :bowl_game            ]
    @extra_point_attempts = hash[ :extra_point_attempts ]
    @extra_points_made    = hash[ :extra_points_made    ]
    @field_goal_attempts  = hash[ :field_goal_attempts  ]
    @field_goals_made     = hash[ :field_goals_made     ]
    @punts                = hash[ :punts                ]
    @punt_yards           = hash[ :punt_yards           ]
  end

end
