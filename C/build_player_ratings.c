#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

player_ratings_s *buildPlayerRatings( const int player_id )
{
     player_ratings_s *player_ratings = NULL;

     if ( (player_ratings = malloc( sizeof(player_ratings_s) )) == NULL ) return NULL;

     memset( player_ratings, '\0', sizeof(player_ratings_s) );

     buildIntoPlayerRatings( player_ratings, player_id );

     return player_ratings;
}

void buildIntoPlayerRatings( player_ratings_s *player_ratings, const int player_id )
{
     /**/     player_ratings->player_id   =  player_id;
     /**/     player_ratings->run_speed   =  rand() %  16;
     /**/     player_ratings->rush_power  =  rand() %  16;
     /**/     player_ratings->max_speed   =  rand() %  16;
     /**/     player_ratings->hit_power   =  rand() %  16;
}
