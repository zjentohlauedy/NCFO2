#include "compares.h"
#include "unit_test.h"

char *_compareConferenceAccolades( conference_accolade_s *expected, conference_accolade_s *actual )
{
     assertEqualsInt( expected->conference_id, actual->conference_id );
     assertEqualsInt( expected->season  ,      actual->season        );
     assertEqualsInt( expected->accolade,      actual->accolade      );

     return NULL;
}
