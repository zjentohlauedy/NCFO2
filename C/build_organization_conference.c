#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

organization_conference_s *buildOrganizationConference( const int organization_id, const int conference_id )
{
     organization_conference_s *organization_conference = NULL;

     if ( (organization_conference = malloc( sizeof(organization_conference_s) )) == NULL ) return NULL;

     memset( organization_conference, '\0', sizeof(organization_conference_s) );

     buildIntoOrganizationConference( organization_conference, organization_id, conference_id );

     return organization_conference;
}

void buildIntoOrganizationConference( organization_conference_s *organization_conference, const int organization_id, const int conference_id )
{
     /**/     organization_conference->organization_id  =  organization_id;
     /**/     organization_conference->conference_id    =  conference_id;
}
