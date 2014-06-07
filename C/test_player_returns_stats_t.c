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


static int load_player_returns_stats_data( void *output, int cols, char *data[], char *names[] )
{
     static player_returns_stats_s player_returns_stats;

     player_returns_stats_s **pos = (player_returns_stats_s **)output;

     if ( cols < 9 ) return SQLITE_ERROR;

     memset( &player_returns_stats, '\0', sizeof(player_returns_stats_s) );

     player_returns_stats.player_id              = atoi( data[ 0] );
     player_returns_stats.season                 = atoi( data[ 1] );
     player_returns_stats.bowl_game              = atoi( data[ 2] );
     player_returns_stats.kick_returns           = atoi( data[ 3] );
     player_returns_stats.kick_return_yards      = atoi( data[ 4] );
     player_returns_stats.kick_return_touchdowns = atoi( data[ 5] );
     player_returns_stats.punt_returns           = atoi( data[ 6] );
     player_returns_stats.punt_return_yards      = atoi( data[ 7] );
     player_returns_stats.punt_return_touchdowns = atoi( data[ 8] );

     *pos = &player_returns_stats;

     return SQLITE_OK;
}

static player_returns_stats_s *get_a_player_returns_stats( int player_id )
{
     player_returns_stats_s *player_returns_stats = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from player_returns_stats_t where player_id = %d", player_id );

     sqlite3_exec( db, query, load_player_returns_stats_data, &player_returns_stats, NULL );

     return player_returns_stats;
}


static void insert_a_player_returns_stats( player_returns_stats_s *player_returns_stats )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into player_returns_stats_t (player_id, season, bowl_game, kick_returns, kick_return_yards, kick_return_touchdowns, punt_returns, punt_return_yards, punt_return_touchdowns)"
               "values (%d, %d, %d, %d, %d, %d, %d, %d, %d)",
               player_returns_stats->player_id,
               player_returns_stats->season,
               player_returns_stats->bowl_game,
               player_returns_stats->kick_returns,
               player_returns_stats->kick_return_yards,
               player_returns_stats->kick_return_touchdowns,
               player_returns_stats->punt_returns,
               player_returns_stats->punt_return_yards,
               player_returns_stats->punt_return_touchdowns );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *player_returns_stats_t_create__ShouldInsertRecordsInThePlayerReturnsStatsTTable()
{
     player_returns_stats_s *expected = buildPlayerReturnsStats( 123, 5, bg_None );

     assertEquals( SQLITE_OK, player_returns_stats_t_create( db, expected ) );

     player_returns_stats_s *actual = get_a_player_returns_stats( expected->player_id );

     comparePlayerReturnsStats( expected, actual );

     sqlite3_exec( db, "delete from player_returns_stats_t", NULL, NULL, NULL );

     free( expected );

     return NULL;
}

static char *player_returns_stats_t_create__ShouldGiveAnErrorIfRecordForSamePlayerSeasonAndBowlExists()
{
     player_returns_stats_s *expected1 = buildPlayerReturnsStats( 123, 5, bg_None );
     player_returns_stats_s *expected2 = buildPlayerReturnsStats( 123, 5, bg_None );

     assertEquals( SQLITE_OK,         player_returns_stats_t_create( db, expected1 ) );
     assertEquals( SQLITE_CONSTRAINT, player_returns_stats_t_create( db, expected2 ) );

     sqlite3_exec( db, "delete from player_returns_stats_t", NULL, NULL, NULL );

     free( expected1 );
     free( expected2 );

     return NULL;
}

static char *player_returns_stats_t_read__ShouldRetrieveMatchingRecord_GivenThePlayerIdSeasonAndBowl()
{
     player_returns_stats_s *expected = buildPlayerReturnsStats( 123, 5, bg_None );

     insert_a_player_returns_stats( expected );

     player_returns_stats_s actual = { 0 };

     actual.player_id = expected->player_id;
     actual.season    = expected->season;
     actual.bowl_game = expected->bowl_game;

     assertEquals( SQLITE_OK, player_returns_stats_t_read( db, &actual ) );

     comparePlayerReturnsStats( expected, &actual );

     sqlite3_exec( db, "delete from player_returns_stats_t", NULL, NULL, NULL );

     free( expected );

     return NULL;
}

static char *player_returns_stats_t_read_by_player__ShouldRetrieveAllMatchingRecords_GivenThePlayerId()
{
     player_returns_stats_s *expected1 = buildPlayerReturnsStats( 123, 5, bg_None );
     player_returns_stats_s *expected2 = buildPlayerReturnsStats( 123, 8, bg_None );

     insert_a_player_returns_stats( expected1 );
     insert_a_player_returns_stats( expected2 );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, player_returns_stats_t_read_by_player( db, expected1->player_id, &list ) );

     assertEquals( 2, list.count );

     player_returns_stats_s *actual = list.data;

     assertNotNull( actual );

     comparePlayerReturnsStats( expected1, &actual[0] );
     comparePlayerReturnsStats( expected2, &actual[1] );

     free( expected1 );
     free( expected2 );
     free( actual );

     sqlite3_exec( db, "delete from player_returns_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *player_returns_stats_t_update__ShouldModifyMatchingRecord_GivenThePlayerIdSeasonAndBowl()
{
     player_returns_stats_s *expected1 = buildPlayerReturnsStats( 123, 5, bg_None );
     player_returns_stats_s *expected2 = buildPlayerReturnsStats( 123, 5, bg_None );

     insert_a_player_returns_stats( expected1 );

     assertEquals( SQLITE_OK, player_returns_stats_t_update( db, expected2 ) );

     player_returns_stats_s *actual = get_a_player_returns_stats( expected1->player_id );

     comparePlayerReturnsStats( expected2, actual );

     sqlite3_exec( db, "delete from player_returns_stats_t", NULL, NULL, NULL );

     free( expected1 );
     free( expected2 );

     return NULL;
}

static char *player_returns_stats_t_delete__ShouldDeleteMatchingRecord_GivenThePlayerIdSeasonAndBowl()
{
     player_returns_stats_s *expected = buildPlayerReturnsStats( 123, 5, bg_None );

     insert_a_player_returns_stats( expected );

     assertEquals( SQLITE_OK, player_returns_stats_t_delete( db, expected ) );

     assertNull( get_a_player_returns_stats( expected->player_id ) );

     sqlite3_exec( db, "delete from player_returns_stats_t", NULL, NULL, NULL );

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
     run_test( player_returns_stats_t_create__ShouldInsertRecordsInThePlayerReturnsStatsTTable,          check_sqlite_error );
     run_test( player_returns_stats_t_create__ShouldGiveAnErrorIfRecordForSamePlayerSeasonAndBowlExists, check_sqlite_error );
     run_test( player_returns_stats_t_read__ShouldRetrieveMatchingRecord_GivenThePlayerIdSeasonAndBowl,  check_sqlite_error );
     run_test( player_returns_stats_t_read_by_player__ShouldRetrieveAllMatchingRecords_GivenThePlayerId, check_sqlite_error );
     run_test( player_returns_stats_t_update__ShouldModifyMatchingRecord_GivenThePlayerIdSeasonAndBowl,  check_sqlite_error );
     run_test( player_returns_stats_t_delete__ShouldDeleteMatchingRecord_GivenThePlayerIdSeasonAndBowl,  check_sqlite_error );
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
