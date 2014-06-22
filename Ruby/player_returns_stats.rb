require 'persistable'

class PlayerReturnsStats < Persistable
  attr_accessor :player_id, :season, :bowl_game, :kick_returns, :kick_return_yards, :kick_return_touchdowns, :punt_returns, :punt_return_yards, :punt_return_touchdowns

  @player_id
  @season
  @bowl_game
  @kick_returns
  @kick_return_yards
  @kick_return_touchdowns
  @punt_returns
  @punt_return_yards
  @punt_return_touchdowns

  def self.build id, season, bowl_game
    player_returns_stats = PlayerReturnsStats.new
    prng                 = Random.new

    player_returns_stats.player_id              = id
    player_returns_stats.season                 = season
    player_returns_stats.bowl_game              = bowl_game
    player_returns_stats.kick_returns           = prng.rand   50
    player_returns_stats.kick_return_yards      = prng.rand 1000
    player_returns_stats.kick_return_touchdowns = prng.rand   10
    player_returns_stats.punt_returns           = prng.rand   50
    player_returns_stats.punt_return_yards      = prng.rand  500
    player_returns_stats.punt_return_touchdowns = prng.rand   10

    player_returns_stats
  end

  def initialize id = nil, season = nil, bowl_game = nil
    @TableName  = "Player_Returns_Stats_T"
    @FieldNames = %w(Player_Id Season Bowl_Game Kick_Returns Kick_Return_Yards Kick_Return_Touchdowns Punt_Returns Punt_Return_Yards Punt_Return_Touchdowns)
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
      player_id:              @player_id,
      season:                 @season,
      bowl_game:              @bowl_game,
      kick_returns:           @kick_returns,
      kick_return_yards:      @kick_return_yards,
      kick_return_touchdowns: @kick_return_touchdowns,
      punt_returns:           @punt_returns,
      punt_return_yards:      @punt_return_yards,
      punt_return_touchdowns: @punt_return_touchdowns
    }
  end

  def key_hash
    { player_id: @player_id, season: @season, bowl_game: @bowl_game }
  end

  def from_hash hash
    @player_id              = hash[ :player_id              ]
    @season                 = hash[ :season                 ]
    @bowl_game              = hash[ :bowl_game              ]
    @kick_returns           = hash[ :kick_returns           ]
    @kick_return_yards      = hash[ :kick_return_yards      ]
    @kick_return_touchdowns = hash[ :kick_return_touchdowns ]
    @punt_returns           = hash[ :punt_returns           ]
    @punt_return_yards      = hash[ :punt_return_yards      ]
    @punt_return_touchdowns = hash[ :punt_return_touchdowns ]
  end

end
