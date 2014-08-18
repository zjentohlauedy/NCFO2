location = File.dirname __FILE__
$: << "#{location}"

require 'utils'

describe Utils do
  describe '::roll' do
    it 'should return a randomly chosen array index given an array' do
      value = Utils::roll [ 1, 1, 1, 1, 1 ]

      expect( value ).to be_a Integer
      expect( value ).to be >= 0
      expect( value ).to be <= 4
    end

    it 'should return an index based on the weights of each index in the passed array' do
      value = Utils::roll [ 0, 0, 0, 0, 0, 123, 0, 0, 0, 0 ]

      expect( value ).to eq 5
    end

    it 'should consider every index of the passed array' do
      value = Utils::roll [ 5, 0, 0, 15, 0, 0, 0, 8, 11, 0, 30, 0, 6 ]

      expect( [ 0, 3, 7, 8, 10, 12 ] ).to include value
    end
  end

  describe '::get_db' do
    it 'should return a sqlite3 database instance' do
      db = Utils::get_db "ncfo_test.db"

      expect( db ).to_not be_nil
      expect( db ).to     be_a   SQLite3::Database

      expect( db.results_as_hash  ).to be true
      expect( db.type_translation ).to be true
    end
  end

  describe '::org_delta' do
    it 'should subtract the team values in one org from those in the other' do
      org1 = { conferences: [{ teams: [{ stats:{ wins: 5, losses: 5, ties: 5, points_scored: 100, points_allowed: 100 }, players: []}]}] }
      org2 = { conferences: [{ teams: [{ stats:{ wins: 2, losses: 3, ties: 4, points_scored:  50, points_allowed:  70 }, players: []}]}] }

      Utils::org_delta org1, org2

      stats = org1[:conferences][0][:teams][0][:stats]

      expect( stats[:wins          ] ).to be  3
      expect( stats[:losses        ] ).to be  2
      expect( stats[:ties          ] ).to be  1
      expect( stats[:points_scored ] ).to be 50
      expect( stats[:points_allowed] ).to be 30
    end

    it 'should subtract the team offense values in one org from those in the other' do
      org1 = { conferences: [{ teams: [{ offense_stats:{ pass_attempts: 20, completions: 20, interceptions: 5, pass_yards: 500, pass_touchdowns: 5, rush_attempts: 20, rush_yards: 500, rush_touchdowns: 5 }, players: []}]}] }
      org2 = { conferences: [{ teams: [{ offense_stats:{ pass_attempts:  9, completions: 11, interceptions: 2, pass_yards: 200, pass_touchdowns: 3, rush_attempts:  7, rush_yards: 300, rush_touchdowns: 3 }, players: []}]}] }

      Utils::org_delta org1, org2

      offense = org1[:conferences][0][:teams][0][:offense_stats]

      expect( offense[:pass_attempts  ] ).to be  11
      expect( offense[:completions    ] ).to be   9
      expect( offense[:interceptions  ] ).to be   3
      expect( offense[:pass_yards     ] ).to be 300
      expect( offense[:pass_touchdowns] ).to be   2
      expect( offense[:rush_attempts  ] ).to be  13
      expect( offense[:rush_yards     ] ).to be 200
      expect( offense[:rush_touchdowns] ).to be   2
    end

    it 'should subtract the team defense values in one org from those in the other' do
      org1 = { conferences: [{ teams: [{ defense_stats:{ sacks: 5, interceptions: 5, return_yards: 100, return_touchdowns: 3 }, players: []}]}] }
      org2 = { conferences: [{ teams: [{ defense_stats:{ sacks: 2, interceptions: 3, return_yards:  40, return_touchdowns: 1 }, players: []}]}] }

      Utils::org_delta org1, org2

      defense = org1[:conferences][0][:teams][0][:defense_stats]

      expect( defense[:sacks            ] ).to be  3
      expect( defense[:interceptions    ] ).to be  2
      expect( defense[:return_yards     ] ).to be 60
      expect( defense[:return_touchdowns] ).to be  2
    end

    it 'should subtract the team kicking values in one org from those in the other' do
      org1 = { conferences: [{ teams: [{ kicking_stats:{ extra_point_attempts: 5, extra_points_made: 5, field_goal_attempts: 5, field_goals_made: 5, punts: 5, punt_yards: 100, kick_returns: 5, kick_return_yards: 100, kick_return_touchdowns: 3, punt_returns: 5, punt_return_yards: 100, punt_return_touchdowns: 3 }, players: []}]}] }
      org2 = { conferences: [{ teams: [{ kicking_stats:{ extra_point_attempts: 2, extra_points_made: 3, field_goal_attempts: 2, field_goals_made: 3, punts: 2, punt_yards:  60, kick_returns: 2, kick_return_yards:  40, kick_return_touchdowns: 1, punt_returns: 3, punt_return_yards:  55, punt_return_touchdowns: 2 }, players: []}]}] }

      Utils::org_delta org1, org2

      kicking = org1[:conferences][0][:teams][0][:kicking_stats]

      expect( kicking[:extra_point_attempts  ] ).to be  3
      expect( kicking[:extra_points_made     ] ).to be  2
      expect( kicking[:field_goal_attempts   ] ).to be  3
      expect( kicking[:field_goals_made      ] ).to be  2
      expect( kicking[:punts                 ] ).to be  3
      expect( kicking[:punt_yards            ] ).to be 40
      expect( kicking[:kick_returns          ] ).to be  3
      expect( kicking[:kick_return_yards     ] ).to be 60
      expect( kicking[:kick_return_touchdowns] ).to be  2
      expect( kicking[:punt_returns          ] ).to be  2
      expect( kicking[:punt_return_yards     ] ).to be 45
      expect( kicking[:punt_return_touchdowns] ).to be  1
    end

    it 'should subtract the player offense values in one org from those in the other' do
      org1 = { conferences: [{ teams: [{ players: [{ stats:{ offense:{ pass_attempts: 5, completions: 5, interceptions: 3, pass_yards: 100, pass_touchdowns: 3, rush_attempts: 5, rush_yards: 100, rush_touchdowns: 3, receptions: 5, receiving_yards: 100, receiving_touchdowns: 3}}}]}]}] }
      org2 = { conferences: [{ teams: [{ players: [{ stats:{ offense:{ pass_attempts: 2, completions: 3, interceptions: 1, pass_yards:  60, pass_touchdowns: 1, rush_attempts: 2, rush_yards:  40, rush_touchdowns: 2, receptions: 2, receiving_yards:  65, receiving_touchdowns: 1}}}]}]}] }

      Utils::org_delta org1, org2

      offense = org1[:conferences][0][:teams][0][:players][0][:stats][:offense]

      expect( offense[:pass_attempts       ] ).to be  3
      expect( offense[:completions         ] ).to be  2
      expect( offense[:interceptions       ] ).to be  2
      expect( offense[:pass_yards          ] ).to be 40
      expect( offense[:pass_touchdowns     ] ).to be  2
      expect( offense[:rush_attempts       ] ).to be  3
      expect( offense[:rush_yards          ] ).to be 60
      expect( offense[:rush_touchdowns     ] ).to be  1
      expect( offense[:receptions          ] ).to be  3
      expect( offense[:receiving_yards     ] ).to be 35
      expect( offense[:receiving_touchdowns] ).to be  2
    end

    it 'should subtract the player defense values in one org from those in the other' do
      org1 = { conferences: [{ teams: [{ players: [{ stats:{ defense:{ sacks: 12, interceptions: 5, return_yards: 100, return_touchdowns: 3}}}]}]}] }
      org2 = { conferences: [{ teams: [{ players: [{ stats:{ defense:{ sacks:  7, interceptions: 2, return_yards:  60, return_touchdowns: 1}}}]}]}] }

      Utils::org_delta org1, org2

      defense = org1[:conferences][0][:teams][0][:players][0][:stats][:defense]

      expect( defense[:sacks            ] ).to be  5
      expect( defense[:interceptions    ] ).to be  3
      expect( defense[:return_yards     ] ).to be 40
      expect( defense[:return_touchdowns] ).to be  2
    end

    it 'should subtract the player kicking values in one org from those in the other' do
      org1 = { conferences: [{ teams: [{ players: [{ stats:{ kicking:{ extra_point_attempts: 7, extra_points_made: 7, field_goal_attempts: 5, field_goals_made: 5, punts: 9, punt_yards: 100}}}]}]}] }
      org2 = { conferences: [{ teams: [{ players: [{ stats:{ kicking:{ extra_point_attempts: 3, extra_points_made: 4, field_goal_attempts: 2, field_goals_made: 3, punts: 5, punt_yards:  60}}}]}]}] }

      Utils::org_delta org1, org2

      kicking = org1[:conferences][0][:teams][0][:players][0][:stats][:kicking]

      expect( kicking[:extra_point_attempts] ).to be  4
      expect( kicking[:extra_points_made   ] ).to be  3
      expect( kicking[:field_goal_attempts ] ).to be  3
      expect( kicking[:field_goals_made    ] ).to be  2
      expect( kicking[:punts               ] ).to be  4
      expect( kicking[:punt_yards          ] ).to be 40
    end

    it 'should subtract the player returns values in one org from those in the other' do
      org1 = { conferences: [{ teams: [{ players: [{ stats:{ returns:{ kick_returns: 5, kick_return_yards: 100, kick_return_touchdowns: 3, punt_returns: 5, punt_return_yards: 100, punt_return_touchdowns: 3}}}]}]}] }
      org2 = { conferences: [{ teams: [{ players: [{ stats:{ returns:{ kick_returns: 2, kick_return_yards:  60, kick_return_touchdowns: 2, punt_returns: 3, punt_return_yards:  40, punt_return_touchdowns: 1}}}]}]}] }

      Utils::org_delta org1, org2

      returns = org1[:conferences][0][:teams][0][:players][0][:stats][:returns]

      expect( returns[:kick_returns          ] ).to be  3
      expect( returns[:kick_return_yards     ] ).to be 40
      expect( returns[:kick_return_touchdowns] ).to be  1
      expect( returns[:punt_returns          ] ).to be  2
      expect( returns[:punt_return_yards     ] ).to be 60
      expect( returns[:punt_return_touchdowns] ).to be  2
    end
  end
end
