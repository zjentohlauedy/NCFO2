class TeamRecords
  attr_accessor :opponents, :overall, :home, :road, :points_scored, :points_allowed

  class Record
    attr_accessor :wins, :losses, :ties

    def initialize
      @wins   = 0
      @losses = 0
      @ties   = 0
    end

    def update( score, opp_score )
      if score > opp_score
        @wins += 1
      elsif score < opp_score
        @losses += 1
      else
        @ties += 1
      end
    end

    def games
      return @wins + @losses + @ties
    end

    def win_pct
      return games > 0 ? wins.to_f / games : 0.0
    end

    def to_s( precision = 2 )
      return sprintf "%#{precision}d-%#{precision}d-%#{precision}d", @wins, @losses, @ties
    end

    def <=>( other )
      if self.wins != other.wins; then return self.wins <=> other.wins end
      return 0
    end
  end


  def initialize
    @overall  = Record.new
    @home     = Record.new
    @road     = Record.new

    @points_scored  = 0
    @points_allowed = 0

    @opponents = Hash.new
  end

  def pts_diff
    return @points_scored - @points_allowed
  end

  def print_records
    printf "%s  %s  %s  %4d  %4d\n", @overall.to_s, @home.to_s, @road.to_s, @points_scored, @points_allowed

    count = 0
    @opponents.sort.each do |opponent, record|
      printf "   %15s %s", opponent, record.to_s(1)

      count += 1

      if count == 3
        print "\n"
        count = 0
      end
    end

    print "\n\n"
  end

  def update( game, is_home )
    score     = is_home ? game.home_score : game.road_score
    opp_score = is_home ? game.road_score : game.home_score
    opponent  = is_home ? game.road_team  : game.home_team

    @points_scored += score
    @points_allowed += opp_score
    @overall.update score, opp_score

    if is_home
      @home.update score, opp_score
    else
      @road.update score, opp_score
    end

    if @opponents.has_key? opponent
      record = @opponents.fetch opponent
    else
      record = Record.new
    end

    record.update score, opp_score
    @opponents.store opponent, record
  end
end
