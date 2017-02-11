#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"
require 'repository'
require 'utils'

describe 'Data Consistency' do
  before :all do
    @repository = Repository.new Utils.get_db "#{location}/../ncfo.db"
  end

  it 'should have team stats match sum of team game stats' do
    team_stats = @repository.custom_read 'select * from team_stats_t where bowl_game = 0 order by season, team_id'

    team_stats.each do |season|
      query = %Q|select sum(wins)           wins,
                        sum(losses)         losses,
                        sum(ties)           ties,
                        sum(home_wins)      home_wins,
                        sum(home_losses)    home_losses,
                        sum(home_ties)      home_ties,
                        sum(road_wins)      road_wins,
                        sum(road_losses)    road_losses,
                        sum(road_ties)      road_ties,
                        sum(points_scored)  points_scored,
                        sum(points_allowed) points_allowed
                from    team_game_stats_t
                where   team_id = #{season[:team_id]}
                and     season  = #{season[:season]}|

      games = @repository.custom_read( query )[0]

      begin
        expect( games[ :wins           ] ).to eq season[ :wins           ]
        expect( games[ :losses         ] ).to eq season[ :losses         ]
        expect( games[ :ties           ] ).to eq season[ :ties           ]
        expect( games[ :home_wins      ] ).to eq season[ :home_wins      ]
        expect( games[ :home_losses    ] ).to eq season[ :home_losses    ]
        expect( games[ :home_ties      ] ).to eq season[ :home_ties      ]
        expect( games[ :road_wins      ] ).to eq season[ :road_wins      ]
        expect( games[ :road_losses    ] ).to eq season[ :road_losses    ]
        expect( games[ :road_ties      ] ).to eq season[ :road_ties      ]
        expect( games[ :points_scored  ] ).to eq season[ :points_scored  ]
        expect( games[ :points_allowed ] ).to eq season[ :points_allowed ]
      rescue Exception => e
        puts "**** Error while verifying team_id: #{season[:team_id]}, season: #{season[:season]}"
        raise e
      end
    end
  end

  it 'should have team offense stats match sum of team game offense stats' do
    team_offense_stats = @repository.custom_read 'select * from team_offense_stats_t where bowl_game = 0 order by season, team_id'

    team_offense_stats.each do |season|
      query = %Q|select sum(pass_attempts)   pass_attempts,
                        sum(completions)     completions,
                        sum(interceptions)   interceptions,
                        sum(pass_yards)      pass_yards,
                        sum(pass_touchdowns) pass_touchdowns,
                        sum(rush_attempts)   rush_attempts,
                        sum(rush_yards)      rush_yards,
                        sum(rush_touchdowns) rush_touchdowns
                from    team_game_offense_stats_t
                where   team_id = #{season[:team_id]}
                and     season  = #{season[:season]}|

      games = @repository.custom_read( query )[0]

      begin
        expect( games[ :pass_attempts   ] ).to eq season[ :pass_attempts   ]
        expect( games[ :completions     ] ).to eq season[ :completions     ]
        expect( games[ :interceptions   ] ).to eq season[ :interceptions   ]
        expect( games[ :pass_yards      ] ).to eq season[ :pass_yards      ]
        expect( games[ :pass_touchdowns ] ).to eq season[ :pass_touchdowns ]
        expect( games[ :rush_attempts   ] ).to eq season[ :rush_attempts   ]
        expect( games[ :rush_yards      ] ).to eq season[ :rush_yards      ]
        expect( games[ :rush_touchdowns ] ).to eq season[ :rush_touchdowns ]
      rescue Exception => e
        puts "**** Error while verifying team_id: #{season[:team_id]}, season: #{season[:season]}"
        raise e
      end
    end
  end

  it 'should have team defense stats match sum of team game defense stats' do
    team_defense_stats = @repository.custom_read 'select * from team_defense_stats_t where bowl_game = 0 order by season, team_id'

    team_defense_stats.each do |season|
      query = %Q|select sum(sacks)             sacks,
                        sum(interceptions)     interceptions,
                        sum(return_yards)      return_yards,
                        sum(return_touchdowns) return_touchdowns,
                        sum(yards_allowed)     yards_allowed
                from    team_game_defense_stats_t
                where   team_id = #{season[:team_id]}
                and     season  = #{season[:season]}|

      games = @repository.custom_read( query )[0]

      begin
        expect( games[ :sacks             ] ).to eq season[ :sacks             ]
        expect( games[ :interceptions     ] ).to eq season[ :interceptions     ]
        expect( games[ :return_yards      ] ).to eq season[ :return_yards      ]
        expect( games[ :return_touchdowns ] ).to eq season[ :return_touchdowns ]
        expect( games[ :yards_allowed     ] ).to eq season[ :yards_allowed     ]
      rescue Exception => e
        puts "**** Error while verifying team_id: #{season[:team_id]}, season: #{season[:season]}"
        raise e
      end
    end
  end

  it 'should have team kicking stats match sum of team game kicking stats' do
    team_kicking_stats = @repository.custom_read 'select * from team_kicking_stats_t where bowl_game = 0 order by season, team_id'

    team_kicking_stats.each do |season|
      query = %Q|select sum(extra_point_attempts)   extra_point_attempts,
                        sum(extra_points_made)      extra_points_made,
                        sum(field_goal_attempts)    field_goal_attempts,
                        sum(field_goals_made)       field_goals_made,
                        sum(punts)                  punts,
                        sum(punt_yards)             punt_yards,
                        sum(kick_returns)           kick_returns,
                        sum(kick_return_yards)      kick_return_yards,
                        sum(kick_return_touchdowns) kick_return_touchdowns,
                        sum(punt_returns)           punt_returns,
                        sum(punt_return_yards)      punt_return_yards,
                        sum(punt_return_touchdowns) punt_return_touchdowns
                from    team_game_kicking_stats_t
                where   team_id = #{season[:team_id]}
                and     season  = #{season[:season]}|

      games = @repository.custom_read( query )[0]

      begin
        expect( games[ :extra_point_attempts   ] ).to eq season[ :extra_point_attempts   ]
        expect( games[ :extra_points_made      ] ).to eq season[ :extra_points_made      ]
        expect( games[ :field_goal_attempts    ] ).to eq season[ :field_goal_attempts    ]
        expect( games[ :field_goals_made       ] ).to eq season[ :field_goals_made       ]
        expect( games[ :punts                  ] ).to eq season[ :punts                  ]
        expect( games[ :punt_yards             ] ).to eq season[ :punt_yards             ]
        expect( games[ :kick_returns           ] ).to eq season[ :kick_returns           ]
        expect( games[ :kick_return_yards      ] ).to eq season[ :kick_return_yards      ]
        expect( games[ :kick_return_touchdowns ] ).to eq season[ :kick_return_touchdowns ]
        expect( games[ :punt_returns           ] ).to eq season[ :punt_returns           ]
        expect( games[ :punt_return_yards      ] ).to eq season[ :punt_return_yards      ]
        expect( games[ :punt_return_touchdowns ] ).to eq season[ :punt_return_touchdowns ]
      rescue Exception => e
        puts "**** Error while verifying team_id: #{season[:team_id]}, season: #{season[:season]}"
        raise e
      end
    end
  end

  it 'should have player offense stats match sum of player game offense stats' do
    player_offense_stats = @repository.custom_read 'select * from player_offense_stats_t where bowl_game = 0 order by season, player_id'

    player_offense_stats.each do |season|
      query = %Q|select sum(pass_attempts)        pass_attempts,
                        sum(completions)          completions,
                        sum(interceptions)        interceptions,
                        sum(pass_yards)           pass_yards,
                        sum(pass_touchdowns)      pass_touchdowns,
                        sum(rush_attempts)        rush_attempts,
                        sum(rush_yards)           rush_yards,
                        sum(rush_touchdowns)      rush_touchdowns,
                        sum(receptions)           receptions,
                        sum(receiving_yards)      receiving_yards,
                        sum(receiving_touchdowns) receiving_touchdowns
                from    player_game_offense_stats_t
                where   player_id = #{season[:player_id]}
                and     season    = #{season[:season]}|

      games = @repository.custom_read( query )[0]

      begin
        expect( games[ :pass_attempts        ] ).to eq season[ :pass_attempts        ]
        expect( games[ :completions          ] ).to eq season[ :completions          ]
        expect( games[ :interceptions        ] ).to eq season[ :interceptions        ]
        expect( games[ :pass_yards           ] ).to eq season[ :pass_yards           ]
        expect( games[ :pass_touchdowns      ] ).to eq season[ :pass_touchdowns      ]
        expect( games[ :rush_attempts        ] ).to eq season[ :rush_attempts        ]
        expect( games[ :rush_yards           ] ).to eq season[ :rush_yards           ]
        expect( games[ :rush_touchdowns      ] ).to eq season[ :rush_touchdowns      ]
        expect( games[ :receptions           ] ).to eq season[ :receptions           ]
        expect( games[ :receiving_yards      ] ).to eq season[ :receiving_yards      ]
        expect( games[ :receiving_touchdowns ] ).to eq season[ :receiving_touchdowns ]
      rescue Exception => e
        puts "**** Error while verifying player_id: #{season[:player_id]}, season: #{season[:season]}"
        raise e
      end
    end
  end

  it 'should have player defense stats match sum of player game defense stats' do
    player_defense_stats = @repository.custom_read 'select * from player_defense_stats_t where bowl_game = 0 order by season, player_id'

    player_defense_stats.each do |season|
      query = %Q|select sum(sacks)             sacks,
                        sum(interceptions)     interceptions,
                        sum(return_yards)      return_yards,
                        sum(return_touchdowns) return_touchdowns
                from    player_game_defense_stats_t
                where   player_id = #{season[:player_id]}
                and     season    = #{season[:season]}|

      games = @repository.custom_read( query )[0]

      begin
        expect( games[ :sacks             ] ).to eq season[ :sacks             ]
        expect( games[ :interceptions     ] ).to eq season[ :interceptions     ]
        expect( games[ :return_yards      ] ).to eq season[ :return_yards      ]
        expect( games[ :return_touchdowns ] ).to eq season[ :return_touchdowns ]
      rescue Exception => e
        puts "**** Error while verifying player_id: #{season[:player_id]}, season: #{season[:season]}"
        raise e
      end
    end
  end

  it 'should have player kicking stats match sum of player game kicking stats' do
    player_kicking_stats = @repository.custom_read 'select * from player_kicking_stats_t where bowl_game = 0 order by season, player_id'

    player_kicking_stats.each do |season|
      query = %Q|select sum(extra_point_attempts)   extra_point_attempts,
                        sum(extra_points_made)      extra_points_made,
                        sum(field_goal_attempts)    field_goal_attempts,
                        sum(field_goals_made)       field_goals_made,
                        sum(punts)                  punts,
                        sum(punt_yards)             punt_yards
                from    player_game_kicking_stats_t
                where   player_id = #{season[:player_id]}
                and     season    = #{season[:season]}|

      games = @repository.custom_read( query )[0]

      begin
        expect( games[ :extra_point_attempts   ] ).to eq season[ :extra_point_attempts   ]
        expect( games[ :extra_points_made      ] ).to eq season[ :extra_points_made      ]
        expect( games[ :field_goal_attempts    ] ).to eq season[ :field_goal_attempts    ]
        expect( games[ :field_goals_made       ] ).to eq season[ :field_goals_made       ]
        expect( games[ :punts                  ] ).to eq season[ :punts                  ]
        expect( games[ :punt_yards             ] ).to eq season[ :punt_yards             ]
      rescue Exception => e
        puts "**** Error while verifying player_id: #{season[:player_id]}, season: #{season[:season]}"
        raise e
      end
    end
  end

  it 'should have player returns stats match sum of player game returns stats' do
    player_returns_stats = @repository.custom_read 'select * from player_returns_stats_t where bowl_game = 0 order by season, player_id'

    player_returns_stats.each do |season|
      query = %Q|select sum(kick_returns)           kick_returns,
                        sum(kick_return_yards)      kick_return_yards,
                        sum(kick_return_touchdowns) kick_return_touchdowns,
                        sum(punt_returns)           punt_returns,
                        sum(punt_return_yards)      punt_return_yards,
                        sum(punt_return_touchdowns) punt_return_touchdowns
                from    player_game_returns_stats_t
                where   player_id = #{season[:player_id]}
                and     season  = #{season[:season]}|

      games = @repository.custom_read( query )[0]

      begin
        expect( games[ :kick_returns           ] ).to eq season[ :kick_returns           ]
        expect( games[ :kick_return_yards      ] ).to eq season[ :kick_return_yards      ]
        expect( games[ :kick_return_touchdowns ] ).to eq season[ :kick_return_touchdowns ]
        expect( games[ :punt_returns           ] ).to eq season[ :punt_returns           ]
        expect( games[ :punt_return_yards      ] ).to eq season[ :punt_return_yards      ]
        expect( games[ :punt_return_touchdowns ] ).to eq season[ :punt_return_touchdowns ]
      rescue Exception => e
        puts "**** Error while verifying player_id: #{season[:player_id]}, season: #{season[:season]}"
        raise e
      end
    end
  end
end
