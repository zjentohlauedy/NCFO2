#include "compares.h"
#include "unit_test.h"

char *_compareConferenceStats( conference_stats_s *expected, conference_stats_s *actual )
{
     assertEqualsInt( expected->conference_id,  actual->conference_id  );
     assertEqualsInt( expected->season,         actual->season         );
     assertEqualsInt( expected->bowl_game,      actual->bowl_game      );
     assertEqualsInt( expected->wins,           actual->wins           );
     assertEqualsInt( expected->losses,         actual->losses         );
     assertEqualsInt( expected->home_wins,      actual->home_wins      );
     assertEqualsInt( expected->home_losses,    actual->home_losses    );
     assertEqualsInt( expected->road_wins,      actual->road_wins      );
     assertEqualsInt( expected->road_losses,    actual->road_losses    );
     assertEqualsInt( expected->points_scored,  actual->points_scored  );
     assertEqualsInt( expected->points_allowed, actual->points_allowed );

     return NULL;
}
