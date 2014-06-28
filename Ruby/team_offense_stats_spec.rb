location = File.dirname __FILE__
$: << "#{location}"

require 'team_offense_stats'

describe TeamOffenseStats do
  describe '::build' do
    it 'should return a new TeamOffenseStats instance with all field values set' do
      team_offense_stats = TeamOffenseStats.build 1, 2, Bowls::NCFOChampionship

      expect( team_offense_stats ).to_not be_nil
      expect( team_offense_stats ).to     be_a   TeamOffenseStats

      expect( team_offense_stats.team_id         ).to eq 1
      expect( team_offense_stats.season          ).to eq 2
      expect( team_offense_stats.bowl_game       ).to eq Bowls::NCFOChampionship
      expect( team_offense_stats.pass_attempts   ).to be_a Integer
      expect( team_offense_stats.completions     ).to be_a Integer
      expect( team_offense_stats.interceptions   ).to be_a Integer
      expect( team_offense_stats.pass_yards      ).to be_a Integer
      expect( team_offense_stats.pass_touchdowns ).to be_a Integer
      expect( team_offense_stats.rush_attempts   ).to be_a Integer
      expect( team_offense_stats.rush_yards      ).to be_a Integer
      expect( team_offense_stats.rush_touchdowns ).to be_a Integer
    end
  end

  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      team_offense_stats = TeamOffenseStats.new
      expect( team_offense_stats.build_insert ).to eq 'INSERT INTO Team_Offense_Stats_T ( Team_Id, Season, Bowl_Game, Pass_Attempts, Completions, Interceptions, Pass_Yards, Pass_Touchdowns, Rush_Attempts, Rush_Yards, Rush_Touchdowns ) VALUES ( :team_id, :season, :bowl_game, :pass_attempts, :completions, :interceptions, :pass_yards, :pass_touchdowns, :rush_attempts, :rush_yards, :rush_touchdowns )'
    end
  end

  describe '#build_select' do
    it 'should return a well formed SQL select statement' do
      team_offense_stats = TeamOffenseStats.new
      expect( team_offense_stats.build_select ).to eq 'SELECT Team_Id, Season, Bowl_Game, Pass_Attempts, Completions, Interceptions, Pass_Yards, Pass_Touchdowns, Rush_Attempts, Rush_Yards, Rush_Touchdowns FROM Team_Offense_Stats_T WHERE Team_Id = :team_id AND Season = :season AND Bowl_Game = :bowl_game'
    end
  end

  describe '#build_select_by_team' do
    it 'should return a well formed SQL select statement' do
      team_offense_stats = TeamOffenseStats.new
      result = team_offense_stats.build_select_by_team
      expect( result ).to eq 'SELECT Team_Id, Season, Bowl_Game, Pass_Attempts, Completions, Interceptions, Pass_Yards, Pass_Touchdowns, Rush_Attempts, Rush_Yards, Rush_Touchdowns FROM Team_Offense_Stats_T WHERE Team_Id = :team_id'
    end
  end

  describe '#build_update' do
    it 'should return a well formed SQL update statement' do
      team_offense_stats = TeamOffenseStats.new
      expect( team_offense_stats.build_update ).to eq 'UPDATE Team_Offense_Stats_T SET Pass_Attempts = :pass_attempts, Completions = :completions, Interceptions = :interceptions, Pass_Yards = :pass_yards, Pass_Touchdowns = :pass_touchdowns, Rush_Attempts = :rush_attempts, Rush_Yards = :rush_yards, Rush_Touchdowns = :rush_touchdowns WHERE Team_Id = :team_id AND Season = :season AND Bowl_Game = :bowl_game'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL delete statement' do
      team_offense_stats = TeamOffenseStats.new
      expect( team_offense_stats.build_delete ).to eq 'DELETE FROM Team_Offense_Stats_T WHERE Team_Id = :team_id AND Season = :season AND Bowl_Game = :bowl_game'
    end
  end

  describe '#to_hash' do
    it 'should return a hash of all fields' do
      team_offense_stats = TeamOffenseStats.new 1, 2, Bowls::NCFOChampionship

      team_offense_stats.pass_attempts   = 314
      team_offense_stats.completions     = 268
      team_offense_stats.interceptions   = 12
      team_offense_stats.pass_yards      = 2884
      team_offense_stats.pass_touchdowns = 35
      team_offense_stats.rush_attempts   = 287
      team_offense_stats.rush_yards      = 1083
      team_offense_stats.rush_touchdowns = 16

      result = team_offense_stats.to_hash

      expect( result.keys.length         ).to eq 11
      expect( result[ :team_id         ] ).to eq 1
      expect( result[ :season          ] ).to eq 2
      expect( result[ :bowl_game       ] ).to eq Bowls::NCFOChampionship
      expect( result[ :pass_attempts   ] ).to eq 314
      expect( result[ :completions     ] ).to eq 268
      expect( result[ :interceptions   ] ).to eq 12
      expect( result[ :pass_yards      ] ).to eq 2884
      expect( result[ :pass_touchdowns ] ).to eq 35
      expect( result[ :rush_attempts   ] ).to eq 287
      expect( result[ :rush_yards      ] ).to eq 1083
      expect( result[ :rush_touchdowns ] ).to eq 16
    end
  end

  describe '#key_hash' do
    it 'should return a hash of the key fields' do
      team_offense_stats = TeamOffenseStats.new 1, 2, Bowls::NCFOChampionship

      result = team_offense_stats.key_hash

      expect( result.keys.length   ).to eq 3
      expect( result[ :team_id   ] ).to eq 1
      expect( result[ :season    ] ).to eq 2
      expect( result[ :bowl_game ] ).to eq Bowls::NCFOChampionship
    end
  end

  describe '#from_hash' do
    it 'should set fields to values from given hash' do
      team_offense_stats = TeamOffenseStats.new
      hash = {
        team_id:         1,
        season:          2,
        bowl_game:       Bowls::NCFOChampionship,
        pass_attempts:   314,
        completions:     268,
        interceptions:   12,
        pass_yards:      2884,
        pass_touchdowns: 35,
        rush_attempts:   287,
        rush_yards:      1083,
        rush_touchdowns: 16
      }

      team_offense_stats.from_hash hash

      expect( team_offense_stats.team_id         ).to eq hash[ :team_id         ]
      expect( team_offense_stats.season          ).to eq hash[ :season          ]
      expect( team_offense_stats.bowl_game       ).to eq hash[ :bowl_game       ]
      expect( team_offense_stats.pass_attempts   ).to eq hash[ :pass_attempts   ]
      expect( team_offense_stats.completions     ).to eq hash[ :completions     ]
      expect( team_offense_stats.interceptions   ).to eq hash[ :interceptions   ]
      expect( team_offense_stats.pass_yards      ).to eq hash[ :pass_yards      ]
      expect( team_offense_stats.pass_touchdowns ).to eq hash[ :pass_touchdowns ]
      expect( team_offense_stats.rush_attempts   ).to eq hash[ :rush_attempts   ]
      expect( team_offense_stats.rush_yards      ).to eq hash[ :rush_yards      ]
      expect( team_offense_stats.rush_touchdowns ).to eq hash[ :rush_touchdowns ]
    end
  end
end
