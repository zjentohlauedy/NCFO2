#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

match_s *buildMatch( const int season, const int week, const int game )
{
     match_s *match = NULL;

     if ( (match = malloc( sizeof(match_s) )) == NULL ) return NULL;

     memset( match, '\0', sizeof(match_s) );

     buildIntoMatch( match, season, week, game );

     return match;
}

void buildIntoMatch( match_s *match, const int season, const int week, const int game )
{
     match->season    = season;
     match->week      = week;
     match->game      = game;

     match->road_team_id = rand() % 48;
     match->home_team_id = rand() % 48;
}
