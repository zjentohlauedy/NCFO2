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

  def self.build id, season, bowl_game
    team_defense_stats = TeamDefenseStats.new
    prng               = Random.new

    team_defense_stats.team_id           = id
    team_defense_stats.season            = season
    team_defense_stats.bowl_game         = bowl_game
    team_defense_stats.sacks             = prng.rand  20
    team_defense_stats.interceptions     = prng.rand  10
    team_defense_stats.return_yards      = prng.rand 500
    team_defense_stats.return_touchdowns = prng.rand  10

    team_defense_stats
  end

  def initialize id = nil, season = nil, bowl_game = nil
    @TableName  = "Team_Defense_Stats_T"
    @FieldNames = %w(Team_Id Season Bowl_Game Sacks Interceptions Return_Yards Return_Touchdowns)
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
