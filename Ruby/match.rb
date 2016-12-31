require 'persistable'

class Match < Persistable
  attr_accessor :season, :week, :game, :road_team_id, :home_team_id

  @season
  @week
  @game
  @road_team_id
  @home_team_id

  def self.build season, week, game
    match = Match.new
    prng  = Random.new

    match.season       = season
    match.week         = week
    match.game         = game
    match.road_team_id = prng.rand 48
    match.home_team_id = prng.rand 48

    match
  end

  def initialize season = nil, week = nil, game = nil
    @TableName  = "Matches_T"
    @FieldNames = %w(Season Week Game Road_Team_Id Home_Team_Id)
    @KeyFields  = %w(Season Week Game)

    @season  = season
    @week    = week
    @game    = game
  end

  def to_hash
    {
      season:       @season,
      week:         @week,
      game:         @game,
      road_team_id: @road_team_id,
      home_team_id: @home_team_id
    }
  end

  def key_hash
    { season: @season, week: @week, game: @game }
  end

  def from_hash hash
    @season       = hash[ :season       ]
    @week         = hash[ :week         ]
    @game         = hash[ :game         ]
    @road_team_id = hash[ :road_team_id ]
    @home_team_id = hash[ :home_team_id ]
  end

end
