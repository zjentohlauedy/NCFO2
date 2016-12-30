require 'persistable'

class PlayerGameDefenseStats < Persistable
  attr_accessor :player_id, :season, :week, :game, :sacks, :interceptions, :return_yards, :return_touchdowns

  @player_id
  @season
  @week
  @game
  @sacks
  @interceptions
  @return_yards
  @return_touchdowns

  def self.build id, season, week, game
    player_game_defense_stats = PlayerGameDefenseStats.new
    prng                      = Random.new

    player_game_defense_stats.player_id         = id
    player_game_defense_stats.season            = season
    player_game_defense_stats.week              = week
    player_game_defense_stats.game              = game
    player_game_defense_stats.sacks             = prng.rand  20
    player_game_defense_stats.interceptions     = prng.rand  10
    player_game_defense_stats.return_yards      = prng.rand 150
    player_game_defense_stats.return_touchdowns = prng.rand   5

    player_game_defense_stats
  end

  def initialize id = nil, season = nil, week = nil, game = nil
    @TableName  = "Player_Game_Defense_Stats_T"
    @FieldNames = %w(Player_Id Season Week Game Sacks Interceptions Return_Yards Return_Touchdowns)
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
      player_id:         @player_id,
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
    { player_id: @player_id, season: @season, week: @week, game: @game }
  end

  def from_hash hash
    @player_id         = hash[ :player_id         ]
    @season            = hash[ :season            ]
    @week              = hash[ :week              ]
    @game              = hash[ :game              ]
    @sacks             = hash[ :sacks             ]
    @interceptions     = hash[ :interceptions     ]
    @return_yards      = hash[ :return_yards      ]
    @return_touchdowns = hash[ :return_touchdowns ]
  end

end
