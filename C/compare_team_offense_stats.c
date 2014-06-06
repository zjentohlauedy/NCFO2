#include "compares.h"
#include "unit_test.h"

char *_compareTeamOffenseStats( team_offense_stats_s *expected, team_offense_stats_s *actual )
{
     assertEqualsInt( expected->team_id,         actual->team_id         );
     assertEqualsInt( expected->season,          actual->season          );
     assertEqualsInt( expected->bowl_game,       actual->bowl_game       );
     assertEqualsInt( expected->pass_attempts,   actual->pass_attempts   );
     assertEqualsInt( expected->completions,     actual->completions     );
     assertEqualsInt( expected->interceptions,   actual->interceptions   );
     assertEqualsInt( expected->pass_yards,      actual->pass_yards      );
     assertEqualsInt( expected->pass_touchdowns, actual->pass_touchdowns );
     assertEqualsInt( expected->rush_attempts,   actual->rush_attempts   );
     assertEqualsInt( expected->rush_yards,      actual->rush_yards      );
     assertEqualsInt( expected->rush_touchdowns, actual->rush_touchdowns );

     return NULL;
}
