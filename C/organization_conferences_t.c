#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "data_list.h"
#include "organization.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int organization_conferences_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const organization_conference_s *organization_conference = (organization_conference_s *)data;

     BIND_INT( statement, 1, organization_conference->organization_id );
     BIND_INT( statement, 2, organization_conference->conference_id   );

     return SQLITE_OK;
}


int organization_conferences_t_create( sqlite3 *db, const organization_conference_s *organization_conference )
{
     static char query[]   = "INSERT INTO Organization_Conferences_T ( Organization_Id, Conference_Id ) "
          /**/                                               "VALUES ( ?,"             "?"           ")";

     return execute_update_old( db, query, organization_conferences_t_create_bindings, organization_conference, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int organization_conferences_t_read_by_organization_bindings( sqlite3_stmt *statement, const void *data )
{
     const int *organization_id = (int *)data;

     return sqlite3_bind_int( statement, 1, *organization_id );
}

static int organization_conferences_t_read_by_organization_retrieve( sqlite3_stmt *statement, const void *data )
{
     data_list_s *data_list = (data_list_s *)data;

     organization_conference_s organization_conference = { 0 };

     organization_conference.organization_id = sqlite3_column_int( statement, 0 );
     organization_conference.conference_id   = sqlite3_column_int( statement, 1 );

     if ( add_to_data_list( data_list, &organization_conference, sizeof(organization_conference_s), 100 ) < 0 ) return SQLITE_ERROR;

     return SQLITE_OK;
}

int organization_conferences_t_read_by_organization( sqlite3 *db, const int organization_id, data_list_s *organization_conferences )
{
     static char query[] = "SELECT Organization_Id, Conference_Id FROM Organization_Conferences_T WHERE Organization_Id = ?";

     return execute_query( db, query, organization_conferences_t_read_by_organization_bindings, &organization_id, organization_conferences_t_read_by_organization_retrieve, organization_conferences );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int organization_conferences_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const organization_conference_s *organization_conference = (organization_conference_s *)data;

     BIND_INT(  statement,  1, organization_conference->organization_id );
     BIND_INT(  statement,  2, organization_conference->conference_id   );

     return SQLITE_OK;
}


int organization_conferences_t_delete( sqlite3 *db, const organization_conference_s *organization_conference )
{
     static char query[] = "DELETE FROM Organization_Conferences_T WHERE Organization_Id = ? AND Conference_Id = ?";

     return execute_update_old( db, query, organization_conferences_t_delete_bindings, organization_conference, NULL, NULL );
}
