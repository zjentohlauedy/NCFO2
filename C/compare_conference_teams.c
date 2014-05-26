#include "compares.h"
#include "unit_test.h"

char *_compareConferenceTeams( conference_team_s *expected, conference_team_s *actual )
{
     assertEqualsInt( expected->conference_id, actual->conference_id );
     assertEqualsInt( expected->team_id,       actual->team_id       );

     return NULL;
}
