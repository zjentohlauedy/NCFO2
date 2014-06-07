#include "compares.h"
#include "unit_test.h"

char *_comparePlayerReturnsStats( player_returns_stats_s *expected, player_returns_stats_s *actual )
{
     assertEqualsInt( expected->player_id,              actual->player_id              );
     assertEqualsInt( expected->season,                 actual->season                 );
     assertEqualsInt( expected->bowl_game,              actual->bowl_game              );
     assertEqualsInt( expected->kick_returns,           actual->kick_returns           );
     assertEqualsInt( expected->kick_return_yards,      actual->kick_return_yards      );
     assertEqualsInt( expected->kick_return_touchdowns, actual->kick_return_touchdowns );
     assertEqualsInt( expected->punt_returns,           actual->punt_returns           );
     assertEqualsInt( expected->punt_return_yards,      actual->punt_return_yards      );
     assertEqualsInt( expected->punt_return_touchdowns, actual->punt_return_touchdowns );

     return NULL;
}
