#include "compares.h"
#include "unit_test.h"

char *_compareOrganizations( organization_s *expected, organization_s *actual )
{
     assertEqualsInt( expected->organization_id, actual->organization_id );
     assertEqualsStr( expected->name,            actual->name            );
     assertEqualsStr( expected->abbreviation,    actual->abbreviation    );
     assertEqualsInt( expected->season,          actual->season          );

     return NULL;
}
