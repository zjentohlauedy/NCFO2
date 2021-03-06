location = File.dirname __FILE__
$: << "#{location}"

require 'organization_conference'

describe OrganizationConference do
  describe '::build' do
    it 'should return a new OrganizationConference instance with all field values set' do
      organization_conference = OrganizationConference.build 1, 2

      expect( organization_conference ).to_not be_nil
      expect( organization_conference ).to     be_a   OrganizationConference

      expect( organization_conference.organization_id ).to eq 1
      expect( organization_conference.conference_id   ).to eq 2
    end
  end

  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      organization_conference = OrganizationConference.new
      expect( organization_conference.build_insert ).to eq 'INSERT INTO Organization_Conferences_T ( Organization_Id, Conference_Id ) VALUES ( :organization_id, :conference_id )'
    end
  end

  describe '#build_select_by_organization' do
    it 'should return a well formed SQL select statement' do
      organization_conference = OrganizationConference.new
      result = organization_conference.build_select_by_organization
      expect( result ).to eq 'SELECT Organization_Id, Conference_Id FROM Organization_Conferences_T WHERE Organization_Id = :organization_id'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL delete statement' do
      organization_conference = OrganizationConference.new
      expect( organization_conference.build_delete ).to eq 'DELETE FROM Organization_Conferences_T WHERE Organization_Id = :organization_id AND Conference_Id = :conference_id'
    end
  end

  describe '#to_hash' do
    it 'should return a hash of all fields' do
      organization_conference = OrganizationConference.new 1, 4

      result = organization_conference.to_hash

      expect( result.keys.length         ).to eq 2
      expect( result[ :organization_id ] ).to eq 1
      expect( result[ :conference_id   ] ).to eq 4
    end
  end

  describe '#key_hash' do
    it 'should return a hash of the key fields' do
      organization_conference = OrganizationConference.new 1, 4

      result = organization_conference.key_hash

      expect( result.keys.length         ).to eq 2
      expect( result[ :organization_id ] ).to eq 1
      expect( result[ :conference_id   ] ).to eq 4
    end
  end

  describe '#from_hash' do
    it 'should set fields to values from given hash' do
      organization_conference = OrganizationConference.new
      hash = { organization_id: 1, conference_id: 3 }

      organization_conference.from_hash hash

      expect( organization_conference.organization_id ).to eq hash[ :organization_id ]
      expect( organization_conference.conference_id   ).to eq hash[ :conference_id   ]
    end
  end
end
