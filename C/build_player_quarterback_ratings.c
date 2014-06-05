#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

player_quarterback_ratings_s *buildPlayerQuarterbackRatings( const int player_id )
{
     player_quarterback_ratings_s *player_quarterback_ratings = NULL;

     if ( (player_quarterback_ratings = malloc( sizeof(player_quarterback_ratings_s) )) == NULL ) return NULL;

     memset( player_quarterback_ratings, '\0', sizeof(player_quarterback_ratings_s) );

     buildIntoPlayerQuarterbackRatings( player_quarterback_ratings, player_id );

     return player_quarterback_ratings;
}

void buildIntoPlayerQuarterbackRatings( player_quarterback_ratings_s *player_quarterback_ratings, const int player_id )
{
     player_quarterback_ratings->player_id         =  player_id;
     player_quarterback_ratings->pass_speed        =  rand() %  16;
     player_quarterback_ratings->pass_control      =  rand() %  16;
     player_quarterback_ratings->pass_accuracy     =  rand() %  16;
     player_quarterback_ratings->avoid_pass_block  =  rand() %  16;
}
