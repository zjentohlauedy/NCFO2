location = File.dirname __FILE__
$: << "#{location}"

require 'team_player'

describe TeamPlayer do
  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      team_player = TeamPlayer.new
      expect( team_player.build_insert ).to eq 'INSERT INTO Team_Players_T ( Team_Id, Season, Player_Id ) VALUES ( ?, ?, ? )'
    end
  end

  describe '#build_select_by_team' do
    it 'should return a well formed SQL select statement' do
      team_player = TeamPlayer.new
      result = team_player.build_select_by_team
      expect( result ).to eq 'SELECT Team_Id, Season, Player_Id FROM Team_Players_T WHERE Team_Id = ?'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL update statement' do
      team_player = TeamPlayer.new
      expect( team_player.build_delete ).to eq 'DELETE FROM Team_Players_T WHERE Team_Id = ? AND Season = ? AND Player_Id = ?'
    end
  end

  describe '#to_hash' do
    it 'should return a hash of all fields' do
      team_player = TeamPlayer.new 1, 2, 4

      result = team_player.to_hash

      expect( result.keys.length   ).to eq 3
      expect( result[ :team_id   ] ).to eq 1
      expect( result[ :season    ] ).to eq 2
      expect( result[ :player_id ] ).to eq 4
    end
  end

  describe '#key_hash' do
    it 'should return a hash of the key fields' do
      team_player = TeamPlayer.new 1, 2, 4

      result = team_player.key_hash

      expect( result.keys.length   ).to eq 3
      expect( result[ :team_id   ] ).to eq 1
      expect( result[ :season    ] ).to eq 2
      expect( result[ :player_id ] ).to eq 4
    end
  end

  describe '#from_hash' do
    it 'should set fields to values from given hash' do
      team_player = TeamPlayer.new
      hash = { team_id: 1, season: 2, player_id: 4 }

      team_player.from_hash hash

      expect( team_player.team_id   ).to eq hash[ :team_id   ]
      expect( team_player.season    ).to eq hash[ :season    ]
      expect( team_player.player_id ).to eq hash[ :player_id ]
    end
  end
end
