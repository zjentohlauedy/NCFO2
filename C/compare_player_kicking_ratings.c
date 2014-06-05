#include "compares.h"
#include "unit_test.h"

char *_comparePlayerKickingRatings( player_kicking_ratings_s *expected, player_kicking_ratings_s *actual )
{
     assertEqualsInt( expected->player_id,        actual->player_id        );
     assertEqualsInt( expected->kicking_ability,  actual->kicking_ability  );
     assertEqualsInt( expected->avoid_kick_block, actual->avoid_kick_block );

     return NULL;
}
