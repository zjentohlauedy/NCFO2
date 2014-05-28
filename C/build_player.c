#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"

player_s *buildPlayer( const int player_id )
{
     player_s *player = NULL;

     if ( (player = malloc( sizeof(player_s) )) == NULL ) return NULL;

     memset( player, '\0', sizeof(player_s) );

     buildIntoPlayer( player, player_id );

     return player;
}

void buildIntoPlayer( player_s *player, const int player_id )
{
     /**/     player->player_id        =              player_id;
     sprintf( player->first_name,         "First%d",  player_id    );
     sprintf( player->last_name,          "Last%d",   player_id    );
     /**/     player->face             =              rand() % 256;
     /**/     player->position         =              rand() %  11;
     /**/     player->number           =              rand() % 100;
     /**/     player->freshman_season  =              rand() %  10;
     /**/     player->maturity         =              rand() %   4;
}
