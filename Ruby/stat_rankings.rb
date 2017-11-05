require 'stats'

class StatRankings

  def initialize( printer, filter, org )
    @players = Array.new
    @org     = org
    @printer = printer
    @filter  = filter
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
      elsif player.get_sort_value == last_player.get_sort_value
        top_players.push player
      else
        break
      end

      i += 1
      last_player = player
    end

    return top_players
  end

  def summarize_ties( players, max=20 )
    if players.length > max
      tied = players.select { |p| p.get_sort_value == players[-1].get_sort_value }

      players.reject! { |p| tied.include? p }

      return TieSummary.new tied.length, tied[-1].get_sort_value
    end

    return nil
  end

  def print_top_players( stat, filter_stat=nil, format='%d' )
    @players.each do |player|
      player.set_sort_key stat
    end

    players = @players.sort
    players = @filter.apply players, filter_stat

    players.select! { |p| (p.send stat) > 0 }

    if players.length == 0
      @printer.print_empty_indicator
      return
    end

    top_players = select_top_players players
    tie_summary = summarize_ties top_players

    last_player = nil
    last_idx    = 0

    top_players.each_with_index do |player, idx|
      tied = (!last_player.nil? and player.get_sort_value == last_player.get_sort_value)

      @printer.print player, format, idx, tied

      last_player = player
      last_idx    = idx
    end

    unless tie_summary.nil?
      @printer.print_tie_message tie_summary, format, last_idx + 1
    end
  end

end


@player_categories = {
  'passing'       => {  'class' => Passing,              'types' => ['QB'],
    'stats'       => [{ 'label' => "Passing Yards",      'stat'  => :pass_yards,           'filter' =>  nil,           'format' => '%4d'   },
                      { 'label' => "Passing TD",         'stat'  => :pass_touchdowns,      'filter' =>  nil,           'format' => '%2d'   },
                      { 'label' => "Completion Pct.",    'stat'  => :completion_pct,       'filter' => :pass_attempts, 'format' => '%6.2f' },
                      { 'label' => "Yards Per Comp.",    'stat'  => :yards_per_comp,       'filter' => :pass_attempts, 'format' => '%5.2f' },
                      { 'label' => "Quarterback Rating", 'stat'  => :qbr,                  'filter' =>  nil,           'format' => '%7.2f' }]},

  'rushing'       => {  'class' => Rushing,              'types' => ['QB','RB','WR','TE'],
    'stats'       => [{ 'label' => "Rushing Yards",      'stat'  => :rush_yards,           'filter' =>  nil,           'format' => '%4d'   },
                      { 'label' => "Rushing TD",         'stat'  => :rush_touchdowns,      'filter' =>  nil,           'format' => '%2d'   },
                      { 'label' => "Yards Per Carry",    'stat'  => :yards_per_carry,      'filter' => :rush_attempts, 'format' => '%5.2f' }]},

  'receiving'     => {  'class' => Receiving,            'types' => ['RB','WR','TE'],
    'stats'       => [{ 'label' => "Receptions",         'stat'  => :receptions,           'filter' =>  nil,           'format' => '%2d'   },
                      { 'label' => "Receiving Yards",    'stat'  => :receiving_yards,      'filter' =>  nil,           'format' => '%4d'   },
                      { 'label' => "Receiving TD",       'stat'  => :receiving_touchdowns, 'filter' =>  nil,           'format' => '%2d'   },
                      { 'label' => "Yards Per Catch",    'stat'  => :yards_per_catch,      'filter' => :receptions,    'format' => '%5.2f' }]},

  'all-purpose'   => {  'class' => AllPurpose,           'types' => ['QB','RB','WR','TE'],
    'stats'       => [{ 'label' => "All Purpose Yards",  'stat'  => :all_purpose_yards,    'filter' =>  nil,           'format' => '%4d'   },
                      { 'label' => "All Purpose TD",     'stat'  => :all_purpose_td,       'filter' =>  nil,           'format' => '%2d'   }]},

  'overall'       => {  'class' => Overall,              'types' => ['QB','RB','WR','TE'],
    'stats'       => [{ 'label' => "Overall Yards",      'stat'  => :overall_yards,        'filter' =>  nil,           'format' => '%4d'   },
                      { 'label' => "Overall TD",         'stat'  => :overall_td,           'filter' =>  nil,           'format' => '%2d'   }]},

  'sacks'         => {  'class' => Sacks,                'types' => ['DL','LB','CB','S'],
    'stats'       => [{ 'label' => "Sacks",              'stat'  => :sacks,                'filter' =>  nil,           'format' => '%2d'   }]},

  'interceptions' => {  'class' => Interceptions,        'types' => ['DL','LB','CB','S'],
    'stats'       => [{ 'label' => "Interceptions",      'stat'  => :interceptions,        'filter' =>  nil,           'format' => '%2d'   },
                      { 'label' => "Int. Return Yards",  'stat'  => :int_return_yards,     'filter' =>  nil,           'format' => '%3d'   },
                      { 'label' => "Int. Return TD",     'stat'  => :int_return_td,        'filter' =>  nil,           'format' => '%2d'   }]},

  'kick-returns'  => {  'class' => KickReturns,          'types' => ['RB','WR','TE'],
    'stats'       => [{ 'label' => "Kick Return Avg.",   'stat'  => :kick_return_avg,      'filter' => :kick_returns,  'format' => '%5.2f' },
                      { 'label' => "Kick Return TD",     'stat'  => :kick_return_td,       'filter' =>  nil,           'format' => '%2d'   }]},

  'punt-returns'  => {  'class' => PuntReturns,          'types' => ['RB','WR','TE'],
    'stats'       => [{ 'label' => "Punt Return Avg.",   'stat'  => :punt_return_avg,      'filter' => :punt_returns,  'format' => '%5.2f' },
                      { 'label' => "Punt Return TD",     'stat'  => :punt_return_td,       'filter' =>  nil,           'format' => '%2d'   }]},

  'kicking'       => {  'class' => Kicking,              'types' => ['K'],
    'stats'       => [{ 'label' => "Points",             'stat'  => :points,               'filter' =>  nil,           'format' => '%3d'   },
                      { 'label' => "Field Goal Pct.",    'stat'  => :fg_pct,               'filter' => :fga,           'format' => '%6.2f' }]},

  'punting'       => {  'class' => Punting,              'types' => ['P'],
    'stats'       => [{ 'label' => "Yards Per Punt",     'stat'  => :punt_avg,             'filter' => :punts,         'format' => '%5.2f' }]}
}
