#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "organization.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int matches_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const match_s *match = (match_s *)data;

     BIND_INT( statement,  1, match->season       );
     BIND_INT( statement,  2, match->week         );
     BIND_INT( statement,  3, match->game         );
     BIND_INT( statement,  4, match->road_team_id );
     BIND_INT( statement,  5, match->home_team_id );

     return SQLITE_OK;
}


int matches_t_create( sqlite3 *db, const match_s *match )
{
     static char query[]   = "INSERT INTO Matches_T ( Season, Week, Game, Road_Team_Id, Home_Team_Id )"
          /**/                              "VALUES ( ?,"    "?,"  "?,"  "?,"          "?"          ")";

     return execute_update_old( db, query, matches_t_create_bindings, match, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

// By Key:
static int matches_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const match_s *match = (match_s *)data;

     BIND_INT( statement, 1, match->season );
     BIND_INT( statement, 2, match->week   );
     BIND_INT( statement, 3, match->game   );

     return SQLITE_OK;
}

static int matches_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     match_s *match = (match_s *)data;

     match->road_team_id = sqlite3_column_int( statement,  0 );
     match->home_team_id = sqlite3_column_int( statement,  1 );

     return SQLITE_OK;
}

int matches_t_read( sqlite3 *db, match_s *match )
{
     static char query[]   = "SELECT Road_Team_Id, Home_Team_Id "
          /**/               "FROM   Matches_T "
          /**/               "WHERE  Season    = ? "
          /**/               "AND    Week      = ? "
          /**/               "AND    Game      = ? ";

     return execute_query( db, query, matches_t_read_bindings, match, matches_t_read_retrieve, match );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int matches_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const match_s *match = (match_s *)data;

     BIND_INT( statement, 1, match->road_team_id );
     BIND_INT( statement, 2, match->home_team_id );

     BIND_INT( statement, 3, match->season );
     BIND_INT( statement, 4, match->week   );
     BIND_INT( statement, 5, match->game   );

     return SQLITE_OK;
}


int matches_t_update( sqlite3 *db, const match_s *match )
{
     static char query[]   = "UPDATE Matches_T "
          /**/
          /**/               "SET    Road_Team_Id = ?,"
          /**/                      "Home_Team_Id = ? "
          /**/
          /**/               "WHERE  Season    = ? "
          /**/               "AND    Week      = ? "
          /**/               "AND    Game      = ? ";

     return execute_update_old( db, query, matches_t_update_bindings, match, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int matches_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const match_s *match = (match_s *)data;

     BIND_INT( statement, 1, match->season    );
     BIND_INT( statement, 2, match->week      );
     BIND_INT( statement, 3, match->game      );

     return SQLITE_OK;
}


int matches_t_delete( sqlite3 *db, const match_s *match )
{
     return execute_update_old( db, "DELETE FROM Matches_T WHERE Season = ? AND Week = ? AND Game = ?", matches_t_delete_bindings, match, NULL, NULL );
}
