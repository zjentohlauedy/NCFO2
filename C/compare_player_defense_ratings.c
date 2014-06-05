#include "compares.h"
#include "unit_test.h"

char *_comparePlayerDefenseRatings( player_defense_ratings_s *expected, player_defense_ratings_s *actual )
{
     assertEqualsInt( expected->player_id,     actual->player_id     );
     assertEqualsInt( expected->interceptions, actual->interceptions );
     assertEqualsInt( expected->quickness,     actual->quickness     );

     return NULL;
}
