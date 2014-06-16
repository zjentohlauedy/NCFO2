require 'persistable'

class ConferenceAccolade < Persistable
  attr_accessor :conference_id, :season, :accolade

  @conference_id
  @season
  @accolade

  def initialize conference_id = nil, season = nil, accolade = nil
    @TableName  = 'Conference_Accolades_T'
    @FieldNames = %w(Conference_Id Season Accolade)
    @KeyFields  = %w(Conference_Id Season Accolade)

    @conference_id = conference_id
    @season        = season
    @accolade      = accolade
  end

  def build_select_by_conference
    custom_select @TableName, @FieldNames, %w(Conference_Id)
  end

  def to_hash
    { conference_id: @conference_id, season: @season, accolade: @accolade }
  end

  def key_hash
    { conference_id: @conference_id,  season: @season, accolade: @accolade }
  end

  def from_hash hash
    @conference_id = hash[ :conference_id ]
    @season        = hash[ :season        ]
    @accolade      = hash[ :accolade      ]
  end

end
