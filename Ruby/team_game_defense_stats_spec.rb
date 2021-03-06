location = File.dirname __FILE__
$: << "#{location}"

require 'team_game_defense_stats'
require 'bowls'

describe TeamGameDefenseStats do
  describe '::build' do
    it 'should return a new TeamGameDefenseStats instance with all field values set' do
      team_game_defense_stats = TeamGameDefenseStats.build 1, 2, 3, 4

      expect( team_game_defense_stats ).to_not be_nil
      expect( team_game_defense_stats ).to     be_a   TeamGameDefenseStats

      expect( team_game_defense_stats.team_id           ).to eq 1
      expect( team_game_defense_stats.season            ).to eq 2
      expect( team_game_defense_stats.week              ).to eq 3
      expect( team_game_defense_stats.game              ).to eq 4
      expect( team_game_defense_stats.sacks             ).to be_a Integer
      expect( team_game_defense_stats.interceptions     ).to be_a Integer
      expect( team_game_defense_stats.return_yards      ).to be_a Integer
      expect( team_game_defense_stats.return_touchdowns ).to be_a Integer
      expect( team_game_defense_stats.yards_allowed     ).to be_a Integer
    end
  end

  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      team_game_defense_stats = TeamGameDefenseStats.new
      expect( team_game_defense_stats.build_insert ).to eq 'INSERT INTO Team_Game_Defense_Stats_T ( Team_Id, Season, Week, Game, Sacks, Interceptions, Return_Yards, Return_Touchdowns, Yards_Allowed ) VALUES ( :team_id, :season, :week, :game, :sacks, :interceptions, :return_yards, :return_touchdowns, :yards_allowed )'
    end
  end

  describe '#build_select' do
    it 'should return a well formed SQL select statement' do
      team_game_defense_stats = TeamGameDefenseStats.new
      expect( team_game_defense_stats.build_select ).to eq 'SELECT Team_Id, Season, Week, Game, Sacks, Interceptions, Return_Yards, Return_Touchdowns, Yards_Allowed FROM Team_Game_Defense_Stats_T WHERE Team_Id = :team_id AND Season = :season AND Week = :week AND Game = :game'
    end
  end

  describe '#build_select_by_team' do
    it 'should return a well formed SQL select statement' do
      team_game_defense_stats = TeamGameDefenseStats.new
      result = team_game_defense_stats.build_select_by_team
      expect( result ).to eq 'SELECT Team_Id, Season, Week, Game, Sacks, Interceptions, Return_Yards, Return_Touchdowns, Yards_Allowed FROM Team_Game_Defense_Stats_T WHERE Team_Id = :team_id'
    end
  end

  describe '#build_update' do
    it 'should return a well formed SQL update statement' do
      team_game_defense_stats = TeamGameDefenseStats.new
      expect( team_game_defense_stats.build_update ).to eq 'UPDATE Team_Game_Defense_Stats_T SET Sacks = :sacks, Interceptions = :interceptions, Return_Yards = :return_yards, Return_Touchdowns = :return_touchdowns, Yards_Allowed = :yards_allowed WHERE Team_Id = :team_id AND Season = :season AND Week = :week AND Game = :game'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL delete statement' do
      team_game_defense_stats = TeamGameDefenseStats.new
      expect( team_game_defense_stats.build_delete ).to eq 'DELETE FROM Team_Game_Defense_Stats_T WHERE Team_Id = :team_id AND Season = :season AND Week = :week AND Game = :game'
    end
  end

  describe '#to_hash' do
    it 'should return a hash of all fields' do
      team_game_defense_stats = TeamGameDefenseStats.new 1, 2, 3, 4

      team_game_defense_stats.sacks             = 18
      team_game_defense_stats.interceptions     = 7
      team_game_defense_stats.return_yards      = 118
      team_game_defense_stats.return_touchdowns = 1
      team_game_defense_stats.yards_allowed     = 317

      result = team_game_defense_stats.to_hash

      expect( result.keys.length           ).to eq 9
      expect( result[ :team_id           ] ).to eq 1
      expect( result[ :season            ] ).to eq 2
      expect( result[ :week              ] ).to eq 3
      expect( result[ :game              ] ).to eq 4
      expect( result[ :sacks             ] ).to eq 18
      expect( result[ :interceptions     ] ).to eq 7
      expect( result[ :return_yards      ] ).to eq 118
      expect( result[ :return_touchdowns ] ).to eq 1
      expect( result[ :yards_allowed     ] ).to eq 317
    end
  end

  describe '#key_hash' do
    it 'should return a hash of the key fields' do
      team_game_defense_stats = TeamGameDefenseStats.new 1, 2, 3, 4

      result = team_game_defense_stats.key_hash

      expect( result.keys.length   ).to eq 4
      expect( result[ :team_id ] ).to eq 1
      expect( result[ :season  ] ).to eq 2
      expect( result[ :week    ] ).to eq 3
      expect( result[ :game    ] ).to eq 4
    end
  end

  describe '#from_hash' do
    it 'should set fields to values from given hash' do
      team_game_defense_stats = TeamGameDefenseStats.new
      hash = {
        team_id:           1,
        season:            2,
        week:              3,
        game:              4,
        sacks:             18,
        interceptions:     7,
        return_yards:      118,
        return_touchdowns: 1,
        yards_allowed:     317
      }

      team_game_defense_stats.from_hash hash

      expect( team_game_defense_stats.team_id           ).to eq hash[ :team_id           ]
      expect( team_game_defense_stats.season            ).to eq hash[ :season            ]
      expect( team_game_defense_stats.week              ).to eq hash[ :week              ]
      expect( team_game_defense_stats.game              ).to eq hash[ :game              ]
      expect( team_game_defense_stats.sacks             ).to eq hash[ :sacks             ]
      expect( team_game_defense_stats.interceptions     ).to eq hash[ :interceptions     ]
      expect( team_game_defense_stats.return_yards      ).to eq hash[ :return_yards      ]
      expect( team_game_defense_stats.return_touchdowns ).to eq hash[ :return_touchdowns ]
      expect( team_game_defense_stats.yards_allowed     ).to eq hash[ :yards_allowed     ]
    end
  end
end
