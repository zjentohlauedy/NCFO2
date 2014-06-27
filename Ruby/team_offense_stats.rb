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

  def self.build id, season, bowl_game
    team_offense_stats = TeamOffenseStats.new
    prng               = Random.new

    team_offense_stats.team_id         = id
    team_offense_stats.season          = season
    team_offense_stats.bowl_game       = bowl_game
    team_offense_stats.pass_attempts   = prng.rand  10
    team_offense_stats.completions     = prng.rand  10
    team_offense_stats.interceptions   = prng.rand   5
    team_offense_stats.pass_yards      = prng.rand   5
    team_offense_stats.pass_touchdowns = prng.rand   5
    team_offense_stats.rush_attempts   = prng.rand   5
    team_offense_stats.rush_yards      = prng.rand 500
    team_offense_stats.rush_touchdowns = prng.rand 500

    team_offense_stats
  end

  def initialize id = nil, season = nil, bowl_game = nil
    @TableName  = "Team_Offense_Stats_T"
    @FieldNames = %w(Team_Id Season Bowl_Game Pass_Attempts Completions Interceptions Pass_Yards Pass_Touchdowns Rush_Attempts Rush_Yards Rush_Touchdowns)
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
