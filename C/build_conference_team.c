#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

conference_team_s *buildConferenceTeam( const int conference_id, const int team_id )
{
     conference_team_s *conference_team = NULL;

     if ( (conference_team = malloc( sizeof(conference_team_s) )) == NULL ) return NULL;

     memset( conference_team, '\0', sizeof(conference_team_s) );

     buildIntoConferenceTeam( conference_team, conference_id, team_id );

     return conference_team;
}

void buildIntoConferenceTeam( conference_team_s *conference_team, const int conference_id, const int team_id )
{
     /**/     conference_team->conference_id  =  conference_id;
     /**/     conference_team->team_id        =  team_id;
}
