#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

player_game_offense_stats_s *buildPlayerGameOffenseStats( const int player_id, const int season, const int week, const int game )
{
     player_game_offense_stats_s *player_game_offense_stats = NULL;

     if ( (player_game_offense_stats = malloc( sizeof(player_game_offense_stats_s) )) == NULL ) return NULL;

     memset( player_game_offense_stats, '\0', sizeof(player_game_offense_stats_s) );

     buildIntoPlayerGameOffenseStats( player_game_offense_stats, player_id, season, week, game );

     return player_game_offense_stats;
}

void buildIntoPlayerGameOffenseStats( player_game_offense_stats_s *player_game_offense_stats, const int player_id, const int season, const int week, const int game )
{
     player_game_offense_stats->player_id = player_id;
     player_game_offense_stats->season    = season;
     player_game_offense_stats->week      = week;
     player_game_offense_stats->game      = game;

     player_game_offense_stats->pass_attempts        = rand() %  13;
     player_game_offense_stats->completions          = rand() %  22;
     player_game_offense_stats->interceptions        = rand() %   5;
     player_game_offense_stats->pass_yards           = rand() % 400;
     player_game_offense_stats->pass_touchdowns      = rand() %   5;
     player_game_offense_stats->rush_attempts        = rand() %  20;
     player_game_offense_stats->rush_yards           = rand() % 150;
     player_game_offense_stats->rush_touchdowns      = rand() %   5;
     player_game_offense_stats->receptions           = rand() %  10;
     player_game_offense_stats->receiving_yards      = rand() % 100;
     player_game_offense_stats->receiving_touchdowns = rand() %   5;
}
