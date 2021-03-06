location = File.dirname __FILE__
$: << "#{location}"

require 'organization'

describe Organization do
  describe '::build' do
    it 'should return a new Organization instance with all field values set' do
      organization = Organization.build 1

      expect( organization ).to_not be_nil
      expect( organization ).to     be_a   Organization

      expect( organization.organization_id ).to eq 1
      expect( organization.name            ).to be_a String
      expect( organization.abbreviation    ).to be_a String
      expect( organization.season          ).to be_a Integer
    end
  end

  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      organization = Organization.new
      expect( organization.build_insert ).to eq 'INSERT INTO Organizations_T ( Organization_Id, Name, Abbreviation, Season ) VALUES ( :organization_id, :name, :abbreviation, :season )'
    end
  end

  describe '#build_select' do
    it 'should return a well formed SQL select statement' do
      organization = Organization.new
      expect( organization.build_select ).to eq 'SELECT Organization_Id, Name, Abbreviation, Season FROM Organizations_T WHERE Organization_Id = :organization_id'
    end
  end

  describe '#build_update' do
    it 'should return a well formed SQL update statement' do
      organization = Organization.new
      expect( organization.build_update ).to eq 'UPDATE Organizations_T SET Name = :name, Abbreviation = :abbreviation, Season = :season WHERE Organization_Id = :organization_id'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL delete statement' do
      organization = Organization.new
      expect( organization.build_delete ).to eq 'DELETE FROM Organizations_T WHERE Organization_Id = :organization_id'
    end
  end

  describe '#to_hash' do
    it 'should return a hash of all fields' do
      organization = Organization.new 1

      organization.name         = 'Test Name'
      organization.abbreviation = 'TN'
      organization.season       =  5

      result = organization.to_hash

      expect( result.keys.length         ).to eq 4
      expect( result[ :organization_id ] ).to eq 1
      expect( result[ :name            ] ).to eq 'Test Name'
      expect( result[ :abbreviation    ] ).to eq 'TN'
      expect( result[ :season          ] ).to eq 5
    end
  end

  describe '#key_hash' do
    it 'should return a hash of the key fields' do
      organization = Organization.new 1

      result = organization.key_hash

      expect( result.keys.length         ).to eq 1
      expect( result[ :organization_id ] ).to eq 1
    end
  end

  describe '#from_hash' do
    it 'should set fields to values from given hash' do
      organization = Organization.new
      hash = { organization_id: 1, name: 'Test Name', abbreviation: 'TN', season: 3 }

      organization.from_hash hash

      expect( organization.organization_id ).to eq hash[ :organization_id ]
      expect( organization.name            ).to eq hash[ :name            ]
      expect( organization.abbreviation    ).to eq hash[ :abbreviation    ]
      expect( organization.season          ).to eq hash[ :season          ]
    end
  end
end
