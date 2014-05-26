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


static int load_conference_stats_data( void *output, int cols, char *data[], char *names[] )
{
     static conference_stats_s conference_stats;

     conference_stats_s **cs = (conference_stats_s **)output;

     if ( cols < 11 ) return SQLITE_ERROR;

     memset( &conference_stats, '\0', sizeof(conference_stats_s) );

     conference_stats.conference_id  = atoi( data[ 0] );
     conference_stats.season         = atoi( data[ 1] );
     conference_stats.bowl_game      = atoi( data[ 2] );
     conference_stats.wins           = atoi( data[ 3] );
     conference_stats.losses         = atoi( data[ 4] );
     conference_stats.home_wins      = atoi( data[ 5] );
     conference_stats.home_losses    = atoi( data[ 6] );
     conference_stats.road_wins      = atoi( data[ 7] );
     conference_stats.road_losses    = atoi( data[ 8] );
     conference_stats.points_scored  = atoi( data[ 9] );
     conference_stats.points_allowed = atoi( data[10] );

     *cs = &conference_stats;

     return SQLITE_OK;
}

static conference_stats_s *get_a_conference_stats( int conference_id )
{
     conference_stats_s *conference_stats = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from conference_stats_t where conference_id = %d", conference_id );

     sqlite3_exec( db, query, load_conference_stats_data, &conference_stats, NULL );

     return conference_stats;
}

static void insert_a_conference_stats( conference_stats_s *conference_stats )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into conference_stats_t (conference_id, season, bowl_game, wins, losses, home_wins, home_losses, road_wins, road_losses, points_scored, points_allowed)"
               "values (%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)",
               conference_stats->conference_id,
               conference_stats->season,
               conference_stats->bowl_game,
               conference_stats->wins,
               conference_stats->losses,
               conference_stats->home_wins,
               conference_stats->home_losses,
               conference_stats->road_wins,
               conference_stats->road_losses,
               conference_stats->points_scored,
               conference_stats->points_allowed );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *conference_stats_t_create__ShouldInsertRecordsInTheConferenceStatsTTable()
{
     conference_stats_s expected = { 0 };

     buildIntoConferenceStats( &expected, 3, 2, bg_RoseBowl );

     assertEquals( SQLITE_OK, conference_stats_t_create( db, &expected ) );

     conference_stats_s *actual = get_a_conference_stats( expected.conference_id );

     compareConferenceStats( &expected, actual );

     sqlite3_exec( db, "delete from conference_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *conference_stats_t_create__ShouldGiveAnErrorIfRecordForSameConferenceSeasonAndBowlExists()
{
     conference_stats_s expected = { 0 };

     buildIntoConferenceStats( &expected, 3, 2, bg_RoseBowl );

     assertEquals( SQLITE_OK, conference_stats_t_create( db, &expected ) );

     expected.wins            = 392;
     expected.losses          = 376;
     expected.home_wins       = 195;
     expected.home_losses     = 189;
     expected.road_wins       = 167;
     expected.road_losses     = 217;
     expected.points_scored   = 2698;
     expected.points_allowed  = 2703;

     assertEquals( SQLITE_CONSTRAINT, conference_stats_t_create( db, &expected ) );

     sqlite3_exec( db, "delete from conference_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *conference_stats_t_read__ShouldRetrieveMatchingRecord_GivenTheConferenceIdSeasonAndBowl()
{
     conference_stats_s expected = { 0 };

     buildIntoConferenceStats( &expected, 3, 2, bg_RoseBowl );

     insert_a_conference_stats( &expected );

     conference_stats_s actual = { 0 };

     actual.conference_id = expected.conference_id;
     actual.season        = expected.season;
     actual.bowl_game     = expected.bowl_game;

     assertEquals( SQLITE_OK, conference_stats_t_read( db, &actual ) );

     compareConferenceStats( &expected, &actual );

     sqlite3_exec( db, "delete from conference_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *conference_stats_t_read_by_conference__ShouldRetrieveAllMatchingRecords_GivenTheConferenceId()
{
     conference_stats_s expected1 = { 0 };
     conference_stats_s expected2 = { 0 };

     buildIntoConferenceStats( &expected1, 3, 2, bg_RoseBowl );
     buildIntoConferenceStats( &expected2, 3, 5, bg_RoseBowl );

     insert_a_conference_stats( &expected1 );
     insert_a_conference_stats( &expected2 );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, conference_stats_t_read_by_conference( db, expected1.conference_id, &list ) );

     assertEquals( 2, list.count );

     conference_stats_s *actual = list.data;

     assertNotNull( actual );

     compareConferenceStats( &expected1, &actual[0] );
     compareConferenceStats( &expected2, &actual[1] );

     sqlite3_exec( db, "delete from conference_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *conference_stats_t_update__ShouldModifyMatchingRecord_GivenTheConferenceIdSeasonAndBowl()
{
     conference_stats_s expected = { 0 };

     buildIntoConferenceStats( &expected, 3, 2, bg_RoseBowl );

     insert_a_conference_stats( &expected );

     expected.wins            = 392;
     expected.losses          = 376;
     expected.home_wins       = 195;
     expected.home_losses     = 189;
     expected.road_wins       = 167;
     expected.road_losses     = 217;
     expected.points_scored   = 2698;
     expected.points_allowed  = 2703;

     assertEquals( SQLITE_OK, conference_stats_t_update( db, &expected ) );

     conference_stats_s *actual = get_a_conference_stats( expected.conference_id );

     compareConferenceStats( &expected, actual );

     sqlite3_exec( db, "delete from conference_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *conference_stats_t_delete__ShouldDeleteMatchingRecord_GivenTheConferenceIdSeasonAndBowl()
{
     conference_stats_s expected = { 0 };

     buildIntoConferenceStats( &expected, 3, 2, bg_RoseBowl );

     insert_a_conference_stats( &expected );

     assertEquals( SQLITE_OK, conference_stats_t_delete( db, &expected ) );

     assertNull( get_a_conference_stats( expected.conference_id ) );

     sqlite3_exec( db, "delete from conference_stats_t", NULL, NULL, NULL );

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
     run_test( conference_stats_t_create__ShouldInsertRecordsInTheConferenceStatsTTable,                     check_sqlite_error );
     run_test( conference_stats_t_create__ShouldGiveAnErrorIfRecordForSameConferenceSeasonAndBowlExists,     check_sqlite_error );
     run_test( conference_stats_t_read__ShouldRetrieveMatchingRecord_GivenTheConferenceIdSeasonAndBowl,      check_sqlite_error );
     run_test( conference_stats_t_read_by_conference__ShouldRetrieveAllMatchingRecords_GivenTheConferenceId, check_sqlite_error );
     run_test( conference_stats_t_update__ShouldModifyMatchingRecord_GivenTheConferenceIdSeasonAndBowl,      check_sqlite_error );
     run_test( conference_stats_t_delete__ShouldDeleteMatchingRecord_GivenTheConferenceIdSeasonAndBowl,      check_sqlite_error );
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
