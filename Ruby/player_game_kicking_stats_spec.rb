location = File.dirname __FILE__
$: << "#{location}"

require 'player_game_kicking_stats'
require 'bowls'

describe PlayerGameKickingStats do
  it 'should return a new PlayerGameKickingStats instance with all field values set' do
    player_game_kicking_stats = PlayerGameKickingStats.build 1, 2, 3, 4

    expect( player_game_kicking_stats ).to_not be_nil
    expect( player_game_kicking_stats ).to     be_a   PlayerGameKickingStats

    expect( player_game_kicking_stats.player_id            ).to eq 1
    expect( player_game_kicking_stats.season               ).to eq 2
    expect( player_game_kicking_stats.week                 ).to eq 3
    expect( player_game_kicking_stats.game                 ).to eq 4
    expect( player_game_kicking_stats.extra_point_attempts ).to be_a Integer
    expect( player_game_kicking_stats.extra_points_made    ).to be_a Integer
    expect( player_game_kicking_stats.field_goal_attempts  ).to be_a Integer
    expect( player_game_kicking_stats.field_goals_made     ).to be_a Integer
    expect( player_game_kicking_stats.punts                ).to be_a Integer
    expect( player_game_kicking_stats.punt_yards           ).to be_a Integer
  end

  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      player_game_kicking_stats = PlayerGameKickingStats.new
      expect( player_game_kicking_stats.build_insert ).to eq 'INSERT INTO Player_Game_Kicking_Stats_T ( Player_Id, Season, Week, Game, Extra_Point_Attempts, Extra_Points_Made, Field_Goal_Attempts, Field_Goals_Made, Punts, Punt_Yards ) VALUES ( :player_id, :season, :week, :game, :extra_point_attempts, :extra_points_made, :field_goal_attempts, :field_goals_made, :punts, :punt_yards )'
    end
  end

  describe '#build_select' do
    it 'should return a well formed SQL select statement' do
      player_game_kicking_stats = PlayerGameKickingStats.new
      expect( player_game_kicking_stats.build_select ).to eq 'SELECT Player_Id, Season, Week, Game, Extra_Point_Attempts, Extra_Points_Made, Field_Goal_Attempts, Field_Goals_Made, Punts, Punt_Yards FROM Player_Game_Kicking_Stats_T WHERE Player_Id = :player_id AND Season = :season AND Week = :week AND Game = :game'
    end
  end

  describe '#build_select_by_player' do
    it 'should return a well formed SQL select statement' do
      player_game_kicking_stats = PlayerGameKickingStats.new
      result = player_game_kicking_stats.build_select_by_player
      expect( result ).to eq 'SELECT Player_Id, Season, Week, Game, Extra_Point_Attempts, Extra_Points_Made, Field_Goal_Attempts, Field_Goals_Made, Punts, Punt_Yards FROM Player_Game_Kicking_Stats_T WHERE Player_Id = :player_id'
    end
  end

  describe '#build_update' do
    it 'should return a well formed SQL update statement' do
      player_game_kicking_stats = PlayerGameKickingStats.new
      expect( player_game_kicking_stats.build_update ).to eq 'UPDATE Player_Game_Kicking_Stats_T SET Extra_Point_Attempts = :extra_point_attempts, Extra_Points_Made = :extra_points_made, Field_Goal_Attempts = :field_goal_attempts, Field_Goals_Made = :field_goals_made, Punts = :punts, Punt_Yards = :punt_yards WHERE Player_Id = :player_id AND Season = :season AND Week = :week AND Game = :game'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL delete statement' do
      player_game_kicking_stats = PlayerGameKickingStats.new
      expect( player_game_kicking_stats.build_delete ).to eq 'DELETE FROM Player_Game_Kicking_Stats_T WHERE Player_Id = :player_id AND Season = :season AND Week = :week AND Game = :game'
    end
  end

  describe '#to_hash' do
    it 'should return a hash of all fields' do
      player_game_kicking_stats = PlayerGameKickingStats.new 1, 2, 3, 4

      player_game_kicking_stats.extra_point_attempts = 23
      player_game_kicking_stats.extra_points_made    = 22
      player_game_kicking_stats.field_goal_attempts  = 26
      player_game_kicking_stats.field_goals_made     = 21
      player_game_kicking_stats.punts                = 31
      player_game_kicking_stats.punt_yards           = 494

      result = player_game_kicking_stats.to_hash

      expect( result.keys.length              ).to eq 10
      expect( result[ :player_id            ] ).to eq 1
      expect( result[ :season               ] ).to eq 2
      expect( result[ :week                 ] ).to eq 3
      expect( result[ :game                 ] ).to eq 4
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
      player_game_kicking_stats = PlayerGameKickingStats.new 1, 2, 3, 4

      result = player_game_kicking_stats.key_hash

      expect( result.keys.length   ).to eq 4
      expect( result[ :player_id ] ).to eq 1
      expect( result[ :season    ] ).to eq 2
      expect( result[ :week      ] ).to eq 3
      expect( result[ :game      ] ).to eq 4
    end
  end

  describe '#from_hash' do
    it 'should set fields to values from given hash' do
      player_game_kicking_stats = PlayerGameKickingStats.new
      hash = {
        player_id:            1,
        season:               2,
        week:                 3,
        game:                 4,
        extra_point_attempts: 23,
        extra_points_made:    22,
        field_goal_attempts:  26,
        field_goals_made:     21,
        punts:                31,
        punt_yards:           494
      }

      player_game_kicking_stats.from_hash hash

      expect( player_game_kicking_stats.player_id            ).to eq hash[ :player_id            ]
      expect( player_game_kicking_stats.season               ).to eq hash[ :season               ]
      expect( player_game_kicking_stats.week                 ).to eq hash[ :week                 ]
      expect( player_game_kicking_stats.game                 ).to eq hash[ :game                 ]
      expect( player_game_kicking_stats.extra_point_attempts ).to eq hash[ :extra_point_attempts ]
      expect( player_game_kicking_stats.extra_points_made    ).to eq hash[ :extra_points_made    ]
      expect( player_game_kicking_stats.field_goal_attempts  ).to eq hash[ :field_goal_attempts  ]
      expect( player_game_kicking_stats.field_goals_made     ).to eq hash[ :field_goals_made     ]
      expect( player_game_kicking_stats.punts                ).to eq hash[ :punts                ]
      expect( player_game_kicking_stats.punt_yards           ).to eq hash[ :punt_yards           ]
    end
  end
end
