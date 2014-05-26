#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "builders.h"
#include "compares.h"
#include "conference.h"
#include "unit_test.h"


static sqlite3 *db;


static int load_conference_accolade_data( void *output, int cols, char *data[], char *names[] )
{
     static conference_accolade_s conference_accolade;

     conference_accolade_s **ca = (conference_accolade_s **)output;

     if ( cols < 3 ) return SQLITE_ERROR;

     memset( &conference_accolade, '\0', sizeof(conference_accolade_s) );

     conference_accolade.conference_id = atoi( data[0] );
     conference_accolade.season        = atoi( data[1] );
     conference_accolade.accolade      = atoi( data[2] );

     *ca = &conference_accolade;

     return SQLITE_OK;
}

static conference_accolade_s *get_a_conference_accolade( int conference_id )
{
     conference_accolade_s *conference_accolade = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from conference_accolades_t where conference_id = %d", conference_id );

     sqlite3_exec( db, query, load_conference_accolade_data, &conference_accolade, NULL );

     return conference_accolade;
}

static void insert_a_conference_accolade( conference_accolade_s *conference_accolade )
{
     char query[999+1];

     snprintf( query, sizeof(query), "insert into conference_accolades_t (conference_id, season, accolade) values (%d, %d, %d)",
               conference_accolade->conference_id,
               conference_accolade->season,
               conference_accolade->accolade );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}

static char *conference_accolades_t_create__ShouldInsertRecordsInTheConferenceAccoladesTTable()
{
     conference_accolade_s expected = { 0 };

     buildIntoConferenceAccolade( &expected, 123, 5, cacc_LibertyBowlChampions );

     assertEquals( SQLITE_OK, conference_accolades_t_create( db, &expected ) );

     conference_accolade_s *actual = get_a_conference_accolade( expected.conference_id );

     compareConferenceAccolades( &expected, actual );

     sqlite3_exec( db, "delete from conference_accolades_t", NULL, NULL, NULL );

     return NULL;
}

static char *conference_accolades_t_create__ShouldGiveAnErrorIfDuplicateRecordIsInserted()
{
     conference_accolade_s expected = { 0 };

     buildIntoConferenceAccolade( &expected, 123, 5, cacc_LibertyBowlChampions );

     assertEquals( SQLITE_OK,         conference_accolades_t_create( db, &expected ) );
     assertEquals( SQLITE_CONSTRAINT, conference_accolades_t_create( db, &expected ) );

     sqlite3_exec( db, "delete from conference_accolades_t", NULL, NULL, NULL );

     return NULL;
}

static char *conference_accolades_t_read_by_conference__ShouldRetrieveMatchingRecord_GivenTheConferenceId()
{
     conference_accolade_s expected = { 0 };

     buildIntoConferenceAccolade( &expected, 123, 5, cacc_LibertyBowlChampions );

     insert_a_conference_accolade( &expected );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, conference_accolades_t_read_by_conference( db, expected.conference_id, &list ) );

     assertEquals( 1, list.count );

     conference_accolade_s *actual = list.data;

     compareConferenceAccolades( &expected, actual );

     free( actual );

     sqlite3_exec( db, "delete from conference_accolades_t", NULL, NULL, NULL );

     return NULL;
}

static char *conference_accolades_t_read_by_conference__ShouldRetrieveAllMatchingRecords_GivenTheConferenceId()
{
     conference_accolade_s expected1 = { 0 };
     conference_accolade_s expected2 = { 0 };

     buildIntoConferenceAccolade( &expected1, 123, 5, cacc_LibertyBowlChampions );
     buildIntoConferenceAccolade( &expected2, 123, 6, cacc_NCFOChampions        );

     insert_a_conference_accolade( &expected1 );
     insert_a_conference_accolade( &expected2 );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, conference_accolades_t_read_by_conference( db, expected1.conference_id, &list ) );

     assertEquals( 2, list.count );

     conference_accolade_s *actual = list.data;

     compareConferenceAccolades( &expected1, &actual[0] );
     compareConferenceAccolades( &expected2, &actual[1] );

     free( actual );

     sqlite3_exec( db, "delete from conference_accolades_t", NULL, NULL, NULL );

     return NULL;
}

static char *conference_accolades_t_delete__ShouldDeleteMatchingRecord_GivenAConferenceAccolade()
{
     conference_accolade_s expected = { 0 };

     buildIntoConferenceAccolade( &expected, 123, 5, cacc_LibertyBowlChampions );

     insert_a_conference_accolade( &expected );

     assertEquals( SQLITE_OK, conference_accolades_t_delete( db, &expected ) );

     assertNull( get_a_conference_accolade( expected.conference_id ) );

     sqlite3_exec( db, "delete from conference_accolades_t", NULL, NULL, NULL );

     return NULL;
}

static char *conference_accolades_t_delete__ShouldOnlyDeleteMatchingRecord_GivenAConferenceAccolade()
{
     conference_accolade_s expected1 = { 0 };
     conference_accolade_s expected2 = { 0 };

     buildIntoConferenceAccolade( &expected1, 123, 5, cacc_LibertyBowlChampions );
     buildIntoConferenceAccolade( &expected2, 123, 5, cacc_OrangeBowlChampions  );

     insert_a_conference_accolade( &expected1 );
     insert_a_conference_accolade( &expected2 );

     assertEquals( SQLITE_OK, conference_accolades_t_delete( db, &expected2 ) );

     conference_accolade_s *actual = get_a_conference_accolade( expected1.conference_id );

     compareConferenceAccolades( &expected1, actual );

     sqlite3_exec( db, "delete from conference_accolades_t", NULL, NULL, NULL );

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
     run_test( conference_accolades_t_create__ShouldInsertRecordsInTheConferenceAccoladesTTable,                 check_sqlite_error );
     run_test( conference_accolades_t_create__ShouldGiveAnErrorIfDuplicateRecordIsInserted,                      check_sqlite_error );
     run_test( conference_accolades_t_read_by_conference__ShouldRetrieveMatchingRecord_GivenTheConferenceId,     check_sqlite_error );
     run_test( conference_accolades_t_read_by_conference__ShouldRetrieveAllMatchingRecords_GivenTheConferenceId, check_sqlite_error );
     run_test( conference_accolades_t_delete__ShouldDeleteMatchingRecord_GivenAConferenceAccolade,               check_sqlite_error );
     run_test( conference_accolades_t_delete__ShouldOnlyDeleteMatchingRecord_GivenAConferenceAccolade,           check_sqlite_error );
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
