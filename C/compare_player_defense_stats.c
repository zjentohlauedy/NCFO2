#include "compares.h"
#include "unit_test.h"

char *_comparePlayerDefenseStats( player_defense_stats_s *expected, player_defense_stats_s *actual )
{
     assertEqualsInt( expected->player_id,         actual->player_id         );
     assertEqualsInt( expected->season,            actual->season            );
     assertEqualsInt( expected->bowl_game,         actual->bowl_game         );
     assertEqualsInt( expected->sacks,             actual->sacks             );
     assertEqualsInt( expected->interceptions,     actual->interceptions     );
     assertEqualsInt( expected->return_yards,      actual->return_yards      );
     assertEqualsInt( expected->return_touchdowns, actual->return_touchdowns );

     return NULL;
}
