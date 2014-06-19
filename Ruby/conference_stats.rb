require 'persistable'

class ConferenceStats < Persistable
  attr_accessor :conference_id, :season, :bowl_game, :wins, :losses, :home_wins, :home_losses, :road_wins, :road_losses, :points_scored, :points_allowed

  @conference_id
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
    @TableName  = "Conference_Stats_T"
    @FieldNames = %w(Conference_Id Season Bowl_Game Wins Losses Home_Wins Home_Losses Road_Wins Road_Losses Points_Scored Points_Allowed)
    @KeyFields  = %w(Conference_Id Season Bowl_Game)

    @conference_id = id
    @season        = season
    @bowl_game     = bowl_game
  end

  def build_select_by_conference
    custom_select @TableName, @FieldNames, %w(Conference_Id)
  end

  def to_hash
    {
      conference_id:  @conference_id,
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
    { conference_id: @conference_id, season: @season, bowl_game: @bowl_game }
  end

  def from_hash hash
    @conference_id   = hash[ :conference_id  ]
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
