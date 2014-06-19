location = File.dirname __FILE__
$: << "#{location}"

require 'team_kicking_stats'

describe TeamKickingStats do
  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      team_kicking_stats = TeamKickingStats.new
      expect( team_kicking_stats.build_insert ).to eq 'INSERT INTO Team_Kicking_Stats_T ( Team_Id, Season, Bowl_Game, Extra_Point_Attempts, Extra_Points_Made, Field_Goal_Attempts, Field_Goals_Made, Punts, Punt_Yards, Kick_Returns, Kick_Return_Yards, Kick_Return_Touchdowns, Punt_Returns, Punt_Return_Yards, Punt_Return_Touchdowns ) VALUES ( :team_id, :season, :bowl_game, :extra_point_attempts, :extra_points_made, :field_goal_attempts, :field_goals_made, :punts, :punt_yards, :kick_returns, :kick_return_yards, :kick_return_touchdowns, :punt_returns, :punt_return_yards, :punt_return_touchdowns )'
    end
  end

  describe '#build_select' do
    it 'should return a well formed SQL select statement' do
      team_kicking_stats = TeamKickingStats.new
      expect( team_kicking_stats.build_select ).to eq 'SELECT Team_Id, Season, Bowl_Game, Extra_Point_Attempts, Extra_Points_Made, Field_Goal_Attempts, Field_Goals_Made, Punts, Punt_Yards, Kick_Returns, Kick_Return_Yards, Kick_Return_Touchdowns, Punt_Returns, Punt_Return_Yards, Punt_Return_Touchdowns FROM Team_Kicking_Stats_T WHERE Team_Id = :team_id AND Season = :season AND Bowl_Game = :bowl_game'
    end
  end

  describe '#build_select_by_team' do
    it 'should return a well formed SQL select statement' do
      team_kicking_stats = TeamKickingStats.new
      result = team_kicking_stats.build_select_by_team
      expect( result ).to eq 'SELECT Team_Id, Season, Bowl_Game, Extra_Point_Attempts, Extra_Points_Made, Field_Goal_Attempts, Field_Goals_Made, Punts, Punt_Yards, Kick_Returns, Kick_Return_Yards, Kick_Return_Touchdowns, Punt_Returns, Punt_Return_Yards, Punt_Return_Touchdowns FROM Team_Kicking_Stats_T WHERE Team_Id = :team_id'
    end
  end

  describe '#build_update' do
    it 'should return a well formed SQL update statement' do
      team_kicking_stats = TeamKickingStats.new
      expect( team_kicking_stats.build_update ).to eq 'UPDATE Team_Kicking_Stats_T SET Extra_Point_Attempts = :extra_point_attempts, Extra_Points_Made = :extra_points_made, Field_Goal_Attempts = :field_goal_attempts, Field_Goals_Made = :field_goals_made, Punts = :punts, Punt_Yards = :punt_yards, Kick_Returns = :kick_returns, Kick_Return_Yards = :kick_return_yards, Kick_Return_Touchdowns = :kick_return_touchdowns, Punt_Returns = :punt_returns, Punt_Return_Yards = :punt_return_yards, Punt_Return_Touchdowns = :punt_return_touchdowns WHERE Team_Id = :team_id AND Season = :season AND Bowl_Game = :bowl_game'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL delete statement' do
      team_kicking_stats = TeamKickingStats.new
      expect( team_kicking_stats.build_delete ).to eq 'DELETE FROM Team_Kicking_Stats_T WHERE Team_Id = :team_id AND Season = :season AND Bowl_Game = :bowl_game'
    end
  end

  describe '#to_hash' do
    it 'should return a hash of all fields' do
      team_kicking_stats = TeamKickingStats.new 1, 2, 3

      team_kicking_stats.extra_point_attempts   = 21
      team_kicking_stats.extra_points_made      = 20
      team_kicking_stats.field_goal_attempts    = 25
      team_kicking_stats.field_goals_made       = 18
      team_kicking_stats.punts                  = 34
      team_kicking_stats.punt_yards             = 162
      team_kicking_stats.kick_returns           = 27
      team_kicking_stats.kick_return_yards      = 141
      team_kicking_stats.kick_return_touchdowns = 1
      team_kicking_stats.punt_returns           = 36
      team_kicking_stats.punt_return_yards      = 108
      team_kicking_stats.punt_return_touchdowns = 2

      result = team_kicking_stats.to_hash

      expect( result.keys.length                ).to eq 15
      expect( result[ :team_id                ] ).to eq 1
      expect( result[ :season                 ] ).to eq 2
      expect( result[ :bowl_game              ] ).to eq 3
      expect( result[ :extra_point_attempts   ] ).to eq 21
      expect( result[ :extra_points_made      ] ).to eq 20
      expect( result[ :field_goal_attempts    ] ).to eq 25
      expect( result[ :field_goals_made       ] ).to eq 18
      expect( result[ :punts                  ] ).to eq 34
      expect( result[ :punt_yards             ] ).to eq 162
      expect( result[ :kick_returns           ] ).to eq 27
      expect( result[ :kick_return_yards      ] ).to eq 141
      expect( result[ :kick_return_touchdowns ] ).to eq 1
      expect( result[ :punt_returns           ] ).to eq 36
      expect( result[ :punt_return_yards      ] ).to eq 108
      expect( result[ :punt_return_touchdowns ] ).to eq 2
    end
  end

  describe '#key_hash' do
    it 'should return a hash of the key fields' do
      team_kicking_stats = TeamKickingStats.new 1, 2, 3

      result = team_kicking_stats.key_hash

      expect( result.keys.length   ).to eq 3
      expect( result[ :team_id   ] ).to eq 1
      expect( result[ :season    ] ).to eq 2
      expect( result[ :bowl_game ] ).to eq 3
    end
  end

  describe '#from_hash' do
    it 'should set fields to values from given hash' do
      team_kicking_stats = TeamKickingStats.new
      hash = {
        team_id:                1,
        season:                 2,
        bowl_game:              3,
        extra_point_attempts:   21,
        extra_points_made:      20,
        field_goal_attempts:    25,
        field_goals_made:       18,
        punts:                  34,
        punt_yards:             162,
        kick_returns:           27,
        kick_return_yards:      141,
        kick_return_touchdowns: 1,
        punt_returns:           36,
        punt_return_yards:      108,
        punt_return_touchdowns: 2
      }

      team_kicking_stats.from_hash hash

      expect( team_kicking_stats.team_id                ).to eq hash[ :team_id                ]
      expect( team_kicking_stats.season                 ).to eq hash[ :season                 ]
      expect( team_kicking_stats.bowl_game              ).to eq hash[ :bowl_game              ]
      expect( team_kicking_stats.extra_point_attempts   ).to eq hash[ :extra_point_attempts   ]
      expect( team_kicking_stats.extra_points_made      ).to eq hash[ :extra_points_made      ]
      expect( team_kicking_stats.field_goal_attempts    ).to eq hash[ :field_goal_attempts    ]
      expect( team_kicking_stats.field_goals_made       ).to eq hash[ :field_goals_made       ]
      expect( team_kicking_stats.punts                  ).to eq hash[ :punts                  ]
      expect( team_kicking_stats.punt_yards             ).to eq hash[ :punt_yards             ]
      expect( team_kicking_stats.kick_returns           ).to eq hash[ :kick_returns           ]
      expect( team_kicking_stats.kick_return_yards      ).to eq hash[ :kick_return_yards      ]
      expect( team_kicking_stats.kick_return_touchdowns ).to eq hash[ :kick_return_touchdowns ]
      expect( team_kicking_stats.punt_returns           ).to eq hash[ :punt_returns           ]
      expect( team_kicking_stats.punt_return_yards      ).to eq hash[ :punt_return_yards      ]
      expect( team_kicking_stats.punt_return_touchdowns ).to eq hash[ :punt_return_touchdowns ]
    end
  end
end
