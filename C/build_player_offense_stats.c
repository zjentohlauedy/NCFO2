#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

player_offense_stats_s *buildPlayerOffenseStats( const int player_id, const int season, const bowl_game_e bowl_game )
{
     player_offense_stats_s *player_offense_stats = NULL;

     if ( (player_offense_stats = malloc( sizeof(player_offense_stats_s) )) == NULL ) return NULL;

     memset( player_offense_stats, '\0', sizeof(player_offense_stats_s) );

     buildIntoPlayerOffenseStats( player_offense_stats, player_id, season, bowl_game );

     return player_offense_stats;
}

void buildIntoPlayerOffenseStats( player_offense_stats_s *player_offense_stats, const int player_id, const int season, const bowl_game_e bowl_game )
{
     player_offense_stats->player_id   = player_id;
     player_offense_stats->season    = season;
     player_offense_stats->bowl_game = bowl_game;

     player_offense_stats->pass_attempts        = rand() %  200;
     player_offense_stats->completions          = rand() %  150;
     player_offense_stats->interceptions        = rand() %  150;
     player_offense_stats->pass_yards           = rand() % 3000;
     player_offense_stats->pass_touchdowns      = rand() %   50;
     player_offense_stats->rush_attempts        = rand() %  200;
     player_offense_stats->rush_yards           = rand() % 1000;
     player_offense_stats->rush_touchdowns      = rand() %   20;
     player_offense_stats->receptions           = rand() %  100;
     player_offense_stats->receiving_yards      = rand() % 1000;
     player_offense_stats->receiving_touchdowns = rand() %   20;
}
