#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

team_offense_stats_s *buildTeamOffenseStats( const int team_id, const int season, const bowl_game_e bowl_game )
{
     team_offense_stats_s *team_offense_stats = NULL;

     if ( (team_offense_stats = malloc( sizeof(team_offense_stats_s) )) == NULL ) return NULL;

     memset( team_offense_stats, '\0', sizeof(team_offense_stats_s) );

     buildIntoTeamOffenseStats( team_offense_stats, team_id, season, bowl_game );

     return team_offense_stats;
}

void buildIntoTeamOffenseStats( team_offense_stats_s *team_offense_stats, const int team_id, const int season, const bowl_game_e bowl_game )
{
     team_offense_stats->team_id   = team_id;
     team_offense_stats->season    = season;
     team_offense_stats->bowl_game = bowl_game;

     team_offense_stats->pass_attempts   = rand() %  200;
     team_offense_stats->completions     = rand() %  150;
     team_offense_stats->pass_yards      = rand() % 3000;
     team_offense_stats->pass_touchdowns = rand() %   50;
     team_offense_stats->rush_attempts   = rand() %  200;
     team_offense_stats->rush_yards      = rand() % 1000;
     team_offense_stats->rush_touchdowns = rand() %   20;
}
