require 'persistable'

class TeamPlayer < Persistable
  attr_accessor :team_id, :season, :player_id

  @team_id
  @season
  @player_id

  def initialize team_id = nil, season = nil, player_id = nil
    @TableName  = 'Team_Players_T'
    @FieldNames = %w(Team_Id Season Player_Id)
    @KeyFields  = %w(Team_Id Season Player_Id)

    @team_id   = team_id
    @season    = season
    @player_id = player_id
  end

  def build_select_by_team
    custom_select @TableName, @FieldNames, %w(Team_Id)
  end

  def to_hash
    { team_id: @team_id, season: @season, player_id: @player_id }
  end

  def key_hash
    { team_id: @team_id, season: @season, player_id: @player_id }
  end

  def from_hash hash
    @team_id   = hash[ :team_id   ]
    @season    = hash[ :season    ]
    @player_id = hash[ :player_id ]
  end

end