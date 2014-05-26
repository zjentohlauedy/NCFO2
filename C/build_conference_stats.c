#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

conference_stats_s *buildConferenceStats( const int conference_id, const int season, const bowl_game_e bowl_game )
{
     conference_stats_s *conference_stats = NULL;

     if ( (conference_stats = malloc( sizeof(conference_stats_s) )) == NULL ) return NULL;

     memset( conference_stats, '\0', sizeof(conference_stats_s) );

     buildIntoConferenceStats( conference_stats, conference_id, season, bowl_game );

     return conference_stats;
}

void buildIntoConferenceStats( conference_stats_s *conference_stats, const int conference_id, const int season, const bowl_game_e bowl_game )
{
     conference_stats->conference_id = conference_id;
     conference_stats->season        = season;
     conference_stats->bowl_game     = bowl_game;

     conference_stats->wins           = rand() %   2;
     conference_stats->losses         = rand() %   2;
     conference_stats->home_wins      = rand() %   2;
     conference_stats->home_losses    = rand() %   2;
     conference_stats->road_wins      = rand() %   2;
     conference_stats->road_losses    = rand() %   2;
     conference_stats->points_scored  = rand() %  50;
     conference_stats->points_allowed = rand() %  50;
}
