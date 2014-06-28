require 'persistable'

class Conference < Persistable
  attr_accessor :conference_id, :name
  attr_accessor :accolades, :stats, :teams

  @conference_id
  @name

  @accolades
  @stats
  @teams

  def self.build id
    conference = Conference.new

    conference.conference_id = id
    conference.name          = "Confname#{id}"

    conference
  end

  def initialize id = nil
    @TableName  = "Conferences_T"
    @FieldNames = %w(Conference_Id Name)
    @KeyFields  = %w(Conference_Id)

    @conference_id = id
  end

  def to_hash
    { conference_id: @conference_id, name: @name }
  end

  def key_hash
    { conference_id: @conference_id }
  end

  def from_hash hash
    @conference_id = hash[ :conference_id ]
    @name          = hash[ :name          ]
  end

end
