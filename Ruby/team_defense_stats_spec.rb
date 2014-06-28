location = File.dirname __FILE__
$: << "#{location}"

require 'team_defense_stats'
require 'bowls'

describe TeamDefenseStats do
  describe '::build' do
    it 'should return a new TeamDefenseStats instance with all field values set' do
      team_defense_stats = TeamDefenseStats.build 1, 2, Bowls::SugarBowl

      expect( team_defense_stats ).to_not be_nil
      expect( team_defense_stats ).to     be_a   TeamDefenseStats

      expect( team_defense_stats.team_id           ).to eq 1
      expect( team_defense_stats.season            ).to eq 2
      expect( team_defense_stats.bowl_game         ).to eq Bowls::SugarBowl
      expect( team_defense_stats.sacks             ).to be_a Integer
      expect( team_defense_stats.interceptions     ).to be_a Integer
      expect( team_defense_stats.return_yards      ).to be_a Integer
      expect( team_defense_stats.return_touchdowns ).to be_a Integer
    end
  end

  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      team_defense_stats = TeamDefenseStats.new
      expect( team_defense_stats.build_insert ).to eq 'INSERT INTO Team_Defense_Stats_T ( Team_Id, Season, Bowl_Game, Sacks, Interceptions, Return_Yards, Return_Touchdowns ) VALUES ( :team_id, :season, :bowl_game, :sacks, :interceptions, :return_yards, :return_touchdowns )'
    end
  end

  describe '#build_select' do
    it 'should return a well formed SQL select statement' do
      team_defense_stats = TeamDefenseStats.new
      expect( team_defense_stats.build_select ).to eq 'SELECT Team_Id, Season, Bowl_Game, Sacks, Interceptions, Return_Yards, Return_Touchdowns FROM Team_Defense_Stats_T WHERE Team_Id = :team_id AND Season = :season AND Bowl_Game = :bowl_game'
    end
  end

  describe '#build_select_by_team' do
    it 'should return a well formed SQL select statement' do
      team_defense_stats = TeamDefenseStats.new
      result = team_defense_stats.build_select_by_team
      expect( result ).to eq 'SELECT Team_Id, Season, Bowl_Game, Sacks, Interceptions, Return_Yards, Return_Touchdowns FROM Team_Defense_Stats_T WHERE Team_Id = :team_id'
    end
  end

  describe '#build_update' do
    it 'should return a well formed SQL update statement' do
      team_defense_stats = TeamDefenseStats.new
      expect( team_defense_stats.build_update ).to eq 'UPDATE Team_Defense_Stats_T SET Sacks = :sacks, Interceptions = :interceptions, Return_Yards = :return_yards, Return_Touchdowns = :return_touchdowns WHERE Team_Id = :team_id AND Season = :season AND Bowl_Game = :bowl_game'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL delete statement' do
      team_defense_stats = TeamDefenseStats.new
      expect( team_defense_stats.build_delete ).to eq 'DELETE FROM Team_Defense_Stats_T WHERE Team_Id = :team_id AND Season = :season AND Bowl_Game = :bowl_game'
    end
  end

  describe '#to_hash' do
    it 'should return a hash of all fields' do
      team_defense_stats = TeamDefenseStats.new 1, 2, Bowls::SugarBowl

      team_defense_stats.sacks             = 18
      team_defense_stats.interceptions     = 7
      team_defense_stats.return_yards      = 118
      team_defense_stats.return_touchdowns = 1

      result = team_defense_stats.to_hash

      expect( result.keys.length           ).to eq 7
      expect( result[ :team_id           ] ).to eq 1
      expect( result[ :season            ] ).to eq 2
      expect( result[ :bowl_game         ] ).to eq Bowls::SugarBowl
      expect( result[ :sacks             ] ).to eq 18
      expect( result[ :interceptions     ] ).to eq 7
      expect( result[ :return_yards      ] ).to eq 118
      expect( result[ :return_touchdowns ] ).to eq 1
    end
  end

  describe '#key_hash' do
    it 'should return a hash of the key fields' do
      team_defense_stats = TeamDefenseStats.new 1, 2, Bowls::SugarBowl

      result = team_defense_stats.key_hash

      expect( result.keys.length   ).to eq 3
      expect( result[ :team_id   ] ).to eq 1
      expect( result[ :season    ] ).to eq 2
      expect( result[ :bowl_game ] ).to eq Bowls::SugarBowl
    end
  end

  describe '#from_hash' do
    it 'should set fields to values from given hash' do
      team_defense_stats = TeamDefenseStats.new
      hash = {
        team_id:           1,
        season:            2,
        bowl_game:         Bowls::SugarBowl,
        sacks:             18,
        interceptions:     7,
        return_yards:      118,
        return_touchdowns: 1
      }

      team_defense_stats.from_hash hash

      expect( team_defense_stats.team_id           ).to eq hash[ :team_id           ]
      expect( team_defense_stats.season            ).to eq hash[ :season            ]
      expect( team_defense_stats.bowl_game         ).to eq hash[ :bowl_game         ]
      expect( team_defense_stats.sacks             ).to eq hash[ :sacks             ]
      expect( team_defense_stats.interceptions     ).to eq hash[ :interceptions     ]
      expect( team_defense_stats.return_yards      ).to eq hash[ :return_yards      ]
      expect( team_defense_stats.return_touchdowns ).to eq hash[ :return_touchdowns ]
    end
  end
end
