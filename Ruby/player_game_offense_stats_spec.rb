location = File.dirname __FILE__
$: << "#{location}"

require 'bowls'
require 'player_game_offense_stats'

describe PlayerGameOffenseStats do
  it 'should return a new PlayerGameOffenseStats instance with all field values set' do
    player_game_offense_stats = PlayerGameOffenseStats.build 1, 2, 3, 4

    expect( player_game_offense_stats ).to_not be_nil
    expect( player_game_offense_stats ).to     be_a   PlayerGameOffenseStats

    expect( player_game_offense_stats.player_id            ).to eq 1
    expect( player_game_offense_stats.season               ).to eq 2
    expect( player_game_offense_stats.week                 ).to eq 3
    expect( player_game_offense_stats.game                 ).to eq 4
    expect( player_game_offense_stats.pass_attempts        ).to be_a Integer
    expect( player_game_offense_stats.completions          ).to be_a Integer
    expect( player_game_offense_stats.interceptions        ).to be_a Integer
    expect( player_game_offense_stats.pass_yards           ).to be_a Integer
    expect( player_game_offense_stats.pass_touchdowns      ).to be_a Integer
    expect( player_game_offense_stats.rush_attempts        ).to be_a Integer
    expect( player_game_offense_stats.rush_yards           ).to be_a Integer
    expect( player_game_offense_stats.rush_touchdowns      ).to be_a Integer
    expect( player_game_offense_stats.receptions           ).to be_a Integer
    expect( player_game_offense_stats.receiving_yards      ).to be_a Integer
    expect( player_game_offense_stats.receiving_touchdowns ).to be_a Integer
    expect( player_game_offense_stats.injuries             ).to be_a Integer
  end

  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      player_game_offense_stats = PlayerGameOffenseStats.new
      expect( player_game_offense_stats.build_insert ).to eq 'INSERT INTO Player_Game_Offense_Stats_T ( Player_Id, Season, Week, Game, Pass_Attempts, Completions, Interceptions, Pass_Yards, Pass_Touchdowns, Rush_Attempts, Rush_Yards, Rush_Touchdowns, Receptions, Receiving_Yards, Receiving_Touchdowns, Injuries ) VALUES ( :player_id, :season, :week, :game, :pass_attempts, :completions, :interceptions, :pass_yards, :pass_touchdowns, :rush_attempts, :rush_yards, :rush_touchdowns, :receptions, :receiving_yards, :receiving_touchdowns, :injuries )'
    end
  end

  describe '#build_select' do
    it 'should return a well formed SQL select statement' do
      player_game_offense_stats = PlayerGameOffenseStats.new
      expect( player_game_offense_stats.build_select ).to eq 'SELECT Player_Id, Season, Week, Game, Pass_Attempts, Completions, Interceptions, Pass_Yards, Pass_Touchdowns, Rush_Attempts, Rush_Yards, Rush_Touchdowns, Receptions, Receiving_Yards, Receiving_Touchdowns, Injuries FROM Player_Game_Offense_Stats_T WHERE Player_Id = :player_id AND Season = :season AND Week = :week AND Game = :game'
    end
  end

  describe '#build_select_by_player' do
    it 'should return a well formed SQL select statement' do
      player_game_offense_stats = PlayerGameOffenseStats.new
      result = player_game_offense_stats.build_select_by_player
      expect( result ).to eq 'SELECT Player_Id, Season, Week, Game, Pass_Attempts, Completions, Interceptions, Pass_Yards, Pass_Touchdowns, Rush_Attempts, Rush_Yards, Rush_Touchdowns, Receptions, Receiving_Yards, Receiving_Touchdowns, Injuries FROM Player_Game_Offense_Stats_T WHERE Player_Id = :player_id'
    end
  end

  describe '#build_update' do
    it 'should return a well formed SQL update statement' do
      player_game_offense_stats = PlayerGameOffenseStats.new
      expect( player_game_offense_stats.build_update ).to eq 'UPDATE Player_Game_Offense_Stats_T SET Pass_Attempts = :pass_attempts, Completions = :completions, Interceptions = :interceptions, Pass_Yards = :pass_yards, Pass_Touchdowns = :pass_touchdowns, Rush_Attempts = :rush_attempts, Rush_Yards = :rush_yards, Rush_Touchdowns = :rush_touchdowns, Receptions = :receptions, Receiving_Yards = :receiving_yards, Receiving_Touchdowns = :receiving_touchdowns, Injuries = :injuries WHERE Player_Id = :player_id AND Season = :season AND Week = :week AND Game = :game'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL delete statement' do
      player_game_offense_stats = PlayerGameOffenseStats.new
      expect( player_game_offense_stats.build_delete ).to eq 'DELETE FROM Player_Game_Offense_Stats_T WHERE Player_Id = :player_id AND Season = :season AND Week = :week AND Game = :game'
    end
  end

  describe '#to_hash' do
    it 'should return a hash of all fields' do
      player_game_offense_stats = PlayerGameOffenseStats.new 1, 2, 3, 4

      player_game_offense_stats.pass_attempts        = 314
      player_game_offense_stats.completions          = 268
      player_game_offense_stats.interceptions        = 12
      player_game_offense_stats.pass_yards           = 2884
      player_game_offense_stats.pass_touchdowns      = 35
      player_game_offense_stats.rush_attempts        = 287
      player_game_offense_stats.rush_yards           = 1083
      player_game_offense_stats.rush_touchdowns      = 16
      player_game_offense_stats.receptions           = 83
      player_game_offense_stats.receiving_yards      = 1146
      player_game_offense_stats.receiving_touchdowns = 12
      player_game_offense_stats.injuries             = 3

      result = player_game_offense_stats.to_hash

      expect( result.keys.length              ).to eq 16
      expect( result[ :player_id            ] ).to eq 1
      expect( result[ :season               ] ).to eq 2
      expect( result[ :week                 ] ).to eq 3
      expect( result[ :game                 ] ).to eq 4
      expect( result[ :pass_attempts        ] ).to eq 314
      expect( result[ :completions          ] ).to eq 268
      expect( result[ :interceptions        ] ).to eq 12
      expect( result[ :pass_yards           ] ).to eq 2884
      expect( result[ :pass_touchdowns      ] ).to eq 35
      expect( result[ :rush_attempts        ] ).to eq 287
      expect( result[ :rush_yards           ] ).to eq 1083
      expect( result[ :rush_touchdowns      ] ).to eq 16
      expect( result[ :receptions           ] ).to eq 83
      expect( result[ :receiving_yards      ] ).to eq 1146
      expect( result[ :receiving_touchdowns ] ).to eq 12
      expect( result[ :injuries             ] ).to eq 3
    end
  end

  describe '#key_hash' do
    it 'should return a hash of the key fields' do
      player_game_offense_stats = PlayerGameOffenseStats.new 1, 2, 3, 4

      result = player_game_offense_stats.key_hash

      expect( result.keys.length   ).to eq 4
      expect( result[ :player_id ] ).to eq 1
      expect( result[ :season    ] ).to eq 2
      expect( result[ :week      ] ).to eq 3
      expect( result[ :game      ] ).to eq 4
    end
  end

  describe '#from_hash' do
    it 'should set fields to values from given hash' do
      player_game_offense_stats = PlayerGameOffenseStats.new
      hash = {
        player_id:            1,
        season:               2,
        week:                 3,
        game:                 4,
        pass_attempts:        314,
        completions:          268,
        interceptions:        12,
        pass_yards:           2884,
        pass_touchdowns:      35,
        rush_attempts:        287,
        rush_yards:           1083,
        rush_touchdowns:      16,
        receptions:           83,
        receiving_yards:      1146,
        receiving_touchdowns: 12,
        injuries:             3
      }

      player_game_offense_stats.from_hash hash

      expect( player_game_offense_stats.player_id            ).to eq hash[ :player_id            ]
      expect( player_game_offense_stats.season               ).to eq hash[ :season               ]
      expect( player_game_offense_stats.week                 ).to eq hash[ :week                 ]
      expect( player_game_offense_stats.game                 ).to eq hash[ :game                 ]
      expect( player_game_offense_stats.pass_attempts        ).to eq hash[ :pass_attempts        ]
      expect( player_game_offense_stats.completions          ).to eq hash[ :completions          ]
      expect( player_game_offense_stats.interceptions        ).to eq hash[ :interceptions        ]
      expect( player_game_offense_stats.pass_yards           ).to eq hash[ :pass_yards           ]
      expect( player_game_offense_stats.pass_touchdowns      ).to eq hash[ :pass_touchdowns      ]
      expect( player_game_offense_stats.rush_attempts        ).to eq hash[ :rush_attempts        ]
      expect( player_game_offense_stats.rush_yards           ).to eq hash[ :rush_yards           ]
      expect( player_game_offense_stats.rush_touchdowns      ).to eq hash[ :rush_touchdowns      ]
      expect( player_game_offense_stats.receptions           ).to eq hash[ :receptions           ]
      expect( player_game_offense_stats.receiving_yards      ).to eq hash[ :receiving_yards      ]
      expect( player_game_offense_stats.receiving_touchdowns ).to eq hash[ :receiving_touchdowns ]
      expect( player_game_offense_stats.injuries             ).to eq hash[ :injuries             ]
    end
  end
end
