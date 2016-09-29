
class Stats
  include Comparable

  def initialize
    @direction = :descending
    @sort_key  = nil
    @format    = '%d'
  end

  def <=>( other )
    if get_sort_key == other.get_sort_key
      if @season == other.season
        return @school <=> other.school
      end

      return @season <=> other.season
    end

    if @direction == :ascending
      get_sort_key.to_f <=> other.get_sort_key.to_f
    else
      other.get_sort_key.to_f <=> get_sort_key.to_f
    end
  end

  def set_direction( direction )
    @direction = direction
  end

  def set_format( format )
    @format = format
  end

  def set_sort_key( key )
    @sort_key = key
  end

  def get_sort_key
    send @sort_key
  end

  def has_season?
    @season.nil? ? false : true
  end

  def has_seasons?
    @seasons.nil? ? false : true
  end

  def get_filter_threshold
    return @threshold
  end

  def to_s
    value = get_sort_key

    print_format = "%-15s "

    print_format << (@season.nil? ? "" : "S%02d ")
    print_format << (@seasons.nil? ? "" : "%d ")
    print_format << "%-20s "
    print_format << @format

    if has_season?
      sprintf print_format, @school, @season, @name, value
    elsif has_seasons?
      sprintf print_format, @school, @seasons, @name, value
    else
      sprintf print_format, @school, @name, value
    end
  end

end

class TieMessage

  def initialize( count, value, format, padding="" )
    @count   = count
    @value   = value
    @format  = format
    @padding = padding
  end

  def get_sort_key
    return nil
  end

  def to_s
    sprintf "   %-32s #{@padding} #{@format}", "#{@count} Teams Tied At", @value
  end
end

class Team < Stats
  attr_reader :wins, :losses, :ties, :points_scored, :points_allowed, :wl_ratio, :total_offense, :total_off_td
  attr_reader :pass_attempts, :completions, :pass_int, :pass_yards, :pass_touchdowns, :rush_attempts, :rush_yards, :rush_touchdowns
  attr_reader :sacks, :interceptions, :int_return_yards, :int_return_touchdowns, :yards_allowed, :pts_diff, :yds_diff
  attr_reader :extra_point_attempts, :extra_points_made, :field_goal_attempts, :field_goals_made, :punts, :punt_yards
  attr_reader :kick_returns, :kick_return_yards, :kick_return_touchdowns, :punt_returns, :punt_return_yards, :punt_return_touchdowns
  attr_reader :comp_pct, :yds_per_comp, :pass_rating, :yds_per_carry, :int_ret_avg, :kick_ret_avg, :punt_ret_avg
  attr_reader :kicking_points, :xp_pct, :fg_pct, :punt_avg, :total_ret_yards, :total_yards, :total_ret_td, :total_td
  attr_reader :season, :school

  def initialize( team )
    super()

    @school                 = team[:location]
    @season                 = team[:stats][:season]
    @seasons                = team[:stats][:seasons]
    @name                   = team[:name]

    @wins                   = team[:stats][:wins]
    @losses                 = team[:stats][:losses]
    @ties                   = team[:stats][:ties]
    @points_scored          = team[:stats][:points_scored]
    @points_allowed         = team[:stats][:points_allowed]

    @pass_attempts          = team[:offense_stats][:pass_attempts]
    @completions            = team[:offense_stats][:completions]
    @pass_int               = team[:offense_stats][:interceptions]
    @pass_yards             = team[:offense_stats][:pass_yards]
    @pass_touchdowns        = team[:offense_stats][:pass_touchdowns]
    @rush_attempts          = team[:offense_stats][:rush_attempts]
    @rush_yards             = team[:offense_stats][:rush_yards]
    @rush_touchdowns        = team[:offense_stats][:rush_touchdowns]

    @sacks                  = team[:defense_stats][:sacks]
    @interceptions          = team[:defense_stats][:interceptions]
    @int_return_yards       = team[:defense_stats][:return_yards]
    @int_return_touchdowns  = team[:defense_stats][:return_touchdowns]
    @yards_allowed          = team[:defense_stats][:yards_allowed] || 0

    @extra_point_attempts   = team[:kicking_stats][:extra_point_attempts]
    @extra_points_made      = team[:kicking_stats][:extra_points_made]
    @field_goal_attempts    = team[:kicking_stats][:field_goal_attempts]
    @field_goals_made       = team[:kicking_stats][:field_goals_made]
    @punts                  = team[:kicking_stats][:punts]
    @punt_yards             = team[:kicking_stats][:punt_yards]

    @kick_returns           = team[:kicking_stats][:kick_returns]
    @kick_return_yards      = team[:kicking_stats][:kick_return_yards]
    @kick_return_touchdowns = team[:kicking_stats][:kick_return_touchdowns]
    @punt_returns           = team[:kicking_stats][:punt_returns]
    @punt_return_yards      = team[:kicking_stats][:punt_return_yards]
    @punt_return_touchdowns = team[:kicking_stats][:punt_return_touchdowns]


    @wl_ratio               = ((@wins + @losses) == 0) ? 0.0 : @wins.to_f / (@wins + @losses).to_f * 100.0
    @pts_diff               = @points_scored - @points_allowed
    @comp_pct               = (@pass_attempts == 0) ? 0.0 : @completions.to_f / @pass_attempts.to_f * 100.0
    @yds_per_comp           = (@completions == 0) ? 0.0 : @pass_yards.to_f / @completions.to_f
    @pass_rating            = calc_qbr
    @yds_per_carry          = (@rush_attempts == 0) ? 0.0 : @rush_yards.to_f / @rush_attempts.to_f
    @int_ret_avg            = (@interceptions == 0) ? 0.0 : @int_return_yards.to_f / @interceptions.to_f
    @kick_ret_avg           = (@kick_returns == 0) ? 0.0 : @kick_return_yards.to_f / @kick_returns.to_f
    @punt_ret_avg           = (@punt_returns == 0) ? 0.0 : @punt_return_yards.to_f / @punt_returns.to_f
    @kicking_points         = (@field_goals_made * 3) + @extra_points_made
    @xp_pct                 = (@extra_point_attempts == 0) ? 0.0 : @extra_points_made.to_f / @extra_point_attempts.to_f * 100.0
    @fg_pct                 = (@field_goal_attempts == 0) ? 0.0 : @field_goals_made.to_f / @field_goal_attempts.to_f * 100.0
    @punt_avg               = (@punts == 0) ? 0.0 : @punt_yards.to_f / @punts.to_f

    @total_offense          = @pass_yards + @rush_yards
    @total_off_td           = @pass_touchdowns + @rush_touchdowns
    @total_ret_yards        = @kick_return_yards + @punt_return_yards + @int_return_yards
    @total_ret_td           = @kick_return_touchdowns + @punt_return_touchdowns + @int_return_touchdowns
    @total_yards            = @total_offense + @total_ret_yards
    @total_td               = @total_off_td + @total_ret_td
    @yds_diff               = (@yards_allowed == 0) ? 0 : @total_offense - @yards_allowed
  end

  def calc_qbr
    qbr = @pass_yards.to_f
    qbr += 2.0 * (@pass_touchdowns.to_f ** 2.0) + 5.0 * @pass_touchdowns.to_f + 1.0
    qbr -= 2.0 * (@pass_int.to_f ** 2.0) + 5.0 * @pass_int.to_f + 1.0
    qbr += (@comp_pct ** (@comp_pct / 100.0)) * 3.0 / 2.0
    return (qbr / 5.0)
  end
end


class StatRankings

  def initialize( org, filter_type = :stat )
    @org         = org
    @teams     = Array.new
    @filter_type = filter_type
  end

  def process_categories( categories )
    categories.each do |key, value|
      compile_stats value.fetch( 'class' ), value.fetch( 'types' )

      value.fetch( 'stats' ).each do |stat|
        print "#{stat.fetch 'label'}\n"
        print_top_teams stat['stat'], stat['direction'], stat['format']
        print "\n"
      end
    end
  end

  def compile_stats( object, types )
    @teams = Array.new

    @org[:conferences].each do |conference|
      conference[:teams].each do |team|
        @teams.push( object.new team )
      end
    end
  end

  def select_top_teams( teams, min=15 )
    top_teams = []
    last_team = nil
    i           = 0

    teams.each do |team|
      if i < min
        top_teams.push team
      elsif team.get_sort_key == last_team.get_sort_key
        top_teams.push team
      else
        break
      end

      i += 1
      last_team = team
    end

    return top_teams
  end

  def summarize_ties( teams, format, max=20 )
    if teams.length > max
      tied = teams.select { |p| p.get_sort_key == teams[-1].get_sort_key }

      teams = teams.reject { |p| tied.include? p }

      padding = ""

      if tied[-1].has_season?;  padding += "    "; end
      if tied[-1].has_seasons?; padding += "  ";   end

      teams.push TieMessage.new tied.length, tied[-1].get_sort_key, format, padding
    end

    return teams
  end

  def print_top_teams( stat, direction=:descending, format='%d' )
    @teams.each do |team|
      team.set_sort_key stat
      team.set_direction direction
      team.set_format format
    end

    teams = @teams.sort

    teams.select! { |p| (p.send stat) > 0 }

    if teams.length == 0
      puts "--"
      return
    end

    top_teams = select_top_teams teams
    top_teams = summarize_ties top_teams, format

    last_team = nil

    top_teams.each_with_index do |team, idx|
      if last_team.nil? or team.get_sort_key != last_team.get_sort_key
        printf "%2d. %s\n", idx + 1, team.to_s
      else
        printf " -  %s\n", team.to_s
      end

      last_team = team
    end
  end

end


@categories = {
  'records'       => {  'class' => Team,                   'types' => [],
    'stats'       => [{ 'label' => "Wins",                 'stat'  => :wins,                   'direction' => :descending, 'format' => '%2d'   },
                      { 'label' => "Win/Loss Ratio",       'stat'  => :wl_ratio,               'direction' => :descending, 'format' => '%6.2f' },
                      { 'label' => "Scoring Differential", 'stat'  => :pts_diff,               'direction' => :descending, 'format' => '%3d'   },
                      { 'label' => "Yards Differential",   'stat'  => :yds_diff,               'direction' => :descending, 'format' => '%4d'   }]},

  'offense'       => {  'class' => Team,                   'types' => [],
    'stats'       => [{ 'label' => "Passing Yards",        'stat'  => :pass_yards,             'direction' => :descending, 'format' => '%4d'   },
                      { 'label' => "Passing TD",           'stat'  => :pass_touchdowns,        'direction' => :descending, 'format' => '%2d'   },
                      { 'label' => "Completion Pct.",      'stat'  => :comp_pct,               'direction' => :descending, 'format' => '%6.2f' },
                      { 'label' => "Yards Per Comp.",      'stat'  => :yds_per_comp,           'direction' => :descending, 'format' => '%5.2f' },
                      { 'label' => "Quarterback Rating",   'stat'  => :pass_rating,            'direction' => :descending, 'format' => '%7.2f' },
                      { 'label' => "Rushing Yards",        'stat'  => :rush_yards,             'direction' => :descending, 'format' => '%4d'   },
                      { 'label' => "Rushing TD",           'stat'  => :rush_touchdowns,        'direction' => :descending, 'format' => '%2d'   },
                      { 'label' => "Yards Per Carry",      'stat'  => :yds_per_carry,          'direction' => :descending, 'format' => '%5.2f' },
                      { 'label' => "Total Offense",        'stat'  => :total_offense,          'direction' => :descending, 'format' => '%4d'   },
                      { 'label' => "Total Offensive TD",   'stat'  => :total_off_td,           'direction' => :descending, 'format' => '%2d'   },
                      { 'label' => "Total Points Scored",  'stat'  => :points_scored,          'direction' => :descending, 'format' => '%3d'   }]},

  'defense'       => {  'class' => Team,                   'types' => [],
    'stats'       => [{ 'label' => "Sacks",                'stat'  => :sacks,                  'direction' => :descending, 'format' => '%2d'   },
                      { 'label' => "Interceptions",        'stat'  => :interceptions,          'direction' => :descending, 'format' => '%2d'   },
                      { 'label' => "Int. Return Yards",    'stat'  => :int_return_yards,       'direction' => :descending, 'format' => '%3d'   },
                      { 'label' => "Int. Return TD",       'stat'  => :int_return_touchdowns,  'direction' => :descending, 'format' => '%2d'   },
                      { 'label' => "Total Yards Allowed",  'stat'  => :yards_allowed,          'direction' => :ascending,  'format' => '%4d'   },
                      { 'label' => "Total Points Allowed", 'stat'  => :points_allowed,         'direction' => :ascending,  'format' => '%3d'   }]},

  'returns'       => {  'class' => Team,                   'types' => [],
    'stats'       => [{ 'label' => "Kick Return Yards",    'stat'  => :kick_return_yards,      'direction' => :descending, 'format' => '%4d'   },
                      { 'label' => "Kick Return Avg.",     'stat'  => :kick_ret_avg,           'direction' => :descending, 'format' => '%5.2f' },
                      { 'label' => "Kick Return TD",       'stat'  => :kick_return_touchdowns, 'direction' => :descending, 'format' => '%2d'   },
                      { 'label' => "Punt Return Yards",    'stat'  => :punt_return_yards,      'direction' => :descending, 'format' => '%3d'   },
                      { 'label' => "Punt Return Avg.",     'stat'  => :punt_ret_avg,           'direction' => :descending, 'format' => '%5.2f' },
                      { 'label' => "Punt Return TD",       'stat'  => :punt_return_touchdowns, 'direction' => :descending, 'format' => '%2d'   },
                      { 'label' => "Total Return Yards",   'stat'  => :total_ret_yards,        'direction' => :descending, 'format' => '%4d'   },
                      { 'label' => "Total Return TD",      'stat'  => :total_ret_td,           'direction' => :descending, 'format' => '%2d'   },
                      { 'label' => "Total Yards",          'stat'  => :total_yards,            'direction' => :descending, 'format' => '%4d'   },
                      { 'label' => "Total Touchdowns",     'stat'  => :total_td,               'direction' => :descending, 'format' => '%4d'   }]},

  'kicking'       => {  'class' => Team,                   'types' => [],
    'stats'       => [{ 'label' => "Field Goal Pct.",      'stat'  => :fg_pct,                 'direction' => :descending, 'format' => '%6.2f' },
                      { 'label' => "Extra Point Pct.",     'stat'  => :xp_pct,                 'direction' => :descending, 'format' => '%6.2f' },
                      { 'label' => "Points Off Kicks",     'stat'  => :kicking_points,         'direction' => :descending, 'format' => '%3d'   },
                      { 'label' => "Yards Per Punt",       'stat'  => :punt_avg,               'direction' => :descending, 'format' => '%5.2f' }]}
}
