class Stats
  include Comparable

  def initialize
    @classes = [ "\u22C5", "\u2236", "\u2234", "\u2237" ]
    @direction = :descending
    @sort_key  = nil
  end

  def <=>( other )
    if get_sort_value == other.get_sort_value
      if @season == other.season
        return @identifier <=> other.identifier
      end

      return @season <=> other.season
    end

    if @direction == :ascending
      get_sort_value.to_f <=> other.get_sort_value.to_f
    else
      other.get_sort_value.to_f <=> get_sort_value.to_f
    end
  end

  def set_direction( direction )
    @direction = direction || :descending
  end

  def set_sort_key( key )
    @sort_key = key
  end

  def get_sort_key
    @sort_key
  end

  def get_sort_value
    send @sort_key
  end

  def get_filter_threshold
    return @threshold
  end

  def get_class
    if @season.nil? || @freshman_season.nil?
      return ''
    end

    return @classes[@season - @freshman_season]
  end
end

class TieSummary
  attr_reader :count, :value

  def initialize( count, value )
    @count   = count
    @value   = value
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
  attr_reader :season, :school, :identifier, :name

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

    @identifier             = @school
  end

  def calc_qbr
    qbr = @pass_yards.to_f
    qbr += 2.0 * (@pass_touchdowns.to_f ** 2.0) + 5.0 * @pass_touchdowns.to_f + 1.0
    qbr -= 2.0 * (@pass_int.to_f ** 2.0) + 5.0 * @pass_int.to_f + 1.0
    qbr += (@comp_pct ** (@comp_pct / 100.0)) * 3.0 / 2.0
    return (qbr / 5.0)
  end
end

class Passing < Stats
  attr_reader :pass_attempts, :pass_yards, :pass_touchdowns, :completion_pct, :yards_per_comp, :qbr, :score, :pass_score, :season, :week, :name, :pos, :school, :freshman_season, :seasons, :identifier

  def initialize( school, player )
    super()

    @school          = school
    @pos             = player[:position]
    @season          = player[:stats][:offense][:season]
    @week            = player[:stats][:offense][:week]
    @name            = "#{player[:last_name]}, #{player[:first_name]}"
    @pass_attempts   = player[:stats][:offense][:pass_attempts]
    @completions     = player[:stats][:offense][:completions]
    @pass_yards      = player[:stats][:offense][:pass_yards]
    @pass_touchdowns = player[:stats][:offense][:pass_touchdowns]
    @interceptions   = player[:stats][:offense][:interceptions]
    @completion_pct  = (@pass_attempts == 0) ? 0.0 : @completions.to_f / @pass_attempts.to_f * 100.0
    @yards_per_comp  = (@completions == 0) ? 0.0 : @pass_yards.to_f / @completions.to_f
    @qbr             = calc_qbr

    @freshman_season = player[:freshman_season]

    @score  = player[:score]
    @pass_score = calc_score player

    @seasons = player[:stats][:offense][:seasons]

    @threshold = 64

    @identifier = @name
  end

  def calc_qbr
    qbr = @pass_yards.to_f
    qbr += 2.0 * (@pass_touchdowns.to_f ** 2.0) + 5.0 * @pass_touchdowns.to_f + 1.0
    qbr -= 2.0 * (@interceptions.to_f ** 2.0) + 5.0 * @interceptions.to_f + 1.0
    qbr += (@completion_pct ** (@completion_pct / 100.0)) * 3.0 / 2.0
    return (qbr / 5.0)
  end

  def calc_score( player )
    if player[:ratings].nil?; return 0 end

    score  = player[:ratings][:pass_control] * 5
    score += player[:ratings][:pass_speed]   * 3
    score += player[:ratings][:pass_accuracy]
    score += player[:ratings][:avoid_pass_block]
  end
end

class Rushing < Stats
  attr_reader :rush_attempts, :rush_yards, :rush_touchdowns, :yards_per_carry, :score, :rush_score, :season, :week, :name, :pos, :school, :freshman_season, :seasons, :identifier

  def initialize( school, player )
    super()

    @school          = school
    @pos             = player[:position]
    @season          = player[:stats][:offense][:season]
    @week            = player[:stats][:offense][:week]
    @name            = "#{player[:last_name]}, #{player[:first_name]}"
    @rush_attempts   = player[:stats][:offense][:rush_attempts]
    @rush_yards      = player[:stats][:offense][:rush_yards]
    @rush_touchdowns = player[:stats][:offense][:rush_touchdowns]
    @yards_per_carry = (@rush_attempts == 0) ? 0.0 : @rush_yards.to_f / @rush_attempts.to_f

    @freshman_season = player[:freshman_season]

    @score  = player[:score]
    @rush_score = calc_score player

    @seasons = player[:stats][:offense][:seasons]

    @threshold = 40

    @identifier = @name
  end

  def calc_score( player )
    if player[:ratings].nil?; return 0 end

    acceleration = (player[:ratings][:run_speed] > player[:ratings][:rush_power]) ? player[:ratings][:run_speed] : player[:ratings][:rush_power]

    score  = player[:ratings][:max_speed] * 4
    score += player[:ratings][:hit_power] * 3
    score += player[:ratings][:ball_control].nil? ? 6 : player[:ratings][:ball_control]
    score += acceleration * 2
  end
end

class Receiving < Stats
  attr_reader :receiving_yards, :receiving_touchdowns, :receptions, :yards_per_catch, :score, :recv_score, :season, :week, :name, :pos, :school, :freshman_season, :seasons, :identifier

  def initialize( school, player )
    super()

    @school               = school
    @pos                  = player[:position]
    @season               = player[:stats][:offense][:season]
    @week                 = player[:stats][:offense][:week]
    @name                 = "#{player[:last_name]}, #{player[:first_name]}"
    @receptions           = player[:stats][:offense][:receptions]
    @receiving_yards      = player[:stats][:offense][:receiving_yards]
    @receiving_touchdowns = player[:stats][:offense][:receiving_touchdowns]
    @yards_per_catch      = (@receptions == 0) ? 0.0 : @receiving_yards.to_f / @receptions.to_f

    @freshman_season = player[:freshman_season]

    @score  = player[:score]
    @recv_score = calc_score player

    @seasons = player[:stats][:offense][:seasons]

    @threshold = 20

    @identifier = @name
  end

  def calc_score( player )
    if player[:ratings].nil?; return 0 end

    acceleration = (player[:ratings][:run_speed] > player[:ratings][:rush_power]) ? player[:ratings][:run_speed] : player[:ratings][:rush_power]

    score  = player[:ratings][:receiving] * 5
    score += player[:ratings][:max_speed] * 3
    score += acceleration * 2
  end
end

class AllPurpose < Stats
  attr_reader :all_purpose_yards, :all_purpose_td, :season, :week, :name, :pos, :school, :freshman_season, :seasons, :identifier

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @season = player[:stats][:offense][:season]
    @week   = player[:stats][:offense][:week]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"

    if @pos == 'QB'
      @all_purpose_yards = player[:stats][:offense][:rush_yards]
      @all_purpose_td    = player[:stats][:offense][:rush_touchdowns]
    else
      @all_purpose_yards = player[:stats][:offense][:rush_yards]      + player[:stats][:offense][:receiving_yards]
      @all_purpose_td    = player[:stats][:offense][:rush_touchdowns] + player[:stats][:offense][:receiving_touchdowns]
    end

    @freshman_season = player[:freshman_season]

    @seasons = player[:stats][:offense][:seasons]

    @identifier = @name
  end
end

class Overall < Stats
  attr_reader :overall_yards, :overall_td, :season, :week, :name, :pos, :school, :freshman_season, :seasons, :identifier

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @season = player[:stats][:offense][:season]
    @week   = player[:stats][:offense][:week]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"

    if @pos == 'QB'
      @overall_yards = player[:stats][:offense][:pass_yards]      + player[:stats][:offense][:rush_yards]
      @overall_td    = player[:stats][:offense][:pass_touchdowns] + player[:stats][:offense][:rush_touchdowns]
    else
      @overall_yards = player[:stats][:offense][:rush_yards]      + player[:stats][:offense][:receiving_yards]
      @overall_td    = player[:stats][:offense][:rush_touchdowns] + player[:stats][:offense][:receiving_touchdowns]
    end

    @freshman_season = player[:freshman_season]

    @seasons = player[:stats][:offense][:seasons]

    @identifier = @name
  end
end

class Sacks < Stats
  attr_reader :sacks, :score, :pr_score, :season, :week, :name, :pos, :school, :freshman_season, :seasons, :identifier

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @season = player[:stats][:defense][:season]
    @week   = player[:stats][:defense][:week]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"
    @sacks  = player[:stats][:defense][:sacks]

    @freshman_season = player[:freshman_season]

    @score  = player[:score]
    @pr_score = calc_score player

    @seasons = player[:stats][:defense][:seasons]

    @identifier = @name
  end

  def calc_score( player )
    if player[:ratings].nil?; return 0 end

    acceleration = (player[:ratings][:run_speed] > player[:ratings][:rush_power]) ? player[:ratings][:run_speed] : player[:ratings][:rush_power]

    score  = player[:ratings][:hit_power] * 6
    score += player[:ratings][:max_speed] * 2
    score += player[:ratings][:quickness]
    score += acceleration
  end

end

class Interceptions < Stats
  attr_reader :interceptions, :int_return_yards, :int_return_td, :score, :cvg_score, :season, :week, :name, :pos, :school, :freshman_season, :seasons, :identifier

  def initialize( school, player )
    super()

    @school           = school
    @pos              = player[:position]
    @season           = player[:stats][:defense][:season]
    @week             = player[:stats][:defense][:week]
    @name             = "#{player[:last_name]}, #{player[:first_name]}"
    @interceptions    = player[:stats][:defense][:interceptions]
    @int_return_yards = player[:stats][:defense][:return_yards]
    @int_return_td    = player[:stats][:defense][:return_touchdowns]
    @avg              = (@interceptions == 0) ? 0.0 : @int_return_yards.to_f / @interceptions.to_f

    @freshman_season = player[:freshman_season]

    @score  = player[:score]
    @cvg_score = calc_score player

    @seasons = player[:stats][:defense][:seasons]

    @identifier = @name
  end

  def calc_score( player )
    if player[:ratings].nil?; return 0 end

    acceleration = (player[:ratings][:run_speed] > player[:ratings][:rush_power]) ? player[:ratings][:run_speed] : player[:ratings][:rush_power]

    score  = player[:ratings][:interceptions] * 5
    score += player[:ratings][:max_speed] * 3
    score += player[:ratings][:quickness]
    score += acceleration
  end
end

class KickReturns < Stats
  attr_reader :kick_returns, :kick_return_yards, :kick_return_td, :kick_return_avg, :season, :week, :name, :pos, :school, :freshman_season, :seasons, :identifier

  def initialize( school, player )
    super()

    @school            = school
    @pos               = player[:position]
    @season            = player[:stats][:returns][:season]
    @week              = player[:stats][:returns][:week]
    @name              = "#{player[:last_name]}, #{player[:first_name]}"
    @kick_returns      = player[:stats][:returns][:kick_returns]
    @kick_return_yards = player[:stats][:returns][:kick_return_yards]
    @kick_return_td    = player[:stats][:returns][:kick_return_touchdowns]
    @kick_return_avg   = (@kick_returns == 0) ? 0.0 : @kick_return_yards.to_f / @kick_returns.to_f

    @freshman_season = player[:freshman_season]

    @seasons = player[:stats][:returns][:seasons]

    @threshold = 20

    @identifier = @name
  end
end

class PuntReturns < Stats
  attr_reader :punt_returns, :punt_return_yards, :punt_return_td, :punt_return_avg, :season, :week, :name, :pos, :school, :freshman_season, :seasons, :identifier

  def initialize( school, player )
    super()

    @school            = school
    @pos               = player[:position]
    @season            = player[:stats][:returns][:season]
    @week              = player[:stats][:returns][:week]
    @name              = "#{player[:last_name]}, #{player[:first_name]}"
    @punt_returns      = player[:stats][:returns][:punt_returns]
    @punt_return_yards = player[:stats][:returns][:punt_return_yards]
    @punt_return_td    = player[:stats][:returns][:punt_return_touchdowns]
    @punt_return_avg   = (@punt_returns == 0) ? 0.0 : @punt_return_yards.to_f / @punt_returns.to_f

    @freshman_season = player[:freshman_season]

    @seasons = player[:stats][:returns][:seasons]

    @threshold = 5

    @identifier = @name
  end

end

class Kicking < Stats
  attr_reader :fga, :points, :fg_pct, :season, :week, :name, :pos, :school, :freshman_season, :seasons, :identifier

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @season = player[:stats][:kicking][:season]
    @week   = player[:stats][:kicking][:week]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"
    @xpa    = player[:stats][:kicking][:extra_point_attempts]
    @xpm    = player[:stats][:kicking][:extra_points_made]
    @fga    = player[:stats][:kicking][:field_goal_attempts]
    @fgm    = player[:stats][:kicking][:field_goals_made]
    @points = (@fgm * 3) + @xpm
    @xp_pct = (@xpa == 0) ? 0.0 : @xpm.to_f / @xpa.to_f * 100.0
    @fg_pct = (@fga == 0) ? 0.0 : @fgm.to_f / @fga.to_f * 100.0

    @freshman_season = player[:freshman_season]

    @seasons = player[:stats][:kicking][:seasons]

    @threshold = 5

    @identifier = @name
  end
end

class Punting < Stats
  attr_reader :punts, :punt_yards, :punt_avg, :season, :week, :name, :pos, :school, :freshman_season, :seasons, :identifier

  def initialize( school, player )
    super()

    @school     = school
    @pos        = player[:position]
    @season     = player[:stats][:kicking][:season]
    @week       = player[:stats][:kicking][:week]
    @name       = "#{player[:last_name]}, #{player[:first_name]}"
    @punts      = player[:stats][:kicking][:punts]
    @punt_yards = player[:stats][:kicking][:punt_yards]
    @punt_avg   = (@punts == 0) ? 0.0 : @punt_yards.to_f / @punts.to_f

    @freshman_season = player[:freshman_season]

    @seasons = player[:stats][:kicking][:seasons]

    @threshold = 5

    @identifier = @name
  end
end
