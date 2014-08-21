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

    if value.is_a? Float
      sprintf "%-2s %-20s %-15s %5.2f", @pos, @name, @school, value
    else
      sprintf "%-2s %-20s %-15s %4d", @pos, @name, @school, value
    end
  end

end

class Passing < Stats
  attr_reader :yards, :td, :pct, :avg, :qbr

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"
    @att    = player[:stats][:offense][:pass_attempts]
    @comp   = player[:stats][:offense][:completions]
    @yards  = player[:stats][:offense][:pass_yards]
    @td     = player[:stats][:offense][:pass_touchdowns]
    @int    = player[:stats][:offense][:interceptions]
    @pct    = (@att == 0) ? 0.0 : @comp.to_f / @att.to_f * 100.0
    @avg    = (@att == 0) ? 0.0 : @yards.to_f / @att.to_f
    @qbr    = calc_qbr
  end

  def calc_qbr
    qbr = @yards.to_f
    qbr += 2.0 * (@td.to_f ** 2.0) + 5.0 * @td.to_f + 1.0
    qbr -= 2.0 * (@int.to_f ** 2.0) + 5.0 * @int.to_f + 1.0
    qbr += (@pct ** (@pct / 100.0)) * 3.0 / 2.0
    return (qbr / 5.0)
  end

end

class Rushing < Stats
  attr_reader :yards, :td, :avg

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"
    @att    = player[:stats][:offense][:rush_attempts]
    @yards  = player[:stats][:offense][:rush_yards]
    @td     = player[:stats][:offense][:rush_touchdowns]
    @avg    = (@att == 0) ? 0.0 : @yards.to_f / @att.to_f
  end

end

class Receiving < Stats
  attr_reader :yards, :td, :rec, :avg

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"
    @rec    = player[:stats][:offense][:receptions]
    @yards  = player[:stats][:offense][:receiving_yards]
    @td     = player[:stats][:offense][:receiving_touchdowns]
    @avg    = (@rec == 0) ? 0.0 : @yards.to_f / @rec.to_f
  end

end

class AllPurpose < Stats
  attr_reader :yards, :td

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
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
  attr_reader :sacks

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"
    @sacks  = player[:stats][:defense][:sacks]
  end

end

class Interceptions < Stats
  attr_reader :int, :yards, :td

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"
    @int    = player[:stats][:defense][:interceptions]
    @yards  = player[:stats][:defense][:return_yards]
    @td     = player[:stats][:defense][:return_touchdowns]
    @avg    = (@int == 0) ? 0.0 : @yards.to_f / @int.to_f
  end

end

class KickReturns < Stats
  attr_reader :yards, :td, :avg

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"
    @ret    = player[:stats][:returns][:kick_returns]
    @yards  = player[:stats][:returns][:kick_return_yards]
    @td     = player[:stats][:returns][:kick_return_touchdowns]
    @avg    = (@ret == 0) ? 0.0 : @yards.to_f / @ret.to_f
  end

end

class PuntReturns < Stats
  attr_reader :yards, :td, :avg

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
    @name   = "#{player[:last_name]}, #{player[:first_name]}"
    @ret    = player[:stats][:returns][:punt_returns]
    @yards  = player[:stats][:returns][:punt_return_yards]
    @td     = player[:stats][:returns][:punt_return_touchdowns]
    @avg    = (@ret == 0) ? 0.0 : @yards.to_f / @ret.to_f
  end

end

class Kicking < Stats
  attr_reader :points, :fg_pct

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
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
  attr_reader :yards, :avg

  def initialize( school, player )
    super()

    @school = school
    @pos    = player[:position]
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
        print_top_players stat.fetch( 'stat' )
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

  def print_top_players( stat, count=15 )
    @players.each do |player|
      player.set_sort_key stat
    end

    @players.sort!

    i = 0

    while i < [count, @players.length].min
      print "#{@players[i].to_s}\n"
      i += 1
    end
  end

end


@categories = {
  'passing'       => {  'class' => Passing,              'types' => ['QB'],
    'stats'       => [{ 'label' => "Passing Yards",      'stat'  => :yards   },
                      { 'label' => "Passing TD",         'stat'  => :td      },
                      { 'label' => "Completion Pct.",    'stat'  => :pct     },
                      { 'label' => "Yards Per Comp.",    'stat'  => :avg     },
                      { 'label' => "Quarterback Rating", 'stat'  => :qbr     }]},

  'rushing'       => {  'class' => Rushing,             'types' => ['QB','RB','WR','TE'],
    'stats'       => [{ 'label' => "Rushing Yards",     'stat'  => :yards   },
                      { 'label' => "Rushing TD",        'stat'  => :td      },
                      { 'label' => "Yards Per Carry",   'stat'  => :avg     }]},

  'receiving'     => {  'class' => Receiving,           'types' => ['RB','WR','TE'],
    'stats'       => [{ 'label' => "Receptions",        'stat'  => :rec     },
                      { 'label' => "Receiving Yards",   'stat'  => :yards   },
                      { 'label' => "Receiving TD",      'stat'  => :td      },
                      { 'label' => "Yards Per Catch",   'stat'  => :avg     }]},

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
    'stats'       => [{ 'label' => "Kick Return Avg.",  'stat'  => :avg     },
                      { 'label' => "Kick Return TD",    'stat'  => :td      }]},

  'punt-returns'  => {  'class' => PuntReturns,         'types' => ['RB','WR','TE'],
    'stats'       => [{ 'label' => "Punt Return Avg.",  'stat'  => :avg     },
                      { 'label' => "Punt Return TD",    'stat'  => :td      }]},

  'kicking'       => {  'class' => Kicking,             'types' => ['K'],
    'stats'       => [{ 'label' => "Points",            'stat'  => :points  },
                      { 'label' => "Field Goal Pct.",   'stat'  => :fg_pct  }]},

  'punting'       => {  'class' => Punting,             'types' => ['P'],
    'stats'       => [{ 'label' => "Yards Per Punt",    'stat'  => :avg  }]}
}