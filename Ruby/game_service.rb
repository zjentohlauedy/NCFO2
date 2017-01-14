require 'player_game_defense_stats'
require 'player_game_kicking_stats'
require 'player_game_offense_stats'
require 'player_game_returns_stats'
require 'team_game_defense_stats'
require 'team_game_kicking_stats'
require 'team_game_offense_stats'
require 'team_game_stats'

class GameService
  def initialize repository
    @repository = repository
  end

  def save( season, week, game, road_team, home_team )
    @repository.create get_team_game_stats( road_team, season, week, game, false )
    @repository.create get_team_game_stats( home_team, season, week, game, true  )

    @repository.create get_team_game_offense_stats( road_team, season, week, game )
    @repository.create get_team_game_offense_stats( home_team, season, week, game )

    @repository.create get_team_game_defense_stats( road_team, season, week, game, home_team )
    @repository.create get_team_game_defense_stats( home_team, season, week, game, road_team )

    @repository.create get_team_game_kicking_stats( road_team, season, week, game )
    @repository.create get_team_game_kicking_stats( home_team, season, week, game )

    save_players season, week, game, road_team[:players]
    save_players season, week, game, home_team[:players]
  end

  private
  def save_players( season, week, game, players )
    players.each do |player|
      if player[:stats][:offense]
        @repository.create get_player_game_offense_stats( player, season, week, game )
      end

      if player[:stats][:defense]
        @repository.create get_player_game_defense_stats( player, season, week, game )
      end

      if player[:stats][:kicking]
        @repository.create get_player_game_kicking_stats( player, season, week, game )
      end

      if player[:stats][:returns]
        @repository.create get_player_game_returns_stats( player, season, week, game )
      end
    end
  end

  def get_team_game_stats( team, season, week, game, home )
    team_game_stats                = TeamGameStats.new team[:team_id], season, week, game
    team_game_stats.wins           = team[:stats][:wins]
    team_game_stats.losses         = team[:stats][:losses]
    team_game_stats.ties           = team[:stats][:ties]
    team_game_stats.points_scored  = team[:stats][:points_scored]
    team_game_stats.points_allowed = team[:stats][:points_allowed]

    if home
      team_game_stats.home_wins   = team[:stats][:wins]
      team_game_stats.home_losses = team[:stats][:losses]
      team_game_stats.home_ties   = team[:stats][:ties]
      team_game_stats.road_wins   = 0
      team_game_stats.road_losses = 0
      team_game_stats.road_ties   = 0
    else
      team_game_stats.home_wins   = 0
      team_game_stats.home_losses = 0
      team_game_stats.home_ties   = 0
      team_game_stats.road_wins   = team[:stats][:wins]
      team_game_stats.road_losses = team[:stats][:losses]
      team_game_stats.road_ties   = team[:stats][:ties]
    end

    return team_game_stats
  end

  def get_team_game_offense_stats( team, season, week, game )
    team_game_offense_stats = TeamGameOffenseStats.new team[:team_id], season, week, game

    team_game_offense_stats.pass_attempts   = team[:offense_stats][:pass_attempts]
    team_game_offense_stats.completions     = team[:offense_stats][:completions]
    team_game_offense_stats.interceptions   = team[:offense_stats][:interceptions]
    team_game_offense_stats.pass_yards      = team[:offense_stats][:pass_yards]
    team_game_offense_stats.pass_touchdowns = team[:offense_stats][:pass_touchdowns]
    team_game_offense_stats.rush_attempts   = team[:offense_stats][:rush_attempts]
    team_game_offense_stats.rush_yards      = team[:offense_stats][:rush_yards]
    team_game_offense_stats.rush_touchdowns = team[:offense_stats][:rush_touchdowns]

    return team_game_offense_stats
  end

  def get_team_game_defense_stats( team, season, week, game, opponent )
    team_game_defense_stats = TeamGameDefenseStats.new team[:team_id], season, week, game

    team_game_defense_stats.sacks             = team[:defense_stats][:sacks]
    team_game_defense_stats.interceptions     = team[:defense_stats][:interceptions]
    team_game_defense_stats.return_yards      = team[:defense_stats][:return_yards]
    team_game_defense_stats.return_touchdowns = team[:defense_stats][:return_touchdowns]
    team_game_defense_stats.yards_allowed     = opponent[:offense_stats][:pass_yards].to_i + opponent[:offense_stats][:rush_yards].to_i

    return team_game_defense_stats
  end

  def get_team_game_kicking_stats( team, season, week, game )
    team_game_kicking_stats = TeamGameKickingStats.new team[:team_id], season, week, game

    team_game_kicking_stats.extra_point_attempts   = team[:kicking_stats][:extra_point_attempts]
    team_game_kicking_stats.extra_points_made      = team[:kicking_stats][:extra_points_made]
    team_game_kicking_stats.field_goal_attempts    = team[:kicking_stats][:field_goal_attempts]
    team_game_kicking_stats.field_goals_made       = team[:kicking_stats][:field_goals_made]
    team_game_kicking_stats.punts                  = team[:kicking_stats][:punts]
    team_game_kicking_stats.punt_yards             = team[:kicking_stats][:punt_yards]
    team_game_kicking_stats.kick_returns           = team[:kicking_stats][:kick_returns]
    team_game_kicking_stats.kick_return_yards      = team[:kicking_stats][:kick_return_yards]
    team_game_kicking_stats.kick_return_touchdowns = team[:kicking_stats][:kick_return_touchdowns]
    team_game_kicking_stats.punt_returns           = team[:kicking_stats][:punt_returns]
    team_game_kicking_stats.punt_return_yards      = team[:kicking_stats][:punt_return_yards]
    team_game_kicking_stats.punt_return_touchdowns = team[:kicking_stats][:punt_return_touchdowns]

    return team_game_kicking_stats
  end

  def get_player_game_offense_stats( player, season, week, game )
    player_game_offense_stats = PlayerGameOffenseStats.new player[:player_id], season, week, game

    player_game_offense_stats.pass_attempts        = player[:stats][:offense][:pass_attempts]
    player_game_offense_stats.completions          = player[:stats][:offense][:completions]
    player_game_offense_stats.interceptions        = player[:stats][:offense][:interceptions]
    player_game_offense_stats.pass_yards           = player[:stats][:offense][:pass_yards]
    player_game_offense_stats.pass_touchdowns      = player[:stats][:offense][:pass_touchdowns]
    player_game_offense_stats.rush_attempts        = player[:stats][:offense][:rush_attempts]
    player_game_offense_stats.rush_yards           = player[:stats][:offense][:rush_yards]
    player_game_offense_stats.rush_touchdowns      = player[:stats][:offense][:rush_touchdowns]
    player_game_offense_stats.receptions           = player[:stats][:offense][:receptions]
    player_game_offense_stats.receiving_yards      = player[:stats][:offense][:receiving_yards]
    player_game_offense_stats.receiving_touchdowns = player[:stats][:offense][:receiving_touchdowns]
    player_game_offense_stats.injuries             = player[:injuries]

    return player_game_offense_stats
  end

  def get_player_game_defense_stats( player, season, week, game )
    player_game_defense_stats = PlayerGameDefenseStats.new player[:player_id], season, week, game

    player_game_defense_stats.sacks             = player[:stats][:defense][:sacks]
    player_game_defense_stats.interceptions     = player[:stats][:defense][:interceptions]
    player_game_defense_stats.return_yards      = player[:stats][:defense][:return_yards]
    player_game_defense_stats.return_touchdowns = player[:stats][:defense][:return_touchdowns]

    return player_game_defense_stats
  end

  def get_player_game_kicking_stats( player, season, week, game )
    player_game_kicking_stats = PlayerGameKickingStats.new player[:player_id], season, week, game

    player_game_kicking_stats.extra_point_attempts = player[:stats][:kicking][:extra_point_attempts]
    player_game_kicking_stats.extra_points_made    = player[:stats][:kicking][:extra_points_made]
    player_game_kicking_stats.field_goal_attempts  = player[:stats][:kicking][:field_goal_attempts]
    player_game_kicking_stats.field_goals_made     = player[:stats][:kicking][:field_goals_made]
    player_game_kicking_stats.punts                = player[:stats][:kicking][:punts]
    player_game_kicking_stats.punt_yards           = player[:stats][:kicking][:punt_yards]

    return player_game_kicking_stats
  end

  def get_player_game_returns_stats( player, season, week, game )
    player_game_returns_stats = PlayerGameReturnsStats.new player[:player_id], season, week, game

    player_game_returns_stats.kick_returns           = player[:stats][:returns][:kick_returns]
    player_game_returns_stats.kick_return_yards      = player[:stats][:returns][:kick_return_yards]
    player_game_returns_stats.kick_return_touchdowns = player[:stats][:returns][:kick_return_touchdowns]
    player_game_returns_stats.punt_returns           = player[:stats][:returns][:punt_returns]
    player_game_returns_stats.punt_return_yards      = player[:stats][:returns][:punt_return_yards]
    player_game_returns_stats.punt_return_touchdowns = player[:stats][:returns][:punt_return_touchdowns]

    return player_game_returns_stats
  end
end
