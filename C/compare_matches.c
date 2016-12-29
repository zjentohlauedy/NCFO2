#include "compares.h"
#include "unit_test.h"

char *_compareMatches( match_s *expected, match_s *actual )
{
     assertEqualsInt( expected->season,       actual->season       );
     assertEqualsInt( expected->week,         actual->week         );
     assertEqualsInt( expected->game,         actual->game         );
     assertEqualsInt( expected->road_team_id, actual->road_team_id );
     assertEqualsInt( expected->home_team_id, actual->home_team_id );

     return NULL;
}
