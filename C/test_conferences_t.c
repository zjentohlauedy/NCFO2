#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "builders.h"
#include "compares.h"
#include "conference.h"
#include "unit_test.h"


static sqlite3 *db;
static char    *result;


static int load_conference_data( void *output, int cols, char *data[], char *names[] )
{
     static conference_s conference;

     conference_s **c = (conference_s **)output;

     if ( cols < 2 ) return SQLITE_ERROR;

     memset( &conference, '\0', sizeof(conference_s) );

     /**/    conference.conference_id = atoi( data[0] );
     strcpy( conference.name,                 data[1] );

     *c = &conference;

     return SQLITE_OK;
}

static conference_s *get_a_conference( int conference_id )
{
     conference_s *conference = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from conferences_t where conference_id = %d", conference_id );

     sqlite3_exec( db, query, load_conference_data, &conference, NULL );

     return conference;
}

static void insert_a_conference( conference_s *conference )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into conferences_t (conference_id, name)"
               "values (%d, '%s')",
               conference->conference_id,
               conference->name );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *conferences_t_create__ShouldInsertRecordsInTheConferencesTTable()
{
     conference_s expected = { 0 };

     buildIntoConference( &expected, 1 );

     assertEquals( SQLITE_OK, conferences_t_create( db, &expected ) );

     conference_s *actual = get_a_conference( expected.conference_id );

     compareConferences( &expected, actual );

     sqlite3_exec( db, "delete from conferences_t", NULL, NULL, NULL );

     return NULL;
}

static char *conferences_t_create__ShouldGiveAnErrorIfConferenceIdAlreadyExists()
{
     conference_s expected = { 0 };

     buildIntoConference( &expected, 1 );

     assertEquals( SQLITE_OK, conferences_t_create( db, &expected ) );

     strcpy( expected.name, "ConferenceName2" );

     assertEquals( SQLITE_CONSTRAINT, conferences_t_create( db, &expected ) );

     sqlite3_exec( db, "delete from conferences_t", NULL, NULL, NULL );

     return NULL;
}

static char *conferences_t_read__ShouldRetrieveMatchingRecord_GivenTheConferenceId()
{
     conference_s expected = { 0 };

     buildIntoConference( &expected, 1 );

     insert_a_conference( &expected );

     conference_s actual = { 0 };

     actual.conference_id = expected.conference_id;

     assertEquals( SQLITE_OK, conferences_t_read( db, &actual ) );

     compareConferences( &expected, &actual );

     sqlite3_exec( db, "delete from conferences_t", NULL, NULL, NULL );

     return NULL;
}

static char *conferences_t_update__ShouldModifyMatchingRecord_GivenTheConferenceId()
{
     conference_s expected = { 0 };

     buildIntoConference( &expected, 1 );

     insert_a_conference( &expected );

     strcpy( expected.name, "ConferenceName2" );

     assertEquals( SQLITE_OK, conferences_t_update( db, &expected ) );

     conference_s *actual = get_a_conference( expected.conference_id );

     compareConferences( &expected, actual );

     sqlite3_exec( db, "delete from conferences_t", NULL, NULL, NULL );

     return NULL;
}

static char *conferences_t_delete__ShouldDeleteMatchingRecord_GivenTheConferenceId()
{
     conference_s expected = { 0 };

     buildIntoConference( &expected, 1 );

     insert_a_conference( &expected );

     assertEquals( SQLITE_OK, conferences_t_delete( db, &expected ) );

     assertNull( get_a_conference( expected.conference_id ) );

     sqlite3_exec( db, "delete from conferences_t", NULL, NULL, NULL );

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
     run_test( conferences_t_create__ShouldInsertRecordsInTheConferencesTTable,       check_sqlite_error );
     run_test( conferences_t_create__ShouldGiveAnErrorIfConferenceIdAlreadyExists,    check_sqlite_error );
     run_test( conferences_t_read__ShouldRetrieveMatchingRecord_GivenTheConferenceId, check_sqlite_error );
     run_test( conferences_t_update__ShouldModifyMatchingRecord_GivenTheConferenceId, check_sqlite_error );
     run_test( conferences_t_delete__ShouldDeleteMatchingRecord_GivenTheConferenceId, check_sqlite_error );
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
