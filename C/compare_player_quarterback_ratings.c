#include "compares.h"
#include "unit_test.h"

char *_comparePlayerQuarterbackRatings( player_quarterback_ratings_s *expected, player_quarterback_ratings_s *actual )
{
     assertEqualsInt( expected->player_id,        actual->player_id        );
     assertEqualsInt( expected->pass_speed,       actual->pass_speed       );
     assertEqualsInt( expected->pass_control,     actual->pass_control     );
     assertEqualsInt( expected->pass_accuracy,    actual->pass_accuracy    );
     assertEqualsInt( expected->avoid_pass_block, actual->avoid_pass_block );

     return NULL;
}
