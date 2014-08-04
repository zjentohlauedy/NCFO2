#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

team_stats_s *buildTeamStats( const int team_id, const int season, const bowl_game_e bowl_game )
{
     team_stats_s *team_stats = NULL;

     if ( (team_stats = malloc( sizeof(team_stats_s) )) == NULL ) return NULL;

     memset( team_stats, '\0', sizeof(team_stats_s) );

     buildIntoTeamStats( team_stats, team_id, season, bowl_game );

     return team_stats;
}

void buildIntoTeamStats( team_stats_s *team_stats, const int team_id, const int season, const bowl_game_e bowl_game )
{
     team_stats->team_id   = team_id;
     team_stats->season    = season;
     team_stats->bowl_game = bowl_game;

     team_stats->wins           = rand() %   10;
     team_stats->losses         = rand() %   10;
     team_stats->ties           = rand() %   10;
     team_stats->home_wins      = rand() %    5;
     team_stats->home_losses    = rand() %    5;
     team_stats->home_ties      = rand() %    5;
     team_stats->road_wins      = rand() %    5;
     team_stats->road_losses    = rand() %    5;
     team_stats->road_ties      = rand() %    5;
     team_stats->points_scored  = rand() %  500;
     team_stats->points_allowed = rand() %  500;
}
