location = File.dirname __FILE__
$: << "#{location}"

require 'conference_stats'

describe ConferenceStats do
  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      conference_stats = ConferenceStats.new
      expect( conference_stats.build_insert ).to eq 'INSERT INTO Conference_Stats_T ( Conference_Id, Season, Bowl_Game, Wins, Losses, Home_Wins, Home_Losses, Road_Wins, Road_Losses, Points_Scored, Points_Allowed ) VALUES ( :conference_id, :season, :bowl_game, :wins, :losses, :home_wins, :home_losses, :road_wins, :road_losses, :points_scored, :points_allowed )'
    end
  end

  describe '#build_select' do
    it 'should return a well formed SQL select statement' do
      conference_stats = ConferenceStats.new
      expect( conference_stats.build_select ).to eq 'SELECT Conference_Id, Season, Bowl_Game, Wins, Losses, Home_Wins, Home_Losses, Road_Wins, Road_Losses, Points_Scored, Points_Allowed FROM Conference_Stats_T WHERE Conference_Id = :conference_id AND Season = :season AND Bowl_Game = :bowl_game'
    end
  end

  describe '#build_select_by_conference' do
    it 'should return a well formed SQL select statement' do
      conference_stats = ConferenceStats.new
      result = conference_stats.build_select_by_conference
      expect( result ).to eq 'SELECT Conference_Id, Season, Bowl_Game, Wins, Losses, Home_Wins, Home_Losses, Road_Wins, Road_Losses, Points_Scored, Points_Allowed FROM Conference_Stats_T WHERE Conference_Id = :conference_id'
    end
  end

  describe '#build_update' do
    it 'should return a well formed SQL update statement' do
      conference_stats = ConferenceStats.new
      expect( conference_stats.build_update ).to eq 'UPDATE Conference_Stats_T SET Wins = :wins, Losses = :losses, Home_Wins = :home_wins, Home_Losses = :home_losses, Road_Wins = :road_wins, Road_Losses = :road_losses, Points_Scored = :points_scored, Points_Allowed = :points_allowed WHERE Conference_Id = :conference_id AND Season = :season AND Bowl_Game = :bowl_game'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL delete statement' do
      conference_stats = ConferenceStats.new
      expect( conference_stats.build_delete ).to eq 'DELETE FROM Conference_Stats_T WHERE Conference_Id = :conference_id AND Season = :season AND Bowl_Game = :bowl_game'
    end
  end

  describe '#to_hash' do
    it 'should return a hash of all fields' do
      conference_stats = ConferenceStats.new 1, 2, 3

      conference_stats.wins           = 8
      conference_stats.losses         = 6
      conference_stats.home_wins      = 5
      conference_stats.home_losses    = 1
      conference_stats.road_wins      = 3
      conference_stats.road_losses    = 5
      conference_stats.points_scored  = 82
      conference_stats.points_allowed = 68

      result = conference_stats.to_hash

      expect( result.keys.length        ).to eq 11
      expect( result[ :conference_id  ] ).to eq 1
      expect( result[ :season         ] ).to eq 2
      expect( result[ :bowl_game      ] ).to eq 3
      expect( result[ :wins           ] ).to eq 8
      expect( result[ :losses         ] ).to eq 6
      expect( result[ :home_wins      ] ).to eq 5
      expect( result[ :home_losses    ] ).to eq 1
      expect( result[ :road_wins      ] ).to eq 3
      expect( result[ :road_losses    ] ).to eq 5
      expect( result[ :points_scored  ] ).to eq 82
      expect( result[ :points_allowed ] ).to eq 68
    end
  end

  describe '#key_hash' do
    it 'should return a hash of the key fields' do
      conference_stats = ConferenceStats.new 1, 2, 3

      result = conference_stats.key_hash

      expect( result.keys.length       ).to eq 3
      expect( result[ :conference_id ] ).to eq 1
      expect( result[ :season        ] ).to eq 2
      expect( result[ :bowl_game     ] ).to eq 3
    end
  end

  describe '#from_hash' do
    it 'should set fields to values from given hash' do
      conference_stats = ConferenceStats.new
      hash = {
        conference_id:  1,
        season:         2,
        bowl_game:      3,
        wins:           8,
        losses:         6,
        home_wins:      5,
        home_losses:    1,
        road_wins:      3,
        road_losses:    5,
        points_scored:  82,
        points_allowed: 68
      }

      conference_stats.from_hash hash

      expect( conference_stats.conference_id  ).to eq hash[ :conference_id  ]
      expect( conference_stats.season         ).to eq hash[ :season         ]
      expect( conference_stats.bowl_game      ).to eq hash[ :bowl_game      ]
      expect( conference_stats.wins           ).to eq hash[ :wins           ]
      expect( conference_stats.losses         ).to eq hash[ :losses         ]
      expect( conference_stats.home_wins      ).to eq hash[ :home_wins      ]
      expect( conference_stats.home_losses    ).to eq hash[ :home_losses    ]
      expect( conference_stats.road_wins      ).to eq hash[ :road_wins      ]
      expect( conference_stats.road_losses    ).to eq hash[ :road_losses    ]
      expect( conference_stats.points_scored  ).to eq hash[ :points_scored  ]
      expect( conference_stats.points_allowed ).to eq hash[ :points_allowed ]
    end
  end
end
