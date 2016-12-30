require 'persistable'

class PlayerGameReturnsStats < Persistable
  attr_accessor :player_id, :season, :week, :game, :kick_returns, :kick_return_yards, :kick_return_touchdowns, :punt_returns, :punt_return_yards, :punt_return_touchdowns

  @player_id
  @season
  @week
  @game
  @kick_returns
  @kick_return_yards
  @kick_return_touchdowns
  @punt_returns
  @punt_return_yards
  @punt_return_touchdowns

  def self.build id, season, week, game
    player_game_returns_stats = PlayerGameReturnsStats.new
    prng                      = Random.new

    player_game_returns_stats.player_id              = id
    player_game_returns_stats.season                 = season
    player_game_returns_stats.week                   = week
    player_game_returns_stats.game                   = game
    player_game_returns_stats.kick_returns           = prng.rand   50
    player_game_returns_stats.kick_return_yards      = prng.rand 1000
    player_game_returns_stats.kick_return_touchdowns = prng.rand   10
    player_game_returns_stats.punt_returns           = prng.rand   50
    player_game_returns_stats.punt_return_yards      = prng.rand  500
    player_game_returns_stats.punt_return_touchdowns = prng.rand   10

    player_game_returns_stats
  end

  def initialize id = nil, season = nil, week = nil, game = nil
    @TableName  = "Player_Game_Returns_Stats_T"
    @FieldNames = %w(Player_Id Season Week Game Kick_Returns Kick_Return_Yards Kick_Return_Touchdowns Punt_Returns Punt_Return_Yards Punt_Return_Touchdowns)
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
      player_id:              @player_id,
      season:                 @season,
      week:                   @week,
      game:                   @game,
      kick_returns:           @kick_returns,
      kick_return_yards:      @kick_return_yards,
      kick_return_touchdowns: @kick_return_touchdowns,
      punt_returns:           @punt_returns,
      punt_return_yards:      @punt_return_yards,
      punt_return_touchdowns: @punt_return_touchdowns
    }
  end

  def key_hash
    { player_id: @player_id, season: @season, week: @week, game: @game }
  end

  def from_hash hash
    @player_id              = hash[ :player_id              ]
    @season                 = hash[ :season                 ]
    @week                   = hash[ :week                   ]
    @game                   = hash[ :game                   ]
    @kick_returns           = hash[ :kick_returns           ]
    @kick_return_yards      = hash[ :kick_return_yards      ]
    @kick_return_touchdowns = hash[ :kick_return_touchdowns ]
    @punt_returns           = hash[ :punt_returns           ]
    @punt_return_yards      = hash[ :punt_return_yards      ]
    @punt_return_touchdowns = hash[ :punt_return_touchdowns ]
  end

end
