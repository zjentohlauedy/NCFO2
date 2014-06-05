#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_offense_ratings_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_offense_ratings_s *player_offense_ratings = (player_offense_ratings_s *)data;

     BIND_INT( statement, 1, player_offense_ratings->player_id    );
     BIND_INT( statement, 2, player_offense_ratings->ball_control );
     BIND_INT( statement, 3, player_offense_ratings->receiving    );

     return SQLITE_OK;
}


int player_offense_ratings_t_create( sqlite3 *db, const player_offense_ratings_s *player_offense_ratings )
{
     static char query[]   = "INSERT INTO Player_Offense_Ratings_T ( Player_Id, Ball_Control, Receiving )"
          /**/                                             "VALUES ( ?,"       "?,"          "?"       ")";

     return execute_update_old( db, query, player_offense_ratings_t_create_bindings, player_offense_ratings, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_offense_ratings_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     const player_offense_ratings_s *player_offense_ratings = (player_offense_ratings_s *)data;

     return sqlite3_bind_int( statement, 1, player_offense_ratings->player_id );
}

static int player_offense_ratings_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     player_offense_ratings_s *player_offense_ratings = (player_offense_ratings_s *)data;

     player_offense_ratings->ball_control  =    sqlite3_column_int(  statement, 0   );
     player_offense_ratings->receiving     =    sqlite3_column_int(  statement, 1   );

     return SQLITE_OK;
}

int player_offense_ratings_t_read( sqlite3 *db, player_offense_ratings_s *player_offense_ratings )
{
     static char query[]   = "SELECT Ball_Control, Receiving FROM Player_Offense_Ratings_T WHERE Player_Id = ?";

     return execute_query( db, query, player_offense_ratings_t_read_bindings, player_offense_ratings, player_offense_ratings_t_read_retrieve, player_offense_ratings );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_offense_ratings_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_offense_ratings_s *player_offense_ratings = (player_offense_ratings_s *)data;

     BIND_INT( statement, 1, player_offense_ratings->ball_control );
     BIND_INT( statement, 2, player_offense_ratings->receiving    );

     BIND_INT( statement, 3, player_offense_ratings->player_id );

     return SQLITE_OK;
}


int player_offense_ratings_t_update( sqlite3 *db, const player_offense_ratings_s *player_offense_ratings )
{
     static char query[]   = "UPDATE Player_Offense_Ratings_T "
          /**/
          /**/               "SET    Ball_Control = ?,"
          /**/                      "Receiving    = ? "
          /**/
          /**/               "WHERE  Player_Id = ?";

     return execute_update_old( db, query, player_offense_ratings_t_update_bindings, player_offense_ratings, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_offense_ratings_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     const player_offense_ratings_s *player_offense_ratings = (player_offense_ratings_s *)data;

     return sqlite3_bind_int( statement, 1, player_offense_ratings->player_id );
}


int player_offense_ratings_t_delete( sqlite3 *db, const player_offense_ratings_s *player_offense_ratings )
{
     return execute_update_old( db, "DELETE FROM Player_Offense_Ratings_T WHERE Player_Id = ?", player_offense_ratings_t_delete_bindings, player_offense_ratings, NULL, NULL );
}
