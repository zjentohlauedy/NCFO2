class Stats
  include Comparable

  def initialize
    @direction = :descending
    @sort_key = nil
  end

  def <=>( other )
    if @direction == :ascending
      get_sort_key.to_f <=> other.get_sort_key.to_f
    else
      other.get_sort_key.to_f <=> get_sort_key.to_f
    end
  end

  def set_direction( direction )
    @direction = direction
  end

  def set_sort_key( key )
    @sort_key = key
  end

  def get_sort_key
    send @sort_key
  end

  def to_s
    value = get_sort_key

    format = "%-2s %-20s "

    format << (@season.nil? ? "" : "S%02d ")
    format << "%-15s "
    format << ((value.is_a? Float) ? "%5.2f" : "%4d")

    if @season.nil?
      sprintf format, @pos, @name, @school, value
    else
      sprintf format, @pos, @name, @season, @school, value
    end
  end

end

class Passing < Stats
  attr_reader :att, :yards, :td, :pct, :avg, :qbr, :score, :pass_score

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @season = player[:stats][:offense][:season]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"
    @att    = player[:stats][:offense][:pass_attempts]
    @comp   = player[:stats][:offense][:completions]
    @yards  = player[:stats][:offense][:pass_yards]
    @td     = player[:stats][:offense][:pass_touchdowns]
    @int    = player[:stats][:offense][:interceptions]
    @pct    = (@att == 0) ? 0.0 : @comp.to_f / @att.to_f * 100.0
    @avg    = (@att == 0) ? 0.0 : @yards.to_f / @att.to_f
    @qbr    = calc_qbr

    @score  = player[:score]
    @pass_score = calc_score player
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
  attr_reader :att, :yards, :td, :avg, :score, :rush_score

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @season = player[:stats][:offense][:season]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"
    @att    = player[:stats][:offense][:rush_attempts]
    @yards  = player[:stats][:offense][:rush_yards]
    @td     = player[:stats][:offense][:rush_touchdowns]
    @avg    = (@att == 0) ? 0.0 : @yards.to_f / @att.to_f

    @score  = player[:score]
    @rush_score = calc_score player
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
  attr_reader :yards, :td, :rec, :avg, :score, :recv_score

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @season = player[:stats][:offense][:season]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"
    @rec    = player[:stats][:offense][:receptions]
    @yards  = player[:stats][:offense][:receiving_yards]
    @td     = player[:stats][:offense][:receiving_touchdowns]
    @avg    = (@rec == 0) ? 0.0 : @yards.to_f / @rec.to_f

    @score  = player[:score]
    @recv_score = calc_score player
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
  attr_reader :yards, :td

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @season = player[:stats][:offense][:season]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"

    if @pos == 'QB'
      @yards = player[:stats][:offense][:rush_yards]
      @td    = player[:stats][:offense][:rush_touchdowns]
    else
      @yards = player[:stats][:offense][:rush_yards]      + player[:stats][:offense][:receiving_yards]
      @td    = player[:stats][:offense][:rush_touchdowns] + player[:stats][:offense][:receiving_touchdowns]
    end
  end

end

class Overall < Stats
  attr_reader :yards, :td

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @season = player[:stats][:offense][:season]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"

    if @pos == 'QB'
      @yards = player[:stats][:offense][:pass_yards]      + player[:stats][:offense][:rush_yards]
      @td    = player[:stats][:offense][:pass_touchdowns] + player[:stats][:offense][:rush_touchdowns]
    else
      @yards = player[:stats][:offense][:rush_yards]      + player[:stats][:offense][:receiving_yards]
      @td    = player[:stats][:offense][:rush_touchdowns] + player[:stats][:offense][:receiving_touchdowns]
    end
  end

end

class Sacks < Stats
  attr_reader :sacks, :score, :pr_score

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @season = player[:stats][:defense][:season]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"
    @sacks  = player[:stats][:defense][:sacks]

    @score  = player[:score]
    @pr_score = calc_score player
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
  attr_reader :int, :yards, :td, :score, :cvg_score

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @season = player[:stats][:defense][:season]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"
    @int    = player[:stats][:defense][:interceptions]
    @yards  = player[:stats][:defense][:return_yards]
    @td     = player[:stats][:defense][:return_touchdowns]
    @avg    = (@int == 0) ? 0.0 : @yards.to_f / @int.to_f

    @score  = player[:score]
    @cvg_score = calc_score player
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
  attr_reader :ret, :yards, :td, :avg

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @season = player[:stats][:returns][:season]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"
    @ret    = player[:stats][:returns][:kick_returns]
    @yards  = player[:stats][:returns][:kick_return_yards]
    @td     = player[:stats][:returns][:kick_return_touchdowns]
    @avg    = (@ret == 0) ? 0.0 : @yards.to_f / @ret.to_f
  end

end

class PuntReturns < Stats
  attr_reader :ret, :yards, :td, :avg

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @season = player[:stats][:returns][:season]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"
    @ret    = player[:stats][:returns][:punt_returns]
    @yards  = player[:stats][:returns][:punt_return_yards]
    @td     = player[:stats][:returns][:punt_return_touchdowns]
    @avg    = (@ret == 0) ? 0.0 : @yards.to_f / @ret.to_f
  end

end

class Kicking < Stats
  attr_reader :fga, :points, :fg_pct

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @season = player[:stats][:kicking][:season]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"
    @xpa    = player[:stats][:kicking][:extra_point_attempts]
    @xpm    = player[:stats][:kicking][:extra_points_made]
    @fga    = player[:stats][:kicking][:field_goal_attempts]
    @fgm    = player[:stats][:kicking][:field_goals_made]
    @points = (@fgm * 3) + @xpm
    @xp_pct = (@xpa == 0) ? 0.0 : @xpm.to_f / @xpa.to_f * 100.0
    @fg_pct = (@fga == 0) ? 0.0 : @fgm.to_f / @fga.to_f * 100.0
  end

end

class Punting < Stats
  attr_reader :punts, :yards, :avg

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @season = player[:stats][:kicking][:season]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"
    @punts  = player[:stats][:kicking][:punts]
    @yards  = player[:stats][:kicking][:punt_yards]
    @avg    = (@punts == 0) ? 0.0 : @yards.to_f / @punts.to_f
  end

end


class StatRankings

  def initialize( org )
    @org        = org
    @players    = Array.new
  end

  def process_categories( categories )
    categories.each do |key, value|
      compile_stats value.fetch( 'class' ), value.fetch( 'types' )

      value.fetch( 'stats' ).each do |stat|
        print "#{stat.fetch 'label'}\n"
        print_top_players stat['stat'], stat['filter']
        print "\n"
      end
    end
  end

  def compile_stats( object, types )
    @players = Array.new

    @org[:conferences].each do |conference|
      conference[:teams].each do |team|
        team[:players].each do |player|
          if types.include? player[:position]
            @players.push( object.new team[:location], player )
          end
        end
      end
    end
  end

  def print_top_players( stat, filter=nil, count=15 )
    @players.each do |player|
      player.set_sort_key stat
    end

    players = @players.sort

    if ! filter.nil?
      players.select! { |p| (p.send filter) > 0 }

      sum = players.reduce(0) { |sum, p| sum + (p.send filter) }

      mean = sum.to_f / players.length

      variance = players.reduce(0) { |var, p| var + ((p.send filter) - mean)**2 }
      variance /= (players.length - 1).to_f

      stddev = Math.sqrt variance

      players.select! { |p| (p.send filter) > (mean - stddev) }
    end

    players.select! { |p| (p.send stat) > 0 }

    if players.length == 0
      puts "--"
      return
    end

    i = 0

    while i < [count, players.length].min
      print "#{players[i].to_s}\n"
      i += 1
    end
  end

end


@categories = {
  'passing'       => {  'class' => Passing,              'types' => ['QB'],
    'stats'       => [{ 'label' => "Passing Yards",      'stat'  => :yards,  'filter' =>  nil },
                      { 'label' => "Passing TD",         'stat'  => :td,     'filter' =>  nil },
                      { 'label' => "Completion Pct.",    'stat'  => :pct,    'filter' => :att },
                      { 'label' => "Yards Per Comp.",    'stat'  => :avg,    'filter' => :att },
                      { 'label' => "Quarterback Rating", 'stat'  => :qbr,    'filter' =>  nil }]},

  'rushing'       => {  'class' => Rushing,             'types' => ['QB','RB','WR','TE'],
    'stats'       => [{ 'label' => "Rushing Yards",     'stat'  => :yards,  'filter' =>  nil },
                      { 'label' => "Rushing TD",        'stat'  => :td,     'filter' =>  nil },
                      { 'label' => "Yards Per Carry",   'stat'  => :avg,    'filter' => :att }]},

  'receiving'     => {  'class' => Receiving,           'types' => ['RB','WR','TE'],
    'stats'       => [{ 'label' => "Receptions",        'stat'  => :rec,    'filter' =>  nil },
                      { 'label' => "Receiving Yards",   'stat'  => :yards,  'filter' =>  nil },
                      { 'label' => "Receiving TD",      'stat'  => :td,     'filter' =>  nil },
                      { 'label' => "Yards Per Catch",   'stat'  => :avg,    'filter' => :rec }]},

  'all-purpose'   => {  'class' => AllPurpose,          'types' => ['QB','RB','WR','TE'],
    'stats'       => [{ 'label' => "All Purpose Yards", 'stat'  => :yards   },
                      { 'label' => "All Purpose TD",    'stat'  => :td      }]},

  'overall'       => {  'class' => Overall,             'types' => ['QB','RB','WR','TE'],
    'stats'       => [{ 'label' => "Overall Yards",     'stat'  => :yards   },
                      { 'label' => "Overall TD",        'stat'  => :td      }]},

  'sacks'         => {  'class' => Sacks,               'types' => ['DL','LB','CB','S'],
    'stats'       => [{ 'label' => "Sacks",             'stat'  => :sacks   }]},

  'interceptions' => {  'class' => Interceptions,       'types' => ['DL','LB','CB','S'],
    'stats'       => [{ 'label' => "Interceptions",     'stat'  => :int     },
                      { 'label' => "Int. Return Yards", 'stat'  => :yards   },
                      { 'label' => "Int. Return TD",    'stat'  => :td      }]},

  'kick-returns'  => {  'class' => KickReturns,         'types' => ['RB','WR','TE'],
    'stats'       => [{ 'label' => "Kick Return Avg.",  'stat'  => :avg,    'filter' => :ret },
                      { 'label' => "Kick Return TD",    'stat'  => :td,     'filter' =>  nil }]},

  'punt-returns'  => {  'class' => PuntReturns,         'types' => ['RB','WR','TE'],
    'stats'       => [{ 'label' => "Punt Return Avg.",  'stat'  => :avg,    'filter' => :ret },
                      { 'label' => "Punt Return TD",    'stat'  => :td,     'filter' =>  nil }]},

  'kicking'       => {  'class' => Kicking,             'types' => ['K'],
    'stats'       => [{ 'label' => "Points",            'stat'  => :points, 'filter' =>  nil },
                      { 'label' => "Field Goal Pct.",   'stat'  => :fg_pct, 'filter' => :fga }]},

  'punting'       => {  'class' => Punting,             'types' => ['P'],
    'stats'       => [{ 'label' => "Yards Per Punt",    'stat'  => :avg, 'filter' => :punts }]}
}
