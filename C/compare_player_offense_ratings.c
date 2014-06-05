#include "compares.h"
#include "unit_test.h"

char *_comparePlayerOffenseRatings( player_offense_ratings_s *expected, player_offense_ratings_s *actual )
{
     assertEqualsInt( expected->player_id,    actual->player_id    );
     assertEqualsInt( expected->ball_control, actual->ball_control );
     assertEqualsInt( expected->receiving,    actual->receiving    );

     return NULL;
}
