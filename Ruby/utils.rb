require 'sqlite3'

module Utils
  def self.roll weights
    prng = Random.new

    x = prng.rand weights.inject( :+ ) + 1

    weights.each_index do |idx|
      return idx unless (x -= weights[idx]) > 0
    end
  end

  def self.get_db file
    db = SQLite3::Database.new file

    db.results_as_hash  = true
    db.type_translation = true

    db
  end

  def self.org_delta( org1, org2 )
    return if org1[:conferences].nil?

    org1[:conferences].each_with_index do |conference1, i|
      conf2 = org2[:conferences][i]

      next if conference1[:teams].nil?

      conference1[:teams].each_with_index do |team1, j|
        team2 = conf2[:teams][j]

        unless team1[:stats].nil?
          team1[:stats][:wins          ] -= team2[:stats][:wins          ]
          team1[:stats][:losses        ] -= team2[:stats][:losses        ]
          team1[:stats][:ties          ] -= team2[:stats][:ties          ]
          team1[:stats][:points_scored ] -= team2[:stats][:points_scored ]
          team1[:stats][:points_allowed] -= team2[:stats][:points_allowed]
        end

        unless team1[:offense_stats].nil?
          team1[:offense_stats][:pass_attempts  ] -= team2[:offense_stats][:pass_attempts  ]
          team1[:offense_stats][:completions    ] -= team2[:offense_stats][:completions    ]
          team1[:offense_stats][:interceptions  ] -= team2[:offense_stats][:interceptions  ]
          team1[:offense_stats][:pass_yards     ] -= team2[:offense_stats][:pass_yards     ]
          team1[:offense_stats][:pass_touchdowns] -= team2[:offense_stats][:pass_touchdowns]
          team1[:offense_stats][:rush_attempts  ] -= team2[:offense_stats][:rush_attempts  ]
          team1[:offense_stats][:rush_yards     ] -= team2[:offense_stats][:rush_yards     ]
          team1[:offense_stats][:rush_touchdowns] -= team2[:offense_stats][:rush_touchdowns]
        end

        unless team1[:defense_stats].nil?
          team1[:defense_stats][:sacks            ] -= team2[:defense_stats][:sacks            ]
          team1[:defense_stats][:interceptions    ] -= team2[:defense_stats][:interceptions    ]
          team1[:defense_stats][:return_yards     ] -= team2[:defense_stats][:return_yards     ]
          team1[:defense_stats][:return_touchdowns] -= team2[:defense_stats][:return_touchdowns]
        end

        unless team1[:kicking_stats].nil?
          team1[:kicking_stats][:extra_point_attempts  ] -= team2[:kicking_stats][:extra_point_attempts  ]
          team1[:kicking_stats][:extra_points_made     ] -= team2[:kicking_stats][:extra_points_made     ]
          team1[:kicking_stats][:field_goal_attempts   ] -= team2[:kicking_stats][:field_goal_attempts   ]
          team1[:kicking_stats][:field_goals_made      ] -= team2[:kicking_stats][:field_goals_made      ]
          team1[:kicking_stats][:punts                 ] -= team2[:kicking_stats][:punts                 ]
          team1[:kicking_stats][:punt_yards            ] -= team2[:kicking_stats][:punt_yards            ]
          team1[:kicking_stats][:kick_returns          ] -= team2[:kicking_stats][:kick_returns          ]
          team1[:kicking_stats][:kick_return_yards     ] -= team2[:kicking_stats][:kick_return_yards     ]
          team1[:kicking_stats][:kick_return_touchdowns] -= team2[:kicking_stats][:kick_return_touchdowns]
          team1[:kicking_stats][:punt_returns          ] -= team2[:kicking_stats][:punt_returns          ]
          team1[:kicking_stats][:punt_return_yards     ] -= team2[:kicking_stats][:punt_return_yards     ]
          team1[:kicking_stats][:punt_return_touchdowns] -= team2[:kicking_stats][:punt_return_touchdowns]
        end

        next if team1[:players].nil?

        team1[:players].each_with_index do |player1, k|
          player2 = team2[:players][k]

          unless player1[:stats][:offense].nil?
            offense1 = player1[:stats][:offense]
            offense2 = player2[:stats][:offense]

            offense1[:pass_attempts       ] -= offense2[:pass_attempts       ]
            offense1[:completions         ] -= offense2[:completions         ]
            offense1[:interceptions       ] -= offense2[:interceptions       ]
            offense1[:pass_yards          ] -= offense2[:pass_yards          ]
            offense1[:pass_touchdowns     ] -= offense2[:pass_touchdowns     ]
            offense1[:rush_attempts       ] -= offense2[:rush_attempts       ]
            offense1[:rush_yards          ] -= offense2[:rush_yards          ]
            offense1[:rush_touchdowns     ] -= offense2[:rush_touchdowns     ]
            offense1[:receptions          ] -= offense2[:receptions          ]
            offense1[:receiving_yards     ] -= offense2[:receiving_yards     ]
            offense1[:receiving_touchdowns] -= offense2[:receiving_touchdowns]
          end

          unless player1[:stats][:defense].nil?
            defense1 = player1[:stats][:defense]
            defense2 = player2[:stats][:defense]

            defense1[:sacks            ] -= defense2[:sacks            ]
            defense1[:interceptions    ] -= defense2[:interceptions    ]
            defense1[:return_yards     ] -= defense2[:return_yards     ]
            defense1[:return_touchdowns] -= defense2[:return_touchdowns]
          end

          unless player1[:stats][:kicking].nil?
            kicking1 = player1[:stats][:kicking]
            kicking2 = player2[:stats][:kicking]

            kicking1[:extra_point_attempts] -= kicking2[:extra_point_attempts]
            kicking1[:extra_points_made   ] -= kicking2[:extra_points_made   ]
            kicking1[:field_goal_attempts ] -= kicking2[:field_goal_attempts ]
            kicking1[:field_goals_made    ] -= kicking2[:field_goals_made    ]
            kicking1[:punts               ] -= kicking2[:punts               ]
            kicking1[:punt_yards          ] -= kicking2[:punt_yards          ]
          end

          unless player1[:stats][:returns].nil?
            returns1 = player1[:stats][:returns]
            returns2 = player2[:stats][:returns]

            returns1[:kick_returns          ] -= returns2[:kick_returns          ]
            returns1[:kick_return_yards     ] -= returns2[:kick_return_yards     ]
            returns1[:kick_return_touchdowns] -= returns2[:kick_return_touchdowns]
            returns1[:punt_returns          ] -= returns2[:punt_returns          ]
            returns1[:punt_return_yards     ] -= returns2[:punt_return_yards     ]
            returns1[:punt_return_touchdowns] -= returns2[:punt_return_touchdowns]
          end
        end
      end
    end
  end
end
