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


static int load_team_game_stats_data( void *output, int cols, char *data[], char *names[] )
{
     static team_game_stats_s team_game_stats;

     team_game_stats_s **ts = (team_game_stats_s **)output;

     if ( cols < 15 ) return SQLITE_ERROR;

     memset( &team_game_stats, '\0', sizeof(team_game_stats_s) );

     team_game_stats.team_id         = atoi( data[ 0] );
     team_game_stats.season          = atoi( data[ 1] );
     team_game_stats.week            = atoi( data[ 2] );
     team_game_stats.game            = atoi( data[ 3] );
     team_game_stats.wins            = atoi( data[ 4] );
     team_game_stats.losses          = atoi( data[ 5] );
     team_game_stats.ties            = atoi( data[ 6] );
     team_game_stats.home_wins       = atoi( data[ 7] );
     team_game_stats.home_losses     = atoi( data[ 8] );
     team_game_stats.home_ties       = atoi( data[ 9] );
     team_game_stats.road_wins       = atoi( data[10] );
     team_game_stats.road_losses     = atoi( data[11] );
     team_game_stats.road_ties       = atoi( data[12] );
     team_game_stats.points_scored   = atoi( data[13] );
     team_game_stats.points_allowed  = atoi( data[14] );

     *ts = &team_game_stats;

     return SQLITE_OK;
}

static team_game_stats_s *get_a_team_game_stats( int team_id )
{
     team_game_stats_s *team_game_stats = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from team_game_stats_t where team_id = %d", team_id );

     sqlite3_exec( db, query, load_team_game_stats_data, &team_game_stats, NULL );

     return team_game_stats;
}

static void insert_a_team_game_stats( team_game_stats_s *team_game_stats )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into team_game_stats_t (team_id, season, week, game, wins, losses, ties, home_wins, home_losses, home_ties, road_wins, road_losses, road_ties, points_scored, points_allowed)"
               "values (%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)",
               team_game_stats->team_id,
               team_game_stats->season,
               team_game_stats->week,
               team_game_stats->game,
               team_game_stats->wins,
               team_game_stats->losses,
               team_game_stats->ties,
               team_game_stats->home_wins,
               team_game_stats->home_losses,
               team_game_stats->home_ties,
               team_game_stats->road_wins,
               team_game_stats->road_losses,
               team_game_stats->road_ties,
               team_game_stats->points_scored,
               team_game_stats->points_allowed );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *team_game_stats_t_create__ShouldInsertRecordsInTheTeamGameStatsTTable()
{
     team_game_stats_s expected = { 0 };

     buildIntoTeamGameStats( &expected, 123, 2, 4, 6 );

     assertEquals( SQLITE_OK, team_game_stats_t_create( db, &expected ) );

     team_game_stats_s *actual = get_a_team_game_stats( expected.team_id );

     compareTeamGameStats( &expected, actual );

     sqlite3_exec( db, "delete from team_game_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_game_stats_t_create__ShouldGiveAnErrorIfRecordForSameTeamSeasonWeekAndGameExists()
{
     team_game_stats_s expected = { 0 };

     buildIntoTeamGameStats( &expected, 123, 2, 4, 6 );

     assertEquals( SQLITE_OK, team_game_stats_t_create( db, &expected ) );

     expected.wins            = 92;
     expected.losses          = 60;
     expected.ties            = 54;
     expected.home_wins       = 50;
     expected.home_losses     = 26;
     expected.home_ties       = 18;
     expected.road_wins       = 42;
     expected.road_losses     = 24;
     expected.road_ties       = 13;
     expected.points_scored   = 199;
     expected.points_allowed  = 141;

     assertEquals( SQLITE_CONSTRAINT, team_game_stats_t_create( db, &expected ) );

     sqlite3_exec( db, "delete from team_game_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_game_stats_t_read__ShouldRetrieveMatchingRecord_GivenTheTeamIdSeasonWeekAndGame()
{
     team_game_stats_s expected = { 0 };

     buildIntoTeamGameStats( &expected, 123, 2, 4, 6 );

     insert_a_team_game_stats( &expected );

     team_game_stats_s actual = { 0 };

     actual.team_id   = expected.team_id;
     actual.season    = expected.season;
     actual.week      = expected.week;
     actual.game      = expected.game;

     assertEquals( SQLITE_OK, team_game_stats_t_read( db, &actual ) );

     compareTeamGameStats( &expected, &actual );

     sqlite3_exec( db, "delete from team_game_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_game_stats_t_read_by_team__ShouldRetrieveAllMatchingRecords_GivenTheTeamId()
{
     team_game_stats_s expected1 = { 0 };
     team_game_stats_s expected2 = { 0 };

     buildIntoTeamGameStats( &expected1, 123, 2, 4,  6 );
     buildIntoTeamGameStats( &expected2, 123, 3, 2, 11 );

     insert_a_team_game_stats( &expected1 );
     insert_a_team_game_stats( &expected2 );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, team_game_stats_t_read_by_team( db, expected1.team_id, &list ) );

     assertEquals( 2, list.count );

     team_game_stats_s *actual = list.data;

     assertNotNull( actual );

     compareTeamGameStats( &expected1, &actual[0] );
     compareTeamGameStats( &expected2, &actual[1] );

     sqlite3_exec( db, "delete from team_game_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_game_stats_t_update__ShouldModifyMatchingRecord_GivenTheTeamIdSeasonWeekAndGame()
{
     team_game_stats_s expected = { 0 };

     buildIntoTeamGameStats( &expected, 123, 2, 4, 6 );

     insert_a_team_game_stats( &expected );

     expected.wins            = 92;
     expected.losses          = 60;
     expected.ties            = 54;
     expected.home_wins       = 50;
     expected.home_losses     = 26;
     expected.home_ties       = 18;
     expected.road_wins       = 42;
     expected.road_losses     = 24;
     expected.road_ties       = 13;
     expected.points_scored   = 199;
     expected.points_allowed  = 141;

     assertEquals( SQLITE_OK, team_game_stats_t_update( db, &expected ) );

     team_game_stats_s *actual = get_a_team_game_stats( expected.team_id );

     compareTeamGameStats( &expected, actual );

     sqlite3_exec( db, "delete from team_game_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_game_stats_t_delete__ShouldDeleteMatchingRecord_GivenTheTeamIdSeasonWeekAndGame()
{
     team_game_stats_s expected = { 0 };

     buildIntoTeamGameStats( &expected, 123, 2, 4, 6 );

     insert_a_team_game_stats( &expected );

     assertEquals( SQLITE_OK, team_game_stats_t_delete( db, &expected ) );

     assertNull( get_a_team_game_stats( expected.team_id ) );

     sqlite3_exec( db, "delete from team_game_stats_t", NULL, NULL, NULL );

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
     run_test( team_game_stats_t_create__ShouldInsertRecordsInTheTeamGameStatsTTable,                 check_sqlite_error );
     run_test( team_game_stats_t_create__ShouldGiveAnErrorIfRecordForSameTeamSeasonWeekAndGameExists, check_sqlite_error );
     run_test( team_game_stats_t_read__ShouldRetrieveMatchingRecord_GivenTheTeamIdSeasonWeekAndGame,  check_sqlite_error );
     run_test( team_game_stats_t_read_by_team__ShouldRetrieveAllMatchingRecords_GivenTheTeamId,       check_sqlite_error );
     run_test( team_game_stats_t_update__ShouldModifyMatchingRecord_GivenTheTeamIdSeasonWeekAndGame,  check_sqlite_error );
     run_test( team_game_stats_t_delete__ShouldDeleteMatchingRecord_GivenTheTeamIdSeasonWeekAndGame,  check_sqlite_error );
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
