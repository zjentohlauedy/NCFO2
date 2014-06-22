require 'persistable'

class PlayerAccolade < Persistable
  attr_accessor :player_id, :season, :accolade

  @player_id
  @season
  @accolade

  def self.build id, season, accolade
    player_accolade = PlayerAccolade.new

    player_accolade.player_id = id
    player_accolade.season    = season
    player_accolade.accolade  = accolade

    player_accolade
  end

  def initialize player_id = nil, season = nil, accolade = nil
    @TableName  = 'Player_Accolades_T'
    @FieldNames = %w(Player_Id Season Accolade)
    @KeyFields  = %w(Player_Id Season Accolade)

    @player_id = player_id
    @season    = season
    @accolade  = accolade
  end

  def build_select_by_player
    custom_select @TableName, @FieldNames, %w(Player_Id)
  end

  def to_hash
    { player_id: @player_id, season: @season, accolade: @accolade }
  end

  def key_hash
    { player_id: @player_id, season: @season, accolade: @accolade }
  end

  def from_hash hash
    @player_id = hash[ :player_id  ]
    @season    = hash[ :season   ]
    @accolade  = hash[ :accolade ]
  end

end
