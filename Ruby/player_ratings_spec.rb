location = File.dirname __FILE__
$: << "#{location}"

require 'player_ratings'

describe PlayerRatings do
  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      player_ratings = PlayerRatings.new
      expect( player_ratings.build_insert ).to eq 'INSERT INTO Player_Ratings_T ( Player_Id, Run_Speed, Rush_Power, Max_Speed, Hit_Power ) VALUES ( :player_id, :run_speed, :rush_power, :max_speed, :hit_power )'
    end
  end

  describe '#build_select' do
    it 'should return a well formed SQL select statement' do
      player_ratings = PlayerRatings.new
      expect( player_ratings.build_select ).to eq 'SELECT Player_Id, Run_Speed, Rush_Power, Max_Speed, Hit_Power FROM Player_Ratings_T WHERE Player_Id = :player_id'
    end
  end

  describe '#build_update' do
    it 'should return a well formed SQL update statement' do
      player_ratings = PlayerRatings.new
      expect( player_ratings.build_update ).to eq 'UPDATE Player_Ratings_T SET Run_Speed = :run_speed, Rush_Power = :rush_power, Max_Speed = :max_speed, Hit_Power = :hit_power WHERE Player_Id = :player_id'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL delete statement' do
      player_ratings = PlayerRatings.new
      expect( player_ratings.build_delete ).to eq 'DELETE FROM Player_Ratings_T WHERE Player_Id = :player_id'
    end
  end

  describe '#to_hash' do
    it 'should return a hash of all fields' do
      player_ratings = PlayerRatings.new 1

      player_ratings.run_speed  = 2
      player_ratings.rush_power = 13
      player_ratings.max_speed  = 1
      player_ratings.hit_power  = 3

      result = player_ratings.to_hash

      expect( result.keys.length    ).to eq 5
      expect( result[ :player_id  ] ).to eq 1
      expect( result[ :run_speed  ] ).to eq 2
      expect( result[ :rush_power ] ).to eq 13
      expect( result[ :max_speed  ] ).to eq 1
      expect( result[ :hit_power  ] ).to eq 3
    end
  end

  describe '#key_hash' do
    it 'should return a hash of the key fields' do
      player_ratings = PlayerRatings.new 1

      result = player_ratings.key_hash

      expect( result.keys.length   ).to eq 1
      expect( result[ :player_id ] ).to eq 1
    end
  end

  describe '#from_hash' do
    it 'should set fields to values from given hash' do
      player_ratings = PlayerRatings.new
      hash = {
        player_id:  1,
        run_speed:  2,
        rush_power: 13,
        max_speed:  1,
        hit_power:  3
      }

      player_ratings.from_hash hash

      expect( player_ratings.player_id  ).to eq hash[ :player_id  ]
      expect( player_ratings.run_speed  ).to eq hash[ :run_speed  ]
      expect( player_ratings.rush_power ).to eq hash[ :rush_power ]
      expect( player_ratings.max_speed  ).to eq hash[ :max_speed  ]
      expect( player_ratings.hit_power  ).to eq hash[ :hit_power  ]
    end
  end
end
