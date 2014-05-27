#include "compares.h"
#include "unit_test.h"

char *_compareTeams( team_s *expected, team_s *actual )
{
     assertEqualsInt( expected->team_id,      actual->team_id      );
     assertEqualsStr( expected->name,         actual->name         );
     assertEqualsStr( expected->location,     actual->location     );
     assertEqualsStr( expected->abbreviation, actual->abbreviation );

     return NULL;
}
