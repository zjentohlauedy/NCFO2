#include "compares.h"
#include "unit_test.h"

char *_compareTeamAccolades( team_accolade_s *expected, team_accolade_s *actual )
{
     assertEqualsInt( expected->team_id,  actual->team_id  );
     assertEqualsInt( expected->season,   actual->season   );
     assertEqualsInt( expected->accolade, actual->accolade );

     return NULL;
}
