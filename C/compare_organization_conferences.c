#include "compares.h"
#include "unit_test.h"

char *_compareOrganizationConferences( organization_conference_s *expected, organization_conference_s *actual )
{
     assertEqualsInt( expected->organization_id, actual->organization_id );
     assertEqualsInt( expected->conference_id,   actual->conference_id   );

     return NULL;
}
