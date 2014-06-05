#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

player_kicking_ratings_s *buildPlayerKickingRatings( const int player_id )
{
     player_kicking_ratings_s *player_kicking_ratings = NULL;

     if ( (player_kicking_ratings = malloc( sizeof(player_kicking_ratings_s) )) == NULL ) return NULL;

     memset( player_kicking_ratings, '\0', sizeof(player_kicking_ratings_s) );

     buildIntoPlayerKickingRatings( player_kicking_ratings, player_id );

     return player_kicking_ratings;
}

void buildIntoPlayerKickingRatings( player_kicking_ratings_s *player_kicking_ratings, const int player_id )
{
     player_kicking_ratings->player_id         =  player_id;
     player_kicking_ratings->kicking_ability   =  rand() %  16;
     player_kicking_ratings->avoid_kick_block  =  rand() %  16;
}
