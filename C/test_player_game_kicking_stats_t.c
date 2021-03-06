#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "builders.h"
#include "compares.h"
#include "player.h"
#include "unit_test.h"


static sqlite3 *db;
static char    *result;


static int load_player_game_kicking_stats_data( void *output, int cols, char *data[], char *names[] )
{
     static player_game_kicking_stats_s player_game_kicking_stats;

     player_game_kicking_stats_s **pos = (player_game_kicking_stats_s **)output;

     if ( cols < 10 ) return SQLITE_ERROR;

     memset( &player_game_kicking_stats, '\0', sizeof(player_game_kicking_stats_s) );

     player_game_kicking_stats.player_id            = atoi( data[ 0] );
     player_game_kicking_stats.season               = atoi( data[ 1] );
     player_game_kicking_stats.week                 = atoi( data[ 2] );
     player_game_kicking_stats.game                 = atoi( data[ 3] );
     player_game_kicking_stats.extra_point_attempts = atoi( data[ 4] );
     player_game_kicking_stats.extra_points_made    = atoi( data[ 5] );
     player_game_kicking_stats.field_goal_attempts  = atoi( data[ 6] );
     player_game_kicking_stats.field_goals_made     = atoi( data[ 7] );
     player_game_kicking_stats.punts                = atoi( data[ 8] );
     player_game_kicking_stats.punt_yards           = atoi( data[ 9] );

     *pos = &player_game_kicking_stats;

     return SQLITE_OK;
}

static player_game_kicking_stats_s *get_a_player_game_kicking_stats( int player_id )
{
     player_game_kicking_stats_s *player_game_kicking_stats = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from player_game_kicking_stats_t where player_id = %d", player_id );

     sqlite3_exec( db, query, load_player_game_kicking_stats_data, &player_game_kicking_stats, NULL );

     return player_game_kicking_stats;
}


static void insert_a_player_game_kicking_stats( player_game_kicking_stats_s *player_game_kicking_stats )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into player_game_kicking_stats_t (player_id, season, week, game, extra_point_attempts, extra_points_made, field_goal_attempts, field_goals_made, punts, punt_yards)"
               "values (%d, %d, %d, %d, %d, %d, %d, %d, %d, %d)",
               player_game_kicking_stats->player_id,
               player_game_kicking_stats->season,
               player_game_kicking_stats->week,
               player_game_kicking_stats->game,
               player_game_kicking_stats->extra_point_attempts,
               player_game_kicking_stats->extra_points_made,
               player_game_kicking_stats->field_goal_attempts,
               player_game_kicking_stats->field_goals_made,
               player_game_kicking_stats->punts,
               player_game_kicking_stats->punt_yards           );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *player_game_kicking_stats_t_create__ShouldInsertRecordsInThePlayerGameKickingStatsTTable()
{
     player_game_kicking_stats_s *expected = buildPlayerGameKickingStats( 123, 5, 7, 12 );

     assertEquals( SQLITE_OK, player_game_kicking_stats_t_create( db, expected ) );

     player_game_kicking_stats_s *actual = get_a_player_game_kicking_stats( expected->player_id );

     comparePlayerGameKickingStats( expected, actual );

     sqlite3_exec( db, "delete from player_game_kicking_stats_t", NULL, NULL, NULL );

     free( expected );

     return NULL;
}

static char *player_game_kicking_stats_t_create__ShouldGiveAnErrorIfRecordForSamePlayerSeasonWeekAndGameExists()
{
     player_game_kicking_stats_s *expected1 = buildPlayerGameKickingStats( 123, 5, 7, 12 );
     player_game_kicking_stats_s *expected2 = buildPlayerGameKickingStats( 123, 5, 7, 12 );

     assertEquals( SQLITE_OK,         player_game_kicking_stats_t_create( db, expected1 ) );
     assertEquals( SQLITE_CONSTRAINT, player_game_kicking_stats_t_create( db, expected2 ) );

     sqlite3_exec( db, "delete from player_game_kicking_stats_t", NULL, NULL, NULL );

     free( expected1 );
     free( expected2 );

     return NULL;
}

static char *player_game_kicking_stats_t_read__ShouldRetrieveMatchingRecord_GivenThePlayerIdSeasonWeekAndGame()
{
     player_game_kicking_stats_s *expected = buildPlayerGameKickingStats( 123, 5, 7, 12 );

     insert_a_player_game_kicking_stats( expected );

     player_game_kicking_stats_s actual = { 0 };

     actual.player_id = expected->player_id;
     actual.season    = expected->season;
     actual.week      = expected->week;
     actual.game      = expected->game;

     assertEquals( SQLITE_OK, player_game_kicking_stats_t_read( db, &actual ) );

     comparePlayerGameKickingStats( expected, &actual );

     sqlite3_exec( db, "delete from player_game_kicking_stats_t", NULL, NULL, NULL );

     free( expected );

     return NULL;
}

static char *player_game_kicking_stats_t_read_by_player__ShouldRetrieveAllMatchingRecords_GivenThePlayerId()
{
     player_game_kicking_stats_s *expected1 = buildPlayerGameKickingStats( 123, 5, 7, 12 );
     player_game_kicking_stats_s *expected2 = buildPlayerGameKickingStats( 123, 8, 3, 18 );

     insert_a_player_game_kicking_stats( expected1 );
     insert_a_player_game_kicking_stats( expected2 );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, player_game_kicking_stats_t_read_by_player( db, expected1->player_id, &list ) );

     assertEquals( 2, list.count );

     player_game_kicking_stats_s *actual = list.data;

     assertNotNull( actual );

     comparePlayerGameKickingStats( expected1, &actual[0] );
     comparePlayerGameKickingStats( expected2, &actual[1] );

     free( expected1 );
     free( expected2 );
     free( actual );

     sqlite3_exec( db, "delete from player_game_kicking_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *player_game_kicking_stats_t_update__ShouldModifyMatchingRecord_GivenThePlayerIdSeasonWeekAndGame()
{
     player_game_kicking_stats_s *expected1 = buildPlayerGameKickingStats( 123, 5, 7, 12 );
     player_game_kicking_stats_s *expected2 = buildPlayerGameKickingStats( 123, 5, 7, 12 );

     insert_a_player_game_kicking_stats( expected1 );

     assertEquals( SQLITE_OK, player_game_kicking_stats_t_update( db, expected2 ) );

     player_game_kicking_stats_s *actual = get_a_player_game_kicking_stats( expected1->player_id );

     comparePlayerGameKickingStats( expected2, actual );

     sqlite3_exec( db, "delete from player_game_kicking_stats_t", NULL, NULL, NULL );

     free( expected1 );
     free( expected2 );

     return NULL;
}

static char *player_game_kicking_stats_t_delete__ShouldDeleteMatchingRecord_GivenThePlayerIdSeasonWeekAndGame()
{
     player_game_kicking_stats_s *expected = buildPlayerGameKickingStats( 123, 5, 7, 12 );

     insert_a_player_game_kicking_stats( expected );

     assertEquals( SQLITE_OK, player_game_kicking_stats_t_delete( db, expected ) );

     assertNull( get_a_player_game_kicking_stats( expected->player_id ) );

     sqlite3_exec( db, "delete from player_game_kicking_stats_t", NULL, NULL, NULL );

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
     run_test( player_game_kicking_stats_t_create__ShouldInsertRecordsInThePlayerGameKickingStatsTTable,          check_sqlite_error );
     run_test( player_game_kicking_stats_t_create__ShouldGiveAnErrorIfRecordForSamePlayerSeasonWeekAndGameExists, check_sqlite_error );
     run_test( player_game_kicking_stats_t_read__ShouldRetrieveMatchingRecord_GivenThePlayerIdSeasonWeekAndGame,  check_sqlite_error );
     run_test( player_game_kicking_stats_t_read_by_player__ShouldRetrieveAllMatchingRecords_GivenThePlayerId,     check_sqlite_error );
     run_test( player_game_kicking_stats_t_update__ShouldModifyMatchingRecord_GivenThePlayerIdSeasonWeekAndGame,  check_sqlite_error );
     run_test( player_game_kicking_stats_t_delete__ShouldDeleteMatchingRecord_GivenThePlayerIdSeasonWeekAndGame,  check_sqlite_error );
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
