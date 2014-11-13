class ScheduleParser

  class Game
    attr_reader :played, :home_team, :road_team, :home_score, :road_score
    attr_accessor :televised, :number, :name

    def initialize( road_team, home_team, road_score = nil, home_score = nil )
      @home_team = home_team
      @road_team = road_team
      @home_score = home_score.to_i
      @road_score = road_score.to_i

      @number = 0

      @played = !home_score.nil?
      @televised = false
      @name = nil
    end
  end

  class Day
    attr_reader :day, :games, :completed
    attr_accessor :televised

    def initialize( day )
      @day = day.to_i
      @games = []
      @completed = false
      @televised = false
    end

    def add_game( game )
      game.number = @games.length + 1
      @games.push game
      check_complete
    end

    def check_complete
      @completed = true

      @games.each do |game|
        if !game.played
          @completed = false
          return
        end
      end
    end

    private :check_complete
  end

  class Schedule
    attr_reader :days

    def initialize
      @days = []
    end

    def add_day( day )
      @days.push day
    end

    def reset_tv
      days.each do |day|
        if day.televised
          day.games.each do |game|
            if game.televised
              game.televised = false
            end
          end

          day.televised = false
        end
      end
    end
  end

  attr_reader :schedule

  def initialize
    @line_count  = 0
    @road_fields = []
    @home_fields = []
    @game_names  = []
    @schedule = Schedule.new
  end


  def parse( line )

    line.chomp!

    @line_count += 1

    if @line_count == 1
      return
    end

    unless @road_fields.length > 0
      @road_fields = line.split ","
    else
      @home_fields = line.split "," unless @home_fields.length > 0
    end

    if @home_fields.length > 0  &&  @road_fields.length > 0

      if @home_fields[0].length == 0
        @game_names  = @road_fields
        @road_fields = @home_fields
        @home_fields = []
        return
      end

      gameday = Day.new @home_fields[0]

      index = 1

      while index < @home_fields.length

        if @road_fields[index].length == 0  &&  @home_fields[index].length == 0
          index += 2
          next
        end

        if @road_fields[index+1] && @road_fields[index+1].length > 0
          game = Game.new @road_fields[index], @home_fields[index], @road_fields[index+1], @home_fields[index+1]
        else
          game =  Game.new @road_fields[index], @home_fields[index]
        end

        if @game_names[index]  &&  @game_names[index].length > 0
          game.name = @game_names[index]
        end

        gameday.add_game game

        index += 2
      end

      schedule.add_day gameday

      @road_fields = []
      @home_fields = []
      @game_names  = []
    end

  end

end
