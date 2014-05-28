#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int players_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_s *player = (player_s *)data;

     BIND_INT( statement, 1, player->player_id       );
     BIND_TXT( statement, 2, player->first_name      );
     BIND_TXT( statement, 3, player->last_name       );
     BIND_INT( statement, 4, player->face            );
     BIND_INT( statement, 5, player->position        );
     BIND_INT( statement, 6, player->number          );
     BIND_INT( statement, 7, player->freshman_season );
     BIND_INT( statement, 8, player->maturity        );

     return SQLITE_OK;
}


int players_t_create( sqlite3 *db, const player_s *player )
{
     static char query[]   = "INSERT INTO Players_T ( Player_Id, First_Name, Last_Name, Face, Position, Number, Freshman_Season, Maturity )"
          /**/                              "VALUES ( ?,"       "?,"        "?,"       "?,"  "?,"      "?,"    "?,"             "?"      ")";

     return execute_update_old( db, query, players_t_create_bindings, player, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int players_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     const player_s *player = (player_s *)data;

     return sqlite3_bind_int( statement, 1, player->player_id );
}

static int players_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     player_s *player = (player_s *)data;

     cpytxt( player->first_name,           sqlite3_column_text( statement, 0 ) );
     cpytxt( player->last_name,            sqlite3_column_text( statement, 1 ) );
     /**/    player->face             =    sqlite3_column_int(  statement, 2   );
     /**/    player->position         =    sqlite3_column_int(  statement, 3   );
     /**/    player->number           =    sqlite3_column_int(  statement, 4   );
     /**/    player->freshman_season  =    sqlite3_column_int(  statement, 5   );
     /**/    player->maturity         =    sqlite3_column_int(  statement, 6   );

     return SQLITE_OK;
}

int players_t_read( sqlite3 *db, player_s *player )
{
     static char query[]   = "SELECT First_Name, Last_Name, Face, Position, Number, Freshman_Season, Maturity FROM Players_T WHERE Player_Id = ?";

     return execute_query( db, query, players_t_read_bindings, player, players_t_read_retrieve, player );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int players_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_s *player = (player_s *)data;

     BIND_TXT( statement, 1, player->first_name      );
     BIND_TXT( statement, 2, player->last_name       );
     BIND_INT( statement, 3, player->face            );
     BIND_INT( statement, 4, player->position        );
     BIND_INT( statement, 5, player->number          );
     BIND_INT( statement, 6, player->freshman_season );
     BIND_INT( statement, 7, player->maturity        );

     BIND_INT( statement, 8, player->player_id );

     return SQLITE_OK;
}


int players_t_update( sqlite3 *db, const player_s *player )
{
     static char query[]   = "UPDATE Players_T "
          /**/
          /**/               "SET    First_Name      = ?,"
          /**/                      "Last_Name       = ?,"
          /**/                      "Face            = ?,"
          /**/                      "Position        = ?,"
          /**/                      "Number          = ?,"
          /**/                      "Freshman_Season = ?,"
          /**/                      "Maturity        = ? "
          /**/
          /**/               "WHERE  Player_Id = ?";

     return execute_update_old( db, query, players_t_update_bindings, player, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int players_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     const player_s *player = (player_s *)data;

     return sqlite3_bind_int( statement, 1, player->player_id );
}


int players_t_delete( sqlite3 *db, const player_s *player )
{
     return execute_update_old( db, "DELETE FROM Players_T WHERE Player_Id = ?", players_t_delete_bindings, player, NULL, NULL );
}
