location = File.dirname __FILE__
$: << "#{location}"

require 'player_accolade'
require 'player_accolades'

describe PlayerAccolade do
  describe '::build' do
    it 'should return a new PlayerAccolade instance with all field values set' do
      player_accolade = PlayerAccolade.build 1, 4, PlayerAccolades::Great_Lake_Passing_Champion

      expect( player_accolade ).to_not be_nil
      expect( player_accolade ).to     be_a   PlayerAccolade

      expect( player_accolade.player_id ).to eq 1
      expect( player_accolade.season    ).to eq 4
      expect( player_accolade.accolade  ).to eq PlayerAccolades::Great_Lake_Passing_Champion
    end
  end

  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      player_accolade = PlayerAccolade.new
      expect( player_accolade.build_insert ).to eq 'INSERT INTO Player_Accolades_T ( Player_Id, Season, Accolade ) VALUES ( :player_id, :season, :accolade )'
    end
  end

  describe '#build_select_by_player' do
    it 'should return a well formed SQL select statement' do
      player_accolade = PlayerAccolade.new
      result = player_accolade.build_select_by_player
      expect( result ).to eq 'SELECT Player_Id, Season, Accolade FROM Player_Accolades_T WHERE Player_Id = :player_id'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL delete statement' do
      player_accolade = PlayerAccolade.new
      expect( player_accolade.build_delete ).to eq 'DELETE FROM Player_Accolades_T WHERE Player_Id = :player_id AND Season = :season AND Accolade = :accolade'
    end
  end

  describe '#to_hash' do
    it 'should return a hash of all fields' do
      player_accolade = PlayerAccolade.new 1, 4, PlayerAccolades::Great_Lake_Passing_Champion

      result = player_accolade.to_hash

      expect( result.keys.length   ).to eq 3
      expect( result[ :player_id ] ).to eq 1
      expect( result[ :season    ] ).to eq 4
      expect( result[ :accolade  ] ).to eq PlayerAccolades::Great_Lake_Passing_Champion
    end
  end

  describe '#key_hash' do
    it 'should return a hash of the key fields' do
      player_accolade = PlayerAccolade.new 1, 4, PlayerAccolades::Great_Lake_Passing_Champion

      result = player_accolade.key_hash

      expect( result.keys.length   ).to eq 3
      expect( result[ :player_id ] ).to eq 1
      expect( result[ :season    ] ).to eq 4
      expect( result[ :accolade  ] ).to eq PlayerAccolades::Great_Lake_Passing_Champion
    end
  end

  describe '#from_hash' do
    it 'should set fields to values from given hash' do
      player_accolade = PlayerAccolade.new
      hash = { player_id: 1, season: 4, accolade: PlayerAccolades::Great_Lake_Passing_Champion }

      player_accolade.from_hash hash

      expect( player_accolade.player_id ).to eq hash[ :player_id ]
      expect( player_accolade.season    ).to eq hash[ :season    ]
      expect( player_accolade.accolade  ).to eq hash[ :accolade  ]
    end
  end
end
