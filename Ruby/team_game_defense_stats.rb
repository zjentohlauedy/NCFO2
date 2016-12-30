require 'persistable'

class TeamGameDefenseStats < Persistable
  attr_accessor :team_id, :season, :week, :game, :sacks, :interceptions, :return_yards, :return_touchdowns

  @team_id
  @season
  @week
  @game
  @sacks
  @interceptions
  @return_yards
  @return_touchdowns

  def self.build id, season, week, game
    team_game_defense_stats = TeamGameDefenseStats.new
    prng                    = Random.new

    team_game_defense_stats.team_id           = id
    team_game_defense_stats.season            = season
    team_game_defense_stats.week              = week
    team_game_defense_stats.game              = game
    team_game_defense_stats.sacks             = prng.rand  20
    team_game_defense_stats.interceptions     = prng.rand  10
    team_game_defense_stats.return_yards      = prng.rand 500
    team_game_defense_stats.return_touchdowns = prng.rand  10

    team_game_defense_stats
  end

  def initialize id = nil, season = nil, week = nil, game = nil
    @TableName  = "Team_Game_Defense_Stats_T"
    @FieldNames = %w(Team_Id Season Week Game Sacks Interceptions Return_Yards Return_Touchdowns)
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
      team_id:           @team_id,
      season:            @season,
      week:              @week,
      game:              @game,
      sacks:             @sacks,
      interceptions:     @interceptions,
      return_yards:      @return_yards,
      return_touchdowns: @return_touchdowns
    }
  end

  def key_hash
    { team_id: @team_id, season: @season, week: @week, game: @game }
  end

  def from_hash hash
    @team_id           = hash[ :team_id           ]
    @season            = hash[ :season            ]
    @week              = hash[ :week              ]
    @game              = hash[ :game              ]
    @sacks             = hash[ :sacks             ]
    @interceptions     = hash[ :interceptions     ]
    @return_yards      = hash[ :return_yards      ]
    @return_touchdowns = hash[ :return_touchdowns ]
  end

end
