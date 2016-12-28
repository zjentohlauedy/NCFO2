#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

player_game_defense_stats_s *buildPlayerGameDefenseStats( const int player_id, const int season, const int week, const int game )
{
     player_game_defense_stats_s *player_game_defense_stats = NULL;

     if ( (player_game_defense_stats = malloc( sizeof(player_game_defense_stats_s) )) == NULL ) return NULL;

     memset( player_game_defense_stats, '\0', sizeof(player_game_defense_stats_s) );

     buildIntoPlayerGameDefenseStats( player_game_defense_stats, player_id, season, week, game );

     return player_game_defense_stats;
}

void buildIntoPlayerGameDefenseStats( player_game_defense_stats_s *player_game_defense_stats, const int player_id, const int season, const int week, const int game )
{
     player_game_defense_stats->player_id = player_id;
     player_game_defense_stats->season    = season;
     player_game_defense_stats->week      = week;
     player_game_defense_stats->game      = game;

     player_game_defense_stats->sacks             = rand() %   5;
     player_game_defense_stats->interceptions     = rand() %   5;
     player_game_defense_stats->return_yards      = rand() %  50;
     player_game_defense_stats->return_touchdowns = rand() %   5;
}
