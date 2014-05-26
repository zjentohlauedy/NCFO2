#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

organization_s *buildOrganization( const int organization_id )
{
     organization_s *organization = NULL;

     if ( (organization = malloc( sizeof(organization_s) )) == NULL ) return NULL;

     memset( organization, '\0', sizeof(organization_s) );

     buildIntoOrganization( organization, organization_id );

     return organization;
}

void buildIntoOrganization( organization_s *organization, const int organization_id )
{
     /**/     organization->organization_id  =                organization_id;
     sprintf( organization->name,           "Organization%d", organization_id );
     sprintf( organization->abbreviation,   "Org%d",          organization_id );
     /**/     organization->season           =                rand() % 10;
}
