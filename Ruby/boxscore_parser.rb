class BoxscoreParser

  Score      = 0
  TeamTotals = 1
  Passing    = 2
  Rushing    = 3
  Receiving  = 4
  Defense    = 5
  Returns    = 6
  Kicking    = 7
  Punting    = 8

  def parse( filename )
    results      = {}
    phase        = Score
    current_team = nil

    File::foreach filename do |line|
      ln = line.strip

      next if empty_line ln

      next_phase = check_phase ln

      if next_phase
        phase = next_phase
        next
      end

      if phase == Score
        parse_score ln, results
        next
      end

      if phase == TeamTotals
        parse_team_totals ln, results
        next
      end

      if player_line ln
        parse_player ln, phase, current_team
        next
      end

      team = ln.split( ':' )[0]
      current_team = results[team]
    end

    return results
  end

  private
  def empty_line( line )
    return line.match %r(^ *$)
  end

  def player_line( line )
    return line.match %r(^[0-9])
  end

  def check_phase( line )
    if line.match %r(^U[A-Z][A-Z] +U[A-Z][A-Z])
      return TeamTotals
    end

    if line.match %r(^Passing +)
      return Passing
    end

    if line.match %r(^Rushing +)
      return Rushing
    end

    if line.match %r(^Receiving +)
      return Receiving
    end

    if line.match %r(^Defense +)
      return Defense
    end

    if line.match %r(^Returns +)
      return Returns
    end

    if line.match %r(^Kicking +)
      return Kicking
    end

    if line.match %r(^Punting +)
      return Punting
    end

    return nil
  end

  def parse_score( line, results )
    team_name = line.match( %r|^([A-Za-z] )+| )[0].gsub( ' ', '' )
    home = results.keys.length == 0 ? false : true
    results[team_name] = { home: home, stats: {}, offense_stats: {}, defense_stats: {}, kicking_stats: {}, players: {} }
  end

  def parse_team_totals( line, results )
    road_team = results.values.select { |x| x[:home] == false }[0]
    home_team = results.values.select { |x| x[:home] == true  }[0]

    fields = line.split %r(:?  +)

    if fields[0] == 'Points Scored'
      extract_points_scored fields, road_team, home_team
    end

    if fields[0] == 'Rushing'
      extract_rushing fields, road_team, home_team
    end

    if fields[0] == 'Passing'
      extract_passing fields, road_team, home_team
    end

    if fields[0] == 'Sacked'
      extract_sacks fields, road_team, home_team
    end

    if fields[0] == 'Interceptions'
      extract_interceptions fields, road_team, home_team
    end

    if fields[0] == 'Kicking'
      extract_kicking fields, road_team, home_team
    end

    if fields[0] == 'Punting'
      extract_punting fields, road_team, home_team
    end
  end

  def extract_points_scored( fields, road_team, home_team )
    road_team[:stats][:wins]   = 0
    road_team[:stats][:losses] = 0
    road_team[:stats][:ties]   = 0
    road_team[:stats][:points_scored] = fields[1].to_i

    home_team[:stats][:wins]   = 0
    home_team[:stats][:losses] = 0
    home_team[:stats][:ties]   = 0
    home_team[:stats][:points_scored] = fields[2].to_i

    if    road_team[:stats][:points_scored] > home_team[:stats][:points_scored]
      road_team[:stats][:wins]   = 1
      home_team[:stats][:losses] = 1
    elsif home_team[:stats][:points_scored] > road_team[:stats][:points_scored]
      road_team[:stats][:losses] = 1
      home_team[:stats][:wins]   = 1
    else
      road_team[:stats][:ties] = 1
      home_team[:stats][:ties] = 1
    end
  end

  def extract_rushing( fields, road_team, home_team )
    road_fields = fields[1].split '-'
    home_fields = fields[2].split '-'

    road_team[:offense_stats][:rush_attempts]   = road_fields[0].to_i
    road_team[:offense_stats][:rush_yards]      = road_fields[1].to_i
    road_team[:offense_stats][:rush_touchdowns] = road_fields[2].to_i

    home_team[:offense_stats][:rush_attempts]   = home_fields[0].to_i
    home_team[:offense_stats][:rush_yards]      = home_fields[1].to_i
    home_team[:offense_stats][:rush_touchdowns] = home_fields[2].to_i
  end

  def extract_passing( fields, road_team, home_team )
    road_fields = fields[1].split '-'
    home_fields = fields[2].split '-'

    road_team[:offense_stats][:completions]     = road_fields[0].to_i
    road_team[:offense_stats][:pass_attempts]   = road_fields[1].to_i
    road_team[:offense_stats][:pass_yards]      = road_fields[2].to_i
    road_team[:offense_stats][:pass_touchdowns] = road_fields[3].to_i

    home_team[:offense_stats][:completions]     = home_fields[0].to_i
    home_team[:offense_stats][:pass_attempts]   = home_fields[1].to_i
    home_team[:offense_stats][:pass_yards]      = home_fields[2].to_i
    home_team[:offense_stats][:pass_touchdowns] = home_fields[3].to_i
  end

  def extract_sacks( fields, road_team, home_team )
    road_team[:defense_stats][:sacks] = fields[2].to_i
    home_team[:defense_stats][:sacks] = fields[1].to_i
  end

  def extract_interceptions( fields, road_team, home_team )
    road_fields = fields[2].split '-'
    home_fields = fields[1].split '-'

    road_team[:defense_stats][:interceptions]     = road_fields[0].to_i
    road_team[:defense_stats][:return_yards]      = road_fields[1].to_i
    road_team[:defense_stats][:return_touchdowns] = road_fields[2].to_i

    home_team[:defense_stats][:interceptions]     = home_fields[0].to_i
    home_team[:defense_stats][:return_yards]      = home_fields[1].to_i
    home_team[:defense_stats][:return_touchdowns] = home_fields[2].to_i
  end

  def extract_kicking( fields, road_team, home_team )
    road_fields = fields[1].split %r([/-])
    home_fields = fields[2].split %r([/-])

    road_team[:kicking_stats][:extra_point_attempts] = road_fields[0].to_i
    road_team[:kicking_stats][:extra_points_made]    = road_fields[1].to_i
    road_team[:kicking_stats][:field_goal_attempts]  = road_fields[2].to_i
    road_team[:kicking_stats][:field_goals_made]     = road_fields[3].to_i

    home_team[:kicking_stats][:extra_point_attempts] = home_fields[0].to_i
    home_team[:kicking_stats][:extra_points_made]    = home_fields[1].to_i
    home_team[:kicking_stats][:field_goal_attempts]  = home_fields[2].to_i
    home_team[:kicking_stats][:field_goals_made]     = home_fields[3].to_i
  end

  def extract_punting( fields, road_team, home_team )
    road_fields = fields[1].split '-'
    home_fields = fields[2].split '-'

    road_team[:kicking_stats][:punts]      = road_fields[0].to_i
    road_team[:kicking_stats][:punt_yards] = road_fields[1].to_i

    home_team[:kicking_stats][:punts]      = home_fields[0].to_i
    home_team[:kicking_stats][:punt_yards] = home_fields[1].to_i
  end

  def parse_player( line, phase, team )
    fields = line.split %r(,? +)

    player = team[:players][fields[0]]

    if player.nil?
      player = { stats: {}}
      team[:players][fields[0]] = player
      player[:last_name]  = fields[ 2]
      player[:first_name] = fields[ 3]
    end

    if phase == Passing
      extract_player_passing fields, player
    end

    if phase == Rushing
      extract_player_rushing fields, player
    end

    if phase == Receiving
      extract_player_receiving fields, player
    end

    if phase == Defense
      extract_player_defense fields, player
    end

    if phase == Returns
      extract_player_returns fields, player
    end

    if phase == Kicking
      extract_player_kicking fields, player
    end

    if phase == Punting
      extract_player_punting fields, player
    end
  end

  def extract_player_passing( fields, player )
    if player[:stats][:offense].nil?
      player[:stats][:offense] = {pass_attempts: 0, completions: 0, interceptions: 0, pass_yards: 0, pass_touchdowns: 0, rush_attempts:  0, rush_yards:  0, rush_touchdowns: 0, receptions: 0, receiving_yards:  0, receiving_touchdowns: 0}
    end

    player[:stats][:offense][:completions]     = fields[ 4].to_i
    player[:stats][:offense][:pass_attempts]   = fields[ 5].to_i
    player[:stats][:offense][:pass_yards]      = fields[ 7].to_i
    player[:stats][:offense][:pass_touchdowns] = fields[ 9].to_i
    player[:stats][:offense][:interceptions]   = fields[10].to_i
  end

  def extract_player_rushing( fields, player )
    if player[:stats][:offense].nil?
      player[:stats][:offense] = {pass_attempts: 0, completions: 0, interceptions: 0, pass_yards: 0, pass_touchdowns: 0, rush_attempts:  0, rush_yards:  0, rush_touchdowns: 0, receptions: 0, receiving_yards:  0, receiving_touchdowns: 0}
    end

    player[:stats][:offense][:rush_attempts]   = fields[4].to_i
    player[:stats][:offense][:rush_yards]      = fields[5].to_i
    player[:stats][:offense][:rush_touchdowns] = fields[7].to_i
  end

  def extract_player_receiving( fields, player )
    if player[:stats][:offense].nil?
      player[:stats][:offense] = {pass_attempts: 0, completions: 0, interceptions: 0, pass_yards: 0, pass_touchdowns: 0, rush_attempts:  0, rush_yards:  0, rush_touchdowns: 0, receptions: 0, receiving_yards:  0, receiving_touchdowns: 0}
    end

    player[:stats][:offense][:receptions]           = fields[4].to_i
    player[:stats][:offense][:receiving_yards]      = fields[5].to_i
    player[:stats][:offense][:receiving_touchdowns] = fields[7].to_i
  end

  def extract_player_defense( fields, player )
    if player[:stats][:defense].nil?
      player[:stats][:defense] = {sacks: 0, interceptions: 0, return_yards:  0, return_touchdowns: 0}
    end

    player[:stats][:defense][:sacks]             = fields[4].to_i
    player[:stats][:defense][:interceptions]     = fields[5].to_i
    player[:stats][:defense][:return_yards]      = fields[6].to_i
    player[:stats][:defense][:return_touchdowns] = fields[8].to_i
  end

  def extract_player_returns( fields, player )
    if player[:stats][:returns].nil?
      player[:stats][:returns] = {kick_returns: 0, kick_return_yards: 0, kick_return_touchdowns: 0, punt_returns: 0, punt_return_yards: 0, punt_return_touchdowns: 0}
    end

    player[:stats][:returns][:kick_returns]           = fields[ 4].to_i
    player[:stats][:returns][:kick_return_yards]      = fields[ 5].to_i
    player[:stats][:returns][:kick_return_touchdowns] = fields[ 7].to_i
    player[:stats][:returns][:punt_returns]           = fields[ 8].to_i
    player[:stats][:returns][:punt_return_yards]      = fields[ 9].to_i
    player[:stats][:returns][:punt_return_touchdowns] = fields[11].to_i
  end

  def extract_player_kicking( fields, player )
    if player[:stats][:kicking].nil?
      player[:stats][:kicking] = {extra_point_attempts: 0, extra_points_made: 0, field_goal_attempts: 0, field_goals_made: 0, punts: 0, punt_yards:   0}
    end

    player[:stats][:kicking][:extra_point_attempts] = fields[4].to_i
    player[:stats][:kicking][:extra_points_made]    = fields[5].to_i
    player[:stats][:kicking][:field_goal_attempts]  = fields[7].to_i
    player[:stats][:kicking][:field_goals_made]     = fields[8].to_i
  end

  def extract_player_punting( fields, player )
    if player[:stats][:kicking].nil?
      player[:stats][:kicking] = {extra_point_attempts: 0, extra_points_made: 0, field_goal_attempts: 0, field_goals_made: 0, punts: 0, punt_yards:   0}
    end

    player[:stats][:kicking][:punts]      = fields[4].to_i
    player[:stats][:kicking][:punt_yards] = fields[5].to_i
  end
end
