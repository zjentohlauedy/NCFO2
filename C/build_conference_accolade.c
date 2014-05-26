#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

conference_accolade_s *buildConferenceAccolade( const int conference_id, const int season, const conference_accolade_e accolade )
{
     conference_accolade_s *conference_accolade = NULL;

     if ( (conference_accolade = malloc( sizeof(conference_accolade_s) )) == NULL ) return NULL;

     memset( conference_accolade, '\0', sizeof(conference_accolade_s) );

     buildIntoConferenceAccolade( conference_accolade, conference_id, season, accolade );

     return conference_accolade;
}

void buildIntoConferenceAccolade( conference_accolade_s *conference_accolade, const int conference_id, const int season, const conference_accolade_e accolade )
{
     conference_accolade->conference_id  =  conference_id;
     conference_accolade->season         =  season;
     conference_accolade->accolade       =  accolade;
}
