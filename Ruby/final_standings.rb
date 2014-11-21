#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"
require 'FileParser'
require 'ScheduleParser'
require 'TeamRecords'


def add_game( team_records, game, home=false )
  return if !game.played

  if home
    # home team
    if team_records.has_key? game.home_team
      team_record = team_records.fetch game.home_team
    else
      team_record = TeamRecords.new
    end

    team_record.update game, home
    team_records.store game.home_team, team_record
  else
    # road team
    if team_records.has_key? game.road_team
      team_record = team_records.fetch game.road_team
    else
      team_record = TeamRecords.new
    end

    team_record.update game, home
    team_records.store game.road_team, team_record
  end
end

def find_conference( conferences, team )
  conferences.each do |conference|
    if conference[:teams].include? team
      return conference
    end
  end

  return nil
end


conferences = [
  { name: "Atlantic",    teams: [ "Delaware",    "Maryland", "New Jersey",    "Pennsylvania",  "Virginia",       "West Virginia"  ], records: {} },
  { name: "Great Lake",  teams: [ "Illinois",    "Michigan", "Minnesota",     "New York",      "Ohio",           "Wisconsin"      ], records: {} },
  { name: "Midwest",     teams: [ "Arkansas",    "Indiana",  "Kansas",        "Kentucky",      "Missouri",       "Tennessee"      ], records: {} },
  { name: "New England", teams: [ "Connecticut", "Maine",    "Massachusetts", "New Hampshire", "Rhode Island",   "Vermont"        ], records: {} },
  { name: "North",       teams: [ "Iowa",        "Montana",  "Nebraska",      "North Dakota",  "South Dakota",   "Wyoming"        ], records: {} },
  { name: "Pacific",     teams: [ "California",  "Idaho",    "Nevada",        "Oregon",        "Utah",           "Washington"     ], records: {} },
  { name: "Southeast",   teams: [ "Alabama",     "Florida",  "Georgia",       "Mississippi",   "North Carolina", "South Carolina" ], records: {} },
  { name: "Southwest",   teams: [ "Arizona",     "Colorado", "Louisiana",     "New Mexico",    "Oklahoma",       "Texas"          ], records: {} }
]

path = ARGV[0] || '.'

filename = "#{path}/schedule.csv"

if ! File.exists? filename
  abort "File '#{filename}' not found.\n"
end

sp = ScheduleParser.new
fp = FileParser.new sp

fp.process_file filename

schedule = sp.schedule

schedule.days.each do |day|
  day.games.each do |game|
    conference = find_conference conferences, game.road_team

    if conference.nil?
      puts "Unable to find team #{game.road_team}!"
      exit
    end

    add_game conference[:records], game, false

    conference = find_conference conferences, game.home_team

    if conference.nil?
      puts "Unable to find team #{game.home_team}!"
      exit
    end

    add_game conference[:records], game, true
  end
end

def compare_team_records( a, b )
  if a.overall.win_pct != b.overall.win_pct
    return a.overall.win_pct <=> b.overall.win_pct
  end

  if a.pts_diff != b.pts_diff
    return a.pts_diff <=> b.pts_diff
  end

  if a.points_scored != b.points_scored
    return a.points_scored <=> b.points_scored
  end

  return 0
end

def compare_head2head( teams )
  workingset = teams.map.with_index { |t,i| {index: i, name: t[:name], wpct: t[:record].overall.win_pct, opponents: t[:record].opponents} }

  swaps = []

  workingset.each do |team|
    subset = workingset.select { |t| t[:wpct] == team[:wpct] }

    if subset.length == 2
      if subset[1][:opponents][subset[0][:name]].win_pct > subset[0][:opponents][subset[1][:name]].win_pct
        swap = [subset[0][:index], subset[1][:index]]

        unless swaps.include? swap
          swaps.push swap
        end
      end
    end
  end

  swaps.each do |swap|
    teams[swap[0]], teams[swap[1]] = teams[swap[1]], teams[swap[0]]
  end
end

conferences.each do |conference|
  conference[:standings] = conference[:records].map { |key, value| { name: key, record: value } }
  conference[:standings].sort! { |a,b| compare_team_records a[:record], b[:record] }
  conference[:standings].reverse!

  compare_head2head conference[:standings]
end

def print_team_record( team )
  printf "%-15s  %2d %2d %2d", team[:name], team[:record].overall.wins, team[:record].overall.losses, team[:record].overall.ties
end

(0..3).each do |i|
  printf "%-15s   W  L  T    ", conferences[i][:name]
end

puts
puts "-                 -  -  -    -                 -  -  -    -                 -  -  -    -                 -  -  -"

(0..5).each do |i|
  print_team_record conferences[0][:standings][i]
  printf "    "
  print_team_record conferences[1][:standings][i]
  printf "    "
  print_team_record conferences[2][:standings][i]
  printf "    "
  print_team_record conferences[3][:standings][i]
  printf "    "
  puts
end

puts
puts

(4..7).each do |i|
  printf "%-15s   W  L  T    ", conferences[i][:name]
end

puts
puts "-                 -  -  -    -                 -  -  -    -                 -  -  -    -                 -  -  -"

(0..5).each do |i|
  print_team_record conferences[4][:standings][i]
  printf "    "
  print_team_record conferences[5][:standings][i]
  printf "    "
  print_team_record conferences[6][:standings][i]
  printf "    "
  print_team_record conferences[7][:standings][i]
  printf "    "
  puts
end
