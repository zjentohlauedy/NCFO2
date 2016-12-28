#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

team_game_stats_s *buildTeamGameStats( const int team_id, const int season, const int week, const int game )
{
     team_game_stats_s *team_game_stats = NULL;

     if ( (team_game_stats = malloc( sizeof(team_game_stats_s) )) == NULL ) return NULL;

     memset( team_game_stats, '\0', sizeof(team_game_stats_s) );

     buildIntoTeamGameStats( team_game_stats, team_id, season, week, game );

     return team_game_stats;
}

void buildIntoTeamGameStats( team_game_stats_s *team_game_stats, const int team_id, const int season, const int week, const int game )
{
     team_game_stats->team_id   = team_id;
     team_game_stats->season    = season;
     team_game_stats->week      = week;
     team_game_stats->game      = game;

     team_game_stats->wins           = rand() %   10;
     team_game_stats->losses         = rand() %   10;
     team_game_stats->ties           = rand() %   10;
     team_game_stats->home_wins      = rand() %    5;
     team_game_stats->home_losses    = rand() %    5;
     team_game_stats->home_ties      = rand() %    5;
     team_game_stats->road_wins      = rand() %    5;
     team_game_stats->road_losses    = rand() %    5;
     team_game_stats->road_ties      = rand() %    5;
     team_game_stats->points_scored  = rand() %  500;
     team_game_stats->points_allowed = rand() %  500;
}
