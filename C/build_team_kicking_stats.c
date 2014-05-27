#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

team_kicking_stats_s *buildTeamKickingStats( const int team_id, const int season, const bowl_game_e bowl_game )
{
     team_kicking_stats_s *team_kicking_stats = NULL;

     if ( (team_kicking_stats = malloc( sizeof(team_kicking_stats_s) )) == NULL ) return NULL;

     memset( team_kicking_stats, '\0', sizeof(team_kicking_stats_s) );

     buildIntoTeamKickingStats( team_kicking_stats, team_id, season, bowl_game );

     return team_kicking_stats;
}

void buildIntoTeamKickingStats( team_kicking_stats_s *team_kicking_stats, const int team_id, const int season, const bowl_game_e bowl_game )
{
     team_kicking_stats->team_id   = team_id;
     team_kicking_stats->season    = season;
     team_kicking_stats->bowl_game = bowl_game;

     team_kicking_stats->extra_point_attempts   = rand() %   75;
     team_kicking_stats->extra_points_made      = rand() %   75;
     team_kicking_stats->field_goal_attempts    = rand() %   30;
     team_kicking_stats->field_goals_made       = rand() %   30;
     team_kicking_stats->punts                  = rand() %   20;
     team_kicking_stats->punt_yards             = rand() %  500;
     team_kicking_stats->kick_returns           = rand() %  100;
     team_kicking_stats->kick_return_yards      = rand() % 1000;
     team_kicking_stats->kick_return_touchdowns = rand() %   10;
     team_kicking_stats->punt_returns           = rand() %   20;
     team_kicking_stats->punt_return_yards      = rand() %  200;
     team_kicking_stats->punt_return_touchdowns = rand() %    5;
}
