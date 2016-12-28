#include "compares.h"
#include "unit_test.h"

char *_comparePlayerGameDefenseStats( player_game_defense_stats_s *expected, player_game_defense_stats_s *actual )
{
     assertEqualsInt( expected->player_id,         actual->player_id         );
     assertEqualsInt( expected->season,            actual->season            );
     assertEqualsInt( expected->week,              actual->week              );
     assertEqualsInt( expected->game,              actual->game              );
     assertEqualsInt( expected->sacks,             actual->sacks             );
     assertEqualsInt( expected->interceptions,     actual->interceptions     );
     assertEqualsInt( expected->return_yards,      actual->return_yards      );
     assertEqualsInt( expected->return_touchdowns, actual->return_touchdowns );

     return NULL;
}
