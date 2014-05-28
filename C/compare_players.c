#include "compares.h"
#include "unit_test.h"

char *_comparePlayers( player_s *expected, player_s *actual )
{
     assertEqualsInt( expected->player_id,       actual->player_id       );
     assertEqualsStr( expected->first_name,      actual->first_name      );
     assertEqualsStr( expected->last_name,       actual->last_name       );
     assertEqualsInt( expected->face,            actual->face            );
     assertEqualsInt( expected->position,        actual->position        );
     assertEqualsInt( expected->number,          actual->number          );
     assertEqualsInt( expected->freshman_season, actual->freshman_season );
     assertEqualsInt( expected->maturity,        actual->maturity        );

     return NULL;
}
