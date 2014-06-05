#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_quarterback_ratings_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_quarterback_ratings_s *player_quarterback_ratings = (player_quarterback_ratings_s *)data;

     BIND_INT( statement, 1, player_quarterback_ratings->player_id        );
     BIND_INT( statement, 2, player_quarterback_ratings->pass_speed       );
     BIND_INT( statement, 3, player_quarterback_ratings->pass_control     );
     BIND_INT( statement, 4, player_quarterback_ratings->pass_accuracy    );
     BIND_INT( statement, 5, player_quarterback_ratings->avoid_pass_block );

     return SQLITE_OK;
}


int player_quarterback_ratings_t_create( sqlite3 *db, const player_quarterback_ratings_s *player_quarterback_ratings )
{
     static char query[]   = "INSERT INTO Player_Quarterback_Ratings_T ( Player_Id, Pass_Speed, Pass_Control, Pass_Accuracy, Avoid_Pass_Block )"
          /**/                                                 "VALUES ( ?,"       "?,"        "?,"          "?,"           "?"              ")";

     return execute_update_old( db, query, player_quarterback_ratings_t_create_bindings, player_quarterback_ratings, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_quarterback_ratings_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     const player_quarterback_ratings_s *player_quarterback_ratings = (player_quarterback_ratings_s *)data;

     return sqlite3_bind_int( statement, 1, player_quarterback_ratings->player_id );
}

static int player_quarterback_ratings_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     player_quarterback_ratings_s *player_quarterback_ratings = (player_quarterback_ratings_s *)data;

     player_quarterback_ratings->pass_speed        =    sqlite3_column_int(  statement, 0   );
     player_quarterback_ratings->pass_control      =    sqlite3_column_int(  statement, 1   );
     player_quarterback_ratings->pass_accuracy     =    sqlite3_column_int(  statement, 2   );
     player_quarterback_ratings->avoid_pass_block  =    sqlite3_column_int(  statement, 3   );

     return SQLITE_OK;
}

int player_quarterback_ratings_t_read( sqlite3 *db, player_quarterback_ratings_s *player_quarterback_ratings )
{
     static char query[]   = "SELECT Pass_Speed, Pass_Control, Pass_Accuracy, Avoid_Pass_Block FROM Player_Quarterback_Ratings_T WHERE Player_Id = ?";

     return execute_query( db, query, player_quarterback_ratings_t_read_bindings, player_quarterback_ratings, player_quarterback_ratings_t_read_retrieve, player_quarterback_ratings );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_quarterback_ratings_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_quarterback_ratings_s *player_quarterback_ratings = (player_quarterback_ratings_s *)data;

     BIND_INT( statement, 1, player_quarterback_ratings->pass_speed       );
     BIND_INT( statement, 2, player_quarterback_ratings->pass_control     );
     BIND_INT( statement, 3, player_quarterback_ratings->pass_accuracy    );
     BIND_INT( statement, 4, player_quarterback_ratings->avoid_pass_block );

     BIND_INT( statement, 5, player_quarterback_ratings->player_id );

     return SQLITE_OK;
}


int player_quarterback_ratings_t_update( sqlite3 *db, const player_quarterback_ratings_s *player_quarterback_ratings )
{
     static char query[]   = "UPDATE Player_Quarterback_Ratings_T "
          /**/
          /**/               "SET    Pass_Speed       = ?,"
          /**/                      "Pass_Control     = ?,"
          /**/                      "Pass_Accuracy    = ?,"
          /**/                      "Avoid_Pass_Block = ? "
          /**/
          /**/               "WHERE  Player_Id = ?";

     return execute_update_old( db, query, player_quarterback_ratings_t_update_bindings, player_quarterback_ratings, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_quarterback_ratings_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     const player_quarterback_ratings_s *player_quarterback_ratings = (player_quarterback_ratings_s *)data;

     return sqlite3_bind_int( statement, 1, player_quarterback_ratings->player_id );
}


int player_quarterback_ratings_t_delete( sqlite3 *db, const player_quarterback_ratings_s *player_quarterback_ratings )
{
     return execute_update_old( db, "DELETE FROM Player_Quarterback_Ratings_T WHERE Player_Id = ?", player_quarterback_ratings_t_delete_bindings, player_quarterback_ratings, NULL, NULL );
}
