#include "compares.h"
#include "unit_test.h"

char *_comparePlayerKickingStats( player_kicking_stats_s *expected, player_kicking_stats_s *actual )
{
     assertEqualsInt( expected->player_id,            actual->player_id            );
     assertEqualsInt( expected->season,               actual->season               );
     assertEqualsInt( expected->bowl_game,            actual->bowl_game            );
     assertEqualsInt( expected->extra_point_attempts, actual->extra_point_attempts );
     assertEqualsInt( expected->extra_points_made,    actual->extra_points_made    );
     assertEqualsInt( expected->field_goal_attempts,  actual->field_goal_attempts  );
     assertEqualsInt( expected->field_goals_made,     actual->field_goals_made     );
     assertEqualsInt( expected->punts,                actual->punts                );
     assertEqualsInt( expected->punt_yards,           actual->punt_yards           );

     return NULL;
}
