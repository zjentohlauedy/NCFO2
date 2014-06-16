require 'persistable'

class Organization < Persistable
  attr_accessor :organization_id, :name, :abbreviation, :season

  @organization_id
  @name
  @abbreviation
  @season

  def initialize id = nil
    @TableName  = "Organizations_T"
    @FieldNames = %w(Organization_Id Name Abbreviation Season)
    @KeyFields  = %w(Organization_Id)

    @organization_id = id
  end

  def to_hash
    { organization_id: @organization_id, name: @name, abbreviation: @abbreviation, season: @season }
  end

  def key_hash
    { organization_id: @organization_id }
  end

  def from_hash hash
    @organization_id = hash[ :organization_id ]
    @name            = hash[ :name            ]
    @abbreviation    = hash[ :abbreviation    ]
    @season          = hash[ :season          ]
  end
end
