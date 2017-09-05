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
    sprintf "   %-35s #{@padding} #{@format}", "#{@count} Players Tied At", @value
  end
end

class Passing < Stats
  attr_reader :att, :yards, :td, :pct, :avg, :qbr, :score, :pass_score, :season, :name

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
  attr_reader :att, :yards, :td, :avg, :score, :rush_score, :season, :name

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
  attr_reader :yards, :td, :rec, :avg, :score, :recv_score, :season, :name

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
  attr_reader :yards, :td, :season, :name

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

    @freshman_season = player[:freshman_season]

    @seasons = player[:stats][:offense][:seasons]
  end

end

class Overall < Stats
  attr_reader :yards, :td, :season, :name

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

    @freshman_season = player[:freshman_season]

    @seasons = player[:stats][:offense][:seasons]
  end

end

class Sacks < Stats
  attr_reader :sacks, :score, :pr_score, :season, :name

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @season = player[:stats][:defense][:season]
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
  attr_reader :int, :yards, :td, :score, :cvg_score, :season, :name

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
  attr_reader :ret, :yards, :td, :avg, :season, :name

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

    @freshman_season = player[:freshman_season]

    @seasons = player[:stats][:returns][:seasons]

    @threshold = 20
  end

end

class PuntReturns < Stats
  attr_reader :ret, :yards, :td, :avg, :season, :name

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

    @freshman_season = player[:freshman_season]

    @seasons = player[:stats][:returns][:seasons]

    @threshold = 5
  end

end

class Kicking < Stats
  attr_reader :fga, :points, :fg_pct, :season, :name

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

    @freshman_season = player[:freshman_season]

    @seasons = player[:stats][:kicking][:seasons]

    @threshold = 5
  end

end

class Punting < Stats
  attr_reader :punts, :yards, :avg, :season, :name

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @season = player[:stats][:kicking][:season]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"
    @punts  = player[:stats][:kicking][:punts]
    @yards  = player[:stats][:kicking][:punt_yards]
    @avg    = (@punts == 0) ? 0.0 : @yards.to_f / @punts.to_f

    @freshman_season = player[:freshman_season]

    @seasons = player[:stats][:kicking][:seasons]

    @threshold = 5
  end

end


class StatRankings

  def initialize( org, filter_type = :stat )
    @org         = org
    @players     = Array.new
    @filter_type = filter_type
  end

  def process_categories( categories )
    categories.each do |key, value|
      compile_stats value.fetch( 'class' ), value.fetch( 'types' )

      value.fetch( 'stats' ).each do |stat|
        print "#{stat.fetch 'label'}\n"
        print_top_players stat['stat'], stat['filter'], stat['format']
        print "\n"
      end
    end
  end

  def compile_stats( object, types )
    @players = Array.new

    @org[:conferences].each do |conference|
      conference[:teams].each do |team|
        next if team[:players].nil?

        team[:players].each do |player|
          if types.include? player[:position]
            @players.push( object.new team[:location], player )
          end
        end
      end
    end
  end

  def select_top_players( players, min=15 )
    top_players = []
    last_player = nil
    i           = 0

    players.each do |player|
      if i < min
        top_players.push player
      elsif player.get_sort_key == last_player.get_sort_key
        top_players.push player
      else
        break
      end

      i += 1
      last_player = player
    end

    return top_players
  end

  def summarize_ties( players, format, max=20 )
    if players.length > max
      tied = players.select { |p| p.get_sort_key == players[-1].get_sort_key }

      players = players.reject { |p| tied.include? p }

      padding = ""

      if tied[-1].has_season?;  padding += "    "; end
      if tied[-1].has_seasons?; padding += "  ";   end

      players.push TieMessage.new tied.length, tied[-1].get_sort_key, format, padding
    end

    return players
  end

  def filter_players( players, filter )
    if filter.nil?; return players; end

    if @filter_type == :absolute
      return players.select { |p| (p.send filter) >= p.get_filter_threshold }
    end

    if @filter_type == :career
      return players.select { |p| (p.send filter) >= (p.get_filter_threshold * 3) }
    end

    filtered_players = players.select { |p| (p.send filter) > 0 }

    sum = filtered_players.reduce(0) { |sum, p| sum + (p.send filter) }

    mean = sum.to_f / filtered_players.length

    variance = filtered_players.reduce(0) { |var, p| var + ((p.send filter) - mean)**2 }
    variance /= (filtered_players.length - 1).to_f

    stddev = Math.sqrt variance

    return filtered_players.select { |p| (p.send filter) > (mean - stddev) }
  end

  def print_top_players( stat, filter=nil, format='%d' )
    @players.each do |player|
      player.set_sort_key stat
      player.set_format format
    end

    players = @players.sort
    players = filter_players players, filter

    players.select! { |p| (p.send stat) > 0 }

    if players.length == 0
      puts "--"
      return
    end

    top_players = select_top_players players
    top_players = summarize_ties top_players, format

    last_player = nil

    top_players.each_with_index do |player, idx|
      if last_player.nil? or player.get_sort_key != last_player.get_sort_key
        printf "%2d. %s\n", idx + 1, player.to_s
      else
        printf " -  %s\n", player.to_s
      end

      last_player = player
    end
  end

end


@categories = {
  'passing'       => {  'class' => Passing,              'types' => ['QB'],
    'stats'       => [{ 'label' => "Passing Yards",      'stat'  => :yards,  'filter' =>  nil,   'format' => '%4d'   },
                      { 'label' => "Passing TD",         'stat'  => :td,     'filter' =>  nil,   'format' => '%2d'   },
                      { 'label' => "Completion Pct.",    'stat'  => :pct,    'filter' => :att,   'format' => '%6.2f' },
                      { 'label' => "Yards Per Comp.",    'stat'  => :avg,    'filter' => :att,   'format' => '%5.2f' },
                      { 'label' => "Quarterback Rating", 'stat'  => :qbr,    'filter' =>  nil,   'format' => '%7.2f' }]},

  'rushing'       => {  'class' => Rushing,              'types' => ['QB','RB','WR','TE'],
    'stats'       => [{ 'label' => "Rushing Yards",      'stat'  => :yards,  'filter' =>  nil,   'format' => '%4d'   },
                      { 'label' => "Rushing TD",         'stat'  => :td,     'filter' =>  nil,   'format' => '%2d'   },
                      { 'label' => "Yards Per Carry",    'stat'  => :avg,    'filter' => :att,   'format' => '%5.2f' }]},

  'receiving'     => {  'class' => Receiving,            'types' => ['RB','WR','TE'],
    'stats'       => [{ 'label' => "Receptions",         'stat'  => :rec,    'filter' =>  nil,   'format' => '%2d'   },
                      { 'label' => "Receiving Yards",    'stat'  => :yards,  'filter' =>  nil,   'format' => '%4d'   },
                      { 'label' => "Receiving TD",       'stat'  => :td,     'filter' =>  nil,   'format' => '%2d'   },
                      { 'label' => "Yards Per Catch",    'stat'  => :avg,    'filter' => :rec,   'format' => '%5.2f' }]},

  'all-purpose'   => {  'class' => AllPurpose,           'types' => ['QB','RB','WR','TE'],
    'stats'       => [{ 'label' => "All Purpose Yards",  'stat'  => :yards,  'filter' =>  nil,   'format' => '%4d'   },
                      { 'label' => "All Purpose TD",     'stat'  => :td,     'filter' =>  nil,   'format' => '%2d'   }]},

  'overall'       => {  'class' => Overall,              'types' => ['QB','RB','WR','TE'],
    'stats'       => [{ 'label' => "Overall Yards",      'stat'  => :yards,  'filter' =>  nil,   'format' => '%4d'   },
                      { 'label' => "Overall TD",         'stat'  => :td,     'filter' =>  nil,   'format' => '%2d'   }]},

  'sacks'         => {  'class' => Sacks,                'types' => ['DL','LB','CB','S'],
    'stats'       => [{ 'label' => "Sacks",              'stat'  => :sacks,  'filter' =>  nil,   'format' => '%2d'   }]},

  'interceptions' => {  'class' => Interceptions,        'types' => ['DL','LB','CB','S'],
    'stats'       => [{ 'label' => "Interceptions",      'stat'  => :int,    'filter' =>  nil,   'format' => '%2d'   },
                      { 'label' => "Int. Return Yards",  'stat'  => :yards,  'filter' =>  nil,   'format' => '%3d'   },
                      { 'label' => "Int. Return TD",     'stat'  => :td,     'filter' =>  nil,   'format' => '%2d'   }]},

  'kick-returns'  => {  'class' => KickReturns,          'types' => ['RB','WR','TE'],
    'stats'       => [{ 'label' => "Kick Return Avg.",   'stat'  => :avg,    'filter' => :ret,   'format' => '%5.2f' },
                      { 'label' => "Kick Return TD",     'stat'  => :td,     'filter' =>  nil,   'format' => '%2d'   }]},

  'punt-returns'  => {  'class' => PuntReturns,          'types' => ['RB','WR','TE'],
    'stats'       => [{ 'label' => "Punt Return Avg.",   'stat'  => :avg,    'filter' => :ret,   'format' => '%5.2f' },
                      { 'label' => "Punt Return TD",     'stat'  => :td,     'filter' =>  nil,   'format' => '%2d'   }]},

  'kicking'       => {  'class' => Kicking,              'types' => ['K'],
    'stats'       => [{ 'label' => "Points",             'stat'  => :points, 'filter' =>  nil,   'format' => '%3d'   },
                      { 'label' => "Field Goal Pct.",    'stat'  => :fg_pct, 'filter' => :fga,   'format' => '%6.2f' }]},

  'punting'       => {  'class' => Punting,              'types' => ['P'],
    'stats'       => [{ 'label' => "Yards Per Punt",     'stat'  => :avg,    'filter' => :punts, 'format' => '%5.2f' }]}
}
