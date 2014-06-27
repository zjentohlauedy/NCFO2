require 'persistable'

class Team < Persistable
  attr_accessor :team_id, :name, :location, :abbreviation
  attr_accessor :accolades, :players, :stats, :offense_stats, :defense_stats, :kicking_stats

  @team_id
  @name
  @location
  @abbreviation

  @accolades
  @players
  @stats
  @offense_stats
  @defense_stats
  @kicking_stats

  def self.build id
    team = Team.new

    team.team_id      = id
    team.name         = "Teamname#{id}"
    team.location     = "Location#{id}"
    team.abbreviation = "TN#{id}"

    team
  end

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
