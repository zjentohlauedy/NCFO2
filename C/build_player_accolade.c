#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

player_accolade_s *buildPlayerAccolade( const int player_id, const int season, const player_accolade_e accolade )
{
     player_accolade_s *player_accolade = NULL;

     if ( (player_accolade = malloc( sizeof(player_accolade_s) )) == NULL ) return NULL;

     memset( player_accolade, '\0', sizeof(player_accolade_s) );

     buildIntoPlayerAccolade( player_accolade, player_id, season, accolade );

     return player_accolade;
}

void buildIntoPlayerAccolade( player_accolade_s *player_accolade, const int player_id, const int season, const player_accolade_e accolade )
{
     player_accolade->player_id =  player_id;
     player_accolade->season    =  season;
     player_accolade->accolade  =  accolade;
}
