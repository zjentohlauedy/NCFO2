location = File.dirname __FILE__
$: << "#{location}"

require 'player_quarterback_ratings'

describe PlayerQuarterbackRatings do
  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      player_quarterback_ratings = PlayerQuarterbackRatings.new
      expect( player_quarterback_ratings.build_insert ).to eq 'INSERT INTO Player_Quarterback_Ratings_T ( Player_Id, Pass_Speed, Pass_Control, Pass_Accuracy, Avoid_Pass_Block ) VALUES ( :player_id, :pass_speed, :pass_control, :pass_accuracy, :avoid_pass_block )'
    end
  end

  describe '#build_select' do
    it 'should return a well formed SQL select statement' do
      player_quarterback_ratings = PlayerQuarterbackRatings.new
      expect( player_quarterback_ratings.build_select ).to eq 'SELECT Player_Id, Pass_Speed, Pass_Control, Pass_Accuracy, Avoid_Pass_Block FROM Player_Quarterback_Ratings_T WHERE Player_Id = :player_id'
    end
  end

  describe '#build_update' do
    it 'should return a well formed SQL update statement' do
      player_quarterback_ratings = PlayerQuarterbackRatings.new
      expect( player_quarterback_ratings.build_update ).to eq 'UPDATE Player_Quarterback_Ratings_T SET Pass_Speed = :pass_speed, Pass_Control = :pass_control, Pass_Accuracy = :pass_accuracy, Avoid_Pass_Block = :avoid_pass_block WHERE Player_Id = :player_id'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL delete statement' do
      player_quarterback_ratings = PlayerQuarterbackRatings.new
      expect( player_quarterback_ratings.build_delete ).to eq 'DELETE FROM Player_Quarterback_Ratings_T WHERE Player_Id = :player_id'
    end
  end

  describe '#to_hash' do
    it 'should return a hash of all fields' do
      player_quarterback_ratings = PlayerQuarterbackRatings.new 1

      player_quarterback_ratings.pass_speed       = 2
      player_quarterback_ratings.pass_control     = 13
      player_quarterback_ratings.pass_accuracy    = 1
      player_quarterback_ratings.avoid_pass_block = 3

      result = player_quarterback_ratings.to_hash

      expect( result.keys.length          ).to eq 5
      expect( result[ :player_id        ] ).to eq 1
      expect( result[ :pass_speed       ] ).to eq 2
      expect( result[ :pass_control     ] ).to eq 13
      expect( result[ :pass_accuracy    ] ).to eq 1
      expect( result[ :avoid_pass_block ] ).to eq 3
    end
  end

  describe '#key_hash' do
    it 'should return a hash of the key fields' do
      player_quarterback_ratings = PlayerQuarterbackRatings.new 1

      result = player_quarterback_ratings.key_hash

      expect( result.keys.length   ).to eq 1
      expect( result[ :player_id ] ).to eq 1
    end
  end

  describe '#from_hash' do
    it 'should set fields to values from given hash' do
      player_quarterback_ratings = PlayerQuarterbackRatings.new
      hash = {
        player_id:        1,
        pass_speed:       2,
        pass_control:     13,
        pass_accuracy:    1,
        avoid_pass_block: 3
      }

      player_quarterback_ratings.from_hash hash

      expect( player_quarterback_ratings.player_id        ).to eq hash[ :player_id        ]
      expect( player_quarterback_ratings.pass_speed       ).to eq hash[ :pass_speed       ]
      expect( player_quarterback_ratings.pass_control     ).to eq hash[ :pass_control     ]
      expect( player_quarterback_ratings.pass_accuracy    ).to eq hash[ :pass_accuracy    ]
      expect( player_quarterback_ratings.avoid_pass_block ).to eq hash[ :avoid_pass_block ]
    end
  end
end
