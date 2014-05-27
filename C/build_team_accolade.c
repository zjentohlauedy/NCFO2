#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

team_accolade_s *buildTeamAccolade( const int team_id, const int season, const team_accolade_e accolade )
{
     team_accolade_s *team_accolade = NULL;

     if ( (team_accolade = malloc( sizeof(team_accolade_s) )) == NULL ) return NULL;

     memset( team_accolade, '\0', sizeof(team_accolade_s) );

     buildIntoTeamAccolade( team_accolade, team_id, season, accolade );

     return team_accolade;
}

void buildIntoTeamAccolade( team_accolade_s *team_accolade, const int team_id, const int season, const team_accolade_e accolade )
{
     team_accolade->team_id   =  team_id;
     team_accolade->season    =  season;
     team_accolade->accolade  =  accolade;
}
