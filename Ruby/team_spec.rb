location = File.dirname __FILE__
$: << "#{location}"

require 'team'

describe Team do
  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      team = Team.new
      expect( team.build_insert ).to eq 'INSERT INTO Teams_T ( Team_Id, Name, Location, Abbreviation ) VALUES ( :team_id, :name, :location, :abbreviation )'
    end
  end

  describe '#build_select' do
    it 'should return a well formed SQL select statement' do
      team = Team.new
      expect( team.build_select ).to eq 'SELECT Team_Id, Name, Location, Abbreviation FROM Teams_T WHERE Team_Id = :team_id'
    end
  end

  describe '#build_update' do
    it 'should return a well formed SQL update statement' do
      team = Team.new
      expect( team.build_update ).to eq 'UPDATE Teams_T SET Name = :name, Location = :location, Abbreviation = :abbreviation WHERE Team_Id = :team_id'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL delete statement' do
      team = Team.new
      expect( team.build_delete ).to eq 'DELETE FROM Teams_T WHERE Team_Id = :team_id'
    end
  end

  describe '#to_hash' do
    it 'should return a hash of all fields' do
      team = Team.new 1

      team.name         = 'Test Name'
      team.location     = 'Test Location'
      team.abbreviation = 'TN'

      result = team.to_hash

      expect( result.keys.length      ).to eq 4
      expect( result[ :team_id      ] ).to eq 1
      expect( result[ :name         ] ).to eq 'Test Name'
      expect( result[ :location     ] ).to eq 'Test Location'
      expect( result[ :abbreviation ] ).to eq 'TN'
    end
  end

  describe '#key_hash' do
    it 'should return a hash of the key fields' do
      team = Team.new 1

      result = team.key_hash

      expect( result.keys.length ).to eq 1
      expect( result[ :team_id ] ).to eq 1
    end
  end

  describe '#from_hash' do
    it 'should set fields to values from given hash' do
      team = Team.new
      hash = { team_id: 1, name: 'Test Name', location: 'Test Location', abbreviation: 'TN' }

      team.from_hash hash

      expect( team.team_id      ).to eq hash[ :team_id      ]
      expect( team.name         ).to eq hash[ :name         ]
      expect( team.location     ).to eq hash[ :location     ]
      expect( team.abbreviation ).to eq hash[ :abbreviation ]
    end
  end
end
