location = File.dirname __FILE__
$: << "#{location}"

require 'match'

describe Match do
  describe '::build' do
    it 'should return a new Match instance with all field values set' do
      match = Match.build 1, 2, 3

      expect( match ).to_not be_nil
      expect( match ).to     be_a   Match

      expect( match.season       ).to eq 1
      expect( match.week         ).to eq 2
      expect( match.game         ).to eq 3
      expect( match.road_team_id ).to be_a Integer
      expect( match.home_team_id ).to be_a Integer
    end
  end

  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      match = Match.new
      expect( match.build_insert ).to eq 'INSERT INTO Matches_T ( Season, Week, Game, Road_Team_Id, Home_Team_Id ) VALUES ( :season, :week, :game, :road_team_id, :home_team_id )'
    end
  end

  describe '#build_select' do
    it 'should return a well formed SQL select statement' do
      match = Match.new
      expect( match.build_select ).to eq 'SELECT Season, Week, Game, Road_Team_Id, Home_Team_Id FROM Matches_T WHERE Season = :season AND Week = :week AND Game = :game'
    end
  end

  describe '#build_update' do
    it 'should return a well formed SQL update statement' do
      match = Match.new
      expect( match.build_update ).to eq 'UPDATE Matches_T SET Road_Team_Id = :road_team_id, Home_Team_Id = :home_team_id WHERE Season = :season AND Week = :week AND Game = :game'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL delete statement' do
      match = Match.new
      expect( match.build_delete ).to eq 'DELETE FROM Matches_T WHERE Season = :season AND Week = :week AND Game = :game'
    end
  end

  describe '#to_hash' do
    it 'should return a hash of all fields' do
      match = Match.new 1, 2, 3

      match.road_team_id = 18
      match.home_team_id = 7

      result = match.to_hash

      expect( result.keys.length      ).to eq 5
      expect( result[ :season       ] ).to eq 1
      expect( result[ :week         ] ).to eq 2
      expect( result[ :game         ] ).to eq 3
      expect( result[ :road_team_id ] ).to eq 18
      expect( result[ :home_team_id ] ).to eq 7
    end
  end

  describe '#key_hash' do
    it 'should return a hash of the key fields' do
      match = Match.new 1, 2, 3

      result = match.key_hash

      expect( result.keys.length   ).to eq 3
      expect( result[ :season    ] ).to eq 1
      expect( result[ :week      ] ).to eq 2
      expect( result[ :game      ] ).to eq 3
    end
  end

  describe '#from_hash' do
    it 'should set fields to values from given hash' do
      match = Match.new
      hash = {
        season:        1,
        week:          2,
        game:          3,
        road_team_id: 18,
        home_team_id:  7,
      }

      match.from_hash hash

      expect( match.season       ).to eq hash[ :season       ]
      expect( match.week         ).to eq hash[ :week         ]
      expect( match.game         ).to eq hash[ :game         ]
      expect( match.road_team_id ).to eq hash[ :road_team_id ]
      expect( match.home_team_id ).to eq hash[ :home_team_id ]
    end
  end
end
