require 'persistable'

class TeamStats < Persistable
  attr_accessor :team_id, :season, :bowl_game, :wins, :losses, :home_wins, :home_losses, :road_wins, :road_losses, :points_scored, :points_allowed

  @team_id
  @season
  @bowl_game
  @wins
  @losses
  @home_wins
  @home_losses
  @road_wins
  @road_losses
  @points_scored
  @points_allowed

  def initialize id = nil, season = nil, bowl_game = nil
    @TableName  = "Team_Stats_T"
    @FieldNames = %w(Team_Id Season Bowl_Game Wins Losses Home_Wins Home_Losses Road_Wins Road_Losses Points_Scored Points_Allowed)
    @KeyFields  = %w(Team_Id Season Bowl_Game)

    @team_id   = id
    @season    = season
    @bowl_game = bowl_game
  end

  def to_hash
    {
      team_id:        @team_id,
      season:         @season,
      bowl_game:      @bowl_game,
      wins:           @wins,
      losses:         @losses,
      home_wins:      @home_wins,
      home_losses:    @home_losses,
      road_wins:      @road_wins,
      road_losses:    @road_losses,
      points_scored:  @points_scored,
      points_allowed: @points_allowed
    }
  end

  def key_hash
    { team_id: @team_id, season: @season, bowl_game: @bowl_game }
  end

  def from_hash hash
    @team_id         = hash[ :team_id        ]
    @season          = hash[ :season         ]
    @bowl_game       = hash[ :bowl_game      ]
    @wins            = hash[ :wins           ]
    @losses          = hash[ :losses         ]
    @home_wins       = hash[ :home_wins      ]
    @home_losses     = hash[ :home_losses    ]
    @road_wins       = hash[ :road_wins      ]
    @road_losses     = hash[ :road_losses    ]
    @points_scored   = hash[ :points_scored  ]
    @points_allowed  = hash[ :points_allowed ]
  end

end
