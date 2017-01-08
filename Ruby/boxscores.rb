#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"
require 'json'
require 'utils'
require 'FileParser'
require 'game_service'
require 'match'
require 'organization'
require 'ProgRunner'
require 'repository'
require 'ScheduleParser'


def get_team( org, team_to_find )
  return nil if org[:conferences].nil?

  org[:conferences].each do |conference|
    return nil if conference[:teams].nil?

    conference[:teams].each do |team|
      location   = team[:location].split.map(&:capitalize).join(' ')

      location.gsub! 'N.', 'North'
      location.gsub! 'S.', 'South'

      if location == team_to_find
        return team
      end
    end
  end
end

def get_playoff_team( teams, team_to_find )
  if teams[:road_team][:location] == team_to_find
    return teams[:road_team]
  end

  return teams[:home_team]
end

def get_total_plays( team, opponent )
  return 0 if team[:offense_stats].nil?
  return 0 if opponent[:defense_stats].nil?

  return team[:offense_stats][:pass_attempts] + team[:offense_stats][:rush_attempts] + opponent[:defense_stats][:sacks]
end

def get_rushing_totals( team )
  return "" if team[:offense_stats].nil?

  return "#{team[:offense_stats][:rush_attempts]}-#{team[:offense_stats][:rush_yards]}-#{team[:offense_stats][:rush_touchdowns]}"
end

def get_passing_totals( team )
  return "" if team[:offense_stats].nil?

  return "#{team[:offense_stats][:completions]}-#{team[:offense_stats][:pass_attempts]}-#{team[:offense_stats][:pass_yards]}-#{team[:offense_stats][:pass_touchdowns]}"
end

def get_total_offense( team )
  return 0 if team[:offense_stats].nil?

  return (team[:offense_stats][:pass_yards] + team[:offense_stats][:rush_yards])
end

def get_ypr( team )
  return 0 if team[:offense_stats].nil?

  return (team[:offense_stats][:rush_attempts] == 0) ? 0.0 : team[:offense_stats][:rush_yards].to_f / team[:offense_stats][:rush_attempts].to_f
end

def get_ypp( team )
  return 0 if team[:offense_stats].nil?

  return (team[:offense_stats][:pass_attempts] == 0) ? 0.0 : team[:offense_stats][:pass_yards].to_f / team[:offense_stats][:pass_attempts].to_f
end

def get_int_totals( team )
  return "" if team[:defense_stats].nil?

  return "#{team[:defense_stats][:interceptions]}-#{team[:defense_stats][:return_yards]}-#{team[:defense_stats][:return_touchdowns]}"
end

def get_return_totals( team )
  return "" if team[:kicking_stats].nil?

  return "#{team[:kicking_stats][:kick_returns] + team[:kicking_stats][:punt_returns]}-"+
    "#{team[:kicking_stats][:kick_return_yards] + team[:kicking_stats][:punt_return_yards]}-"+
    "#{team[:kicking_stats][:kick_return_touchdowns] + team[:kicking_stats][:punt_return_touchdowns]}"
end

def get_kicking_totals( team )
  return "" if team[:kicking_stats].nil?

  return "#{team[:kicking_stats][:extra_points_made]}/#{team[:kicking_stats][:extra_point_attempts]}-"+
    "#{team[:kicking_stats][:field_goals_made]}/#{team[:kicking_stats][:field_goal_attempts]}"
end

def get_punting_totals( team )
  return "" if team[:kicking_stats].nil?

  return "#{team[:kicking_stats][:punts]}-#{team[:kicking_stats][:punt_yards]}"
end

def print_passers( stream, team )
  players = []

  team[:players].each do |p|
    next if p[:stats][:offense].nil?

    if p[:stats][:offense][:pass_attempts] > 0
      players.push p
    end
  end

  players.sort! { |p1, p2| p1[:stats][:offense][:pass_yards] <=> p2[:stats][:offense][:pass_yards] }
  players.reverse!

  players.each do |player|
    stream.printf "%2d %-2s %-20s %2d  %2d %5.1f%%   %3d %4.1f %2d  %2d\n",
    player[:number],
    player[:position],
    "#{player[:last_name]}, #{player[:first_name]}",
    player[:stats][:offense][:completions],
    player[:stats][:offense][:pass_attempts],
    (player[:stats][:offense][:pass_attempts] == 0) ? 0.0 : player[:stats][:offense][:completions].to_f / player[:stats][:offense][:pass_attempts].to_f * 100.0,
    player[:stats][:offense][:pass_yards],
    (player[:stats][:offense][:completions] == 0) ? 0.0 : player[:stats][:offense][:pass_yards].to_f / player[:stats][:offense][:completions].to_f,
    player[:stats][:offense][:pass_touchdowns],
    player[:stats][:offense][:interceptions]
  end
end

def print_rushers( stream, team )
  players = []

  team[:players].each do |p|
    next if p[:stats][:offense].nil?

    if p[:stats][:offense][:rush_attempts] > 0
      players.push p
    end
  end

  players.sort! { |p1, p2| p1[:stats][:offense][:rush_yards] <=> p2[:stats][:offense][:rush_yards] }
  players.reverse!

  players.each do |player|
    stream.printf "%2d %-2s %-20s %2d              %3d %4.1f %2d\n",
    player[:number],
    player[:position],
    "#{player[:last_name]}, #{player[:first_name]}",
    player[:stats][:offense][:rush_attempts],
    player[:stats][:offense][:rush_yards],
    (player[:stats][:offense][:rush_attempts] == 0) ? 0.0 : player[:stats][:offense][:rush_yards].to_f / player[:stats][:offense][:rush_attempts].to_f,
    player[:stats][:offense][:rush_touchdowns]
  end
end

def print_receivers( stream, team )
  players = []

  team[:players].each do |p|
    next if p[:stats][:offense].nil?

    if p[:stats][:offense][:receptions] > 0
      players.push p
    end
  end

  players.sort! { |p1, p2| p1[:stats][:offense][:receiving_yards] <=> p2[:stats][:offense][:receiving_yards] }
  players.reverse!

  players.each do |player|
    stream.printf "%2d %-2s %-20s %2d              %3d %4.1f %2d\n",
    player[:number],
    player[:position],
    "#{player[:last_name]}, #{player[:first_name]}",
    player[:stats][:offense][:receptions],
    player[:stats][:offense][:receiving_yards],
    (player[:stats][:offense][:receptions] == 0) ? 0.0 : player[:stats][:offense][:receiving_yards].to_f / player[:stats][:offense][:receptions].to_f,
    player[:stats][:offense][:receiving_touchdowns]
  end
end

def print_defense( stream, team )
  players = []

  team[:players].each do |p|
    next if p[:stats][:defense].nil?

    if p[:stats][:defense][:sacks] > 0 or p[:stats][:defense][:interceptions] > 0
      players.push p
    end
  end

  players.sort_by! { |p| [p[:stats][:defense][:sacks],p[:stats][:defense][:interceptions],p[:stats][:defense][:return_yards]] }
  players.reverse!

  players.each do |player|
    stream.printf "%2d %-2s %-20s %2d  %2d    %3d %4.1f %2d\n",
    player[:number],
    player[:position],
    "#{player[:last_name]}, #{player[:first_name]}",
    player[:stats][:defense][:sacks],
    player[:stats][:defense][:interceptions],
    player[:stats][:defense][:return_yards],
    (player[:stats][:defense][:interceptions] == 0) ? 0.0 : player[:stats][:defense][:return_yards].to_f / player[:stats][:defense][:interceptions].to_f,
    player[:stats][:defense][:return_touchdowns]
  end
end

def print_returns( stream, team )
  players = []

  team[:players].each do |p|
    next if p[:stats][:returns].nil?

    if p[:stats][:returns][:kick_returns] > 0 or p[:stats][:returns][:punt_returns] > 0
      players.push p
    end
  end

  players.sort_by! { |p| [p[:stats][:returns][:kick_return_yards], p[:stats][:returns][:punt_return_yards]] }
  players.reverse!

  players.each do |player|
    stream.printf "%2d %-2s %-20s %2d   %3d %4.1f %2d  %2d   %3d %4.1f %2d\n",
    player[:number],
    player[:position],
    "#{player[:last_name]}, #{player[:first_name]}",
    player[:stats][:returns][:kick_returns],
    player[:stats][:returns][:kick_return_yards],
    (player[:stats][:returns][:kick_returns] == 0) ? 0.0 : player[:stats][:returns][:kick_return_yards].to_f / player[:stats][:returns][:kick_returns].to_f,
    player[:stats][:returns][:kick_return_touchdowns],
    player[:stats][:returns][:punt_returns],
    player[:stats][:returns][:punt_return_yards],
    (player[:stats][:returns][:punt_returns] == 0) ? 0.0 : player[:stats][:returns][:punt_return_yards].to_f / player[:stats][:returns][:punt_returns].to_f,
    player[:stats][:returns][:punt_return_touchdowns]
  end
end

def print_kicking( stream, team )
  players = []

  team[:players].each do |p|
    next if p[:stats][:kicking].nil?

    if p[:stats][:kicking][:extra_point_attempts] > 0 or p[:stats][:kicking][:field_goal_attempts] > 0
      players.push p
    end
  end

  players.each do |player|
    stream.printf "%2d %-2s %-20s %2d  %2d %5.1f%%   %2d  %2d %5.1f%%   %2d\n",
    player[:number],
    player[:position],
    "#{player[:last_name]}, #{player[:first_name]}",
    player[:stats][:kicking][:extra_point_attempts],
    player[:stats][:kicking][:extra_points_made],
    (player[:stats][:kicking][:extra_point_attempts] == 0) ? 0.0 : player[:stats][:kicking][:extra_points_made].to_f / player[:stats][:kicking][:extra_point_attempts].to_f * 100.0,
    player[:stats][:kicking][:field_goal_attempts],
    player[:stats][:kicking][:field_goals_made],
    (player[:stats][:kicking][:field_goal_attempts] == 0) ? 0.0 : player[:stats][:kicking][:field_goals_made].to_f / player[:stats][:kicking][:field_goal_attempts].to_f * 100.0,
    (player[:stats][:kicking][:field_goals_made] * 3) + player[:stats][:kicking][:extra_points_made]
  end
end

def print_punting( stream, team )
  players = []

  team[:players].each do |p|
    next if p[:stats][:kicking].nil?

    if p[:stats][:kicking][:punts] > 0
      players.push p
    end
  end

  players.each do |player|
    stream.printf "%2d %-2s %-20s %2d   %3d %4.1f\n",
    player[:number],
    player[:position],
    "#{player[:last_name]}, #{player[:first_name]}",
    player[:stats][:kicking][:punts],
    player[:stats][:kicking][:punt_yards],
    (player[:stats][:kicking][:punts] == 0) ? 0.0 : player[:stats][:kicking][:punt_yards].to_f / player[:stats][:kicking][:punts].to_f
  end
end

def print_boxscore( stream, roadteam, hometeam )
    stream.puts
    stream.printf "%27s  %d\n", roadteam[:location].gsub(/([A-Za-z])/, '\1 '), roadteam[:stats][:points_scored]
    stream.printf "%27s  %d\n", hometeam[:location].gsub(/([A-Za-z])/, '\1 '), hometeam[:stats][:points_scored]
    stream.puts
    stream.puts

    stream.puts "                 #{roadteam[:abbreviation]}           #{hometeam[:abbreviation]}"

    stream.printf "Points Scored:   %-2d             %-2d\n",   roadteam[:stats][:points_scored],      hometeam[:stats][:points_scored]
    stream.printf "Plays:           %-2d             %-2d\n",   get_total_plays( roadteam, hometeam ), get_total_plays( hometeam, roadteam )
    stream.printf "Rushing:         %-14s " +      "%-14s\n",   get_rushing_totals( roadteam ),        get_rushing_totals( hometeam )
    stream.printf "Passing:         %-14s " +      "%-14s\n",   get_passing_totals( roadteam ),        get_passing_totals( hometeam )
    stream.printf "Total Offense:   %-3d            %-3d\n",    get_total_offense( roadteam ),         get_total_offense( hometeam )
    stream.printf "Yards/Rush:      %-4.1f           %-4.1f\n", get_ypr( roadteam ),                   get_ypr( hometeam )
    stream.printf "Yards/Pass:      %-4.1f           %-4.1f\n", get_ypp( roadteam ),                   get_ypp( hometeam )
    stream.printf "Sacked:          %-2d             %-2d\n",   hometeam[:defense_stats][:sacks],      roadteam[:defense_stats][:sacks]
    stream.printf "Interceptions:   %-14s " +      "%-14s\n",   get_int_totals( hometeam ),            get_int_totals( roadteam )
    stream.printf "Kick Returns:    %-14s " +      "%-14s\n",   get_return_totals( roadteam ),         get_return_totals( hometeam )
    stream.printf "Kicking:         %-14s " +      "%-14s\n",   get_kicking_totals( roadteam ),        get_kicking_totals( hometeam )
    stream.printf "Punting:         %-14s " +      "%-14s\n",   get_punting_totals( roadteam ),        get_punting_totals( hometeam )
    stream.puts
    stream.puts

    stream.puts "      Passing             Cmp Att   Pct. Yards Avg. TD Int."
    stream.puts "#{roadteam[:location]}:"
    print_passers stream, roadteam
    stream.puts "#{hometeam[:location]}:"
    print_passers stream, hometeam
    stream.puts
    stream.puts

    stream.puts "      Rushing             Car            Yards Avg. TD"
    stream.puts "#{roadteam[:location]}:"
    print_rushers stream, roadteam
    stream.puts "#{hometeam[:location]}:"
    print_rushers stream, hometeam
    stream.puts
    stream.puts

    stream.puts "      Receiving           Rec            Yards Avg. TD"
    stream.puts "#{roadteam[:location]}:"
    print_receivers stream, roadteam
    stream.puts "#{hometeam[:location]}:"
    print_receivers stream, hometeam
    stream.puts
    stream.puts

    stream.puts "      Defense           Sacks Int. Yards Avg. TD"
    stream.puts "#{roadteam[:location]}:"
    print_defense stream, roadteam
    stream.puts "#{hometeam[:location]}:"
    print_defense stream, hometeam
    stream.puts
    stream.puts

    stream.puts "      Returns              KR Yards Avg. TD  PR Yards Avg. TD"
    stream.puts "#{roadteam[:location]}:"
    print_returns stream, roadteam
    stream.puts "#{hometeam[:location]}:"
    print_returns stream, hometeam
    stream.puts
    stream.puts

    stream.puts "      Kicking             XPA XPM   Pct.  FGA FGM   Pct.  Pts."
    stream.puts "#{roadteam[:location]}:"
    print_kicking stream, roadteam
    stream.puts "#{hometeam[:location]}:"
    print_kicking stream, hometeam
    stream.puts
    stream.puts

    stream.puts "      Punting           Punts Yards Avg."
    stream.puts "#{roadteam[:location]}:"
    print_punting stream, roadteam
    stream.puts "#{hometeam[:location]}:"
    print_punting stream, hometeam
    stream.puts
    stream.puts
end

def get_data( cwd, path )
  extract_data = ProgRunner.new "#{cwd}/../C", "extract_data"

  extract_data.execute "#{path}/ncfo1.nes", "#{path}/ncfo1.nst", "#{path}/ncfo2.nes", "#{path}/ncfo2.nst"

  org = JSON.parse extract_data.get_output, {:symbolize_names => true}

  if File.file?("#{path}/.ncfo1.nst.bak") and File.file?("#{path}/.ncfo2.nst.bak")
    extract_data.execute "#{path}/ncfo1.nes", "#{path}/.ncfo1.nst.bak", "#{path}/ncfo2.nes", "#{path}/.ncfo2.nst.bak"

    old_org = JSON.parse extract_data.get_output, {:symbolize_names => true}

    Utils::org_delta org, old_org
  end

  return org
end

def get_bowl_game_data( cwd, path, game_name )
  game = game_name.gsub ' ', '_'

  extract_data = ProgRunner.new "#{cwd}/../C", "extract_bowl_game_data"

  extract_data.execute "#{path}/playoffs.nes", "#{path}/#{game}.nst"

  unless extract_data.success?
    puts "ERROR: #{extract_data.get_output}"

    return nil
  end

  return JSON.parse extract_data.get_output, {:symbolize_names => true}
end

def get_aa_game_data( cwd, path, game_name )
  game = game_name.gsub ' ', '_'

  extract_data = ProgRunner.new "#{cwd}/../C", "extract_aa_game_data"

  extract_data.execute "#{path}/all_americans.nes", "#{path}/#{game}.nst"

  unless extract_data.success?
    puts "ERROR: #{extract_data.get_output}"

    return nil
  end

  return JSON.parse extract_data.get_output, {:symbolize_names => true}
end


path = ARGV[0] || '.'

@repository = Repository.new Utils.get_db "#{path}/../ncfo.db"
@game_service = GameService.new @repository

@repository.start_transaction

@org = Organization.new 1

@repository.read @org

# parse schedule
@sp = ScheduleParser.new
@fp = FileParser.new @sp

@fp.process_file "#{path}/schedule.csv"

@schedule = @sp.schedule

@week = nil
@schedule.days.each do |day|
  break if ! day.completed
  @week = day
end

exit if @week.nil?

puts "Week: #{@week.day}"

@week_folder = sprintf "#{path}/W%02d", @week.day

puts "Creating boxscore files in #{@week_folder}/"
Dir.mkdir @week_folder

if @week.day <= 10
  org = get_data location, path

  @week.games.each do |game|
    roadteam = get_team org, game.road_team
    hometeam = get_team org, game.home_team

    if game.name.nil?
      boxscore_file = sprintf "#{@week_folder}/W%02dG%02d.txt", @week.day, game.number
    else
      boxscore_file = "#{@week_folder}/#{game.name.gsub ' ', '_'}.txt"
    end

    puts "Saving #{game.road_team} @ #{game.home_team} to #{boxscore_file}"

    File.open( boxscore_file, 'w' ) do |stream|
      print_boxscore stream, roadteam, hometeam
    end

    match = Match.new @org.season, @week.day, game.number
    match.road_team_id = roadteam[:team_id]
    match.home_team_id = hometeam[:team_id]

    @repository.create match

    @game_service.save @org.season, @week.day, game.number, roadteam, hometeam
  end
elsif @week.day <= 13
  @week.games.each do |game|
    next if game.name.nil?

    data = get_bowl_game_data location, path, game.name

    next if data.nil?

    boxscore_file = "#{@week_folder}/#{game.name.gsub ' ', '_'}.txt"

    puts "Saving #{game.road_team} @ #{game.home_team} to #{boxscore_file}"

    road_team = get_playoff_team data, game.road_team
    home_team = get_playoff_team data, game.home_team

    File.open( boxscore_file, 'w' ) do |stream|
      print_boxscore stream, road_team, home_team
    end
  end
else
  @week.games.each do |game|
    next if game.name.nil?

    data = get_aa_game_data location, path, game.name

    next if data.nil?

    boxscore_file = "#{@week_folder}/#{game.name.gsub ' ', '_'}.txt"

    puts "Saving #{game.road_team} @ #{game.home_team} to #{boxscore_file}"

    File.open( boxscore_file, 'w' ) do |stream|
      print_boxscore stream, data[:road_team], data[:home_team]
    end
  end
end

@repository.end_transaction
