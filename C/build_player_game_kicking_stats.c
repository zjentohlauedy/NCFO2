#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

player_game_kicking_stats_s *buildPlayerGameKickingStats( const int player_id, const int season, const int week, const int game )
{
     player_game_kicking_stats_s *player_game_kicking_stats = NULL;

     if ( (player_game_kicking_stats = malloc( sizeof(player_game_kicking_stats_s) )) == NULL ) return NULL;

     memset( player_game_kicking_stats, '\0', sizeof(player_game_kicking_stats_s) );

     buildIntoPlayerGameKickingStats( player_game_kicking_stats, player_id, season, week, game );

     return player_game_kicking_stats;
}

void buildIntoPlayerGameKickingStats( player_game_kicking_stats_s *player_game_kicking_stats, const int player_id, const int season, const int week, const int game )
{
     player_game_kicking_stats->player_id = player_id;
     player_game_kicking_stats->season    = season;
     player_game_kicking_stats->week      = week;
     player_game_kicking_stats->game      = game;

     player_game_kicking_stats->extra_point_attempts = rand() %   5;
     player_game_kicking_stats->extra_points_made    = rand() %   5;
     player_game_kicking_stats->field_goal_attempts  = rand() %   5;
     player_game_kicking_stats->field_goals_made     = rand() %   5;
     player_game_kicking_stats->punts                = rand() %   5;
     player_game_kicking_stats->punt_yards           = rand() % 100;
}
