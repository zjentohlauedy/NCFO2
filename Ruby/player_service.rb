require 'positions'

class PlayerService

  def initialize repository
    @repository = repository

    @OffensePositions = [ Positions::Runningback,   Positions::WideReceiver, Positions::TightEnd    ]
    @DefensePositions = [ Positions::DefensiveLine, Positions::Linebacker,   Positions::Cornerback, Positions::Safety ]
    @KickingPositions = [ Positions::Kicker,        Positions::Punter        ]
  end

  def get player_id
    player = @repository.read Player.new player_id

    unless player.nil?
      get_accolades player
      get_ratings   player

      get_quarterback_ratings player unless player.position != Positions::Quarterback
      get_offense_ratings     player if     @OffensePositions.include? player.position
      get_defense_ratings     player if     @DefensePositions.include? player.position
      get_kicking_ratings     player if     @KickingPositions.include? player.position

      get_offense_stats player
      get_defense_stats player
      get_kicking_stats player
      get_returns_stats player
    end

    player
  end

  def get_accolades player
    player_accolade = PlayerAccolade.new

    results = @repository.custom_read player_accolade.build_select_by_player, player.player_id

    unless results.nil? or results.length == 0
      player.accolades = []

      results.each do |result|
        player_accolade.from_hash result

        player.accolades.push player_accolade

        player_accolade = PlayerAccolade.new
      end
    end
  end

  def get_ratings player
    player_ratings = PlayerRatings.new player.player_id

    player_ratings = @repository.read player_ratings

    player.ratings = player_ratings
  end

  def get_quarterback_ratings player
    player_quarterback_ratings = PlayerQuarterbackRatings.new player.player_id

    player_quarterback_ratings = @repository.read player_quarterback_ratings

    player.quarterback_ratings = player_quarterback_ratings
  end

  def get_offense_ratings player
    player_offense_ratings = PlayerOffenseRatings.new player.player_id

    player_offense_ratings = @repository.read player_offense_ratings

    player.offense_ratings = player_offense_ratings
  end

  def get_defense_ratings player
    player_defense_ratings = PlayerDefenseRatings.new player.player_id

    player_defense_ratings = @repository.read player_defense_ratings

    player.defense_ratings = player_defense_ratings
  end

  def get_kicking_ratings player
    player_kicking_ratings = PlayerKickingRatings.new player.player_id

    player_kicking_ratings = @repository.read player_kicking_ratings

    player.kicking_ratings = player_kicking_ratings
  end

  def get_offense_stats player
    player_offense_stats = PlayerOffenseStats.new

    results = @repository.custom_read player_offense_stats.build_select_by_player, player.player_id

    unless results.nil? or results.length == 0
      player.offense_stats = []

      results.each do |result|
        player_offense_stats.from_hash result

        player.offense_stats.push player_offense_stats

        player_offense_stats = PlayerOffenseStats.new
      end
    end
  end

  def get_defense_stats player
    player_defense_stats = PlayerDefenseStats.new

    results = @repository.custom_read player_defense_stats.build_select_by_player, player.player_id

    unless results.nil? or results.length == 0
      player.defense_stats = []

      results.each do |result|
        player_defense_stats.from_hash result

        player.defense_stats.push player_defense_stats

        player_defense_stats = PlayerDefenseStats.new
      end
    end
  end

  def get_kicking_stats player
    player_kicking_stats = PlayerKickingStats.new

    results = @repository.custom_read player_kicking_stats.build_select_by_player, player.player_id

    unless results.nil? or results.length == 0
      player.kicking_stats = []

      results.each do |result|
        player_kicking_stats.from_hash result

        player.kicking_stats.push player_kicking_stats

        player_kicking_stats = PlayerKickingStats.new
      end
    end
  end

  def get_returns_stats player
    player_returns_stats = PlayerReturnsStats.new

    results = @repository.custom_read player_returns_stats.build_select_by_player, player.player_id

    unless results.nil? or results.length == 0
      player.returns_stats = []

      results.each do |result|
        player_returns_stats.from_hash result

        player.returns_stats.push player_returns_stats

        player_returns_stats = PlayerReturnsStats.new
      end
    end
  end

  def save player
    save_accolades           player.accolades
    save_ratings             player.ratings
    save_quarterback_ratings player.quarterback_ratings
    save_offense_ratings     player.offense_ratings
    save_defense_ratings     player.defense_ratings
    save_kicking_ratings     player.kicking_ratings
    save_offense_stats       player.offense_stats
    save_defense_stats       player.defense_stats
    save_kicking_stats       player.kicking_stats
    save_returns_stats       player.returns_stats

    @repository.create player
  end

  def save_accolades accolades
    return if accolades.nil?

    if accolades.is_a? Array
      accolades.each do |accolade|
        @repository.create accolade
      end
    else
      @repository.create accolades
    end
  end

  def save_ratings ratings
    return if ratings.nil?

    @repository.create ratings
  end

  def save_quarterback_ratings ratings
    return if ratings.nil?

    @repository.create ratings
  end

  def save_offense_ratings ratings
    return if ratings.nil?

    @repository.create ratings
  end

  def save_defense_ratings ratings
    return if ratings.nil?

    @repository.create ratings
  end

  def save_kicking_ratings ratings
    return if ratings.nil?

    @repository.create ratings
  end

  def save_offense_stats offense_stats
    return if offense_stats.nil?

    if offense_stats.is_a? Array
      offense_stats.each do |stats|
        @repository.create stats
      end
    else
      @repository.create offense_stats
    end
  end

  def save_defense_stats defense_stats
    return if defense_stats.nil?

    if defense_stats.is_a? Array
      defense_stats.each do |stats|
        @repository.create stats
      end
    else
      @repository.create defense_stats
    end
  end

  def save_kicking_stats kicking_stats
    return if kicking_stats.nil?

    if kicking_stats.is_a? Array
      kicking_stats.each do |stats|
        @repository.create stats
      end
    else
      @repository.create kicking_stats
    end
  end

  def save_returns_stats returns_stats
    return if returns_stats.nil?

    if returns_stats.is_a? Array
      returns_stats.each do |stats|
        @repository.create stats
      end
    else
      @repository.create returns_stats
    end
  end
end
