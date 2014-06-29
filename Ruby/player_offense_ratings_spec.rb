location = File.dirname __FILE__
$: << "#{location}"

require 'player_offense_ratings'

describe PlayerOffenseRatings do
  describe '::build' do
    it 'should return a new PlayerOffenseRatings instance with all field values set' do
      player_offense_ratings = PlayerOffenseRatings.build 1

      expect( player_offense_ratings ).to_not be_nil
      expect( player_offense_ratings ).to     be_a   PlayerOffenseRatings

      expect( player_offense_ratings.player_id    ).to eq 1
      expect( player_offense_ratings.ball_control ).to be_a Integer
      expect( player_offense_ratings.receiving    ).to be_a Integer
    end
  end

  describe '::generate' do
    it 'should return a new PlayerRatings instance with generated field values' do
      player_offense_ratings = PlayerOffenseRatings.generate 1

      expect( player_offense_ratings ).to_not be_nil
      expect( player_offense_ratings ).to     be_a   PlayerOffenseRatings

      expect( player_offense_ratings.player_id    ).to eq 1
      expect( player_offense_ratings.ball_control ).to be_a Integer
      expect( player_offense_ratings.ball_control ).to be >=  0
      expect( player_offense_ratings.ball_control ).to be <= 15
      expect( player_offense_ratings.receiving    ).to be_a Integer
      expect( player_offense_ratings.receiving    ).to be >=  0
      expect( player_offense_ratings.receiving    ).to be <= 15
    end
  end

  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      player_offense_ratings = PlayerOffenseRatings.new
      expect( player_offense_ratings.build_insert ).to eq 'INSERT INTO Player_Offense_Ratings_T ( Player_Id, Ball_Control, Receiving ) VALUES ( :player_id, :ball_control, :receiving )'
    end
  end

  describe '#build_select' do
    it 'should return a well formed SQL select statement' do
      player_offense_ratings = PlayerOffenseRatings.new
      expect( player_offense_ratings.build_select ).to eq 'SELECT Player_Id, Ball_Control, Receiving FROM Player_Offense_Ratings_T WHERE Player_Id = :player_id'
    end
  end

  describe '#build_update' do
    it 'should return a well formed SQL update statement' do
      player_offense_ratings = PlayerOffenseRatings.new
      expect( player_offense_ratings.build_update ).to eq 'UPDATE Player_Offense_Ratings_T SET Ball_Control = :ball_control, Receiving = :receiving WHERE Player_Id = :player_id'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL delete statement' do
      player_offense_ratings = PlayerOffenseRatings.new
      expect( player_offense_ratings.build_delete ).to eq 'DELETE FROM Player_Offense_Ratings_T WHERE Player_Id = :player_id'
    end
  end

  describe '#to_hash' do
    it 'should return a hash of all fields' do
      player_offense_ratings = PlayerOffenseRatings.new 1

      player_offense_ratings.ball_control = 2
      player_offense_ratings.receiving    = 13

      result = player_offense_ratings.to_hash

      expect( result.keys.length      ).to eq 3
      expect( result[ :player_id    ] ).to eq 1
      expect( result[ :ball_control ] ).to eq 2
      expect( result[ :receiving    ] ).to eq 13
    end
  end

  describe '#key_hash' do
    it 'should return a hash of the key fields' do
      player_offense_ratings = PlayerOffenseRatings.new 1

      result = player_offense_ratings.key_hash

      expect( result.keys.length   ).to eq 1
      expect( result[ :player_id ] ).to eq 1
    end
  end

  describe '#from_hash' do
    it 'should set fields to values from given hash' do
      player_offense_ratings = PlayerOffenseRatings.new
      hash = {
        player_id:    1,
        ball_control: 13,
        receiving:    3
      }

      player_offense_ratings.from_hash hash

      expect( player_offense_ratings.player_id    ).to eq hash[ :player_id    ]
      expect( player_offense_ratings.ball_control ).to eq hash[ :ball_control ]
      expect( player_offense_ratings.receiving    ).to eq hash[ :receiving    ]
    end
  end
end
