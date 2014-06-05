#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

player_offense_ratings_s *buildPlayerOffenseRatings( const int player_id )
{
     player_offense_ratings_s *player_offense_ratings = NULL;

     if ( (player_offense_ratings = malloc( sizeof(player_offense_ratings_s) )) == NULL ) return NULL;

     memset( player_offense_ratings, '\0', sizeof(player_offense_ratings_s) );

     buildIntoPlayerOffenseRatings( player_offense_ratings, player_id );

     return player_offense_ratings;
}

void buildIntoPlayerOffenseRatings( player_offense_ratings_s *player_offense_ratings, const int player_id )
{
     player_offense_ratings->player_id     =  player_id;
     player_offense_ratings->ball_control  =  rand() %  16;
     player_offense_ratings->receiving     =  rand() %  16;
}
