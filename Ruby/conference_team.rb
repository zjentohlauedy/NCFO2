require 'persistable'

class ConferenceTeam < Persistable
  attr_accessor :conference_id, :team_id

  @conference_id
  @team_id

  def initialize conference_id = nil, team_id = nil
    @TableName  = 'Conference_Teams_T'
    @FieldNames = %w(Conference_Id Team_Id)
    @KeyFields  = %w(Conference_Id Team_Id)

    @conference_id = conference_id
    @team_id       = team_id
  end

  def build_select_by_conference
    custom_select @TableName, @FieldNames, %w(Conference_Id)
  end

  def to_hash
    { conference_id: @conference_id, team_id: @team_id }
  end

  def key_hash
    { conference_id: @conference_id, team_id: @team_id }
  end

  def from_hash hash
    @conference_id = hash[ :conference_id ]
    @team_id       = hash[ :team_id       ]
  end

end
