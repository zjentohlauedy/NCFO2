require 'persistable'

class PlayerGameOffenseStats < Persistable
  attr_accessor :player_id, :season, :week, :game, :pass_attempts, :completions, :interceptions, :pass_yards, :pass_touchdowns, :rush_attempts, :rush_yards, :rush_touchdowns, :receptions, :receiving_yards, :receiving_touchdowns

  @player_id
  @season
  @week
  @game
  @pass_attempts
  @completions
  @interceptions
  @pass_yards
  @pass_touchdowns
  @rush_attempts
  @rush_yards
  @rush_touchdowns
  @receptions
  @receiving_yards
  @receiving_touchdowns

  def self.build id, season, week, game
    player_game_offense_stats = PlayerGameOffenseStats.new
    prng                      = Random.new

    player_game_offense_stats.player_id            = id
    player_game_offense_stats.season               = season
    player_game_offense_stats.week                 = week
    player_game_offense_stats.game                 = game
    player_game_offense_stats.pass_attempts        = prng.rand  200
    player_game_offense_stats.completions          = prng.rand  150
    player_game_offense_stats.interceptions        = prng.rand  150
    player_game_offense_stats.pass_yards           = prng.rand 3000
    player_game_offense_stats.pass_touchdowns      = prng.rand   50
    player_game_offense_stats.rush_attempts        = prng.rand  200
    player_game_offense_stats.rush_yards           = prng.rand 1000
    player_game_offense_stats.rush_touchdowns      = prng.rand   20
    player_game_offense_stats.receptions           = prng.rand  100
    player_game_offense_stats.receiving_yards      = prng.rand 1000
    player_game_offense_stats.receiving_touchdowns = prng.rand   20

    player_game_offense_stats
  end

  def initialize id = nil, season = nil, week = nil, game = nil
    @TableName  = "Player_Game_Offense_Stats_T"
    @FieldNames = %w(Player_Id Season Week Game Pass_Attempts Completions Interceptions Pass_Yards Pass_Touchdowns Rush_Attempts Rush_Yards Rush_Touchdowns Receptions Receiving_Yards Receiving_Touchdowns)
    @KeyFields  = %w(Player_Id Season Week Game)

    @player_id = id
    @season    = season
    @week      = week
    @game      = game
  end

  def build_select_by_player
    custom_select @TableName, @FieldNames, %w(Player_Id)
  end

  def to_hash
    {
      player_id:            @player_id,
      season:               @season,
      week:                 @week,
      game:                 @game,
      pass_attempts:        @pass_attempts,
      completions:          @completions,
      interceptions:        @interceptions,
      pass_yards:           @pass_yards,
      pass_touchdowns:      @pass_touchdowns,
      rush_attempts:        @rush_attempts,
      rush_yards:           @rush_yards,
      rush_touchdowns:      @rush_touchdowns,
      receptions:           @receptions,
      receiving_yards:      @receiving_yards,
      receiving_touchdowns: @receiving_touchdowns
    }
  end

  def key_hash
    { player_id: @player_id, season: @season, week: @week, game: @game }
  end

  def from_hash hash
    @player_id            = hash[ :player_id            ]
    @season               = hash[ :season               ]
    @week                 = hash[ :week                 ]
    @game                 = hash[ :game                 ]
    @pass_attempts        = hash[ :pass_attempts        ]
    @completions          = hash[ :completions          ]
    @interceptions        = hash[ :interceptions        ]
    @pass_yards           = hash[ :pass_yards           ]
    @pass_touchdowns      = hash[ :pass_touchdowns      ]
    @rush_attempts        = hash[ :rush_attempts        ]
    @rush_yards           = hash[ :rush_yards           ]
    @rush_touchdowns      = hash[ :rush_touchdowns      ]
    @receptions           = hash[ :receptions           ]
    @receiving_yards      = hash[ :receiving_yards      ]
    @receiving_touchdowns = hash[ :receiving_touchdowns ]
  end

end
