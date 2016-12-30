require 'persistable'

class TeamGameStats < Persistable
  attr_accessor :team_id, :season, :week, :game, :wins, :losses, :ties, :home_wins, :home_losses, :home_ties, :road_wins, :road_losses, :road_ties, :points_scored, :points_allowed

  @team_id
  @season
  @week
  @game
  @wins
  @losses
  @ties
  @home_wins
  @home_losses
  @home_ties
  @road_wins
  @road_losses
  @road_ties
  @points_scored
  @points_allowed

  def self.build id, season, week, game
    team_game_stats = TeamGameStats.new
    prng            = Random.new

    team_game_stats.team_id        = id
    team_game_stats.season         = season
    team_game_stats.week           = week
    team_game_stats.game           = game
    team_game_stats.wins           = prng.rand  10
    team_game_stats.losses         = prng.rand  10
    team_game_stats.ties           = prng.rand  10
    team_game_stats.home_wins      = prng.rand   5
    team_game_stats.home_losses    = prng.rand   5
    team_game_stats.home_ties      = prng.rand   5
    team_game_stats.road_wins      = prng.rand   5
    team_game_stats.road_losses    = prng.rand   5
    team_game_stats.road_ties      = prng.rand   5
    team_game_stats.points_scored  = prng.rand 500
    team_game_stats.points_allowed = prng.rand 500

    team_game_stats
  end

  def initialize id = nil, season = nil, week = nil, game = nil
    @TableName  = "Team_Game_Stats_T"
    @FieldNames = %w(Team_Id Season Week Game Wins Losses Ties Home_Wins Home_Losses Home_Ties Road_Wins Road_Losses Road_Ties Points_Scored Points_Allowed)
    @KeyFields  = %w(Team_Id Season Week Game)

    @team_id   = id
    @season    = season
    @week      = week
    @game      = game
  end

  def build_select_by_team
    custom_select @TableName, @FieldNames, %w(Team_Id)
  end

  def to_hash
    {
      team_id:        @team_id,
      season:         @season,
      week:           @week,
      game:           @game,
      wins:           @wins,
      losses:         @losses,
      ties:           @ties,
      home_wins:      @home_wins,
      home_losses:    @home_losses,
      home_ties:      @home_ties,
      road_wins:      @road_wins,
      road_losses:    @road_losses,
      road_ties:      @road_ties,
      points_scored:  @points_scored,
      points_allowed: @points_allowed
    }
  end

  def key_hash
    { team_id: @team_id, season: @season, week: @week, game: @game }
  end

  def from_hash hash
    @team_id        = hash[ :team_id        ]
    @season         = hash[ :season         ]
    @week           = hash[ :week           ]
    @game           = hash[ :game           ]
    @wins           = hash[ :wins           ]
    @losses         = hash[ :losses         ]
    @ties           = hash[ :ties           ]
    @home_wins      = hash[ :home_wins      ]
    @home_losses    = hash[ :home_losses    ]
    @home_ties      = hash[ :home_ties      ]
    @road_wins      = hash[ :road_wins      ]
    @road_losses    = hash[ :road_losses    ]
    @road_ties      = hash[ :road_ties      ]
    @points_scored  = hash[ :points_scored  ]
    @points_allowed = hash[ :points_allowed ]
  end

end
