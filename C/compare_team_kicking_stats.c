#include "compares.h"
#include "unit_test.h"

char *_compareTeamKickingStats( team_kicking_stats_s *expected, team_kicking_stats_s *actual )
{
     assertEqualsInt( expected->team_id,                actual->team_id                );
     assertEqualsInt( expected->season,                 actual->season                 );
     assertEqualsInt( expected->bowl_game,              actual->bowl_game              );
     assertEqualsInt( expected->extra_point_attempts,   actual->extra_point_attempts   );
     assertEqualsInt( expected->extra_points_made,      actual->extra_points_made      );
     assertEqualsInt( expected->field_goal_attempts,    actual->field_goal_attempts    );
     assertEqualsInt( expected->field_goals_made,       actual->field_goals_made       );
     assertEqualsInt( expected->punts,                  actual->punts                  );
     assertEqualsInt( expected->punt_yards,             actual->punt_yards             );
     assertEqualsInt( expected->kick_returns,           actual->kick_returns           );
     assertEqualsInt( expected->kick_return_yards,      actual->kick_return_yards      );
     assertEqualsInt( expected->kick_return_touchdowns, actual->kick_return_touchdowns );
     assertEqualsInt( expected->punt_returns,           actual->punt_returns           );
     assertEqualsInt( expected->punt_return_yards,      actual->punt_return_yards      );
     assertEqualsInt( expected->punt_return_touchdowns, actual->punt_return_touchdowns );

     return NULL;
}
