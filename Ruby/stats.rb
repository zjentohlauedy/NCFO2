class Stats
  include Comparable

  def initialize
    @classes = [ "\u22C5", "\u2236", "\u2234", "\u2237" ]
    @direction = :descending
    @sort_key  = nil
    @format    = '%d'
  end

  def <=>( other )
    if get_sort_key == other.get_sort_key
      if @season == other.season
        return @name <=> other.name
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

  def get_class
    if @season.nil? || @freshman_season.nil?
      return ''
    end

    return @classes[@season - @freshman_season]
  end

  def to_s
    value = get_sort_key

    print_format = "%-2s %-24s "

    print_format << (@season.nil? ? "" : "S%02d ")
    print_format << (@seasons.nil? ? "" : "%d ")
    print_format << "%-15s "
    print_format << @format

    if has_season?
      sprintf print_format, @pos, @name + get_class, @season, @school, value
    elsif has_seasons?
      sprintf print_format, @pos, @name + get_class, @seasons, @school, value
    else
      sprintf print_format, @pos, @name + get_class, @school, value
    end
  end

end

class TieSummary
  attr_reader :count, :value

  def initialize( count, value )
    @count   = count
    @value   = value
  end
end

class Passing < Stats
  attr_reader :att, :yards, :td, :pct, :avg, :qbr, :score, :pass_score, :season, :week, :name, :pos, :school

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @season = player[:stats][:offense][:season]
    @week   = player[:stats][:offense][:week]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"
    @att    = player[:stats][:offense][:pass_attempts]
    @comp   = player[:stats][:offense][:completions]
    @yards  = player[:stats][:offense][:pass_yards]
    @td     = player[:stats][:offense][:pass_touchdowns]
    @int    = player[:stats][:offense][:interceptions]
    @pct    = (@att == 0) ? 0.0 : @comp.to_f / @att.to_f * 100.0
    @avg    = (@comp == 0) ? 0.0 : @yards.to_f / @comp.to_f
    @qbr    = calc_qbr

    @freshman_season = player[:freshman_season]

    @score  = player[:score]
    @pass_score = calc_score player

    @seasons = player[:stats][:offense][:seasons]

    @threshold = 64
  end

  def calc_qbr
    qbr = @yards.to_f
    qbr += 2.0 * (@td.to_f ** 2.0) + 5.0 * @td.to_f + 1.0
    qbr -= 2.0 * (@int.to_f ** 2.0) + 5.0 * @int.to_f + 1.0
    qbr += (@pct ** (@pct / 100.0)) * 3.0 / 2.0
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
  attr_reader :att, :yards, :td, :avg, :score, :rush_score, :season, :week, :name, :pos, :school

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @season = player[:stats][:offense][:season]
    @week   = player[:stats][:offense][:week]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"
    @att    = player[:stats][:offense][:rush_attempts]
    @yards  = player[:stats][:offense][:rush_yards]
    @td     = player[:stats][:offense][:rush_touchdowns]
    @avg    = (@att == 0) ? 0.0 : @yards.to_f / @att.to_f

    @freshman_season = player[:freshman_season]

    @score  = player[:score]
    @rush_score = calc_score player

    @seasons = player[:stats][:offense][:seasons]

    @threshold = 40
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
  attr_reader :yards, :td, :rec, :avg, :score, :recv_score, :season, :week, :name, :pos, :school

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @season = player[:stats][:offense][:season]
    @week   = player[:stats][:offense][:week]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"
    @rec    = player[:stats][:offense][:receptions]
    @yards  = player[:stats][:offense][:receiving_yards]
    @td     = player[:stats][:offense][:receiving_touchdowns]
    @avg    = (@rec == 0) ? 0.0 : @yards.to_f / @rec.to_f

    @freshman_season = player[:freshman_season]

    @score  = player[:score]
    @recv_score = calc_score player

    @seasons = player[:stats][:offense][:seasons]

    @threshold = 20
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
  attr_reader :yards, :td, :season, :week, :name, :pos, :school

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @season = player[:stats][:offense][:season]
    @week   = player[:stats][:offense][:week]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"

    if @pos == 'QB'
      @yards = player[:stats][:offense][:rush_yards]
      @td    = player[:stats][:offense][:rush_touchdowns]
    else
      @yards = player[:stats][:offense][:rush_yards]      + player[:stats][:offense][:receiving_yards]
      @td    = player[:stats][:offense][:rush_touchdowns] + player[:stats][:offense][:receiving_touchdowns]
    end

    @freshman_season = player[:freshman_season]

    @seasons = player[:stats][:offense][:seasons]
  end

end

class Overall < Stats
  attr_reader :yards, :td, :season, :week, :name, :pos, :school

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @season = player[:stats][:offense][:season]
    @week   = player[:stats][:offense][:week]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"

    if @pos == 'QB'
      @yards = player[:stats][:offense][:pass_yards]      + player[:stats][:offense][:rush_yards]
      @td    = player[:stats][:offense][:pass_touchdowns] + player[:stats][:offense][:rush_touchdowns]
    else
      @yards = player[:stats][:offense][:rush_yards]      + player[:stats][:offense][:receiving_yards]
      @td    = player[:stats][:offense][:rush_touchdowns] + player[:stats][:offense][:receiving_touchdowns]
    end

    @freshman_season = player[:freshman_season]

    @seasons = player[:stats][:offense][:seasons]
  end

end

class Sacks < Stats
  attr_reader :sacks, :score, :pr_score, :season, :week, :name, :pos, :school

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
  attr_reader :int, :yards, :td, :score, :cvg_score, :season, :week, :name, :pos, :school

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @season = player[:stats][:defense][:season]
    @week   = player[:stats][:defense][:week]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"
    @int    = player[:stats][:defense][:interceptions]
    @yards  = player[:stats][:defense][:return_yards]
    @td     = player[:stats][:defense][:return_touchdowns]
    @avg    = (@int == 0) ? 0.0 : @yards.to_f / @int.to_f

    @freshman_season = player[:freshman_season]

    @score  = player[:score]
    @cvg_score = calc_score player

    @seasons = player[:stats][:defense][:seasons]
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
  attr_reader :ret, :yards, :td, :avg, :season, :week, :name, :pos, :school

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @season = player[:stats][:returns][:season]
    @week   = player[:stats][:returns][:week]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"
    @ret    = player[:stats][:returns][:kick_returns]
    @yards  = player[:stats][:returns][:kick_return_yards]
    @td     = player[:stats][:returns][:kick_return_touchdowns]
    @avg    = (@ret == 0) ? 0.0 : @yards.to_f / @ret.to_f

    @freshman_season = player[:freshman_season]

    @seasons = player[:stats][:returns][:seasons]

    @threshold = 20
  end

end

class PuntReturns < Stats
  attr_reader :ret, :yards, :td, :avg, :season, :week, :name, :pos, :school

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @season = player[:stats][:returns][:season]
    @week   = player[:stats][:returns][:week]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"
    @ret    = player[:stats][:returns][:punt_returns]
    @yards  = player[:stats][:returns][:punt_return_yards]
    @td     = player[:stats][:returns][:punt_return_touchdowns]
    @avg    = (@ret == 0) ? 0.0 : @yards.to_f / @ret.to_f

    @freshman_season = player[:freshman_season]

    @seasons = player[:stats][:returns][:seasons]

    @threshold = 5
  end

end

class Kicking < Stats
  attr_reader :fga, :points, :fg_pct, :season, :week, :name, :pos, :school

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
  end

end

class Punting < Stats
  attr_reader :punts, :yards, :avg, :season, :week, :name, :pos, :school

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @season = player[:stats][:kicking][:season]
    @week   = player[:stats][:kicking][:week]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"
    @punts  = player[:stats][:kicking][:punts]
    @yards  = player[:stats][:kicking][:punt_yards]
    @avg    = (@punts == 0) ? 0.0 : @yards.to_f / @punts.to_f

    @freshman_season = player[:freshman_season]

    @seasons = player[:stats][:kicking][:seasons]

    @threshold = 5
  end

end
