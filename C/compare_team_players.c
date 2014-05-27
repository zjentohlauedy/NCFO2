#include "compares.h"
#include "unit_test.h"

char *_compareTeamPlayers( team_player_s *expected, team_player_s *actual )
{
     assertEqualsInt( expected->team_id,   actual->team_id   );
     assertEqualsInt( expected->season,    actual->season    );
     assertEqualsInt( expected->player_id, actual->player_id );

     return NULL;
}
