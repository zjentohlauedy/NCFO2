location = File.dirname __FILE__
$: << "#{location}"

require 'sqlite3'
require 'repository'
require 'game_service'

db = SQLite3::Database.new 'ncfo_test.db'

db.results_as_hash  = true
db.type_translation = true


describe 'GameService' do
  describe '#save' do
    before :each do
      @repository    = Repository.new db
      @game_service  = GameService.new @repository

      @repository.start_transaction

      @road_team = {team_id: 14, stats: {}, offense_stats: {}, defense_stats: {}, kicking_stats: {}}
      @home_team = {team_id: 16, stats: {}, offense_stats: {}, defense_stats: {}, kicking_stats: {}}
    end

    after :each do
      @repository.cancel_transaction
    end

    it 'should create team game stats records for both teams' do
      season    = 1
      week      = 2
      game      = 3

      @road_team[:stats] = {wins: 1, losses: 0, ties: 0, points_scored: 35, points_allowed: 14}
      @home_team[:stats] = {wins: 0, losses: 1, ties: 0, points_scored: 14, points_allowed: 35}

      @game_service.save season, week, game, @road_team, @home_team

      actual = db.execute 'select * from team_game_stats_t'

      actual.sort! { |a,b| a[:team_id] <=> b[:team_id] }

      expect( actual.length               ).to eq  2
      expect( actual[0]["Team_Id"       ] ).to eq 14
      expect( actual[0]["Season"        ] ).to eq  1
      expect( actual[0]["Week"          ] ).to eq  2
      expect( actual[0]["Game"          ] ).to eq  3
      expect( actual[0]["Wins"          ] ).to eq  1
      expect( actual[0]["Losses"        ] ).to eq  0
      expect( actual[0]["Ties"          ] ).to eq  0
      expect( actual[0]["Home_Wins"     ] ).to eq  0
      expect( actual[0]["Home_Losses"   ] ).to eq  0
      expect( actual[0]["Home_Ties"     ] ).to eq  0
      expect( actual[0]["Road_Wins"     ] ).to eq  1
      expect( actual[0]["Road_Losses"   ] ).to eq  0
      expect( actual[0]["Road_Ties"     ] ).to eq  0
      expect( actual[0]["Points_Scored" ] ).to eq 35
      expect( actual[0]["Points_Allowed"] ).to eq 14

      expect( actual[1]["Team_Id"       ] ).to eq 16
      expect( actual[1]["Season"        ] ).to eq  1
      expect( actual[1]["Week"          ] ).to eq  2
      expect( actual[1]["Game"          ] ).to eq  3
      expect( actual[1]["Wins"          ] ).to eq  0
      expect( actual[1]["Losses"        ] ).to eq  1
      expect( actual[1]["Ties"          ] ).to eq  0
      expect( actual[1]["Home_Wins"     ] ).to eq  0
      expect( actual[1]["Home_Losses"   ] ).to eq  1
      expect( actual[1]["Home_Ties"     ] ).to eq  0
      expect( actual[1]["Road_Wins"     ] ).to eq  0
      expect( actual[1]["Road_Losses"   ] ).to eq  0
      expect( actual[1]["Road_Ties"     ] ).to eq  0
      expect( actual[1]["Points_Scored" ] ).to eq 14
      expect( actual[1]["Points_Allowed"] ).to eq 35
    end

    it 'should create team game offense stats records for both teams' do
      season    = 1
      week      = 2
      game      = 3

      @road_team[:offense_stats] = {pass_attempts: 13, completions: 9, interceptions: 0, pass_yards: 228, pass_touchdowns: 4, rush_attempts: 8, rush_yards: 55, rush_touchdowns: 1}
      @home_team[:offense_stats] = {pass_attempts: 16, completions: 7, interceptions: 2, pass_yards: 205, pass_touchdowns: 1, rush_attempts: 9, rush_yards: 96, rush_touchdowns: 1}

      @game_service.save season, week, game, @road_team, @home_team

      actual = db.execute 'select * from team_game_offense_stats_t'

      actual.sort! { |a,b| a[:team_id] <=> b[:team_id] }

      expect( actual.length                 ).to eq   2
      expect( actual[0]["Team_Id"         ] ).to eq  14
      expect( actual[0]["Season"          ] ).to eq   1
      expect( actual[0]["Week"            ] ).to eq   2
      expect( actual[0]["Game"            ] ).to eq   3
      expect( actual[0]["Pass_Attempts"   ] ).to eq  13
      expect( actual[0]["Completions"     ] ).to eq   9
      expect( actual[0]["Interceptions"   ] ).to eq   0
      expect( actual[0]["Pass_Yards"      ] ).to eq 228
      expect( actual[0]["Pass_Touchdowns" ] ).to eq   4
      expect( actual[0]["Rush_Attempts"   ] ).to eq   8
      expect( actual[0]["Rush_Yards"      ] ).to eq  55
      expect( actual[0]["Rush_Touchdowns" ] ).to eq   1

      expect( actual[1]["Team_Id"         ] ).to eq  16
      expect( actual[1]["Season"          ] ).to eq   1
      expect( actual[1]["Week"            ] ).to eq   2
      expect( actual[1]["Game"            ] ).to eq   3
      expect( actual[1]["Pass_Attempts"   ] ).to eq  16
      expect( actual[1]["Completions"     ] ).to eq   7
      expect( actual[1]["Interceptions"   ] ).to eq   2
      expect( actual[1]["Pass_Yards"      ] ).to eq 205
      expect( actual[1]["Pass_Touchdowns" ] ).to eq   1
      expect( actual[1]["Rush_Attempts"   ] ).to eq   9
      expect( actual[1]["Rush_Yards"      ] ).to eq  96
      expect( actual[1]["Rush_Touchdowns" ] ).to eq   1
    end

    it 'should create team game defense stats records for both teams' do
      season    = 1
      week      = 2
      game      = 3

      @road_team[:defense_stats] = {sacks: 4, interceptions: 1, return_yards:  5, return_touchdowns: 0}
      @road_team[:offense_stats] = {pass_yards: 228, rush_yards: 55}
      @home_team[:defense_stats] = {sacks: 2, interceptions: 2, return_yards: 55, return_touchdowns: 1}
      @home_team[:offense_stats] = {pass_yards: 205, rush_yards: 96}

      @game_service.save season, week, game, @road_team, @home_team

      actual = db.execute 'select * from team_game_defense_stats_t'

      actual.sort! { |a,b| a[:team_id] <=> b[:team_id] }

      expect( actual.length                   ).to eq   2
      expect( actual[0]["Team_Id"           ] ).to eq  14
      expect( actual[0]["Season"            ] ).to eq   1
      expect( actual[0]["Week"              ] ).to eq   2
      expect( actual[0]["Game"              ] ).to eq   3
      expect( actual[0]["Sacks"             ] ).to eq   4
      expect( actual[0]["Interceptions"     ] ).to eq   1
      expect( actual[0]["Return_Yards"      ] ).to eq   5
      expect( actual[0]["Return_Touchdowns" ] ).to eq   0
      expect( actual[0]["Yards_Allowed"     ] ).to eq 301

      expect( actual[1]["Team_Id"           ] ).to eq  16
      expect( actual[1]["Season"            ] ).to eq   1
      expect( actual[1]["Week"              ] ).to eq   2
      expect( actual[1]["Game"              ] ).to eq   3
      expect( actual[1]["Sacks"             ] ).to eq   2
      expect( actual[1]["Interceptions"     ] ).to eq   2
      expect( actual[1]["Return_Yards"      ] ).to eq  55
      expect( actual[1]["Return_Touchdowns" ] ).to eq   1
      expect( actual[1]["Yards_Allowed"     ] ).to eq 283
    end

    it 'should create team game kicking stats records for both teams' do
      season    = 1
      week      = 2
      game      = 3

      @road_team[:kicking_stats] = {extra_point_attempts: 5, extra_points_made: 5, field_goal_attempts: 0, field_goals_made: 0, punts: 1, punt_yards:  43, kick_returns: 3, kick_return_yards:  72, kick_return_touchdowns: 0, punt_returns: 3, punt_return_yards: 86, punt_return_touchdowns: 1}
      @home_team[:kicking_stats] = {extra_point_attempts: 3, extra_points_made: 2, field_goal_attempts: 2, field_goals_made: 1, punts: 3, punt_yards: 139, kick_returns: 5, kick_return_yards: 172, kick_return_touchdowns: 1, punt_returns: 1, punt_return_yards: 23, punt_return_touchdowns: 0}

      @game_service.save season, week, game, @road_team, @home_team

      actual = db.execute 'select * from team_game_kicking_stats_t'

      actual.sort! { |a,b| a[:team_id] <=> b[:team_id] }

      expect( actual.length                        ).to eq   2
      expect( actual[0]["Team_Id"                ] ).to eq  14
      expect( actual[0]["Season"                 ] ).to eq   1
      expect( actual[0]["Week"                   ] ).to eq   2
      expect( actual[0]["Game"                   ] ).to eq   3
      expect( actual[0]["Extra_Point_Attempts"   ] ).to eq   5
      expect( actual[0]["Extra_Points_Made"      ] ).to eq   5
      expect( actual[0]["Field_Goal_Attempts"    ] ).to eq   0
      expect( actual[0]["Field_Goals_Made"       ] ).to eq   0
      expect( actual[0]["Punts"                  ] ).to eq   1
      expect( actual[0]["Punt_Yards"             ] ).to eq  43
      expect( actual[0]["Kick_Returns"           ] ).to eq   3
      expect( actual[0]["Kick_Return_Yards"      ] ).to eq  72
      expect( actual[0]["Kick_Return_Touchdowns" ] ).to eq   0
      expect( actual[0]["Punt_Returns"           ] ).to eq   3
      expect( actual[0]["Punt_Return_Yards"      ] ).to eq  86
      expect( actual[0]["Punt_Return_Touchdowns" ] ).to eq   1

      expect( actual[1]["Team_Id"                ] ).to eq  16
      expect( actual[1]["Season"                 ] ).to eq   1
      expect( actual[1]["Week"                   ] ).to eq   2
      expect( actual[1]["Game"                   ] ).to eq   3
      expect( actual[1]["Extra_Point_Attempts"   ] ).to eq   3
      expect( actual[1]["Extra_Points_Made"      ] ).to eq   2
      expect( actual[1]["Field_Goal_Attempts"    ] ).to eq   2
      expect( actual[1]["Field_Goals_Made"       ] ).to eq   1
      expect( actual[1]["Punts"                  ] ).to eq   3
      expect( actual[1]["Punt_Yards"             ] ).to eq 139
      expect( actual[1]["Kick_Returns"           ] ).to eq   5
      expect( actual[1]["Kick_Return_Yards"      ] ).to eq 172
      expect( actual[1]["Kick_Return_Touchdowns" ] ).to eq   1
      expect( actual[1]["Punt_Returns"           ] ).to eq   1
      expect( actual[1]["Punt_Return_Yards"      ] ).to eq  23
      expect( actual[1]["Punt_Return_Touchdowns" ] ).to eq   0
    end
  end
end
