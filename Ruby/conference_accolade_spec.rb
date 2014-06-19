location = File.dirname __FILE__
$: << "#{location}"

require 'conference_accolade'

describe ConferenceAccolade do
  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      conference_accolade = ConferenceAccolade.new
      expect( conference_accolade.build_insert ).to eq 'INSERT INTO Conference_Accolades_T ( Conference_Id, Season, Accolade ) VALUES ( :conference_id, :season, :accolade )'
    end
  end

  describe '#build_select_by_conference' do
    it 'should return a well formed SQL select statement' do
      conference_accolade = ConferenceAccolade.new
      result = conference_accolade.build_select_by_conference
      expect( result ).to eq 'SELECT Conference_Id, Season, Accolade FROM Conference_Accolades_T WHERE Conference_Id = :conference_id'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL delete statement' do
      conference_accolade = ConferenceAccolade.new
      expect( conference_accolade.build_delete ).to eq 'DELETE FROM Conference_Accolades_T WHERE Conference_Id = :conference_id AND Season = :season AND Accolade = :accolade'
    end
  end

  describe '#to_hash' do
    it 'should return a hash of all fields' do
      conference_accolade = ConferenceAccolade.new 1, 4, 3

      result = conference_accolade.to_hash

      expect( result.keys.length       ).to eq 3
      expect( result[ :conference_id ] ).to eq 1
      expect( result[ :season        ] ).to eq 4
      expect( result[ :accolade      ] ).to eq 3
    end
  end

  describe '#key_hash' do
    it 'should return a hash of the key fields' do
      conference_accolade = ConferenceAccolade.new 1, 4, 3

      result = conference_accolade.key_hash

      expect( result.keys.length       ).to eq 3
      expect( result[ :conference_id ] ).to eq 1
      expect( result[ :season        ] ).to eq 4
      expect( result[ :accolade      ] ).to eq 3
    end
  end

  describe '#from_hash' do
    it 'should set fields to values from given hash' do
      conference_accolade = ConferenceAccolade.new
      hash = { conference_id: 1, season: 4, accolade: 3 }

      conference_accolade.from_hash hash

      expect( conference_accolade.conference_id ).to eq hash[ :conference_id ]
      expect( conference_accolade.season        ).to eq hash[ :season        ]
      expect( conference_accolade.accolade      ).to eq hash[ :accolade      ]
    end
  end
end
