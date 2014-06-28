location = File.dirname __FILE__
$: << "#{location}"

require 'conference_team'

describe ConferenceTeam do
  describe '::build' do
    it 'should return a new ConferenceTeam instance with all field values set' do
      conference_team = ConferenceTeam.build 1, 2

      expect( conference_team ).to_not be_nil
      expect( conference_team ).to     be_a   ConferenceTeam

      expect( conference_team.conference_id ).to eq 1
      expect( conference_team.team_id       ).to eq 2
    end
  end

  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      conference_team = ConferenceTeam.new
      expect( conference_team.build_insert ).to eq 'INSERT INTO Conference_Teams_T ( Conference_Id, Team_Id ) VALUES ( :conference_id, :team_id )'
    end
  end

  describe '#build_select_by_conference' do
    it 'should return a well formed SQL select statement' do
      conference_team = ConferenceTeam.new
      result = conference_team.build_select_by_conference
      expect( result ).to eq 'SELECT Conference_Id, Team_Id FROM Conference_Teams_T WHERE Conference_Id = :conference_id'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL delete statement' do
      conference_team = ConferenceTeam.new
      expect( conference_team.build_delete ).to eq 'DELETE FROM Conference_Teams_T WHERE Conference_Id = :conference_id AND Team_Id = :team_id'
    end
  end

  describe '#to_hash' do
    it 'should return a hash of all fields' do
      conference_team = ConferenceTeam.new 1, 4

      result = conference_team.to_hash

      expect( result.keys.length       ).to eq 2
      expect( result[ :conference_id ] ).to eq 1
      expect( result[ :team_id       ] ).to eq 4
    end
  end

  describe '#key_hash' do
    it 'should return a hash of the key fields' do
      conference_team = ConferenceTeam.new 1, 4

      result = conference_team.key_hash

      expect( result.keys.length       ).to eq 2
      expect( result[ :conference_id ] ).to eq 1
      expect( result[ :team_id       ] ).to eq 4
    end
  end

  describe '#from_hash' do
    it 'should set fields to values from given hash' do
      conference_team = ConferenceTeam.new
      hash = { conference_id: 1, team_id: 3 }

      conference_team.from_hash hash

      expect( conference_team.conference_id ).to eq hash[ :conference_id ]
      expect( conference_team.team_id       ).to eq hash[ :team_id       ]
    end
  end
end
