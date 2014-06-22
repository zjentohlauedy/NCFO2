location = File.dirname __FILE__
$: << "#{location}"

require 'player_defense_stats'
require 'bowls'

describe PlayerDefenseStats do
  it 'should return a new PlayerDefenseStats instance with all field values set' do
    player_defense_stats = PlayerDefenseStats.build 1, 2, Bowls::RoseBowl

    expect( player_defense_stats ).to_not be_nil
    expect( player_defense_stats ).to     be_a   PlayerDefenseStats

    expect( player_defense_stats.player_id         ).to eq 1
    expect( player_defense_stats.season            ).to eq 2
    expect( player_defense_stats.bowl_game         ).to eq Bowls::RoseBowl
    expect( player_defense_stats.sacks             ).to be_a Integer
    expect( player_defense_stats.interceptions     ).to be_a Integer
    expect( player_defense_stats.return_yards      ).to be_a Integer
    expect( player_defense_stats.return_touchdowns ).to be_a Integer
  end

  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      player_defense_stats = PlayerDefenseStats.new
      expect( player_defense_stats.build_insert ).to eq 'INSERT INTO Player_Defense_Stats_T ( Player_Id, Season, Bowl_Game, Sacks, Interceptions, Return_Yards, Return_Touchdowns ) VALUES ( :player_id, :season, :bowl_game, :sacks, :interceptions, :return_yards, :return_touchdowns )'
    end
  end

  describe '#build_select' do
    it 'should return a well formed SQL select statement' do
      player_defense_stats = PlayerDefenseStats.new
      expect( player_defense_stats.build_select ).to eq 'SELECT Player_Id, Season, Bowl_Game, Sacks, Interceptions, Return_Yards, Return_Touchdowns FROM Player_Defense_Stats_T WHERE Player_Id = :player_id AND Season = :season AND Bowl_Game = :bowl_game'
    end
  end

  describe '#build_select_by_player' do
    it 'should return a well formed SQL select statement' do
      player_defense_stats = PlayerDefenseStats.new
      result = player_defense_stats.build_select_by_player
      expect( result ).to eq 'SELECT Player_Id, Season, Bowl_Game, Sacks, Interceptions, Return_Yards, Return_Touchdowns FROM Player_Defense_Stats_T WHERE Player_Id = :player_id'
    end
  end

  describe '#build_update' do
    it 'should return a well formed SQL update statement' do
      player_defense_stats = PlayerDefenseStats.new
      expect( player_defense_stats.build_update ).to eq 'UPDATE Player_Defense_Stats_T SET Sacks = :sacks, Interceptions = :interceptions, Return_Yards = :return_yards, Return_Touchdowns = :return_touchdowns WHERE Player_Id = :player_id AND Season = :season AND Bowl_Game = :bowl_game'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL delete statement' do
      player_defense_stats = PlayerDefenseStats.new
      expect( player_defense_stats.build_delete ).to eq 'DELETE FROM Player_Defense_Stats_T WHERE Player_Id = :player_id AND Season = :season AND Bowl_Game = :bowl_game'
    end
  end

  describe '#to_hash' do
    it 'should return a hash of all fields' do
      player_defense_stats = PlayerDefenseStats.new 1, 2, Bowls::RoseBowl

      player_defense_stats.sacks             = 18
      player_defense_stats.interceptions     = 5
      player_defense_stats.return_yards      = 116
      player_defense_stats.return_touchdowns = 1

      result = player_defense_stats.to_hash

      expect( result.keys.length           ).to eq 7
      expect( result[ :player_id         ] ).to eq 1
      expect( result[ :season            ] ).to eq 2
      expect( result[ :bowl_game         ] ).to eq Bowls::RoseBowl
      expect( result[ :sacks             ] ).to eq 18
      expect( result[ :interceptions     ] ).to eq 5
      expect( result[ :return_yards      ] ).to eq 116
      expect( result[ :return_touchdowns ] ).to eq 1
    end
  end

  describe '#key_hash' do
    it 'should return a hash of the key fields' do
      player_defense_stats = PlayerDefenseStats.new 1, 2, Bowls::RoseBowl

      result = player_defense_stats.key_hash

      expect( result.keys.length   ).to eq 3
      expect( result[ :player_id ] ).to eq 1
      expect( result[ :season    ] ).to eq 2
      expect( result[ :bowl_game ] ).to eq Bowls::RoseBowl
    end
  end

  describe '#from_hash' do
    it 'should set fields to values from given hash' do
      player_defense_stats = PlayerDefenseStats.new
      hash = {
        player_id:         1,
        season:            2,
        bowl_game:         Bowls::RoseBowl,
        sacks:             18,
        interceptions:     5,
        return_yards:      116,
        return_touchdowns: 1
      }

      player_defense_stats.from_hash hash

      expect( player_defense_stats.player_id         ).to eq hash[ :player_id         ]
      expect( player_defense_stats.season            ).to eq hash[ :season            ]
      expect( player_defense_stats.bowl_game         ).to eq hash[ :bowl_game         ]
      expect( player_defense_stats.sacks             ).to eq hash[ :sacks             ]
      expect( player_defense_stats.interceptions     ).to eq hash[ :interceptions     ]
      expect( player_defense_stats.return_yards      ).to eq hash[ :return_yards      ]
      expect( player_defense_stats.return_touchdowns ).to eq hash[ :return_touchdowns ]
    end
  end
end
