require 'persistable'

class TeamAccolade < Persistable
  attr_accessor :team_id, :season, :accolade

  @team_id
  @season
  @accolade

  def initialize team_id = nil, season = nil, accolade = nil
    @TableName  = 'Team_Accolades_T'
    @FieldNames = %w(Team_Id Season Accolade)
    @KeyFields  = %w(Team_Id Season Accolade)

    @team_id  = team_id
    @season   = season
    @accolade = accolade
  end

  def build_select_by_team
    custom_select @TableName, @FieldNames, %w(Team_Id)
  end

  def to_hash
    { team_id: @team_id, season: @season, accolade: @accolade }
  end

  def key_hash
    { team_id: @team_id,  season: @season, accolade: @accolade }
  end

  def from_hash hash
    @team_id  = hash[ :team_id  ]
    @season   = hash[ :season   ]
    @accolade = hash[ :accolade ]
  end

end
