location = File.dirname __FILE__
$: << "#{location}"

require 'boxscore_parser'

describe 'BoxscoreParser' do
  describe '#parse' do
    before :each do
      @boxscore_parser = BoxscoreParser.new
    end

    it 'should return a hash' do
      actual = @boxscore_parser.parse 'test_boxscore'

      expect( actual ).to be_a Hash
    end

    it 'should return a hash with both teams as keys' do
      actual = @boxscore_parser.parse 'test_boxscore'

      expect( actual                     ).to be_a Hash
      expect( actual.keys.length         ).to eq   2
      expect( actual['Tennessee'][:home] ).to eq   false
      expect( actual['Missouri' ][:home] ).to eq   true
    end

    it 'should parse team stats for both teams' do
      actual = @boxscore_parser.parse 'test_boxscore'

      expect( actual ).to be_a Hash

      stats = actual['Tennessee'][:stats]

      expect( stats                   ).to be_a Hash
      expect( stats[ :wins          ] ).to eq 0
      expect( stats[ :losses        ] ).to eq 1
      expect( stats[ :ties          ] ).to eq 0
      expect( stats[ :points_scored ] ).to eq 28

      stats = actual['Missouri'][:stats]

      expect( stats                   ).to be_a Hash
      expect( stats[ :wins          ] ).to eq 1
      expect( stats[ :losses        ] ).to eq 0
      expect( stats[ :ties          ] ).to eq 0
      expect( stats[ :points_scored ] ).to eq 31
    end

    it 'should parse team stats when teams are tied' do
      actual = @boxscore_parser.parse 'test_boxscore_tie'

      expect( actual ).to be_a Hash

      stats = actual['Tennessee'][:stats]

      expect( stats                   ).to be_a Hash
      expect( stats[ :wins          ] ).to eq 0
      expect( stats[ :losses        ] ).to eq 0
      expect( stats[ :ties          ] ).to eq 1
      expect( stats[ :points_scored ] ).to eq 28

      stats = actual['Missouri'][:stats]

      expect( stats                   ).to be_a Hash
      expect( stats[ :wins          ] ).to eq 0
      expect( stats[ :losses        ] ).to eq 0
      expect( stats[ :ties          ] ).to eq 1
      expect( stats[ :points_scored ] ).to eq 28
    end

    it 'should parse team offense stats for both teams' do
      actual = @boxscore_parser.parse 'test_boxscore'

      expect( actual ).to be_a Hash

      offense_stats = actual['Tennessee'][:offense_stats]

      expect( offense_stats                     ).to be_a Hash
      expect( offense_stats[ :completions     ] ).to eq  10
      expect( offense_stats[ :pass_attempts   ] ).to eq  13
      expect( offense_stats[ :pass_touchdowns ] ).to eq   4
      expect( offense_stats[ :pass_yards      ] ).to eq 249
      expect( offense_stats[ :rush_attempts   ] ).to eq   9
      expect( offense_stats[ :rush_touchdowns ] ).to eq   0
      expect( offense_stats[ :rush_yards      ] ).to eq  10

      offense_stats = actual['Missouri'][:offense_stats]

      expect( offense_stats                   ).to be_a Hash
      expect( offense_stats[ :completions     ] ).to eq   8
      expect( offense_stats[ :pass_attempts   ] ).to eq   9
      expect( offense_stats[ :pass_touchdowns ] ).to eq   2
      expect( offense_stats[ :pass_yards      ] ).to eq 187
      expect( offense_stats[ :rush_attempts   ] ).to eq  11
      expect( offense_stats[ :rush_touchdowns ] ).to eq   1
      expect( offense_stats[ :rush_yards      ] ).to eq  95
    end

    it 'should parse team defense stats for both teams' do
      actual = @boxscore_parser.parse 'test_boxscore'

      expect( actual ).to be_a Hash

      defense_stats = actual['Tennessee'][:defense_stats]

      expect( defense_stats                       ).to be_a Hash
      expect( defense_stats[ :sacks             ] ).to eq  5
      expect( defense_stats[ :interceptions     ] ).to eq  1
      expect( defense_stats[ :return_yards      ] ).to eq 12
      expect( defense_stats[ :return_touchdowns ] ).to eq  0

      defense_stats = actual['Missouri'][:defense_stats]

      expect( defense_stats                       ).to be_a Hash
      expect( defense_stats[ :sacks             ] ).to eq  3
      expect( defense_stats[ :interceptions     ] ).to eq  3
      expect( defense_stats[ :return_yards      ] ).to eq 73
      expect( defense_stats[ :return_touchdowns ] ).to eq  1
    end

    it 'should parse team kicking stats for both teams' do
      actual = @boxscore_parser.parse 'test_boxscore'

      expect( actual ).to be_a Hash

      kicking_stats = actual['Tennessee'][:kicking_stats]

      expect( kicking_stats                          ).to be_a Hash
      expect( kicking_stats[ :extra_point_attempts ] ).to eq   4
      expect( kicking_stats[ :extra_points_made    ] ).to eq   4
      expect( kicking_stats[ :field_goal_attempts  ] ).to eq   0
      expect( kicking_stats[ :field_goals_made     ] ).to eq   0
      expect( kicking_stats[ :punts                ] ).to eq   1
      expect( kicking_stats[ :punt_yards           ] ).to eq  54

      kicking_stats = actual['Missouri'][:kicking_stats]

      expect( kicking_stats                          ).to be_a Hash
      expect( kicking_stats[ :extra_point_attempts ] ).to eq   4
      expect( kicking_stats[ :extra_points_made    ] ).to eq   4
      expect( kicking_stats[ :field_goal_attempts  ] ).to eq   1
      expect( kicking_stats[ :field_goals_made     ] ).to eq   1
      expect( kicking_stats[ :punts                ] ).to eq   1
      expect( kicking_stats[ :punt_yards           ] ).to eq  37
    end

    it 'should parse players for both teams' do
      actual = @boxscore_parser.parse 'test_boxscore'

      expect( actual ).to be_a Hash

      expect( actual['Tennessee'][:players] ).to be_a Hash
      expect( actual['Missouri' ][:players] ).to be_a Hash
    end

    it 'should create keys in players hash for every player in file' do
      actual = @boxscore_parser.parse 'test_boxscore'

      expect( actual ).to be_a Hash

      players = actual['Tennessee'][:players]

      expect( players       ).to be_a Hash
      expect( players['6' ] ).to be_a Hash
      expect( players['8' ] ).to be_a Hash
      expect( players['14'] ).to be_a Hash
      expect( players['16'] ).to be_a Hash
      expect( players['24'] ).to be_a Hash
      expect( players['31'] ).to be_a Hash
      expect( players['36'] ).to be_a Hash
      expect( players['53'] ).to be_a Hash
      expect( players['54'] ).to be_a Hash
      expect( players['80'] ).to be_a Hash
      expect( players['84'] ).to be_a Hash
      expect( players['86'] ).to be_a Hash
      expect( players['93'] ).to be_a Hash
      expect( players['94'] ).to be_a Hash

      players = actual['Missouri'][:players]

      expect( players       ).to be_a Hash
      expect( players['1' ] ).to be_a Hash
      expect( players['4' ] ).to be_a Hash
      expect( players['5' ] ).to be_a Hash
      expect( players['26'] ).to be_a Hash
      expect( players['38'] ).to be_a Hash
      expect( players['44'] ).to be_a Hash
      expect( players['59'] ).to be_a Hash
      expect( players['80'] ).to be_a Hash
      expect( players['82'] ).to be_a Hash
      expect( players['89'] ).to be_a Hash
      expect( players['97'] ).to be_a Hash
      expect( players['99'] ).to be_a Hash
    end

    it 'should parse player offense stats for passers' do
      actual = @boxscore_parser.parse 'test_boxscore'

      expect( actual ).to be_a Hash

      players = actual['Tennessee'][:players]

      offense_stats = players['14'][:stats][:offense]

      expect( offense_stats[ :pass_attempts        ] ).to eq  10
      expect( offense_stats[ :completions          ] ).to eq   9
      expect( offense_stats[ :interceptions        ] ).to eq   3
      expect( offense_stats[ :pass_yards           ] ).to eq 235
      expect( offense_stats[ :pass_touchdowns      ] ).to eq   4
      expect( offense_stats[ :rush_attempts        ] ).to eq   0
      expect( offense_stats[ :rush_yards           ] ).to eq   0
      expect( offense_stats[ :rush_touchdowns      ] ).to eq   0
      expect( offense_stats[ :receptions           ] ).to eq   0
      expect( offense_stats[ :receiving_yards      ] ).to eq   0
      expect( offense_stats[ :receiving_touchdowns ] ).to eq   0

      offense_stats = players['8'][:stats][:offense]

      expect( offense_stats[ :pass_attempts        ] ).to eq   3
      expect( offense_stats[ :completions          ] ).to eq   1
      expect( offense_stats[ :interceptions        ] ).to eq   0
      expect( offense_stats[ :pass_yards           ] ).to eq  14
      expect( offense_stats[ :pass_touchdowns      ] ).to eq   0
      expect( offense_stats[ :rush_attempts        ] ).to eq   0
      expect( offense_stats[ :rush_yards           ] ).to eq   0
      expect( offense_stats[ :rush_touchdowns      ] ).to eq   0
      expect( offense_stats[ :receptions           ] ).to eq   0
      expect( offense_stats[ :receiving_yards      ] ).to eq   0
      expect( offense_stats[ :receiving_touchdowns ] ).to eq   0

      players = actual['Missouri'][:players]

      offense_stats = players['4'][:stats][:offense]

      expect( offense_stats[ :pass_attempts        ] ).to eq   9
      expect( offense_stats[ :completions          ] ).to eq   8
      expect( offense_stats[ :interceptions        ] ).to eq   1
      expect( offense_stats[ :pass_yards           ] ).to eq 187
      expect( offense_stats[ :pass_touchdowns      ] ).to eq   2
      expect( offense_stats[ :rush_attempts        ] ).to eq   0
      expect( offense_stats[ :rush_yards           ] ).to eq   0
      expect( offense_stats[ :rush_touchdowns      ] ).to eq   0
      expect( offense_stats[ :receptions           ] ).to eq   0
      expect( offense_stats[ :receiving_yards      ] ).to eq   0
      expect( offense_stats[ :receiving_touchdowns ] ).to eq   0
    end

    it 'should parse player offense stats for rushers' do
      actual = @boxscore_parser.parse 'test_boxscore'

      expect( actual ).to be_a Hash

      players = actual['Tennessee'][:players]

      offense_stats = players['31'][:stats][:offense]

      expect( offense_stats[ :pass_attempts        ] ).to eq   0
      expect( offense_stats[ :completions          ] ).to eq   0
      expect( offense_stats[ :interceptions        ] ).to eq   0
      expect( offense_stats[ :pass_yards           ] ).to eq   0
      expect( offense_stats[ :pass_touchdowns      ] ).to eq   0
      expect( offense_stats[ :rush_attempts        ] ).to eq   5
      expect( offense_stats[ :rush_yards           ] ).to eq  10
      expect( offense_stats[ :rush_touchdowns      ] ).to eq   0
      expect( offense_stats[ :receptions           ] ).to eq   2
      expect( offense_stats[ :receiving_yards      ] ).to eq  63
      expect( offense_stats[ :receiving_touchdowns ] ).to eq   1

      offense_stats = players['36'][:stats][:offense]

      expect( offense_stats[ :pass_attempts        ] ).to eq   0
      expect( offense_stats[ :completions          ] ).to eq   0
      expect( offense_stats[ :interceptions        ] ).to eq   0
      expect( offense_stats[ :pass_yards           ] ).to eq   0
      expect( offense_stats[ :pass_touchdowns      ] ).to eq   0
      expect( offense_stats[ :rush_attempts        ] ).to eq   4
      expect( offense_stats[ :rush_yards           ] ).to eq   0
      expect( offense_stats[ :rush_touchdowns      ] ).to eq   0
      expect( offense_stats[ :receptions           ] ).to eq   2
      expect( offense_stats[ :receiving_yards      ] ).to eq  70
      expect( offense_stats[ :receiving_touchdowns ] ).to eq   1

      players = actual['Missouri'][:players]

      offense_stats = players['38'][:stats][:offense]

      expect( offense_stats[ :pass_attempts        ] ).to eq   0
      expect( offense_stats[ :completions          ] ).to eq   0
      expect( offense_stats[ :interceptions        ] ).to eq   0
      expect( offense_stats[ :pass_yards           ] ).to eq   0
      expect( offense_stats[ :pass_touchdowns      ] ).to eq   0
      expect( offense_stats[ :rush_attempts        ] ).to eq  11
      expect( offense_stats[ :rush_yards           ] ).to eq  95
      expect( offense_stats[ :rush_touchdowns      ] ).to eq   1
      expect( offense_stats[ :receptions           ] ).to eq   0
      expect( offense_stats[ :receiving_yards      ] ).to eq   0
      expect( offense_stats[ :receiving_touchdowns ] ).to eq   0
    end

    it 'should parse player offense stats for receivers' do
      actual = @boxscore_parser.parse 'test_boxscore'

      expect( actual ).to be_a Hash

      players = actual['Tennessee'][:players]

      offense_stats = players['80'][:stats][:offense]

      expect( offense_stats[ :pass_attempts        ] ).to eq   0
      expect( offense_stats[ :completions          ] ).to eq   0
      expect( offense_stats[ :interceptions        ] ).to eq   0
      expect( offense_stats[ :pass_yards           ] ).to eq   0
      expect( offense_stats[ :pass_touchdowns      ] ).to eq   0
      expect( offense_stats[ :rush_attempts        ] ).to eq   0
      expect( offense_stats[ :rush_yards           ] ).to eq   0
      expect( offense_stats[ :rush_touchdowns      ] ).to eq   0
      expect( offense_stats[ :receptions           ] ).to eq   1
      expect( offense_stats[ :receiving_yards      ] ).to eq  12
      expect( offense_stats[ :receiving_touchdowns ] ).to eq   1

      offense_stats = players['84'][:stats][:offense]

      expect( offense_stats[ :pass_attempts        ] ).to eq   0
      expect( offense_stats[ :completions          ] ).to eq   0
      expect( offense_stats[ :interceptions        ] ).to eq   0
      expect( offense_stats[ :pass_yards           ] ).to eq   0
      expect( offense_stats[ :pass_touchdowns      ] ).to eq   0
      expect( offense_stats[ :rush_attempts        ] ).to eq   0
      expect( offense_stats[ :rush_yards           ] ).to eq   0
      expect( offense_stats[ :rush_touchdowns      ] ).to eq   0
      expect( offense_stats[ :receptions           ] ).to eq   1
      expect( offense_stats[ :receiving_yards      ] ).to eq  37
      expect( offense_stats[ :receiving_touchdowns ] ).to eq   0

      offense_stats = players['86'][:stats][:offense]

      expect( offense_stats[ :pass_attempts        ] ).to eq   0
      expect( offense_stats[ :completions          ] ).to eq   0
      expect( offense_stats[ :interceptions        ] ).to eq   0
      expect( offense_stats[ :pass_yards           ] ).to eq   0
      expect( offense_stats[ :pass_touchdowns      ] ).to eq   0
      expect( offense_stats[ :rush_attempts        ] ).to eq   0
      expect( offense_stats[ :rush_yards           ] ).to eq   0
      expect( offense_stats[ :rush_touchdowns      ] ).to eq   0
      expect( offense_stats[ :receptions           ] ).to eq   4
      expect( offense_stats[ :receiving_yards      ] ).to eq  67
      expect( offense_stats[ :receiving_touchdowns ] ).to eq   1

      players = actual['Missouri'][:players]

      offense_stats = players['80'][:stats][:offense]

      expect( offense_stats[ :pass_attempts        ] ).to eq   0
      expect( offense_stats[ :completions          ] ).to eq   0
      expect( offense_stats[ :interceptions        ] ).to eq   0
      expect( offense_stats[ :pass_yards           ] ).to eq   0
      expect( offense_stats[ :pass_touchdowns      ] ).to eq   0
      expect( offense_stats[ :rush_attempts        ] ).to eq   0
      expect( offense_stats[ :rush_yards           ] ).to eq   0
      expect( offense_stats[ :rush_touchdowns      ] ).to eq   0
      expect( offense_stats[ :receptions           ] ).to eq   3
      expect( offense_stats[ :receiving_yards      ] ).to eq  66
      expect( offense_stats[ :receiving_touchdowns ] ).to eq   1

      offense_stats = players['82'][:stats][:offense]

      expect( offense_stats[ :pass_attempts        ] ).to eq   0
      expect( offense_stats[ :completions          ] ).to eq   0
      expect( offense_stats[ :interceptions        ] ).to eq   0
      expect( offense_stats[ :pass_yards           ] ).to eq   0
      expect( offense_stats[ :pass_touchdowns      ] ).to eq   0
      expect( offense_stats[ :rush_attempts        ] ).to eq   0
      expect( offense_stats[ :rush_yards           ] ).to eq   0
      expect( offense_stats[ :rush_touchdowns      ] ).to eq   0
      expect( offense_stats[ :receptions           ] ).to eq   1
      expect( offense_stats[ :receiving_yards      ] ).to eq  12
      expect( offense_stats[ :receiving_touchdowns ] ).to eq   0

      offense_stats = players['89'][:stats][:offense]

      expect( offense_stats[ :pass_attempts        ] ).to eq   0
      expect( offense_stats[ :completions          ] ).to eq   0
      expect( offense_stats[ :interceptions        ] ).to eq   0
      expect( offense_stats[ :pass_yards           ] ).to eq   0
      expect( offense_stats[ :pass_touchdowns      ] ).to eq   0
      expect( offense_stats[ :rush_attempts        ] ).to eq   0
      expect( offense_stats[ :rush_yards           ] ).to eq   0
      expect( offense_stats[ :rush_touchdowns      ] ).to eq   0
      expect( offense_stats[ :receptions           ] ).to eq   4
      expect( offense_stats[ :receiving_yards      ] ).to eq 109
      expect( offense_stats[ :receiving_touchdowns ] ).to eq   1
    end

    it 'should parse player returns stats for kick returners' do
      actual = @boxscore_parser.parse 'test_boxscore'

      expect( actual ).to be_a Hash

      players = actual['Tennessee'][:players]

      returns_stats = players['86'][:stats][:returns]

      expect( returns_stats[ :kick_returns           ] ).to eq   6
      expect( returns_stats[ :kick_return_yards      ] ).to eq 114
      expect( returns_stats[ :kick_return_touchdowns ] ).to eq   0
      expect( returns_stats[ :punt_returns           ] ).to eq   1
      expect( returns_stats[ :punt_return_yards      ] ).to eq   0
      expect( returns_stats[ :punt_return_touchdowns ] ).to eq   0

      players = actual['Missouri'][:players]

      returns_stats = players['80'][:stats][:returns]

      expect( returns_stats[ :kick_returns           ] ).to eq   4
      expect( returns_stats[ :kick_return_yards      ] ).to eq 131
      expect( returns_stats[ :kick_return_touchdowns ] ).to eq   0
      expect( returns_stats[ :punt_returns           ] ).to eq   1
      expect( returns_stats[ :punt_return_yards      ] ).to eq   6
      expect( returns_stats[ :punt_return_touchdowns ] ).to eq   0
    end

    it 'should parse player defense stats for defenders' do
      actual = @boxscore_parser.parse 'test_boxscore'

      expect( actual ).to be_a Hash

      players = actual['Tennessee'][:players]

      defense_stats = players['24'][:stats][:defense]

      expect( defense_stats[ :sacks             ] ).to eq   0
      expect( defense_stats[ :interceptions     ] ).to eq   1
      expect( defense_stats[ :return_yards      ] ).to eq  12
      expect( defense_stats[ :return_touchdowns ] ).to eq   0

      defense_stats = players['53'][:stats][:defense]

      expect( defense_stats[ :sacks             ] ).to eq   1
      expect( defense_stats[ :interceptions     ] ).to eq   0
      expect( defense_stats[ :return_yards      ] ).to eq   0
      expect( defense_stats[ :return_touchdowns ] ).to eq   0

      defense_stats = players['54'][:stats][:defense]

      expect( defense_stats[ :sacks             ] ).to eq   1
      expect( defense_stats[ :interceptions     ] ).to eq   0
      expect( defense_stats[ :return_yards      ] ).to eq   0
      expect( defense_stats[ :return_touchdowns ] ).to eq   0

      defense_stats = players['93'][:stats][:defense]

      expect( defense_stats[ :sacks             ] ).to eq   2
      expect( defense_stats[ :interceptions     ] ).to eq   0
      expect( defense_stats[ :return_yards      ] ).to eq   0
      expect( defense_stats[ :return_touchdowns ] ).to eq   0

      defense_stats = players['94'][:stats][:defense]

      expect( defense_stats[ :sacks             ] ).to eq   1
      expect( defense_stats[ :interceptions     ] ).to eq   0
      expect( defense_stats[ :return_yards      ] ).to eq   0
      expect( defense_stats[ :return_touchdowns ] ).to eq   0

      players = actual['Missouri'][:players]

      defense_stats = players['26'][:stats][:defense]

      expect( defense_stats[ :sacks             ] ).to eq   0
      expect( defense_stats[ :interceptions     ] ).to eq   1
      expect( defense_stats[ :return_yards      ] ).to eq  60
      expect( defense_stats[ :return_touchdowns ] ).to eq   1

      defense_stats = players['44'][:stats][:defense]

      expect( defense_stats[ :sacks             ] ).to eq   0
      expect( defense_stats[ :interceptions     ] ).to eq   2
      expect( defense_stats[ :return_yards      ] ).to eq  13
      expect( defense_stats[ :return_touchdowns ] ).to eq   0

      defense_stats = players['59'][:stats][:defense]

      expect( defense_stats[ :sacks             ] ).to eq   1
      expect( defense_stats[ :interceptions     ] ).to eq   0
      expect( defense_stats[ :return_yards      ] ).to eq   0
      expect( defense_stats[ :return_touchdowns ] ).to eq   0

      defense_stats = players['97'][:stats][:defense]

      expect( defense_stats[ :sacks             ] ).to eq   1
      expect( defense_stats[ :interceptions     ] ).to eq   0
      expect( defense_stats[ :return_yards      ] ).to eq   0
      expect( defense_stats[ :return_touchdowns ] ).to eq   0

      defense_stats = players['99'][:stats][:defense]

      expect( defense_stats[ :sacks             ] ).to eq   1
      expect( defense_stats[ :interceptions     ] ).to eq   0
      expect( defense_stats[ :return_yards      ] ).to eq   0
      expect( defense_stats[ :return_touchdowns ] ).to eq   0
    end

    it 'should parse player kicking stats for kickers' do
      actual = @boxscore_parser.parse 'test_boxscore'

      expect( actual ).to be_a Hash

      players = actual['Tennessee'][:players]

      kicking_stats = players['16'][:stats][:kicking]

      expect( kicking_stats[ :extra_point_attempts ] ).to eq   4
      expect( kicking_stats[ :extra_points_made    ] ).to eq   4
      expect( kicking_stats[ :field_goal_attempts  ] ).to eq   0
      expect( kicking_stats[ :field_goals_made     ] ).to eq   0
      expect( kicking_stats[ :punts                ] ).to eq   0
      expect( kicking_stats[ :punt_yards           ] ).to eq   0

      players = actual['Missouri'][:players]

      kicking_stats = players['5'][:stats][:kicking]

      expect( kicking_stats[ :extra_point_attempts ] ).to eq   4
      expect( kicking_stats[ :extra_points_made    ] ).to eq   4
      expect( kicking_stats[ :field_goal_attempts  ] ).to eq   1
      expect( kicking_stats[ :field_goals_made     ] ).to eq   1
      expect( kicking_stats[ :punts                ] ).to eq   0
      expect( kicking_stats[ :punt_yards           ] ).to eq   0
    end

    it 'should parse player kicking stats for punters' do
      actual = @boxscore_parser.parse 'test_boxscore'

      expect( actual ).to be_a Hash

      players = actual['Tennessee'][:players]

      kicking_stats = players['6'][:stats][:kicking]

      expect( kicking_stats[ :extra_point_attempts ] ).to eq   0
      expect( kicking_stats[ :extra_points_made    ] ).to eq   0
      expect( kicking_stats[ :field_goal_attempts  ] ).to eq   0
      expect( kicking_stats[ :field_goals_made     ] ).to eq   0
      expect( kicking_stats[ :punts                ] ).to eq   1
      expect( kicking_stats[ :punt_yards           ] ).to eq  54

      players = actual['Missouri'][:players]

      kicking_stats = players['1'][:stats][:kicking]

      expect( kicking_stats[ :extra_point_attempts ] ).to eq   0
      expect( kicking_stats[ :extra_points_made    ] ).to eq   0
      expect( kicking_stats[ :field_goal_attempts  ] ).to eq   0
      expect( kicking_stats[ :field_goals_made     ] ).to eq   0
      expect( kicking_stats[ :punts                ] ).to eq   1
      expect( kicking_stats[ :punt_yards           ] ).to eq  37
    end

    it 'should only give defensive stats to defensive players' do
      actual = @boxscore_parser.parse 'test_boxscore'

      expect( actual ).to be_a Hash

      players = actual['Tennessee'][:players]

      expect( players       ).to be_a Hash
      expect( players['6' ][:stats][:defense] ).to be_nil
      expect( players['8' ][:stats][:defense] ).to be_nil
      expect( players['14'][:stats][:defense] ).to be_nil
      expect( players['16'][:stats][:defense] ).to be_nil
      expect( players['31'][:stats][:defense] ).to be_nil
      expect( players['36'][:stats][:defense] ).to be_nil
      expect( players['80'][:stats][:defense] ).to be_nil
      expect( players['84'][:stats][:defense] ).to be_nil
      expect( players['86'][:stats][:defense] ).to be_nil

      players = actual['Missouri'][:players]

      expect( players       ).to be_a Hash
      expect( players['1' ][:stats][:defense] ).to be_nil
      expect( players['4' ][:stats][:defense] ).to be_nil
      expect( players['5' ][:stats][:defense] ).to be_nil
      expect( players['38'][:stats][:defense] ).to be_nil
      expect( players['80'][:stats][:defense] ).to be_nil
      expect( players['82'][:stats][:defense] ).to be_nil
      expect( players['89'][:stats][:defense] ).to be_nil
    end

    it 'should only give offensive stats to offensive players' do
      actual = @boxscore_parser.parse 'test_boxscore'

      expect( actual ).to be_a Hash

      players = actual['Tennessee'][:players]

      expect( players       ).to be_a Hash
      expect( players['6' ][:stats][:offense] ).to be_nil
      expect( players['16'][:stats][:offense] ).to be_nil
      expect( players['24'][:stats][:offense] ).to be_nil
      expect( players['53'][:stats][:offense] ).to be_nil
      expect( players['54'][:stats][:offense] ).to be_nil
      expect( players['93'][:stats][:offense] ).to be_nil
      expect( players['94'][:stats][:offense] ).to be_nil

      players = actual['Missouri'][:players]

      expect( players       ).to be_a Hash
      expect( players['1' ][:stats][:offense] ).to be_nil
      expect( players['5' ][:stats][:offense] ).to be_nil
      expect( players['26'][:stats][:offense] ).to be_nil
      expect( players['44'][:stats][:offense] ).to be_nil
      expect( players['59'][:stats][:offense] ).to be_nil
      expect( players['97'][:stats][:offense] ).to be_nil
      expect( players['99'][:stats][:offense] ).to be_nil
    end

    it 'should only give kicking stats to kickers' do
      actual = @boxscore_parser.parse 'test_boxscore'

      expect( actual ).to be_a Hash

      players = actual['Tennessee'][:players]

      expect( players       ).to be_a Hash
      expect( players['8' ][:stats][:kicking] ).to be_nil
      expect( players['14'][:stats][:kicking] ).to be_nil
      expect( players['24'][:stats][:kicking] ).to be_nil
      expect( players['31'][:stats][:kicking] ).to be_nil
      expect( players['36'][:stats][:kicking] ).to be_nil
      expect( players['53'][:stats][:kicking] ).to be_nil
      expect( players['54'][:stats][:kicking] ).to be_nil
      expect( players['80'][:stats][:kicking] ).to be_nil
      expect( players['84'][:stats][:kicking] ).to be_nil
      expect( players['86'][:stats][:kicking] ).to be_nil
      expect( players['93'][:stats][:kicking] ).to be_nil
      expect( players['94'][:stats][:kicking] ).to be_nil

      players = actual['Missouri'][:players]

      expect( players       ).to be_a Hash
      expect( players['4' ][:stats][:kicking] ).to be_nil
      expect( players['26'][:stats][:kicking] ).to be_nil
      expect( players['38'][:stats][:kicking] ).to be_nil
      expect( players['44'][:stats][:kicking] ).to be_nil
      expect( players['59'][:stats][:kicking] ).to be_nil
      expect( players['80'][:stats][:kicking] ).to be_nil
      expect( players['82'][:stats][:kicking] ).to be_nil
      expect( players['89'][:stats][:kicking] ).to be_nil
      expect( players['97'][:stats][:kicking] ).to be_nil
      expect( players['99'][:stats][:kicking] ).to be_nil
    end

    it 'should only give returns stats to players that had returns' do
      actual = @boxscore_parser.parse 'test_boxscore'

      expect( actual ).to be_a Hash

      players = actual['Tennessee'][:players]

      expect( players       ).to be_a Hash
      expect( players['6' ][:stats][:returns] ).to be_nil
      expect( players['8' ][:stats][:returns] ).to be_nil
      expect( players['14'][:stats][:returns] ).to be_nil
      expect( players['16'][:stats][:returns] ).to be_nil
      expect( players['24'][:stats][:returns] ).to be_nil
      expect( players['31'][:stats][:returns] ).to be_nil
      expect( players['36'][:stats][:returns] ).to be_nil
      expect( players['53'][:stats][:returns] ).to be_nil
      expect( players['54'][:stats][:returns] ).to be_nil
      expect( players['80'][:stats][:returns] ).to be_nil
      expect( players['84'][:stats][:returns] ).to be_nil
      expect( players['93'][:stats][:returns] ).to be_nil
      expect( players['94'][:stats][:returns] ).to be_nil

      players = actual['Missouri'][:players]

      expect( players       ).to be_a Hash
      expect( players['1' ][:stats][:returns] ).to be_nil
      expect( players['4' ][:stats][:returns] ).to be_nil
      expect( players['5' ][:stats][:returns] ).to be_nil
      expect( players['26'][:stats][:returns] ).to be_nil
      expect( players['38'][:stats][:returns] ).to be_nil
      expect( players['44'][:stats][:returns] ).to be_nil
      expect( players['59'][:stats][:returns] ).to be_nil
      expect( players['82'][:stats][:returns] ).to be_nil
      expect( players['89'][:stats][:returns] ).to be_nil
      expect( players['97'][:stats][:returns] ).to be_nil
      expect( players['99'][:stats][:returns] ).to be_nil
    end
  end
end
