class TeamService

  def initialize repository, player_service
    @repository     = repository
    @player_service = player_service
  end

  def save team
    save_accolades     team.accolades
    save_players       team.players
    save_team_stats    team.stats
    save_offense_stats team.offense_stats
    save_defense_stats team.defense_stats
    save_kicking_stats team.kicking_stats

    @repository.create team
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

  def save_players players
    return if players.nil?

    if players.is_a? Array
      players.each do |player|
        save_player player
      end
    else
      save_player players
    end
  end

  def save_player player
    return if player.nil?

    @repository.create player

    unless player.player.nil?
      @player_service.save player.player
    end
  end

  def save_team_stats team_stats
    return if team_stats.nil?

    if team_stats.is_a? Array
      team_stats.each do |stats|
        @repository.create stats
      end
    else
      @repository.create team_stats
    end
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
end
