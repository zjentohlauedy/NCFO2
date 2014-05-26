#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "conference.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int conferences_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const conference_s *conference = (conference_s *)data;

     BIND_INT( statement, 1, conference->conference_id );
     BIND_TXT( statement, 2, conference->name          );

     return SQLITE_OK;
}


int conferences_t_create( sqlite3 *db, const conference_s *conference )
{
     static char query[]   = "INSERT INTO Conferences_T ( Conference_Id, Name )"
          /**/                                  "VALUES ( ?,"           "?"  ")";

     return execute_update_old( db, query, conferences_t_create_bindings, conference, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int conferences_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     const conference_s *conference = (conference_s *)data;

     return sqlite3_bind_int( statement, 1, conference->conference_id );
}

static int conferences_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     conference_s *conference = (conference_s *)data;

     cpytxt( conference->name, sqlite3_column_text( statement, 0 ) );

     return SQLITE_OK;
}

int conferences_t_read( sqlite3 *db, conference_s *conference )
{
     static char query[] = "SELECT Name FROM Conferences_T WHERE Conference_Id = ?";

     return execute_query( db, query, conferences_t_read_bindings, conference, conferences_t_read_retrieve, conference );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int conferences_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const conference_s *conference = (conference_s *)data;

     BIND_TXT( statement, 1, conference->name );

     BIND_INT( statement, 2, conference->conference_id );

     return SQLITE_OK;
}


int conferences_t_update( sqlite3 *db, const conference_s *conference )
{
     static char query[]   = "UPDATE Conferences_T "
          /**/
          /**/               "SET    Name = ? "
          /**/
          /**/               "WHERE  Conference_Id = ?";

     return execute_update_old( db, query, conferences_t_update_bindings, conference, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int conferences_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     const conference_s *conference = (conference_s *)data;

     return sqlite3_bind_int( statement, 1, conference->conference_id );
}


int conferences_t_delete( sqlite3 *db, const conference_s *conference )
{
     return execute_update_old( db, "DELETE FROM Conferences_T WHERE Conference_Id = ?", conferences_t_delete_bindings, conference, NULL, NULL );
}
