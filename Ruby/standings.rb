require 'TeamRecords'

class Standings

  def initialize( schedule )
    @schedule = schedule
    @conferences = [
      { name: "Atlantic",    teams: [ "Delaware",    "Maryland", "New Jersey",    "Pennsylvania",  "Virginia",       "West Virginia"  ], records: {} },
      { name: "Great Lake",  teams: [ "Illinois",    "Michigan", "Minnesota",     "New York",      "Ohio",           "Wisconsin"      ], records: {} },
      { name: "Midwest",     teams: [ "Arkansas",    "Indiana",  "Kansas",        "Kentucky",      "Missouri",       "Tennessee"      ], records: {} },
      { name: "New England", teams: [ "Connecticut", "Maine",    "Massachusetts", "New Hampshire", "Rhode Island",   "Vermont"        ], records: {} },
      { name: "North",       teams: [ "Iowa",        "Montana",  "Nebraska",      "North Dakota",  "South Dakota",   "Wyoming"        ], records: {} },
      { name: "Pacific",     teams: [ "California",  "Idaho",    "Nevada",        "Oregon",        "Utah",           "Washington"     ], records: {} },
      { name: "Southeast",   teams: [ "Alabama",     "Florida",  "Georgia",       "Mississippi",   "North Carolina", "South Carolina" ], records: {} },
      { name: "Southwest",   teams: [ "Arizona",     "Colorado", "Louisiana",     "New Mexico",    "Oklahoma",       "Texas"          ], records: {} }
    ]
  end

  def process()
    weeks_played = 0

    @schedule.days.each do |day|
      next if day.day > 10

      day.games.each do |game|
        road_conference = find_conference game.road_team
        home_conference = find_conference game.home_team

        add_game road_conference[:records], game, false
        add_game home_conference[:records], game, true
      end

      weeks_played += 1
    end

    @conferences.each do |conference|
      conference[:standings] = conference[:records].map { |key, value| { name: key, record: value } }
      conference[:standings].sort! { |a,b| compare_team_records a[:record], b[:record] }
      conference[:standings].reverse!

      if weeks_played == 10
        compare_head2head conference[:standings]

        conference[:standings][0][:clinched] = true
      elsif (conference[:standings][0][:record].overall.wins - conference[:standings][1][:record].overall.wins) > 10 - weeks_played
        conference[:standings][0][:clinched] = true
      end
    end

    return @conferences
  end

  def find_conference( team )
    @conferences.each do |conference|
      if conference[:teams].include? team
        return conference
      end
    end

    raise "Unable to find conference for team #{team}"
  end

  def add_game( team_records, game, home=false )
    return if !game.played

    team = home ? game.home_team : game.road_team

    if team_records[team].nil?
      team_records[team] = TeamRecords.new
    end

    team_records[team].update game, home
  end

  def compare_team_records( a, b )
    if a.overall.win_pct != b.overall.win_pct
      return a.overall.win_pct <=> b.overall.win_pct
    end

    if a.overall.losses != b.overall.losses
      return b.overall.losses <=> a.overall.losses
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
end
