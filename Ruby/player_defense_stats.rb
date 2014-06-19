require 'persistable'

class PlayerDefenseStats < Persistable
  attr_accessor :player_id, :season, :bowl_game, :sacks, :interceptions, :return_yards, :return_touchdowns

  @player_id
  @season
  @bowl_game
  @sacks
  @interceptions
  @return_yards
  @return_touchdowns

  def initialize id = nil, season = nil, bowl_game = nil
    @TableName  = "Player_Defense_Stats_T"
    @FieldNames = %w(Player_Id Season Bowl_Game Sacks Interceptions Return_Yards Return_Touchdowns)
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
      player_id:         @player_id,
      season:            @season,
      bowl_game:         @bowl_game,
      sacks:             @sacks,
      interceptions:     @interceptions,
      return_yards:      @return_yards,
      return_touchdowns: @return_touchdowns
    }
  end

  def key_hash
    { player_id: @player_id, season: @season, bowl_game: @bowl_game }
  end

  def from_hash hash
    @player_id         = hash[ :player_id         ]
    @season            = hash[ :season            ]
    @bowl_game         = hash[ :bowl_game         ]
    @sacks             = hash[ :sacks             ]
    @interceptions     = hash[ :interceptions     ]
    @return_yards      = hash[ :return_yards      ]
    @return_touchdowns = hash[ :return_touchdowns ]
  end

end
