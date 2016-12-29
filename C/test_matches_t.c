#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "builders.h"
#include "compares.h"
#include "team.h"
#include "unit_test.h"


static sqlite3 *db;
static char    *result;


static int load_match_data( void *output, int cols, char *data[], char *names[] )
{
     static match_s match;

     match_s **tds = (match_s **)output;

     if ( cols < 5 ) return SQLITE_ERROR;

     memset( &match, '\0', sizeof(match_s) );

     match.season       = atoi( data[ 0] );
     match.week         = atoi( data[ 1] );
     match.game         = atoi( data[ 2] );
     match.road_team_id = atoi( data[ 3] );
     match.home_team_id = atoi( data[ 4] );

     *tds = &match;

     return SQLITE_OK;
}

static match_s *get_a_match( int season, int week, int game )
{
     match_s *match = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from matches_t where season = %d and week = %d and game = %d", season, week, game );

     sqlite3_exec( db, query, load_match_data, &match, NULL );

     return match;
}


static void insert_a_match( match_s *match )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into matches_t (season, week, game, road_team_id, home_team_id)"
               "values (%d, %d, %d, %d, %d)",
               match->season,
               match->week,
               match->game,
               match->road_team_id,
               match->home_team_id );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *matches_t_create__ShouldInsertRecordsInTheMatchesTTable()
{
     match_s *expected = buildMatch( 5, 7, 12 );

     assertEquals( SQLITE_OK, matches_t_create( db, expected ) );

     match_s *actual = get_a_match( expected->season, expected->week, expected->game );

     compareMatches( expected, actual );

     sqlite3_exec( db, "delete from matches_t", NULL, NULL, NULL );

     free( expected );

     return NULL;
}

static char *matches_t_create__ShouldGiveAnErrorIfRecordForSameSeasonWeekAndGameExists()
{
     match_s *expected1 = buildMatch( 5, 7, 12 );
     match_s *expected2 = buildMatch( 5, 7, 12 );

     assertEquals( SQLITE_OK,         matches_t_create( db, expected1 ) );
     assertEquals( SQLITE_CONSTRAINT, matches_t_create( db, expected2 ) );

     sqlite3_exec( db, "delete from matches_t", NULL, NULL, NULL );

     free( expected1 );
     free( expected2 );

     return NULL;
}

static char *matches_t_read__ShouldRetrieveMatchingRecord_GivenTheSeasonWeekAndGame()
{
     match_s *expected = buildMatch( 5, 7, 12 );

     insert_a_match( expected );

     match_s actual = { 0 };

     actual.season    = expected->season;
     actual.week      = expected->week;
     actual.game      = expected->game;

     assertEquals( SQLITE_OK, matches_t_read( db, &actual ) );

     compareMatches( expected, &actual );

     sqlite3_exec( db, "delete from matches_t", NULL, NULL, NULL );

     free( expected );

     return NULL;
}

static char *matches_t_update__ShouldModifyMatchingRecord_GivenTheSeasonWeekAndGame()
{
     match_s *expected1 = buildMatch( 5, 7, 12 );
     match_s *expected2 = buildMatch( 5, 7, 12 );

     insert_a_match( expected1 );

     assertEquals( SQLITE_OK, matches_t_update( db, expected2 ) );

     match_s *actual = get_a_match( expected1->season, expected1->week, expected1->game );

     compareMatches( expected2, actual );

     sqlite3_exec( db, "delete from matches_t", NULL, NULL, NULL );

     free( expected1 );
     free( expected2 );

     return NULL;
}

static char *matches_t_delete__ShouldDeleteMatchingRecord_GivenTheSeasonWeekAndGame()
{
     match_s *expected = buildMatch( 5, 7, 12 );

     insert_a_match( expected );

     assertEquals( SQLITE_OK, matches_t_delete( db, expected ) );

     assertNull( get_a_match( expected->season, expected->week, expected->game ) );

     sqlite3_exec( db, "delete from matches_t", NULL, NULL, NULL );

     free( expected );

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
     run_test( matches_t_create__ShouldInsertRecordsInTheMatchesTTable,                   check_sqlite_error );
     run_test( matches_t_create__ShouldGiveAnErrorIfRecordForSameSeasonWeekAndGameExists, check_sqlite_error );
     run_test( matches_t_read__ShouldRetrieveMatchingRecord_GivenTheSeasonWeekAndGame,    check_sqlite_error );
     run_test( matches_t_update__ShouldModifyMatchingRecord_GivenTheSeasonWeekAndGame,    check_sqlite_error );
     run_test( matches_t_delete__ShouldDeleteMatchingRecord_GivenTheSeasonWeekAndGame,    check_sqlite_error );
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
