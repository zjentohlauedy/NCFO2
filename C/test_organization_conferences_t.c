#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "builders.h"
#include "compares.h"
#include "organization.h"
#include "unit_test.h"


static sqlite3 *db;


static int load_organization_conference_data( void *output, int cols, char *data[], char *names[] )
{
     static organization_conference_s organization_conference;

     organization_conference_s **oc = (organization_conference_s **)output;

     if ( cols < 2 ) return SQLITE_ERROR;

     memset( &organization_conference, '\0', sizeof(organization_conference_s) );

     organization_conference.organization_id = atoi( data[0] );
     organization_conference.conference_id   = atoi( data[1] );

     *oc = &organization_conference;

     return SQLITE_OK;
}

static organization_conference_s *get_a_organization_conference( int organization_id )
{
     organization_conference_s *organization_conference = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from organization_conferences_t where organization_id = %d", organization_id );

     sqlite3_exec( db, query, load_organization_conference_data, &organization_conference, NULL );

     return organization_conference;
}

static void insert_a_organization_conference( organization_conference_s *organization_conference )
{
     char query[999+1];

     snprintf( query, sizeof(query), "insert into organization_conferences_t (organization_id, conference_id) values (%d, %d)",
               organization_conference->organization_id,
               organization_conference->conference_id );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *organization_conferences_t_create__ShouldInsertRecordsInTheOrganizationConferencesTTable()
{
     organization_conference_s expected = { 0 };

     buildIntoOrganizationConference( &expected, 8, 123 );

     assertEquals( SQLITE_OK, organization_conferences_t_create( db, &expected) );

     organization_conference_s *actual = get_a_organization_conference( expected.organization_id );

     compareOrganizationConferences( &expected, actual );

     sqlite3_exec( db, "delete from organization_conferences_t", NULL, NULL, NULL );

     return NULL;
}

static char *organization_conferences_t_create__ShouldGiveAnErrorIfDuplicateRecordIsInserted()
{
     organization_conference_s expected = { 0 };

     buildIntoOrganizationConference( &expected, 8, 123 );

     assertEquals( SQLITE_OK,         organization_conferences_t_create( db, &expected) );
     assertEquals( SQLITE_CONSTRAINT, organization_conferences_t_create( db, &expected) );

     sqlite3_exec( db, "delete from organization_conferences_t", NULL, NULL, NULL );

     return NULL;
}

static char *organization_conferences_t_read_by_organization__ShouldRetrieveMatchingRecord_GivenTheOrganizationId()
{
     organization_conference_s expected = { 0 };

     buildIntoOrganizationConference( &expected, 8, 123 );

     insert_a_organization_conference( &expected );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, organization_conferences_t_read_by_organization( db, expected.organization_id, &list) );

     assertEquals( 1, list.count );

     organization_conference_s *actual   = list.data;

     compareOrganizationConferences( &expected, &actual[0] );

     free( actual );

     sqlite3_exec( db, "delete from organization_conferences_t", NULL, NULL, NULL );

     return NULL;
}

static char *organization_conferences_t_read_by_organization__ShouldRetrieveAllMatchingRecords_GivenTheOrganizationId()
{
     organization_conference_s expected1 = { 0 };
     organization_conference_s expected2 = { 0 };

     buildIntoOrganizationConference( &expected1, 8, 123 );
     buildIntoOrganizationConference( &expected2, 8, 321 );

     insert_a_organization_conference( &expected1 );
     insert_a_organization_conference( &expected2 );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, organization_conferences_t_read_by_organization( db, expected1.organization_id, &list) );

     assertEquals( 2, list.count );

     organization_conference_s *actual   = list.data;

     compareOrganizationConferences( &expected1, &actual[0] );
     compareOrganizationConferences( &expected2, &actual[1] );

     free( actual );

     sqlite3_exec( db, "delete from organization_conferences_t", NULL, NULL, NULL );

     return NULL;
}

static char *organization_conferences_t_delete__ShouldDeleteMatchingRecord_GivenAOrganizationConference()
{
     organization_conference_s expected = { 0 };

     buildIntoOrganizationConference( &expected, 8, 123 );

     insert_a_organization_conference( &expected );

     assertEquals( SQLITE_OK, organization_conferences_t_delete( db, &expected) );

     assertNull( get_a_organization_conference( expected.organization_id ) );

     sqlite3_exec( db, "delete from organization_conferences_t", NULL, NULL, NULL );

     return NULL;
}

static char *organization_conferences_t_delete__ShouldOnlyDeleteMatchingRecord_GivenAOrganizationConference()
{
     organization_conference_s expected1 = { 0 };
     organization_conference_s expected2 = { 0 };

     buildIntoOrganizationConference( &expected1, 8, 123 );
     buildIntoOrganizationConference( &expected2, 8, 321 );

     insert_a_organization_conference( &expected1 );
     insert_a_organization_conference( &expected2 );

     assertEquals( SQLITE_OK, organization_conferences_t_delete( db, &expected2) );

     organization_conference_s *actual = get_a_organization_conference( expected1.organization_id );

     compareOrganizationConferences( &expected1, actual );

     sqlite3_exec( db, "delete from organization_conferences_t", NULL, NULL, NULL );

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
     run_test( organization_conferences_t_create__ShouldInsertRecordsInTheOrganizationConferencesTTable,                 check_sqlite_error );
     run_test( organization_conferences_t_create__ShouldGiveAnErrorIfDuplicateRecordIsInserted,                          check_sqlite_error );
     run_test( organization_conferences_t_read_by_organization__ShouldRetrieveMatchingRecord_GivenTheOrganizationId,     check_sqlite_error );
     run_test( organization_conferences_t_read_by_organization__ShouldRetrieveAllMatchingRecords_GivenTheOrganizationId, check_sqlite_error );
     run_test( organization_conferences_t_delete__ShouldDeleteMatchingRecord_GivenAOrganizationConference,               check_sqlite_error );
     run_test( organization_conferences_t_delete__ShouldOnlyDeleteMatchingRecord_GivenAOrganizationConference,           check_sqlite_error );
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
