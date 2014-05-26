#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "data_list.h"
#include "conference.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int conference_accolades_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const conference_accolade_s *conference_accolade = (conference_accolade_s *)data;

     BIND_INT( statement, 1, conference_accolade->conference_id );
     BIND_INT( statement, 2, conference_accolade->season        );
     BIND_INT( statement, 3, conference_accolade->accolade      );

     return SQLITE_OK;
}


int conference_accolades_t_create( sqlite3 *db, const conference_accolade_s *conference_accolade )
{
     static char query[]   = "INSERT INTO Conference_Accolades_T ( Conference_Id, Season, Accolade ) "
          /**/                                           "VALUES ( ?,"           "?,"    "?"      ")";

     return execute_update_old( db, query, conference_accolades_t_create_bindings, conference_accolade, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int conference_accolades_t_read_by_conference_bindings( sqlite3_stmt *statement, const void *data )
{
     const int *conference_id = (int *)data;

     return sqlite3_bind_int( statement, 1, *conference_id );
}

static int conference_accolades_t_read_by_conference_retrieve( sqlite3_stmt *statement, const void *data )
{
     data_list_s *data_list = (data_list_s *)data;

     conference_accolade_s conference_accolade = { 0 };

     conference_accolade.conference_id = sqlite3_column_int( statement, 0 );
     conference_accolade.season        = sqlite3_column_int( statement, 1 );
     conference_accolade.accolade      = sqlite3_column_int( statement, 2 );

     if ( add_to_data_list( data_list, &conference_accolade, sizeof(conference_accolade_s), 10 ) < 0 ) return SQLITE_ERROR;

     return SQLITE_OK;
}

int conference_accolades_t_read_by_conference( sqlite3 *db, const int conference_id, data_list_s *conference_accolades )
{
     static char query[] = "SELECT Conference_Id, Season, Accolade FROM Conference_Accolades_T WHERE Conference_Id = ?";

     return execute_query( db, query, conference_accolades_t_read_by_conference_bindings, &conference_id, conference_accolades_t_read_by_conference_retrieve, conference_accolades );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int conference_accolades_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const conference_accolade_s *conference_accolade = (conference_accolade_s *)data;

     BIND_INT( statement, 1, conference_accolade->conference_id );
     BIND_INT( statement, 2, conference_accolade->season        );
     BIND_INT( statement, 3, conference_accolade->accolade      );

     return SQLITE_OK;
}


int conference_accolades_t_delete( sqlite3 *db, const conference_accolade_s *conference_accolade )
{
     static char query[] = "DELETE FROM Conference_Accolades_T WHERE Conference_Id = ? AND Season = ? AND Accolade = ?";

     return execute_update_old( db, query, conference_accolades_t_delete_bindings, conference_accolade, NULL, NULL );
}
