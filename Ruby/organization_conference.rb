require 'persistable'

class OrganizationConference < Persistable
  attr_accessor :organization_id, :conference_id

  @organization_id
  @conference_id

  def initialize organization_id = nil, conference_id = nil
    @TableName  = 'Organization_Conferences_T'
    @FieldNames = %w(Organization_Id Conference_Id)
    @KeyFields  = %w(Organization_Id Conference_Id)

    @organization_id = organization_id
    @conference_id   = conference_id
  end

  def build_select_by_organization
    custom_select @TableName, @FieldNames, %w(Organization_Id)
  end

  def to_hash
    { organization_id: @organization_id, conference_id: @conference_id }
  end

  def key_hash
    { organization_id: @organization_id, conference_id: @conference_id }
  end

  def from_hash hash
    @organization_id = hash[ :organization_id ]
    @conference_id   = hash[ :conference_id   ]
  end

end
