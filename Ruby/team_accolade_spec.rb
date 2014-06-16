location = File.dirname __FILE__
$: << "#{location}"

require 'team_accolade'

describe TeamAccolade do
  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      team_accolade = TeamAccolade.new
      expect( team_accolade.build_insert ).to eq 'INSERT INTO Team_Accolades_T ( Team_Id, Season, Accolade ) VALUES ( ?, ?, ? )'
    end
  end

  describe '#build_select_by_team' do
    it 'should return a well formed SQL select statement' do
      team_accolade = TeamAccolade.new
      result = team_accolade.build_select_by_team
      expect( result ).to eq 'SELECT Team_Id, Season, Accolade FROM Team_Accolades_T WHERE Team_Id = ?'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL update statement' do
      team_accolade = TeamAccolade.new
      expect( team_accolade.build_delete ).to eq 'DELETE FROM Team_Accolades_T WHERE Team_Id = ? AND Season = ? AND Accolade = ?'
    end
  end

  describe '#to_hash' do
    it 'should return a hash of all fields' do
      team_accolade = TeamAccolade.new 1, 4, 3

      result = team_accolade.to_hash

      expect( result.keys.length   ).to eq 3
      expect( result[ :team_id   ] ).to eq 1
      expect( result[ :season    ] ).to eq 4
      expect( result[ :accolade  ] ).to eq 3
    end
  end

  describe '#key_hash' do
    it 'should return a hash of the key fields' do
      team_accolade = TeamAccolade.new 1, 4, 3

      result = team_accolade.key_hash

      expect( result.keys.length   ).to eq 3
      expect( result[ :team_id   ] ).to eq 1
      expect( result[ :season    ] ).to eq 4
      expect( result[ :accolade  ] ).to eq 3
    end
  end

  describe '#from_hash' do
    it 'should set fields to values from given hash' do
      team_accolade = TeamAccolade.new
      hash = { team_id: 1, season: 4, accolade: 3 }

      team_accolade.from_hash hash

      expect( team_accolade.team_id  ).to eq hash[ :team_id  ]
      expect( team_accolade.season   ).to eq hash[ :season   ]
      expect( team_accolade.accolade ).to eq hash[ :accolade ]
    end
  end
end
