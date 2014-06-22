location = File.dirname __FILE__
$: << "#{location}"

require 'player_kicking_stats'
require 'bowls'

describe PlayerKickingStats do
  it 'should return a new PlayerKickingStats instance with all field values set' do
    player_kicking_stats = PlayerKickingStats.build 1, 2, Bowls::RoseBowl

    expect( player_kicking_stats ).to_not be_nil
    expect( player_kicking_stats ).to     be_a   PlayerKickingStats

    expect( player_kicking_stats.player_id            ).to eq 1
    expect( player_kicking_stats.season               ).to eq 2
    expect( player_kicking_stats.bowl_game            ).to eq Bowls::RoseBowl
    expect( player_kicking_stats.extra_point_attempts ).to be_a Integer
    expect( player_kicking_stats.extra_points_made    ).to be_a Integer
    expect( player_kicking_stats.field_goal_attempts  ).to be_a Integer
    expect( player_kicking_stats.field_goals_made     ).to be_a Integer
    expect( player_kicking_stats.punts                ).to be_a Integer
    expect( player_kicking_stats.punt_yards           ).to be_a Integer
  end

  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      player_kicking_stats = PlayerKickingStats.new
      expect( player_kicking_stats.build_insert ).to eq 'INSERT INTO Player_Kicking_Stats_T ( Player_Id, Season, Bowl_Game, Extra_Point_Attempts, Extra_Points_Made, Field_Goal_Attempts, Field_Goals_Made, Punts, Punt_Yards ) VALUES ( :player_id, :season, :bowl_game, :extra_point_attempts, :extra_points_made, :field_goal_attempts, :field_goals_made, :punts, :punt_yards )'
    end
  end

  describe '#build_select' do
    it 'should return a well formed SQL select statement' do
      player_kicking_stats = PlayerKickingStats.new
      expect( player_kicking_stats.build_select ).to eq 'SELECT Player_Id, Season, Bowl_Game, Extra_Point_Attempts, Extra_Points_Made, Field_Goal_Attempts, Field_Goals_Made, Punts, Punt_Yards FROM Player_Kicking_Stats_T WHERE Player_Id = :player_id AND Season = :season AND Bowl_Game = :bowl_game'
    end
  end

  describe '#build_select_by_player' do
    it 'should return a well formed SQL select statement' do
      player_kicking_stats = PlayerKickingStats.new
      result = player_kicking_stats.build_select_by_player
      expect( result ).to eq 'SELECT Player_Id, Season, Bowl_Game, Extra_Point_Attempts, Extra_Points_Made, Field_Goal_Attempts, Field_Goals_Made, Punts, Punt_Yards FROM Player_Kicking_Stats_T WHERE Player_Id = :player_id'
    end
  end

  describe '#build_update' do
    it 'should return a well formed SQL update statement' do
      player_kicking_stats = PlayerKickingStats.new
      expect( player_kicking_stats.build_update ).to eq 'UPDATE Player_Kicking_Stats_T SET Extra_Point_Attempts = :extra_point_attempts, Extra_Points_Made = :extra_points_made, Field_Goal_Attempts = :field_goal_attempts, Field_Goals_Made = :field_goals_made, Punts = :punts, Punt_Yards = :punt_yards WHERE Player_Id = :player_id AND Season = :season AND Bowl_Game = :bowl_game'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL delete statement' do
      player_kicking_stats = PlayerKickingStats.new
      expect( player_kicking_stats.build_delete ).to eq 'DELETE FROM Player_Kicking_Stats_T WHERE Player_Id = :player_id AND Season = :season AND Bowl_Game = :bowl_game'
    end
  end

  describe '#to_hash' do
    it 'should return a hash of all fields' do
      player_kicking_stats = PlayerKickingStats.new 1, 2, Bowls::RoseBowl

      player_kicking_stats.extra_point_attempts = 23
      player_kicking_stats.extra_points_made    = 22
      player_kicking_stats.field_goal_attempts  = 26
      player_kicking_stats.field_goals_made     = 21
      player_kicking_stats.punts                = 31
      player_kicking_stats.punt_yards           = 494

      result = player_kicking_stats.to_hash

      expect( result.keys.length              ).to eq 9
      expect( result[ :player_id            ] ).to eq 1
      expect( result[ :season               ] ).to eq 2
      expect( result[ :bowl_game            ] ).to eq Bowls::RoseBowl
      expect( result[ :extra_point_attempts ] ).to eq 23
      expect( result[ :extra_points_made    ] ).to eq 22
      expect( result[ :field_goal_attempts  ] ).to eq 26
      expect( result[ :field_goals_made     ] ).to eq 21
      expect( result[ :punts                ] ).to eq 31
      expect( result[ :punt_yards           ] ).to eq 494
    end
  end

  describe '#key_hash' do
    it 'should return a hash of the key fields' do
      player_kicking_stats = PlayerKickingStats.new 1, 2, Bowls::RoseBowl

      result = player_kicking_stats.key_hash

      expect( result.keys.length   ).to eq 3
      expect( result[ :player_id ] ).to eq 1
      expect( result[ :season    ] ).to eq 2
      expect( result[ :bowl_game ] ).to eq Bowls::RoseBowl
    end
  end

  describe '#from_hash' do
    it 'should set fields to values from given hash' do
      player_kicking_stats = PlayerKickingStats.new
      hash = {
        player_id:            1,
        season:               2,
        bowl_game:            Bowls::RoseBowl,
        extra_point_attempts: 23,
        extra_points_made:    22,
        field_goal_attempts:  26,
        field_goals_made:     21,
        punts:                31,
        punt_yards:           494
      }

      player_kicking_stats.from_hash hash

      expect( player_kicking_stats.player_id            ).to eq hash[ :player_id            ]
      expect( player_kicking_stats.season               ).to eq hash[ :season               ]
      expect( player_kicking_stats.bowl_game            ).to eq hash[ :bowl_game            ]
      expect( player_kicking_stats.extra_point_attempts ).to eq hash[ :extra_point_attempts ]
      expect( player_kicking_stats.extra_points_made    ).to eq hash[ :extra_points_made    ]
      expect( player_kicking_stats.field_goal_attempts  ).to eq hash[ :field_goal_attempts  ]
      expect( player_kicking_stats.field_goals_made     ).to eq hash[ :field_goals_made     ]
      expect( player_kicking_stats.punts                ).to eq hash[ :punts                ]
      expect( player_kicking_stats.punt_yards           ).to eq hash[ :punt_yards           ]
    end
  end
end
