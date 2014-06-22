location = File.dirname __FILE__
$: << "#{location}"

require 'player'

describe Player do
  describe '::build' do
    it 'should return a new Player instance with all field values set' do
      player = Player.build 1

      expect( player ).to_not be_nil
      expect( player ).to     be_a   Player

      expect( player.player_id       ).to eq 1
      expect( player.first_name      ).to be_a String
      expect( player.last_name       ).to be_a String
      expect( player.face            ).to be_a Integer
      expect( player.position        ).to be_a Integer
      expect( player.number          ).to be_a Integer
      expect( player.freshman_season ).to be_a Integer
      expect( player.maturity        ).to be_a Integer
    end
  end

  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      player = Player.new
      expect( player.build_insert ).to eq 'INSERT INTO Players_T ( Player_Id, First_Name, Last_Name, Face, Position, Number, Freshman_Season, Maturity ) VALUES ( :player_id, :first_name, :last_name, :face, :position, :number, :freshman_season, :maturity )'
    end
  end

  describe '#build_select' do
    it 'should return a well formed SQL select statement' do
      player = Player.new
      expect( player.build_select ).to eq 'SELECT Player_Id, First_Name, Last_Name, Face, Position, Number, Freshman_Season, Maturity FROM Players_T WHERE Player_Id = :player_id'
    end
  end

  describe '#build_update' do
    it 'should return a well formed SQL update statement' do
      player = Player.new
      expect( player.build_update ).to eq 'UPDATE Players_T SET First_Name = :first_name, Last_Name = :last_name, Face = :face, Position = :position, Number = :number, Freshman_Season = :freshman_season, Maturity = :maturity WHERE Player_Id = :player_id'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL delete statement' do
      player = Player.new
      expect( player.build_delete ).to eq 'DELETE FROM Players_T WHERE Player_Id = :player_id'
    end
  end

  describe '#to_hash' do
    it 'should return a hash of all fields' do
      player = Player.new 1

      player.first_name      = 'First Name'
      player.last_name       = 'Last Name'
      player.face            = 123
      player.position        = 2
      player.number          = 13
      player.freshman_season = 1
      player.maturity        = 3

      result = player.to_hash

      expect( result.keys.length         ).to eq 8
      expect( result[ :player_id       ] ).to eq 1
      expect( result[ :first_name      ] ).to eq 'First Name'
      expect( result[ :last_name       ] ).to eq 'Last Name'
      expect( result[ :face            ] ).to eq 123
      expect( result[ :position        ] ).to eq 2
      expect( result[ :number          ] ).to eq 13
      expect( result[ :freshman_season ] ).to eq 1
      expect( result[ :maturity        ] ).to eq 3
    end
  end

  describe '#key_hash' do
    it 'should return a hash of the key fields' do
      player = Player.new 1

      result = player.key_hash

      expect( result.keys.length   ).to eq 1
      expect( result[ :player_id ] ).to eq 1
    end
  end

  describe '#from_hash' do
    it 'should set fields to values from given hash' do
      player = Player.new
      hash = {
        player_id:       1,
        first_name:      'First Name',
        last_name:       'Last Name',
        face:            123,
        position:        2,
        number:          13,
        freshman_season: 1,
        maturity:        3
      }

      player.from_hash hash

      expect( player.player_id       ).to eq hash[ :player_id       ]
      expect( player.first_name      ).to eq hash[ :first_name      ]
      expect( player.last_name       ).to eq hash[ :last_name       ]
      expect( player.face            ).to eq hash[ :face            ]
      expect( player.position        ).to eq hash[ :position        ]
      expect( player.number          ).to eq hash[ :number          ]
      expect( player.freshman_season ).to eq hash[ :freshman_season ]
      expect( player.maturity        ).to eq hash[ :maturity        ]
    end
  end
end
