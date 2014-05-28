#ifndef __INC_PLAYER_H__
#define __INC_PLAYER_H__

#include <sqlite3.h>
#include "data_list.h"

typedef enum
{
     pos_None          =  0,
     pos_Quarterback   =  1,
     pos_Runningback   =  2,
     pos_WideReceiver  =  3,
     pos_TightEnd      =  4,
     pos_OffensiveLine =  5,
     pos_DefensiveLine =  6,
     pos_Linebacker    =  7,
     pos_Cornerback    =  8,
     pos_Safety        =  9,
     pos_Kicker        = 10,
     pos_Punter        = 11

} position_e;

typedef struct
{
     int         player_id;
     char        first_name      [ 20 + 1 ];
     char        last_name       [ 20 + 1 ];
     int         face;
     position_e  position;
     int         number;
     int         freshman_season;
     int         maturity;

} player_s;


int players_t_create( sqlite3 *db, const player_s *player );
int players_t_read(   sqlite3 *db,       player_s *player );
int players_t_update( sqlite3 *db, const player_s *player );
int players_t_delete( sqlite3 *db, const player_s *player );

#endif
