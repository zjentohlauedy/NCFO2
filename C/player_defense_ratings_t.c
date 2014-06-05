#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_defense_ratings_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_defense_ratings_s *player_defense_ratings = (player_defense_ratings_s *)data;

     BIND_INT( statement, 1, player_defense_ratings->player_id     );
     BIND_INT( statement, 2, player_defense_ratings->interceptions );
     BIND_INT( statement, 3, player_defense_ratings->quickness     );

     return SQLITE_OK;
}


int player_defense_ratings_t_create( sqlite3 *db, const player_defense_ratings_s *player_defense_ratings )
{
     static char query[]   = "INSERT INTO Player_Defense_Ratings_T ( Player_Id, Interceptions, Quickness )"
          /**/                                             "VALUES ( ?,"       "?,"           "?"       ")";

     return execute_update_old( db, query, player_defense_ratings_t_create_bindings, player_defense_ratings, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_defense_ratings_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     const player_defense_ratings_s *player_defense_ratings = (player_defense_ratings_s *)data;

     return sqlite3_bind_int( statement, 1, player_defense_ratings->player_id );
}

static int player_defense_ratings_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     player_defense_ratings_s *player_defense_ratings = (player_defense_ratings_s *)data;

     player_defense_ratings->interceptions  =    sqlite3_column_int(  statement, 0   );
     player_defense_ratings->quickness      =    sqlite3_column_int(  statement, 1   );

     return SQLITE_OK;
}

int player_defense_ratings_t_read( sqlite3 *db, player_defense_ratings_s *player_defense_ratings )
{
     static char query[]   = "SELECT Interceptions, Quickness FROM Player_Defense_Ratings_T WHERE Player_Id = ?";

     return execute_query( db, query, player_defense_ratings_t_read_bindings, player_defense_ratings, player_defense_ratings_t_read_retrieve, player_defense_ratings );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_defense_ratings_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_defense_ratings_s *player_defense_ratings = (player_defense_ratings_s *)data;

     BIND_INT( statement, 1, player_defense_ratings->interceptions );
     BIND_INT( statement, 2, player_defense_ratings->quickness     );

     BIND_INT( statement, 3, player_defense_ratings->player_id );

     return SQLITE_OK;
}


int player_defense_ratings_t_update( sqlite3 *db, const player_defense_ratings_s *player_defense_ratings )
{
     static char query[]   = "UPDATE Player_Defense_Ratings_T "
          /**/
          /**/               "SET    Interceptions = ?,"
          /**/                      "Quickness     = ? "
          /**/
          /**/               "WHERE  Player_Id = ?";

     return execute_update_old( db, query, player_defense_ratings_t_update_bindings, player_defense_ratings, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_defense_ratings_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     const player_defense_ratings_s *player_defense_ratings = (player_defense_ratings_s *)data;

     return sqlite3_bind_int( statement, 1, player_defense_ratings->player_id );
}


int player_defense_ratings_t_delete( sqlite3 *db, const player_defense_ratings_s *player_defense_ratings )
{
     return execute_update_old( db, "DELETE FROM Player_Defense_Ratings_T WHERE Player_Id = ?", player_defense_ratings_t_delete_bindings, player_defense_ratings, NULL, NULL );
}
