#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_ratings_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_ratings_s *player_ratings = (player_ratings_s *)data;

     BIND_INT( statement, 1, player_ratings->player_id  );
     BIND_INT( statement, 2, player_ratings->run_speed  );
     BIND_INT( statement, 3, player_ratings->rush_power );
     BIND_INT( statement, 4, player_ratings->max_speed  );
     BIND_INT( statement, 5, player_ratings->hit_power  );

     return SQLITE_OK;
}


int player_ratings_t_create( sqlite3 *db, const player_ratings_s *player_ratings )
{
     static char query[]   = "INSERT INTO Player_Ratings_T ( Player_Id, Run_Speed, Rush_Power, Max_Speed, Hit_Power )"
          /**/                                     "VALUES ( ?,"       "?,"       "?,"        "?,"       "?"       ")";

     return execute_update_old( db, query, player_ratings_t_create_bindings, player_ratings, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_ratings_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     const player_ratings_s *player_ratings = (player_ratings_s *)data;

     return sqlite3_bind_int( statement, 1, player_ratings->player_id );
}

static int player_ratings_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     player_ratings_s *player_ratings = (player_ratings_s *)data;

     player_ratings->run_speed   =    sqlite3_column_int(  statement, 0   );
     player_ratings->rush_power  =    sqlite3_column_int(  statement, 1   );
     player_ratings->max_speed   =    sqlite3_column_int(  statement, 2   );
     player_ratings->hit_power   =    sqlite3_column_int(  statement, 3   );

     return SQLITE_OK;
}

int player_ratings_t_read( sqlite3 *db, player_ratings_s *player_ratings )
{
     static char query[]   = "SELECT Run_Speed, Rush_Power, Max_Speed, Hit_Power FROM Player_Ratings_T WHERE Player_Id = ?";

     return execute_query( db, query, player_ratings_t_read_bindings, player_ratings, player_ratings_t_read_retrieve, player_ratings );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_ratings_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_ratings_s *player_ratings = (player_ratings_s *)data;

     BIND_INT( statement, 1, player_ratings->run_speed  );
     BIND_INT( statement, 2, player_ratings->rush_power );
     BIND_INT( statement, 3, player_ratings->max_speed  );
     BIND_INT( statement, 4, player_ratings->hit_power  );

     BIND_INT( statement, 5, player_ratings->player_id );

     return SQLITE_OK;
}


int player_ratings_t_update( sqlite3 *db, const player_ratings_s *player_ratings )
{
     static char query[]   = "UPDATE Player_Ratings_T "
          /**/
          /**/               "SET    Run_Speed  = ?,"
          /**/                      "Rush_Power = ?,"
          /**/                      "Max_Speed  = ?,"
          /**/                      "Hit_Power  = ? "
          /**/
          /**/               "WHERE  Player_Id = ?";

     return execute_update_old( db, query, player_ratings_t_update_bindings, player_ratings, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_ratings_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     const player_ratings_s *player_ratings = (player_ratings_s *)data;

     return sqlite3_bind_int( statement, 1, player_ratings->player_id );
}


int player_ratings_t_delete( sqlite3 *db, const player_ratings_s *player_ratings )
{
     return execute_update_old( db, "DELETE FROM Player_Ratings_T WHERE Player_Id = ?", player_ratings_t_delete_bindings, player_ratings, NULL, NULL );
}
