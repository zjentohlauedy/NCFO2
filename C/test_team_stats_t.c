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


static int load_team_stats_data( void *output, int cols, char *data[], char *names[] )
{
     static team_stats_s team_stats;

     team_stats_s **ts = (team_stats_s **)output;

     if ( cols < 11 ) return SQLITE_ERROR;

     memset( &team_stats, '\0', sizeof(team_stats_s) );

     team_stats.team_id         = atoi( data[ 0] );
     team_stats.season          = atoi( data[ 1] );
     team_stats.bowl_game       = atoi( data[ 2] );
     team_stats.wins            = atoi( data[ 3] );
     team_stats.losses          = atoi( data[ 4] );
     team_stats.home_wins       = atoi( data[ 5] );
     team_stats.home_losses     = atoi( data[ 6] );
     team_stats.road_wins       = atoi( data[ 7] );
     team_stats.road_losses     = atoi( data[ 8] );
     team_stats.points_scored   = atoi( data[ 9] );
     team_stats.points_allowed  = atoi( data[10] );

     *ts = &team_stats;

     return SQLITE_OK;
}

static team_stats_s *get_a_team_stats( int team_id )
{
     team_stats_s *team_stats = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from team_stats_t where team_id = %d", team_id );

     sqlite3_exec( db, query, load_team_stats_data, &team_stats, NULL );

     return team_stats;
}

static void insert_a_team_stats( team_stats_s *team_stats )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into team_stats_t (team_id, season, bowl_game, wins, losses, home_wins, home_losses, road_wins, road_losses, points_scored, points_allowed)"
               "values (%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)",
               team_stats->team_id,
               team_stats->season,
               team_stats->bowl_game,
               team_stats->wins,
               team_stats->losses,
               team_stats->home_wins,
               team_stats->home_losses,
               team_stats->road_wins,
               team_stats->road_losses,
               team_stats->points_scored,
               team_stats->points_allowed );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *team_stats_t_create__ShouldInsertRecordsInTheTeamStatsTTable()
{
     team_stats_s expected = { 0 };

     buildIntoTeamStats( &expected, 123, 2, bg_None );

     assertEquals( SQLITE_OK, team_stats_t_create( db, &expected ) );

     team_stats_s *actual = get_a_team_stats( expected.team_id );

     compareTeamStats( &expected, actual );

     sqlite3_exec( db, "delete from team_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_stats_t_create__ShouldGiveAnErrorIfRecordForSameTeamSeasonAndBowlExists()
{
     team_stats_s expected = { 0 };

     buildIntoTeamStats( &expected, 123, 2, bg_None );

     assertEquals( SQLITE_OK, team_stats_t_create( db, &expected ) );

     expected.wins            = 92;
     expected.losses          = 60;
     expected.home_wins       = 50;
     expected.home_losses     = 26;
     expected.road_wins       = 42;
     expected.road_losses     = 24;
     expected.points_scored   = 199;
     expected.points_allowed  = 141;

     assertEquals( SQLITE_CONSTRAINT, team_stats_t_create( db, &expected ) );

     sqlite3_exec( db, "delete from team_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_stats_t_read__ShouldRetrieveMatchingRecord_GivenTheTeamIdSeasonAndBowl()
{
     team_stats_s expected = { 0 };

     buildIntoTeamStats( &expected, 123, 2, bg_None );

     insert_a_team_stats( &expected );

     team_stats_s actual = { 0 };

     actual.team_id   = expected.team_id;
     actual.season    = expected.season;
     actual.bowl_game = expected.bowl_game;

     assertEquals( SQLITE_OK, team_stats_t_read( db, &actual ) );

     compareTeamStats( &expected, &actual );

     sqlite3_exec( db, "delete from team_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_stats_t_read_by_team__ShouldRetrieveAllMatchingRecords_GivenTheTeamId()
{
     team_stats_s expected1 = { 0 };
     team_stats_s expected2 = { 0 };

     buildIntoTeamStats( &expected1, 123, 2, bg_None );
     buildIntoTeamStats( &expected2, 123, 3, bg_None );

     insert_a_team_stats( &expected1 );
     insert_a_team_stats( &expected2 );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, team_stats_t_read_by_team( db, expected1.team_id, &list ) );

     assertEquals( 2, list.count );

     team_stats_s *actual = list.data;

     assertNotNull( actual );

     compareTeamStats( &expected1, &actual[0] );
     compareTeamStats( &expected2, &actual[1] );

     sqlite3_exec( db, "delete from team_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_stats_t_update__ShouldModifyMatchingRecord_GivenTheTeamIdSeasonAndBowl()
{
     team_stats_s expected = { 0 };

     buildIntoTeamStats( &expected, 123, 2, bg_None );

     insert_a_team_stats( &expected );

     expected.wins            = 92;
     expected.losses          = 60;
     expected.home_wins       = 50;
     expected.home_losses     = 26;
     expected.road_wins       = 42;
     expected.road_losses     = 24;
     expected.points_scored   = 199;
     expected.points_allowed  = 141;

     assertEquals( SQLITE_OK, team_stats_t_update( db, &expected ) );

     team_stats_s *actual = get_a_team_stats( expected.team_id );

     compareTeamStats( &expected, actual );

     sqlite3_exec( db, "delete from team_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_stats_t_delete__ShouldDeleteMatchingRecord_GivenTheTeamIdSeasonAndBowl()
{
     team_stats_s expected = { 0 };

     buildIntoTeamStats( &expected, 123, 2, bg_None );

     insert_a_team_stats( &expected );

     assertEquals( SQLITE_OK, team_stats_t_delete( db, &expected ) );

     assertNull( get_a_team_stats( expected.team_id ) );

     sqlite3_exec( db, "delete from team_stats_t", NULL, NULL, NULL );

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
     run_test( team_stats_t_create__ShouldInsertRecordsInTheTeamStatsTTable,                 check_sqlite_error );
     run_test( team_stats_t_create__ShouldGiveAnErrorIfRecordForSameTeamSeasonAndBowlExists, check_sqlite_error );
     run_test( team_stats_t_read__ShouldRetrieveMatchingRecord_GivenTheTeamIdSeasonAndBowl,  check_sqlite_error );
     run_test( team_stats_t_read_by_team__ShouldRetrieveAllMatchingRecords_GivenTheTeamId,   check_sqlite_error );
     run_test( team_stats_t_update__ShouldModifyMatchingRecord_GivenTheTeamIdSeasonAndBowl,  check_sqlite_error );
     run_test( team_stats_t_delete__ShouldDeleteMatchingRecord_GivenTheTeamIdSeasonAndBowl,  check_sqlite_error );
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
