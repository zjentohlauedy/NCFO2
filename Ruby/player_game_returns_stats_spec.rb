location = File.dirname __FILE__
$: << "#{location}"

require 'player_game_returns_stats'
require 'bowls'

describe PlayerGameReturnsStats do
  it 'should return a new PlayerGameReturnsStats instance with all field values set' do
    player_game_returns_stats = PlayerGameReturnsStats.build 1, 2, 3, 4

    expect( player_game_returns_stats ).to_not be_nil
    expect( player_game_returns_stats ).to     be_a   PlayerGameReturnsStats

    expect( player_game_returns_stats.player_id              ).to eq 1
    expect( player_game_returns_stats.season                 ).to eq 2
    expect( player_game_returns_stats.week                   ).to eq 3
    expect( player_game_returns_stats.game                   ).to eq 4
    expect( player_game_returns_stats.kick_returns           ).to be_a Integer
    expect( player_game_returns_stats.kick_return_yards      ).to be_a Integer
    expect( player_game_returns_stats.kick_return_touchdowns ).to be_a Integer
    expect( player_game_returns_stats.punt_returns           ).to be_a Integer
    expect( player_game_returns_stats.punt_return_yards      ).to be_a Integer
    expect( player_game_returns_stats.punt_return_touchdowns ).to be_a Integer
  end

  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      player_game_returns_stats = PlayerGameReturnsStats.new
      expect( player_game_returns_stats.build_insert ).to eq 'INSERT INTO Player_Game_Returns_Stats_T ( Player_Id, Season, Week, Game, Kick_Returns, Kick_Return_Yards, Kick_Return_Touchdowns, Punt_Returns, Punt_Return_Yards, Punt_Return_Touchdowns ) VALUES ( :player_id, :season, :week, :game, :kick_returns, :kick_return_yards, :kick_return_touchdowns, :punt_returns, :punt_return_yards, :punt_return_touchdowns )'
    end
  end

  describe '#build_select' do
    it 'should return a well formed SQL select statement' do
      player_game_returns_stats = PlayerGameReturnsStats.new
      expect( player_game_returns_stats.build_select ).to eq 'SELECT Player_Id, Season, Week, Game, Kick_Returns, Kick_Return_Yards, Kick_Return_Touchdowns, Punt_Returns, Punt_Return_Yards, Punt_Return_Touchdowns FROM Player_Game_Returns_Stats_T WHERE Player_Id = :player_id AND Season = :season AND Week = :week AND Game = :game'
    end
  end

  describe '#build_select_by_player' do
    it 'should return a well formed SQL select statement' do
      player_game_returns_stats = PlayerGameReturnsStats.new
      result = player_game_returns_stats.build_select_by_player
      expect( result ).to eq 'SELECT Player_Id, Season, Week, Game, Kick_Returns, Kick_Return_Yards, Kick_Return_Touchdowns, Punt_Returns, Punt_Return_Yards, Punt_Return_Touchdowns FROM Player_Game_Returns_Stats_T WHERE Player_Id = :player_id'
    end
  end

  describe '#build_update' do
    it 'should return a well formed SQL update statement' do
      player_game_returns_stats = PlayerGameReturnsStats.new
      expect( player_game_returns_stats.build_update ).to eq 'UPDATE Player_Game_Returns_Stats_T SET Kick_Returns = :kick_returns, Kick_Return_Yards = :kick_return_yards, Kick_Return_Touchdowns = :kick_return_touchdowns, Punt_Returns = :punt_returns, Punt_Return_Yards = :punt_return_yards, Punt_Return_Touchdowns = :punt_return_touchdowns WHERE Player_Id = :player_id AND Season = :season AND Week = :week AND Game = :game'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL delete statement' do
      player_game_returns_stats = PlayerGameReturnsStats.new
      expect( player_game_returns_stats.build_delete ).to eq 'DELETE FROM Player_Game_Returns_Stats_T WHERE Player_Id = :player_id AND Season = :season AND Week = :week AND Game = :game'
    end
  end

  describe '#to_hash' do
    it 'should return a hash of all fields' do
      player_game_returns_stats = PlayerGameReturnsStats.new 1, 2, 3, 4

      player_game_returns_stats.kick_returns           = 34
      player_game_returns_stats.kick_return_yards      = 583
      player_game_returns_stats.kick_return_touchdowns = 2
      player_game_returns_stats.punt_returns           = 27
      player_game_returns_stats.punt_return_yards      = 255
      player_game_returns_stats.punt_return_touchdowns = 1

      result = player_game_returns_stats.to_hash

      expect( result.keys.length                ).to eq 10
      expect( result[ :player_id              ] ).to eq 1
      expect( result[ :season                 ] ).to eq 2
      expect( result[ :week                   ] ).to eq 3
      expect( result[ :game                   ] ).to eq 4
      expect( result[ :kick_returns           ] ).to eq 34
      expect( result[ :kick_return_yards      ] ).to eq 583
      expect( result[ :kick_return_touchdowns ] ).to eq 2
      expect( result[ :punt_returns           ] ).to eq 27
      expect( result[ :punt_return_yards      ] ).to eq 255
      expect( result[ :punt_return_touchdowns ] ).to eq 1
    end
  end

  describe '#key_hash' do
    it 'should return a hash of the key fields' do
      player_game_returns_stats = PlayerGameReturnsStats.new 1, 2, 3, 4

      result = player_game_returns_stats.key_hash

      expect( result.keys.length   ).to eq 4
      expect( result[ :player_id ] ).to eq 1
      expect( result[ :season    ] ).to eq 2
      expect( result[ :week      ] ).to eq 3
      expect( result[ :game      ] ).to eq 4
    end
  end

  describe '#from_hash' do
    it 'should set fields to values from given hash' do
      player_game_returns_stats = PlayerGameReturnsStats.new
      hash = {
        player_id:              1,
        season:                 2,
        week:                   3,
        game:                   4,
        kick_returns:           34,
        kick_return_yards:      583,
        kick_return_touchdowns: 2,
        punt_returns:           27,
        punt_return_yards:      255,
        punt_return_touchdowns: 1
      }

      player_game_returns_stats.from_hash hash

      expect( player_game_returns_stats.player_id              ).to eq hash[ :player_id              ]
      expect( player_game_returns_stats.season                 ).to eq hash[ :season                 ]
      expect( player_game_returns_stats.week                   ).to eq hash[ :week                   ]
      expect( player_game_returns_stats.game                   ).to eq hash[ :game                   ]
      expect( player_game_returns_stats.kick_returns           ).to eq hash[ :kick_returns           ]
      expect( player_game_returns_stats.kick_return_yards      ).to eq hash[ :kick_return_yards      ]
      expect( player_game_returns_stats.kick_return_touchdowns ).to eq hash[ :kick_return_touchdowns ]
      expect( player_game_returns_stats.punt_returns           ).to eq hash[ :punt_returns           ]
      expect( player_game_returns_stats.punt_return_yards      ).to eq hash[ :punt_return_yards      ]
      expect( player_game_returns_stats.punt_return_touchdowns ).to eq hash[ :punt_return_touchdowns ]
    end
  end
end
