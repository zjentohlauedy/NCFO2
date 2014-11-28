require 'team'
require 'team_accolade'
require 'team_accolades'
require 'team_player'
require 'team_stats'
require 'team_offense_stats'
require 'team_defense_stats'
require 'team_kicking_stats'

class TeamService

  def initialize repository, player_service
    @repository     = repository
    @player_service = player_service
  end

  def get_teams
    team = Team.new

    results = @repository.custom_read team.build_select_all

    teams = []

    unless results.nil? or results.length == 0
      results.each do |result|
        team.from_hash result

        teams.push team

        team = Team.new
      end
    end

    return teams
  end

  def get team_id
    team = @repository.read Team.new team_id

    unless team.nil?
      get_accolades     team
      get_players       team
      get_stats         team
      get_offense_stats team
      get_defense_stats team
      get_kicking_stats team
    end

    team
  end

  def get_accolades team
    team_accolade = TeamAccolade.new

    results = @repository.custom_read team_accolade.build_select_by_team, team.team_id

    unless results.nil? or results.length == 0
      team.accolades = []

      results.each do |result|
        team_accolade.from_hash result

        team.accolades.push team_accolade

        team_accolade = TeamAccolade.new
      end
    end
  end

  def get_players team
    team_players = TeamPlayer.new

    results = @repository.custom_read team_players.build_select_by_team, team.team_id

    unless results.nil? or results.length == 0
      team.players = []

      results.each do |result|
        team_players.from_hash result

        unless team_players.player_id.nil?
          team_players.player = @player_service.get team_players.player_id
        end

        team.players.push team_players

        team_players = TeamPlayer.new
      end
    end
  end

  def get_stats team
    team_stats = TeamStats.new

    results = @repository.custom_read team_stats.build_select_by_team, team.team_id

    unless results.nil? or results.length == 0
      team.stats = []

      results.each do |result|
        team_stats.from_hash result

        team.stats.push team_stats

        team_stats = TeamStats.new
      end
    end
  end

  def get_offense_stats team
    team_offense_stats = TeamOffenseStats.new

    results = @repository.custom_read team_offense_stats.build_select_by_team, team.team_id

    unless results.nil? or results.length == 0
      team.offense_stats = []

      results.each do |result|
        team_offense_stats.from_hash result

        team.offense_stats.push team_offense_stats

        team_offense_stats = TeamOffenseStats.new
      end
    end
  end

  def get_defense_stats team
    team_defense_stats = TeamDefenseStats.new

    results = @repository.custom_read team_defense_stats.build_select_by_team, team.team_id

    unless results.nil? or results.length == 0
      team.defense_stats = []

      results.each do |result|
        team_defense_stats.from_hash result

        team.defense_stats.push team_defense_stats

        team_defense_stats = TeamDefenseStats.new
      end
    end
  end

  def get_kicking_stats team
    team_kicking_stats = TeamKickingStats.new

    results = @repository.custom_read team_kicking_stats.build_select_by_team, team.team_id

    unless results.nil? or results.length == 0
      team.kicking_stats = []

      results.each do |result|
        team_kicking_stats.from_hash result

        team.kicking_stats.push team_kicking_stats

        team_kicking_stats = TeamKickingStats.new
      end
    end
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
