location = File.dirname __FILE__
$: << "#{location}"

require 'organization'

describe Organization do
  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      organization = Organization.new
      expect( organization.build_insert ).to eq 'INSERT INTO Organizations_T ( Organization_Id, Name, Abbreviation, Season ) VALUES ( ?, ?, ?, ? )'
    end
  end

  describe '#build_select' do
    it 'should return a well formed SQL select statement' do
      organization = Organization.new
      expect( organization.build_select ).to eq 'SELECT Organization_Id, Name, Abbreviation, Season FROM Organizations_T WHERE Organization_Id = ?'
    end
  end

  describe '#build_update' do
    it 'should return a well formed SQL update statement' do
      organization = Organization.new
      expect( organization.build_update ).to eq 'UPDATE Organizations_T SET Name = ?, Abbreviation = ?, Season = ? WHERE Organization_Id = ?'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL update statement' do
      organization = Organization.new
      expect( organization.build_delete ).to eq 'DELETE FROM Organizations_T WHERE Organization_Id = ?'
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
end
