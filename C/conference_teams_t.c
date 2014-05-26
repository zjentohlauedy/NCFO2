#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "data_list.h"
#include "conference.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int conference_teams_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const conference_team_s *conference_team = (conference_team_s *)data;

     BIND_INT( statement, 1, conference_team->conference_id );
     BIND_INT( statement, 2, conference_team->team_id       );

     return SQLITE_OK;
}


int conference_teams_t_create( sqlite3 *db, const conference_team_s *conference_team )
{
     static char query[]   = "INSERT INTO Conference_Teams_T ( Conference_Id, Team_Id ) "
          /**/                                       "VALUES ( ?,"           "?"     ")";

     return execute_update_old( db, query, conference_teams_t_create_bindings, conference_team, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int conference_teams_t_read_by_conference_bindings( sqlite3_stmt *statement, const void *data )
{
     const int *conference_id = (int *)data;

     return sqlite3_bind_int( statement, 1, *conference_id );
}

static int conference_teams_t_read_by_conference_retrieve( sqlite3_stmt *statement, const void *data )
{
     data_list_s *data_list = (data_list_s *)data;

     conference_team_s conference_team = { 0 };

     conference_team.conference_id = sqlite3_column_int( statement, 0 );
     conference_team.team_id       = sqlite3_column_int( statement, 1 );

     if ( add_to_data_list( data_list, &conference_team, sizeof(conference_team_s), 100 ) < 0 ) return SQLITE_ERROR;

     return SQLITE_OK;
}

int conference_teams_t_read_by_conference( sqlite3 *db, const int conference_id, data_list_s *conference_teams )
{
     static char query[] = "SELECT Conference_Id, Team_Id FROM Conference_Teams_T WHERE Conference_Id = ?";

     return execute_query( db, query, conference_teams_t_read_by_conference_bindings, &conference_id, conference_teams_t_read_by_conference_retrieve, conference_teams );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int conference_teams_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const conference_team_s *conference_team = (conference_team_s *)data;

     BIND_INT(  statement,  1, conference_team->conference_id );
     BIND_INT(  statement,  2, conference_team->team_id       );

     return SQLITE_OK;
}


int conference_teams_t_delete( sqlite3 *db, const conference_team_s *conference_team )
{
     static char query[] = "DELETE FROM Conference_Teams_T WHERE Conference_Id = ? AND Team_Id = ?";

     return execute_update_old( db, query, conference_teams_t_delete_bindings, conference_team, NULL, NULL );
}
