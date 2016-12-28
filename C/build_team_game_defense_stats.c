#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

team_game_defense_stats_s *buildTeamGameDefenseStats( const int team_id, const int season, const int week, const int game )
{
     team_game_defense_stats_s *team_game_defense_stats = NULL;

     if ( (team_game_defense_stats = malloc( sizeof(team_game_defense_stats_s) )) == NULL ) return NULL;

     memset( team_game_defense_stats, '\0', sizeof(team_game_defense_stats_s) );

     buildIntoTeamGameDefenseStats( team_game_defense_stats, team_id, season, week, game );

     return team_game_defense_stats;
}

void buildIntoTeamGameDefenseStats( team_game_defense_stats_s *team_game_defense_stats, const int team_id, const int season, const int week, const int game )
{
     team_game_defense_stats->team_id   = team_id;
     team_game_defense_stats->season    = season;
     team_game_defense_stats->week      = week;
     team_game_defense_stats->game      = game;

     team_game_defense_stats->sacks             = rand() %   10;
     team_game_defense_stats->interceptions     = rand() %    5;
     team_game_defense_stats->return_yards      = rand() %  100;
     team_game_defense_stats->return_touchdowns = rand() %    5;
}
