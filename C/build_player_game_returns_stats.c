#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

player_game_returns_stats_s *buildPlayerGameReturnsStats( const int player_id, const int season, const int week, const int game )
{
     player_game_returns_stats_s *player_game_returns_stats = NULL;

     if ( (player_game_returns_stats = malloc( sizeof(player_game_returns_stats_s) )) == NULL ) return NULL;

     memset( player_game_returns_stats, '\0', sizeof(player_game_returns_stats_s) );

     buildIntoPlayerGameReturnsStats( player_game_returns_stats, player_id, season, week, game );

     return player_game_returns_stats;
}

void buildIntoPlayerGameReturnsStats( player_game_returns_stats_s *player_game_returns_stats, const int player_id, const int season, const int week, const int game )
{
     player_game_returns_stats->player_id = player_id;
     player_game_returns_stats->season    = season;
     player_game_returns_stats->week      = week;
     player_game_returns_stats->game      = game;

     player_game_returns_stats->kick_returns           = rand() %   8;
     player_game_returns_stats->kick_return_yards      = rand() % 120;
     player_game_returns_stats->kick_return_touchdowns = rand() %   5;
     player_game_returns_stats->punt_returns           = rand() %   8;
     player_game_returns_stats->punt_return_yards      = rand() %  50;
     player_game_returns_stats->punt_return_touchdowns = rand() %   5;
}
