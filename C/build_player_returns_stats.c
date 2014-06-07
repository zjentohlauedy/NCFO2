#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

player_returns_stats_s *buildPlayerReturnsStats( const int player_id, const int season, const bowl_game_e bowl_game )
{
     player_returns_stats_s *player_returns_stats = NULL;

     if ( (player_returns_stats = malloc( sizeof(player_returns_stats_s) )) == NULL ) return NULL;

     memset( player_returns_stats, '\0', sizeof(player_returns_stats_s) );

     buildIntoPlayerReturnsStats( player_returns_stats, player_id, season, bowl_game );

     return player_returns_stats;
}

void buildIntoPlayerReturnsStats( player_returns_stats_s *player_returns_stats, const int player_id, const int season, const bowl_game_e bowl_game )
{
     player_returns_stats->player_id   = player_id;
     player_returns_stats->season    = season;
     player_returns_stats->bowl_game = bowl_game;

     player_returns_stats->kick_returns           = rand() %   50;
     player_returns_stats->kick_return_yards      = rand() % 1000;
     player_returns_stats->kick_return_touchdowns = rand() %   10;
     player_returns_stats->punt_returns           = rand() %   50;
     player_returns_stats->punt_return_yards      = rand() %  500;
     player_returns_stats->punt_return_touchdowns = rand() %   10;
}
