location = File.dirname __FILE__
$: << "#{location}"

require 'conference'

describe Conference do
  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      conference = Conference.new
      expect( conference.build_insert ).to eq 'INSERT INTO Conferences_T ( Conference_Id, Name ) VALUES ( :conference_id, :name )'
    end
  end

  describe '#build_select' do
    it 'should return a well formed SQL select statement' do
      conference = Conference.new
      expect( conference.build_select ).to eq 'SELECT Conference_Id, Name FROM Conferences_T WHERE Conference_Id = :conference_id'
    end
  end

  describe '#build_update' do
    it 'should return a well formed SQL update statement' do
      conference = Conference.new
      expect( conference.build_update ).to eq 'UPDATE Conferences_T SET Name = :name WHERE Conference_Id = :conference_id'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL delete statement' do
      conference = Conference.new
      expect( conference.build_delete ).to eq 'DELETE FROM Conferences_T WHERE Conference_Id = :conference_id'
    end
  end

  describe '#to_hash' do
    it 'should return a hash of all fields' do
      conference = Conference.new 1

      conference.name         = 'Test Name'

      result = conference.to_hash

      expect( result.keys.length         ).to eq 2
      expect( result[ :conference_id ] ).to eq 1
      expect( result[ :name          ] ).to eq 'Test Name'
    end
  end

  describe '#key_hash' do
    it 'should return a hash of the key fields' do
      conference = Conference.new 1

      result = conference.key_hash

      expect( result.keys.length         ).to eq 1
      expect( result[ :conference_id ] ).to eq 1
    end
  end

  describe '#from_hash' do
    it 'should set fields to values from given hash' do
      conference = Conference.new
      hash = { conference_id: 1, name: 'Test Name' }

      conference.from_hash hash

      expect( conference.conference_id ).to eq hash[ :conference_id ]
      expect( conference.name          ).to eq hash[ :name          ]
    end
  end
end
