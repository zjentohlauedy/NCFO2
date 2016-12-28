#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

team_game_offense_stats_s *buildTeamGameOffenseStats( const int team_id, const int season, const int week, const int game )
{
     team_game_offense_stats_s *team_game_offense_stats = NULL;

     if ( (team_game_offense_stats = malloc( sizeof(team_game_offense_stats_s) )) == NULL ) return NULL;

     memset( team_game_offense_stats, '\0', sizeof(team_game_offense_stats_s) );

     buildIntoTeamGameOffenseStats( team_game_offense_stats, team_id, season, week, game );

     return team_game_offense_stats;
}

void buildIntoTeamGameOffenseStats( team_game_offense_stats_s *team_game_offense_stats, const int team_id, const int season, const int week, const int game )
{
     team_game_offense_stats->team_id   = team_id;
     team_game_offense_stats->season    = season;
     team_game_offense_stats->week      = week;
     team_game_offense_stats->game      = game;

     team_game_offense_stats->pass_attempts   = rand() %  20;
     team_game_offense_stats->completions     = rand() %  15;
     team_game_offense_stats->interceptions   = rand() %  15;
     team_game_offense_stats->pass_yards      = rand() % 300;
     team_game_offense_stats->pass_touchdowns = rand() %   5;
     team_game_offense_stats->rush_attempts   = rand() %  20;
     team_game_offense_stats->rush_yards      = rand() % 100;
     team_game_offense_stats->rush_touchdowns = rand() %   5;
}
