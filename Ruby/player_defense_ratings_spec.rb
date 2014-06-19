location = File.dirname __FILE__
$: << "#{location}"

require 'player_defense_ratings'

describe PlayerDefenseRatings do
  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      player_defense_ratings = PlayerDefenseRatings.new
      expect( player_defense_ratings.build_insert ).to eq 'INSERT INTO Player_Defense_Ratings_T ( Player_Id, Interceptions, Quickness ) VALUES ( :player_id, :interceptions, :quickness )'
    end
  end

  describe '#build_select' do
    it 'should return a well formed SQL select statement' do
      player_defense_ratings = PlayerDefenseRatings.new
      expect( player_defense_ratings.build_select ).to eq 'SELECT Player_Id, Interceptions, Quickness FROM Player_Defense_Ratings_T WHERE Player_Id = :player_id'
    end
  end

  describe '#build_update' do
    it 'should return a well formed SQL update statement' do
      player_defense_ratings = PlayerDefenseRatings.new
      expect( player_defense_ratings.build_update ).to eq 'UPDATE Player_Defense_Ratings_T SET Interceptions = :interceptions, Quickness = :quickness WHERE Player_Id = :player_id'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL delete statement' do
      player_defense_ratings = PlayerDefenseRatings.new
      expect( player_defense_ratings.build_delete ).to eq 'DELETE FROM Player_Defense_Ratings_T WHERE Player_Id = :player_id'
    end
  end

  describe '#to_hash' do
    it 'should return a hash of all fields' do
      player_defense_ratings = PlayerDefenseRatings.new 1

      player_defense_ratings.interceptions = 2
      player_defense_ratings.quickness    = 13

      result = player_defense_ratings.to_hash

      expect( result.keys.length       ).to eq 3
      expect( result[ :player_id     ] ).to eq 1
      expect( result[ :interceptions ] ).to eq 2
      expect( result[ :quickness     ] ).to eq 13
    end
  end

  describe '#key_hash' do
    it 'should return a hash of the key fields' do
      player_defense_ratings = PlayerDefenseRatings.new 1

      result = player_defense_ratings.key_hash

      expect( result.keys.length   ).to eq 1
      expect( result[ :player_id ] ).to eq 1
    end
  end

  describe '#from_hash' do
    it 'should set fields to values from given hash' do
      player_defense_ratings = PlayerDefenseRatings.new
      hash = {
        player_id:     1,
        interceptions: 13,
        quickness:     3
      }

      player_defense_ratings.from_hash hash

      expect( player_defense_ratings.player_id     ).to eq hash[ :player_id     ]
      expect( player_defense_ratings.interceptions ).to eq hash[ :interceptions ]
      expect( player_defense_ratings.quickness     ).to eq hash[ :quickness     ]
    end
  end
end
