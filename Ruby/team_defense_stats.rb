require 'persistable'

class TeamDefenseStats < Persistable
  attr_accessor :team_id, :season, :bowl_game, :sacks, :interceptions, :return_yards, :return_touchdowns

  @team_id
  @season
  @bowl_game
  @sacks
  @interceptions
  @return_yards
  @return_touchdowns

  def initialize id = nil, season = nil, bowl_game = nil
    @TableName  = "Team_Defense_Stats_T"
    @FieldNames = %w(Team_Id Season Bowl_Game Sacks Interceptions Return_Yards Return_Touchdowns)
    @KeyFields  = %w(Team_Id Season Bowl_Game)

    @team_id   = id
    @season    = season
    @bowl_game = bowl_game
  end

  def to_hash
    {
      team_id:           @team_id,
      season:            @season,
      bowl_game:         @bowl_game,
      sacks:             @sacks,
      interceptions:     @interceptions,
      return_yards:      @return_yards,
      return_touchdowns: @return_touchdowns
    }
  end

  def key_hash
    { team_id: @team_id, season: @season, bowl_game: @bowl_game }
  end

  def from_hash hash
    @team_id           = hash[ :team_id           ]
    @season            = hash[ :season            ]
    @bowl_game         = hash[ :bowl_game         ]
    @sacks             = hash[ :sacks             ]
    @interceptions     = hash[ :interceptions     ]
    @return_yards      = hash[ :return_yards      ]
    @return_touchdowns = hash[ :return_touchdowns ]
  end

end
