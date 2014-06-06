#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

player_defense_stats_s *buildPlayerDefenseStats( const int player_id, const int season, const bowl_game_e bowl_game )
{
     player_defense_stats_s *player_defense_stats = NULL;

     if ( (player_defense_stats = malloc( sizeof(player_defense_stats_s) )) == NULL ) return NULL;

     memset( player_defense_stats, '\0', sizeof(player_defense_stats_s) );

     buildIntoPlayerDefenseStats( player_defense_stats, player_id, season, bowl_game );

     return player_defense_stats;
}

void buildIntoPlayerDefenseStats( player_defense_stats_s *player_defense_stats, const int player_id, const int season, const bowl_game_e bowl_game )
{
     player_defense_stats->player_id   = player_id;
     player_defense_stats->season    = season;
     player_defense_stats->bowl_game = bowl_game;

     player_defense_stats->sacks             = rand() %  20;
     player_defense_stats->interceptions     = rand() %  10;
     player_defense_stats->return_yards      = rand() % 150;
     player_defense_stats->return_touchdowns = rand() %   5;
}
