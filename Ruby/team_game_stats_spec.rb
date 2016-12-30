location = File.dirname __FILE__
$: << "#{location}"

require 'team_game_stats'

describe TeamGameStats do
  describe '::build' do
    it 'should return a new TeamGameStats instance with all field values set' do
      team_game_stats = TeamGameStats.build 1, 2, 3, 4

      expect( team_game_stats ).to_not be_nil
      expect( team_game_stats ).to     be_a   TeamGameStats

      expect( team_game_stats.team_id        ).to eq 1
      expect( team_game_stats.season         ).to eq 2
      expect( team_game_stats.week           ).to eq 3
      expect( team_game_stats.game           ).to eq 4
      expect( team_game_stats.wins           ).to be_a Integer
      expect( team_game_stats.losses         ).to be_a Integer
      expect( team_game_stats.ties           ).to be_a Integer
      expect( team_game_stats.home_wins      ).to be_a Integer
      expect( team_game_stats.home_losses    ).to be_a Integer
      expect( team_game_stats.home_ties      ).to be_a Integer
      expect( team_game_stats.road_wins      ).to be_a Integer
      expect( team_game_stats.road_losses    ).to be_a Integer
      expect( team_game_stats.road_ties      ).to be_a Integer
      expect( team_game_stats.points_scored  ).to be_a Integer
      expect( team_game_stats.points_allowed ).to be_a Integer
    end
  end

  describe '#build_insert' do
    it 'should return a well formed SQL insert statement' do
      team_game_stats = TeamGameStats.new
      expect( team_game_stats.build_insert ).to eq 'INSERT INTO Team_Game_Stats_T ( Team_Id, Season, Week, Game, Wins, Losses, Ties, Home_Wins, Home_Losses, Home_Ties, Road_Wins, Road_Losses, Road_Ties, Points_Scored, Points_Allowed ) VALUES ( :team_id, :season, :week, :game, :wins, :losses, :ties, :home_wins, :home_losses, :home_ties, :road_wins, :road_losses, :road_ties, :points_scored, :points_allowed )'
    end
  end

  describe '#build_select' do
    it 'should return a well formed SQL select statement' do
      team_game_stats = TeamGameStats.new
      expect( team_game_stats.build_select ).to eq 'SELECT Team_Id, Season, Week, Game, Wins, Losses, Ties, Home_Wins, Home_Losses, Home_Ties, Road_Wins, Road_Losses, Road_Ties, Points_Scored, Points_Allowed FROM Team_Game_Stats_T WHERE Team_Id = :team_id AND Season = :season AND Week = :week AND Game = :game'
    end
  end

  describe '#build_select_by_team' do
    it 'should return a well formed SQL select statement' do
      team_game_stats = TeamGameStats.new
      result = team_game_stats.build_select_by_team
      expect( result ).to eq 'SELECT Team_Id, Season, Week, Game, Wins, Losses, Ties, Home_Wins, Home_Losses, Home_Ties, Road_Wins, Road_Losses, Road_Ties, Points_Scored, Points_Allowed FROM Team_Game_Stats_T WHERE Team_Id = :team_id'
    end
  end

  describe '#build_update' do
    it 'should return a well formed SQL update statement' do
      team_game_stats = TeamGameStats.new
      expect( team_game_stats.build_update ).to eq 'UPDATE Team_Game_Stats_T SET Wins = :wins, Losses = :losses, Ties = :ties, Home_Wins = :home_wins, Home_Losses = :home_losses, Home_Ties = :home_ties, Road_Wins = :road_wins, Road_Losses = :road_losses, Road_Ties = :road_ties, Points_Scored = :points_scored, Points_Allowed = :points_allowed WHERE Team_Id = :team_id AND Season = :season AND Week = :week AND Game = :game'
    end
  end

  describe '#build_delete' do
    it 'should return a well formed SQL delete statement' do
      team_game_stats = TeamGameStats.new
      expect( team_game_stats.build_delete ).to eq 'DELETE FROM Team_Game_Stats_T WHERE Team_Id = :team_id AND Season = :season AND Week = :week AND Game = :game'
    end
  end

  describe '#to_hash' do
    it 'should return a hash of all fields' do
      team_game_stats = TeamGameStats.new 1, 2, 3, 4

      team_game_stats.wins           = 8
      team_game_stats.losses         = 6
      team_game_stats.ties           = 1
      team_game_stats.home_wins      = 5
      team_game_stats.home_losses    = 1
      team_game_stats.home_ties      = 0
      team_game_stats.road_wins      = 3
      team_game_stats.road_losses    = 5
      team_game_stats.road_ties      = 2
      team_game_stats.points_scored  = 82
      team_game_stats.points_allowed = 68

      result = team_game_stats.to_hash

      expect( result.keys.length        ).to eq 15
      expect( result[ :team_id        ] ).to eq 1
      expect( result[ :season         ] ).to eq 2
      expect( result[ :week           ] ).to eq 3
      expect( result[ :game           ] ).to eq 4
      expect( result[ :wins           ] ).to eq 8
      expect( result[ :losses         ] ).to eq 6
      expect( result[ :ties           ] ).to eq 1
      expect( result[ :home_wins      ] ).to eq 5
      expect( result[ :home_losses    ] ).to eq 1
      expect( result[ :home_ties      ] ).to eq 0
      expect( result[ :road_wins      ] ).to eq 3
      expect( result[ :road_losses    ] ).to eq 5
      expect( result[ :road_ties      ] ).to eq 2
      expect( result[ :points_scored  ] ).to eq 82
      expect( result[ :points_allowed ] ).to eq 68
    end
  end

  describe '#key_hash' do
    it 'should return a hash of the key fields' do
      team_game_stats = TeamGameStats.new 1, 2, 3, 4

      result = team_game_stats.key_hash

      expect( result.keys.length   ).to eq 4
      expect( result[ :team_id   ] ).to eq 1
      expect( result[ :season    ] ).to eq 2
      expect( result[ :week      ] ).to eq 3
      expect( result[ :game      ] ).to eq 4
    end
  end

  describe '#from_hash' do
    it 'should set fields to values from given hash' do
      team_game_stats = TeamGameStats.new
      hash = {
        team_id:        1,
        season:         2,
        week:           3,
        game:           4,
        wins:           8,
        losses:         6,
        ties:           1,
        home_wins:      5,
        home_losses:    1,
        home_ties:      0,
        road_wins:      3,
        road_losses:    5,
        road_ties:      2,
        points_scored:  82,
        points_allowed: 68
      }

      team_game_stats.from_hash hash

      expect( team_game_stats.team_id        ).to eq hash[ :team_id        ]
      expect( team_game_stats.season         ).to eq hash[ :season         ]
      expect( team_game_stats.week           ).to eq hash[ :week           ]
      expect( team_game_stats.game           ).to eq hash[ :game           ]
      expect( team_game_stats.wins           ).to eq hash[ :wins           ]
      expect( team_game_stats.losses         ).to eq hash[ :losses         ]
      expect( team_game_stats.ties           ).to eq hash[ :ties           ]
      expect( team_game_stats.home_wins      ).to eq hash[ :home_wins      ]
      expect( team_game_stats.home_losses    ).to eq hash[ :home_losses    ]
      expect( team_game_stats.home_ties      ).to eq hash[ :home_ties      ]
      expect( team_game_stats.road_wins      ).to eq hash[ :road_wins      ]
      expect( team_game_stats.road_losses    ).to eq hash[ :road_losses    ]
      expect( team_game_stats.road_ties      ).to eq hash[ :road_ties      ]
      expect( team_game_stats.points_scored  ).to eq hash[ :points_scored  ]
      expect( team_game_stats.points_allowed ).to eq hash[ :points_allowed ]
    end
  end
end
