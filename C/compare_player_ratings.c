#include "compares.h"
#include "unit_test.h"

char *_comparePlayerRatings( player_ratings_s *expected, player_ratings_s *actual )
{
     assertEqualsInt( expected->player_id,  actual->player_id  );
     assertEqualsInt( expected->run_speed,  actual->run_speed  );
     assertEqualsInt( expected->rush_power, actual->rush_power );
     assertEqualsInt( expected->max_speed,  actual->max_speed  );
     assertEqualsInt( expected->hit_power,  actual->hit_power  );

     return NULL;
}
