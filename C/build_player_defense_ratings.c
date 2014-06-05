#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

player_defense_ratings_s *buildPlayerDefenseRatings( const int player_id )
{
     player_defense_ratings_s *player_defense_ratings = NULL;

     if ( (player_defense_ratings = malloc( sizeof(player_defense_ratings_s) )) == NULL ) return NULL;

     memset( player_defense_ratings, '\0', sizeof(player_defense_ratings_s) );

     buildIntoPlayerDefenseRatings( player_defense_ratings, player_id );

     return player_defense_ratings;
}

void buildIntoPlayerDefenseRatings( player_defense_ratings_s *player_defense_ratings, const int player_id )
{
     player_defense_ratings->player_id      =  player_id;
     player_defense_ratings->interceptions  =  rand() %  16;
     player_defense_ratings->quickness      =  rand() %  16;
}
