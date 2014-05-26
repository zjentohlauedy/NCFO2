#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

conference_s *buildConference( const int conference_id )
{
     conference_s *conference = NULL;

     if ( (conference = malloc( sizeof(conference_s) )) == NULL ) return NULL;

     memset( conference, '\0', sizeof(conference_s) );

     buildIntoConference( conference, conference_id );

     return conference;
}

void buildIntoConference( conference_s *conference, const int conference_id )
{
     /**/     conference->conference_id  =              conference_id;
     sprintf( conference->name,         "Conference%d", conference_id );
}
