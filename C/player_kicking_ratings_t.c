#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_kicking_ratings_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_kicking_ratings_s *player_kicking_ratings = (player_kicking_ratings_s *)data;

     BIND_INT( statement, 1, player_kicking_ratings->player_id        );
     BIND_INT( statement, 2, player_kicking_ratings->kicking_ability  );
     BIND_INT( statement, 3, player_kicking_ratings->avoid_kick_block );

     return SQLITE_OK;
}


int player_kicking_ratings_t_create( sqlite3 *db, const player_kicking_ratings_s *player_kicking_ratings )
{
     static char query[]   = "INSERT INTO Player_Kicking_Ratings_T ( Player_Id, Kicking_Ability, Avoid_Kick_Block )"
          /**/                                             "VALUES ( ?,"       "?,"          "?"       ")";

     return execute_update_old( db, query, player_kicking_ratings_t_create_bindings, player_kicking_ratings, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_kicking_ratings_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     const player_kicking_ratings_s *player_kicking_ratings = (player_kicking_ratings_s *)data;

     return sqlite3_bind_int( statement, 1, player_kicking_ratings->player_id );
}

static int player_kicking_ratings_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     player_kicking_ratings_s *player_kicking_ratings = (player_kicking_ratings_s *)data;

     player_kicking_ratings->kicking_ability   =    sqlite3_column_int(  statement, 0   );
     player_kicking_ratings->avoid_kick_block  =    sqlite3_column_int(  statement, 1   );

     return SQLITE_OK;
}

int player_kicking_ratings_t_read( sqlite3 *db, player_kicking_ratings_s *player_kicking_ratings )
{
     static char query[]   = "SELECT Kicking_Ability, Avoid_Kick_Block FROM Player_Kicking_Ratings_T WHERE Player_Id = ?";

     return execute_query( db, query, player_kicking_ratings_t_read_bindings, player_kicking_ratings, player_kicking_ratings_t_read_retrieve, player_kicking_ratings );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_kicking_ratings_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_kicking_ratings_s *player_kicking_ratings = (player_kicking_ratings_s *)data;

     BIND_INT( statement, 1, player_kicking_ratings->kicking_ability  );
     BIND_INT( statement, 2, player_kicking_ratings->avoid_kick_block );

     BIND_INT( statement, 3, player_kicking_ratings->player_id );

     return SQLITE_OK;
}


int player_kicking_ratings_t_update( sqlite3 *db, const player_kicking_ratings_s *player_kicking_ratings )
{
     static char query[]   = "UPDATE Player_Kicking_Ratings_T "
          /**/
          /**/               "SET    Kicking_Ability  = ?,"
          /**/                      "Avoid_Kick_Block = ? "
          /**/
          /**/               "WHERE  Player_Id = ?";

     return execute_update_old( db, query, player_kicking_ratings_t_update_bindings, player_kicking_ratings, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_kicking_ratings_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     const player_kicking_ratings_s *player_kicking_ratings = (player_kicking_ratings_s *)data;

     return sqlite3_bind_int( statement, 1, player_kicking_ratings->player_id );
}


int player_kicking_ratings_t_delete( sqlite3 *db, const player_kicking_ratings_s *player_kicking_ratings )
{
     return execute_update_old( db, "DELETE FROM Player_Kicking_Ratings_T WHERE Player_Id = ?", player_kicking_ratings_t_delete_bindings, player_kicking_ratings, NULL, NULL );
}
