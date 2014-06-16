require 'persistable'

class Team < Persistable
  attr_accessor :team_id, :name, :location, :abbreviation

  @team_id
  @name
  @location
  @abbreviation

  def initialize id = nil
    @TableName  = "Teams_T"
    @FieldNames = %w(Team_Id Name Location Abbreviation)
    @KeyFields  = %w(Team_Id)

    @team_id = id
  end

  def to_hash
    { team_id: @team_id, name: @name, location: @location, abbreviation: @abbreviation }
  end

  def key_hash
    { team_id: @team_id }
  end

  def from_hash hash
    @team_id      = hash[ :team_id      ]
    @name         = hash[ :name         ]
    @location     = hash[ :location     ]
    @abbreviation = hash[ :abbreviation ]
  end

end
