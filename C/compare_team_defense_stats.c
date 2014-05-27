#include "compares.h"
#include "unit_test.h"

char *_compareTeamDefenseStats( team_defense_stats_s *expected, team_defense_stats_s *actual )
{
     assertEqualsInt( expected->team_id,           actual->team_id           );
     assertEqualsInt( expected->season,            actual->season            );
     assertEqualsInt( expected->bowl_game,         actual->bowl_game         );
     assertEqualsInt( expected->sacks,             actual->sacks             );
     assertEqualsInt( expected->interceptions,     actual->interceptions     );
     assertEqualsInt( expected->return_yards,      actual->return_yards      );
     assertEqualsInt( expected->return_touchdowns, actual->return_touchdowns );

     return NULL;
}
