location = File.dirname __FILE__
$: << "#{location}"

require 'player_kicking_ratings'

describe PlayerKickingRatings do
  describe '::build' do
    it 'should return a new PlayerKickingRatings instance with all field values set' do
      player_kicking_ratings = PlayerKickingRatings.build 1

      expect( player_kicking_ratings ).to_not be_nil
      expect( player_kicking_ratings ).to     be_a   PlayerKickingRatings

      expect( player_kicking_ratings.player_id        ).to eq 1
      expect( player_kicking_ratings.kicking_ability  ).to be_a Integer
      expect( player_kicking_ratings.avoid_kick_block ).to be_a Integer
    end
  end

  describe '::generate' do
    it 'should return a new PlayerRatings instance with generated field values' do
      player_kicking_ratings = PlayerKickingRatings.generate 1

      expect( player_kicking_ratings ).to_not be_nil
      expect( player_kicking_ratings ).to     be_a   PlayerKickingRatings

      expect( player_kicking_ratings.player_id        ).to eq 1
      expect( player_kicking_ratings.kicking_ability  ).to be_a Integer
      expect( player_kicking_ratings.kicking_ability  ).to be >=  0
      expect( player_kicking_ratings.kicking_ability  ).to be <= 15
      expect( player_kicking_ratings.avoid_kick_block ).to be_a Integer
      expect( player_kicking_ratings.avoid_kick_block ).to be >=  0
      expect( player_kicking_ratings.avoid_kick_block ).to be <= 15
    end
  end

  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      player_kicking_ratings = PlayerKickingRatings.new
      expect( player_kicking_ratings.build_insert ).to eq 'INSERT INTO Player_Kicking_Ratings_T ( Player_Id, Kicking_Ability, Avoid_Kick_Block ) VALUES ( :player_id, :kicking_ability, :avoid_kick_block )'
    end
  end

  describe '#build_select' do
    it 'should return a well formed SQL select statement' do
      player_kicking_ratings = PlayerKickingRatings.new
      expect( player_kicking_ratings.build_select ).to eq 'SELECT Player_Id, Kicking_Ability, Avoid_Kick_Block FROM Player_Kicking_Ratings_T WHERE Player_Id = :player_id'
    end
  end

  describe '#build_update' do
    it 'should return a well formed SQL update statement' do
      player_kicking_ratings = PlayerKickingRatings.new
      expect( player_kicking_ratings.build_update ).to eq 'UPDATE Player_Kicking_Ratings_T SET Kicking_Ability = :kicking_ability, Avoid_Kick_Block = :avoid_kick_block WHERE Player_Id = :player_id'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL delete statement' do
      player_kicking_ratings = PlayerKickingRatings.new
      expect( player_kicking_ratings.build_delete ).to eq 'DELETE FROM Player_Kicking_Ratings_T WHERE Player_Id = :player_id'
    end
  end

  describe '#to_hash' do
    it 'should return a hash of all fields' do
      player_kicking_ratings = PlayerKickingRatings.new 1

      player_kicking_ratings.kicking_ability  = 2
      player_kicking_ratings.avoid_kick_block = 13

      result = player_kicking_ratings.to_hash

      expect( result.keys.length          ).to eq 3
      expect( result[ :player_id        ] ).to eq 1
      expect( result[ :kicking_ability  ] ).to eq 2
      expect( result[ :avoid_kick_block ] ).to eq 13
    end
  end

  describe '#key_hash' do
    it 'should return a hash of the key fields' do
      player_kicking_ratings = PlayerKickingRatings.new 1

      result = player_kicking_ratings.key_hash

      expect( result.keys.length   ).to eq 1
      expect( result[ :player_id ] ).to eq 1
    end
  end

  describe '#from_hash' do
    it 'should set fields to values from given hash' do
      player_kicking_ratings = PlayerKickingRatings.new
      hash = {
        player_id:        1,
        kicking_ability:  13,
        avoid_kick_block: 3
      }

      player_kicking_ratings.from_hash hash

      expect( player_kicking_ratings.player_id        ).to eq hash[ :player_id        ]
      expect( player_kicking_ratings.kicking_ability  ).to eq hash[ :kicking_ability  ]
      expect( player_kicking_ratings.avoid_kick_block ).to eq hash[ :avoid_kick_block ]
    end
  end
end
