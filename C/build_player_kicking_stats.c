#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

player_kicking_stats_s *buildPlayerKickingStats( const int player_id, const int season, const bowl_game_e bowl_game )
{
     player_kicking_stats_s *player_kicking_stats = NULL;

     if ( (player_kicking_stats = malloc( sizeof(player_kicking_stats_s) )) == NULL ) return NULL;

     memset( player_kicking_stats, '\0', sizeof(player_kicking_stats_s) );

     buildIntoPlayerKickingStats( player_kicking_stats, player_id, season, bowl_game );

     return player_kicking_stats;
}

void buildIntoPlayerKickingStats( player_kicking_stats_s *player_kicking_stats, const int player_id, const int season, const bowl_game_e bowl_game )
{
     player_kicking_stats->player_id   = player_id;
     player_kicking_stats->season    = season;
     player_kicking_stats->bowl_game = bowl_game;

     player_kicking_stats->extra_point_attempts = rand() %  20;
     player_kicking_stats->extra_points_made    = rand() %  20;
     player_kicking_stats->field_goal_attempts  = rand() %  20;
     player_kicking_stats->field_goals_made     = rand() %  20;
     player_kicking_stats->punts                = rand() %  50;
     player_kicking_stats->punt_yards           = rand() % 300;
}
