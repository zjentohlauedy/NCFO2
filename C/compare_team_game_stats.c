#include "compares.h"
#include "unit_test.h"

char *_compareTeamGameStats( team_game_stats_s *expected, team_game_stats_s *actual )
{
     assertEqualsInt( expected->team_id,        actual->team_id        );
     assertEqualsInt( expected->season,         actual->season         );
     assertEqualsInt( expected->week,           actual->week           );
     assertEqualsInt( expected->game,           actual->game           );
     assertEqualsInt( expected->wins,           actual->wins           );
     assertEqualsInt( expected->losses,         actual->losses         );
     assertEqualsInt( expected->ties,           actual->ties           );
     assertEqualsInt( expected->home_wins,      actual->home_wins      );
     assertEqualsInt( expected->home_losses,    actual->home_losses    );
     assertEqualsInt( expected->home_ties,      actual->home_ties      );
     assertEqualsInt( expected->road_wins,      actual->road_wins      );
     assertEqualsInt( expected->road_losses,    actual->road_losses    );
     assertEqualsInt( expected->road_ties,      actual->road_ties      );
     assertEqualsInt( expected->points_scored,  actual->points_scored  );
     assertEqualsInt( expected->points_allowed, actual->points_allowed );

     return NULL;
}
