#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

team_game_kicking_stats_s *buildTeamGameKickingStats( const int team_id, const int season, const int week, const int game )
{
     team_game_kicking_stats_s *team_game_kicking_stats = NULL;

     if ( (team_game_kicking_stats = malloc( sizeof(team_game_kicking_stats_s) )) == NULL ) return NULL;

     memset( team_game_kicking_stats, '\0', sizeof(team_game_kicking_stats_s) );

     buildIntoTeamGameKickingStats( team_game_kicking_stats, team_id, season, week, game );

     return team_game_kicking_stats;
}

void buildIntoTeamGameKickingStats( team_game_kicking_stats_s *team_game_kicking_stats, const int team_id, const int season, const int week, const int game )
{
     team_game_kicking_stats->team_id = team_id;
     team_game_kicking_stats->season  = season;
     team_game_kicking_stats->week    = week;
     team_game_kicking_stats->game    = game;

     team_game_kicking_stats->extra_point_attempts   = rand() %   10;
     team_game_kicking_stats->extra_points_made      = rand() %   10;
     team_game_kicking_stats->field_goal_attempts    = rand() %   10;
     team_game_kicking_stats->field_goals_made       = rand() %   10;
     team_game_kicking_stats->punts                  = rand() %   10;
     team_game_kicking_stats->punt_yards             = rand() %  100;
     team_game_kicking_stats->kick_returns           = rand() %   10;
     team_game_kicking_stats->kick_return_yards      = rand() %  200;
     team_game_kicking_stats->kick_return_touchdowns = rand() %    5;
     team_game_kicking_stats->punt_returns           = rand() %   10;
     team_game_kicking_stats->punt_return_yards      = rand() %  100;
     team_game_kicking_stats->punt_return_touchdowns = rand() %    5;
}
