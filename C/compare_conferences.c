#include "compares.h"
#include "unit_test.h"

char *_compareConferences( conference_s *expected, conference_s *actual )
{
     assertEqualsInt( expected->conference_id, actual->conference_id );
     assertEqualsStr( expected->name,          actual->name          );

     return NULL;
}
