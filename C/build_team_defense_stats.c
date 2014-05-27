#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

team_defense_stats_s *buildTeamDefenseStats( const int team_id, const int season, const bowl_game_e bowl_game )
{
     team_defense_stats_s *team_defense_stats = NULL;

     if ( (team_defense_stats = malloc( sizeof(team_defense_stats_s) )) == NULL ) return NULL;

     memset( team_defense_stats, '\0', sizeof(team_defense_stats_s) );

     buildIntoTeamDefenseStats( team_defense_stats, team_id, season, bowl_game );

     return team_defense_stats;
}

void buildIntoTeamDefenseStats( team_defense_stats_s *team_defense_stats, const int team_id, const int season, const bowl_game_e bowl_game )
{
     team_defense_stats->team_id   = team_id;
     team_defense_stats->season    = season;
     team_defense_stats->bowl_game = bowl_game;

     team_defense_stats->sacks             = rand() %   20;
     team_defense_stats->interceptions     = rand() %   10;
     team_defense_stats->return_yards      = rand() %  500;
     team_defense_stats->return_touchdowns = rand() %   10;
}
