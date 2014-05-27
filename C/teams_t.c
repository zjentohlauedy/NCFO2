#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "team.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int teams_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_s *team = (team_s *)data;

     BIND_INT( statement, 1, team->team_id      );
     BIND_TXT( statement, 2, team->name         );
     BIND_TXT( statement, 3, team->location     );
     BIND_TXT( statement, 4, team->abbreviation );

     return SQLITE_OK;
}


int teams_t_create( sqlite3 *db, const team_s *team )
{
     static char query[]   = "INSERT INTO Teams_T ( Team_Id, Name, Location, Abbreviation )"
          /**/                            "VALUES ( ?,"     "?,"  "?,"      "?"          ")";

     return execute_update_old( db, query, teams_t_create_bindings, team, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int teams_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     const team_s *team = (team_s *)data;

     return sqlite3_bind_int( statement, 1, team->team_id );
}

static int teams_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     team_s *team = (team_s *)data;

     cpytxt( team->name,             sqlite3_column_text( statement, 0 ) );
     cpytxt( team->location,         sqlite3_column_text( statement, 1 ) );
     cpytxt( team->abbreviation,     sqlite3_column_text( statement, 2 ) );

     return SQLITE_OK;
}

int teams_t_read( sqlite3 *db, team_s *team )
{
     static char query[]   = "SELECT Name, Location, Abbreviation FROM Teams_T WHERE Team_Id = ?";

     return execute_query( db, query, teams_t_read_bindings, team, teams_t_read_retrieve, team );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int teams_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_s *team = (team_s *)data;

     BIND_TXT( statement, 1, team->name         );
     BIND_TXT( statement, 2, team->location     );
     BIND_TXT( statement, 3, team->abbreviation );

     BIND_INT( statement, 4, team->team_id );

     return SQLITE_OK;
}


int teams_t_update( sqlite3 *db, const team_s *team )
{
     static char query[]   = "UPDATE Teams_T "
          /**/
          /**/               "SET    Name         = ?,"
          /**/                      "Location     = ?,"
          /**/                      "Abbreviation = ? "
          /**/
          /**/               "WHERE  Team_Id = ?";

     return execute_update_old( db, query, teams_t_update_bindings, team, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int teams_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     const team_s *team = (team_s *)data;

     return sqlite3_bind_int( statement, 1, team->team_id );
}


int teams_t_delete( sqlite3 *db, const team_s *team )
{
     return execute_update_old( db, "DELETE FROM Teams_T WHERE Team_Id = ?", teams_t_delete_bindings, team, NULL, NULL );
}
