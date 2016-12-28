#include "compares.h"
#include "unit_test.h"

char *_comparePlayerGameOffenseStats( player_game_offense_stats_s *expected, player_game_offense_stats_s *actual )
{
     assertEqualsInt( expected->player_id,            actual->player_id            );
     assertEqualsInt( expected->season,               actual->season               );
     assertEqualsInt( expected->week,                 actual->week                 );
     assertEqualsInt( expected->game,                 actual->game                 );
     assertEqualsInt( expected->pass_attempts,        actual->pass_attempts        );
     assertEqualsInt( expected->completions,          actual->completions          );
     assertEqualsInt( expected->interceptions,        actual->interceptions        );
     assertEqualsInt( expected->pass_yards,           actual->pass_yards           );
     assertEqualsInt( expected->pass_touchdowns,      actual->pass_touchdowns      );
     assertEqualsInt( expected->rush_attempts,        actual->rush_attempts        );
     assertEqualsInt( expected->rush_yards,           actual->rush_yards           );
     assertEqualsInt( expected->rush_touchdowns,      actual->rush_touchdowns      );
     assertEqualsInt( expected->receptions,           actual->receptions           );
     assertEqualsInt( expected->receiving_yards,      actual->receiving_yards      );
     assertEqualsInt( expected->receiving_touchdowns, actual->receiving_touchdowns );

     return NULL;
}
