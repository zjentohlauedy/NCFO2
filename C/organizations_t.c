#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "data_list.h"
#include "organization.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int organizations_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const organization_s *organization = (organization_s *)data;

     BIND_INT( statement, 1, organization->organization_id );
     BIND_TXT( statement, 2, organization->name            );
     BIND_TXT( statement, 3, organization->abbreviation    );
     BIND_INT( statement, 4, organization->season          );

     return SQLITE_OK;
}


int organizations_t_create( sqlite3 *db, const organization_s *organization )
{
     static char query[]   = "INSERT INTO Organizations_T ( Organization_Id, Name, Abbreviation, Season )"
          /**/                                    "VALUES ( ?,"             "?,"  "?,"          "?"    ")";

     return execute_update_old( db, query, organizations_t_create_bindings, organization, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

// By key:
static int organizations_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     const organization_s *organization = (organization_s *)data;

     return sqlite3_bind_int( statement, 1, organization->organization_id );
}

static int organizations_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     organization_s *organization = (organization_s *)data;

     cpytxt( organization->name,           sqlite3_column_text( statement, 0 ) );
     cpytxt( organization->abbreviation,   sqlite3_column_text( statement, 1 ) );
     /**/    organization->season        = sqlite3_column_int(  statement, 2   );

     return SQLITE_OK;
}

int organizations_t_read( sqlite3 *db, organization_s *organization )
{
     static char query[] = "SELECT Name, Abbreviation, Season FROM Organizations_T WHERE Organization_Id = ?";

     return execute_query( db, query, organizations_t_read_bindings, organization, organizations_t_read_retrieve, organization );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int organizations_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const organization_s *organization = (organization_s *)data;

     BIND_TXT( statement, 1, organization->name         );
     BIND_TXT( statement, 2, organization->abbreviation );
     BIND_INT( statement, 3, organization->season       );

     BIND_INT( statement, 4, organization->organization_id );

     return SQLITE_OK;
}


int organizations_t_update( sqlite3 *db, const organization_s *organization )
{
     static char query[]   = "UPDATE Organizations_T "
          /**/
          /**/               "SET    Name         = ?, "
          /**/                      "Abbreviation = ?, "
          /**/                      "Season       = ?  "
          /**/
          /**/               "WHERE  Organization_Id = ?";

     return execute_update_old( db, query, organizations_t_update_bindings, organization, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int organizations_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     const organization_s *organization = (organization_s *)data;

     return sqlite3_bind_int( statement, 1, organization->organization_id );
}


int organizations_t_delete( sqlite3 *db, const organization_s *organization )
{
     return execute_update_old( db, "DELETE FROM Organizations_T WHERE Organization_Id = ?", organizations_t_delete_bindings, organization, NULL, NULL );
}
