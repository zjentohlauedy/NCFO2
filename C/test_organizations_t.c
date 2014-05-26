#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "builders.h"
#include "compares.h"
#include "organization.h"
#include "unit_test.h"


static sqlite3 *db;
static char    *result;


static int load_organization_data( void *output, int cols, char *data[], char *names[] )
{
     static organization_s organization;

     organization_s **o = (organization_s **)output;

     if ( cols < 4 ) return SQLITE_ERROR;

     memset( &organization, '\0', sizeof(organization_s) );

     /**/    organization.organization_id = atoi( data[0] );
     strcpy( organization.name,                   data[1] );
     strcpy( organization.abbreviation,           data[2] );
     /**/    organization.season          = atoi( data[3] );

     *o = &organization;

     return SQLITE_OK;
}

static organization_s *get_a_organization( int organization_id )
{
     organization_s *organization = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from organizations_t where organization_id = %d", organization_id );

     sqlite3_exec( db, query, load_organization_data, &organization, NULL );

     return organization;
}

static void insert_a_organization( organization_s *organization )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into organizations_t (organization_id, name, abbreviation, season)"
               "values (%d, '%s', '%s', %d)",
               organization->organization_id,
               organization->name,
               organization->abbreviation,
               organization->season );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *organizations_t_create__ShouldInsertRecordsInTheOrganizationsTTable()
{
     organization_s expected = { 0 };

     buildIntoOrganization( &expected, 1 );

     assertEquals( SQLITE_OK, organizations_t_create( db, &expected ) );

     organization_s *actual = get_a_organization( expected.organization_id );

     compareOrganizations( &expected, actual );

     sqlite3_exec( db, "delete from organizations_t", NULL, NULL, NULL );

     return NULL;
}

static char *organizations_t_create__ShouldGiveAnErrorIfOrganizationIdAlreadyExists()
{
     organization_s expected = { 0 };

     buildIntoOrganization( &expected, 1 );

     assertEquals( SQLITE_OK, organizations_t_create( db, &expected ) );

     strcpy( expected.name, "OrganizationName2" );

     assertEquals( SQLITE_CONSTRAINT, organizations_t_create( db, &expected ) );

     sqlite3_exec( db, "delete from organizations_t", NULL, NULL, NULL );

     return NULL;
}

static char *organizations_t_read__ShouldRetrieveMatchingRecord_GivenTheOrganizationId()
{
     organization_s expected = { 0 };

     buildIntoOrganization( &expected, 1 );

     insert_a_organization( &expected );

     organization_s actual = { 0 };

     actual.organization_id = expected.organization_id;

     assertEquals( SQLITE_OK, organizations_t_read( db, &actual ) );

     compareOrganizations( &expected, &actual );

     sqlite3_exec( db, "delete from organizations_t", NULL, NULL, NULL );

     return NULL;
}

static char *organizations_t_update__ShouldModifyMatchingRecord_GivenTheOrganizationId()
{
     organization_s expected = { 0 };

     buildIntoOrganization( &expected, 1 );

     insert_a_organization( &expected );

     strcpy( expected.name, "OrganizationName2" );

     assertEquals( SQLITE_OK, organizations_t_update( db, &expected ) );

     organization_s *actual = get_a_organization( expected.organization_id );

     compareOrganizations( &expected, actual );

     sqlite3_exec( db, "delete from organizations_t", NULL, NULL, NULL );

     return NULL;
}

static char *organizations_t_delete__ShouldDeleteMatchingRecord_GivenTheOrganizationId()
{
     organization_s expected = { 0 };

     buildIntoOrganization( &expected, 1 );

     insert_a_organization( &expected );

     assertEquals( SQLITE_OK, organizations_t_delete( db, &expected ) );

     assertNull( get_a_organization( expected.organization_id ) );

     sqlite3_exec( db, "delete from organizations_t", NULL, NULL, NULL );

     return NULL;
}

static void check_sqlite_error()
{
     if ( sqlite3_errcode( db ) != 0 )
     {
          printf( "sqlite3 error message: %s\n", sqlite3_errmsg( db ) );
     }
}

static void run_all_tests()
{
     run_test( organizations_t_create__ShouldInsertRecordsInTheOrganizationsTTable,       check_sqlite_error );
     run_test( organizations_t_create__ShouldGiveAnErrorIfOrganizationIdAlreadyExists,    check_sqlite_error );
     run_test( organizations_t_read__ShouldRetrieveMatchingRecord_GivenTheOrganizationId, check_sqlite_error );
     run_test( organizations_t_update__ShouldModifyMatchingRecord_GivenTheOrganizationId, check_sqlite_error );
     run_test( organizations_t_delete__ShouldDeleteMatchingRecord_GivenTheOrganizationId, check_sqlite_error );
}


int main( int argc, char *argv[] )
{
     if ( argc < 2 )
     {
          printf( "Must supply a db file name.\n" );

          return EXIT_FAILURE;
     }

     sqlite3_open( argv[1], &db );
     sqlite3_exec( db, "begin", NULL, NULL, NULL );


     run_all_tests( db );

     show_test_results();


     sqlite3_exec( db, "rollback", NULL, NULL, NULL );
     sqlite3_close( db );

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
