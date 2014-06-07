#include "compares.h"
#include "unit_test.h"

char *_comparePlayerAccolades( player_accolade_s *expected, player_accolade_s *actual )
{
     assertEqualsInt( expected->player_id, actual->player_id );
     assertEqualsInt( expected->season,    actual->season    );
     assertEqualsInt( expected->accolade,  actual->accolade  );

     return NULL;
}
