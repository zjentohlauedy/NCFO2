location = File.dirname __FILE__
$: << "#{location}"

require 'team_stats'

describe TeamStats do
  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      team_stats = TeamStats.new
      expect( team_stats.build_insert ).to eq 'INSERT INTO Team_Stats_T ( Team_Id, Season, Bowl_Game, Wins, Losses, Home_Wins, Home_Losses, Road_Wins, Road_Losses, Points_Scored, Points_Allowed ) VALUES ( ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ? )'
    end
  end

  describe '#build_select' do
    it 'should return a well formed SQL select statement' do
      team_stats = TeamStats.new
      expect( team_stats.build_select ).to eq 'SELECT Team_Id, Season, Bowl_Game, Wins, Losses, Home_Wins, Home_Losses, Road_Wins, Road_Losses, Points_Scored, Points_Allowed FROM Team_Stats_T WHERE Team_Id = ? AND Season = ? AND Bowl_Game = ?'
    end
  end

  describe '#build_update' do
    it 'should return a well formed SQL update statement' do
      team_stats = TeamStats.new
      expect( team_stats.build_update ).to eq 'UPDATE Team_Stats_T SET Wins = ?, Losses = ?, Home_Wins = ?, Home_Losses = ?, Road_Wins = ?, Road_Losses = ?, Points_Scored = ?, Points_Allowed = ? WHERE Team_Id = ? AND Season = ? AND Bowl_Game = ?'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL update statement' do
      team_stats = TeamStats.new
      expect( team_stats.build_delete ).to eq 'DELETE FROM Team_Stats_T WHERE Team_Id = ? AND Season = ? AND Bowl_Game = ?'
    end
  end

  describe '#to_hash' do
    it 'should return a hash of all fields' do
      team_stats = TeamStats.new 1, 2, 3

      team_stats.wins           = 8
      team_stats.losses         = 6
      team_stats.home_wins      = 5
      team_stats.home_losses    = 1
      team_stats.road_wins      = 3
      team_stats.road_losses    = 5
      team_stats.points_scored  = 82
      team_stats.points_allowed = 68

      result = team_stats.to_hash

      expect( result.keys.length        ).to eq 11
      expect( result[ :team_id        ] ).to eq 1
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
      team_stats = TeamStats.new 1, 2, 3

      result = team_stats.key_hash

      expect( result.keys.length   ).to eq 3
      expect( result[ :team_id   ] ).to eq 1
      expect( result[ :season    ] ).to eq 2
      expect( result[ :bowl_game ] ).to eq 3
    end
  end

  describe '#from_hash' do
    it 'should set fields to values from given hash' do
      team_stats = TeamStats.new
      hash = {
        team_id:        1,
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

      team_stats.from_hash hash

      expect( team_stats.team_id        ).to eq hash[ :team_id        ]
      expect( team_stats.season         ).to eq hash[ :season         ]
      expect( team_stats.bowl_game      ).to eq hash[ :bowl_game      ]
      expect( team_stats.wins           ).to eq hash[ :wins           ]
      expect( team_stats.losses         ).to eq hash[ :losses         ]
      expect( team_stats.home_wins      ).to eq hash[ :home_wins      ]
      expect( team_stats.home_losses    ).to eq hash[ :home_losses    ]
      expect( team_stats.road_wins      ).to eq hash[ :road_wins      ]
      expect( team_stats.road_losses    ).to eq hash[ :road_losses    ]
      expect( team_stats.points_scored  ).to eq hash[ :points_scored  ]
      expect( team_stats.points_allowed ).to eq hash[ :points_allowed ]
    end
  end
end
