class PlayerService

  def initialize repository
    @repository = repository
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
