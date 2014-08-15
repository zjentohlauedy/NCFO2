#!/usr/bin/env ruby
#
location = File.dirname __FILE__

$: << "#{location}"
require 'json'
require 'ProgRunner'
require 'top_players'


def org_delta( org1, org2 )
  org1[:conferences].each_with_index do |conference1, i|
    conf2 = org2[:conferences][i]

    conference1[:teams].each_with_index do |team1, j|
      team2 = conf2[:teams][j]

      team1[:stats][:wins          ] -= team2[:stats][:wins          ]
      team1[:stats][:losses        ] -= team2[:stats][:losses        ]
      team1[:stats][:ties          ] -= team2[:stats][:ties          ]
      team1[:stats][:points_scored ] -= team2[:stats][:points_scored ]
      team1[:stats][:points_allowed] -= team2[:stats][:points_allowed]

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


path = ARGV[0] or '.'

extract_data = ProgRunner.new "#{location}/../C", "extract_data"

extract_data.execute "#{path}/ncfo1.nes", "#{path}/ncfo1.nst", "#{path}/ncfo2.nes", "#{path}/ncfo2.nst"

org = JSON.parse extract_data.get_output, {:symbolize_names => true}

if File.file?("#{path}/.ncfo1.nst.bak") and File.file?("#{path}/.ncfo2.nst.bak")
  extract_data.execute "#{path}/ncfo1.nes", "#{path}/.ncfo1.nst.bak", "#{path}/ncfo2.nes", "#{path}/.ncfo2.nst.bak"

  old_org = JSON.parse extract_data.get_output, {:symbolize_names => true}

  org_delta org, old_org
end

sr = StatRankings.new org

sr.process_categories @categories
