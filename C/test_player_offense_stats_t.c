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


static int load_player_offense_stats_data( void *output, int cols, char *data[], char *names[] )
{
     static player_offense_stats_s player_offense_stats;

     player_offense_stats_s **pos = (player_offense_stats_s **)output;

     if ( cols < 14 ) return SQLITE_ERROR;

     memset( &player_offense_stats, '\0', sizeof(player_offense_stats_s) );

     player_offense_stats.player_id            = atoi( data[ 0] );
     player_offense_stats.season               = atoi( data[ 1] );
     player_offense_stats.bowl_game            = atoi( data[ 2] );
     player_offense_stats.pass_attempts        = atoi( data[ 3] );
     player_offense_stats.completions          = atoi( data[ 4] );
     player_offense_stats.interceptions        = atoi( data[ 5] );
     player_offense_stats.pass_yards           = atoi( data[ 6] );
     player_offense_stats.pass_touchdowns      = atoi( data[ 7] );
     player_offense_stats.rush_attempts        = atoi( data[ 8] );
     player_offense_stats.rush_yards           = atoi( data[ 9] );
     player_offense_stats.rush_touchdowns      = atoi( data[10] );
     player_offense_stats.receptions           = atoi( data[11] );
     player_offense_stats.receiving_yards      = atoi( data[12] );
     player_offense_stats.receiving_touchdowns = atoi( data[13] );

     *pos = &player_offense_stats;

     return SQLITE_OK;
}

static player_offense_stats_s *get_a_player_offense_stats( int player_id )
{
     player_offense_stats_s *player_offense_stats = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from player_offense_stats_t where player_id = %d", player_id );

     sqlite3_exec( db, query, load_player_offense_stats_data, &player_offense_stats, NULL );

     return player_offense_stats;
}


static void insert_a_player_offense_stats( player_offense_stats_s *player_offense_stats )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into player_offense_stats_t (player_id, season, bowl_game, pass_attempts, completions, interceptions, pass_yards, pass_touchdowns, rush_attempts, rush_yards, rush_touchdowns, receptions, receiving_yards, receiving_touchdowns)"
               "values (%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)",
               player_offense_stats->player_id,
               player_offense_stats->season,
               player_offense_stats->bowl_game,
               player_offense_stats->pass_attempts,
               player_offense_stats->completions,
               player_offense_stats->interceptions,
               player_offense_stats->pass_yards,
               player_offense_stats->pass_touchdowns,
               player_offense_stats->rush_attempts,
               player_offense_stats->rush_yards,
               player_offense_stats->rush_touchdowns,
               player_offense_stats->receptions,
               player_offense_stats->receiving_yards,
               player_offense_stats->receiving_touchdowns );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *player_offense_stats_t_create__ShouldInsertRecordsInThePlayerOffenseStatsTTable()
{
     player_offense_stats_s *expected = buildPlayerOffenseStats( 123, 5, bg_None );

     assertEquals( SQLITE_OK, player_offense_stats_t_create( db, expected ) );

     player_offense_stats_s *actual = get_a_player_offense_stats( expected->player_id );

     comparePlayerOffenseStats( expected, actual );

     sqlite3_exec( db, "delete from player_offense_stats_t", NULL, NULL, NULL );

     free( expected );

     return NULL;
}

static char *player_offense_stats_t_create__ShouldGiveAnErrorIfRecordForSamePlayerSeasonAndBowlExists()
{
     player_offense_stats_s *expected1 = buildPlayerOffenseStats( 123, 5, bg_None );
     player_offense_stats_s *expected2 = buildPlayerOffenseStats( 123, 5, bg_None );

     assertEquals( SQLITE_OK,         player_offense_stats_t_create( db, expected1 ) );
     assertEquals( SQLITE_CONSTRAINT, player_offense_stats_t_create( db, expected2 ) );

     sqlite3_exec( db, "delete from player_offense_stats_t", NULL, NULL, NULL );

     free( expected1 );
     free( expected2 );

     return NULL;
}

static char *player_offense_stats_t_read__ShouldRetrieveMatchingRecord_GivenThePlayerIdSeasonAndBowl()
{
     player_offense_stats_s *expected = buildPlayerOffenseStats( 123, 5, bg_None );

     insert_a_player_offense_stats( expected );

     player_offense_stats_s actual = { 0 };

     actual.player_id = expected->player_id;
     actual.season    = expected->season;
     actual.bowl_game = expected->bowl_game;

     assertEquals( SQLITE_OK, player_offense_stats_t_read( db, &actual ) );

     comparePlayerOffenseStats( expected, &actual );

     sqlite3_exec( db, "delete from player_offense_stats_t", NULL, NULL, NULL );

     free( expected );

     return NULL;
}

static char *player_offense_stats_t_read_by_player__ShouldRetrieveAllMatchingRecords_GivenThePlayerId()
{
     player_offense_stats_s *expected1 = buildPlayerOffenseStats( 123, 5, bg_None );
     player_offense_stats_s *expected2 = buildPlayerOffenseStats( 123, 8, bg_None );

     insert_a_player_offense_stats( expected1 );
     insert_a_player_offense_stats( expected2 );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, player_offense_stats_t_read_by_player( db, expected1->player_id, &list ) );

     assertEquals( 2, list.count );

     player_offense_stats_s *actual = list.data;

     assertNotNull( actual );

     comparePlayerOffenseStats( expected1, &actual[0] );
     comparePlayerOffenseStats( expected2, &actual[1] );

     free( expected1 );
     free( expected2 );
     free( actual );

     sqlite3_exec( db, "delete from player_offense_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *player_offense_stats_t_update__ShouldModifyMatchingRecord_GivenThePlayerIdSeasonAndBowl()
{
     player_offense_stats_s *expected1 = buildPlayerOffenseStats( 123, 5, bg_None );
     player_offense_stats_s *expected2 = buildPlayerOffenseStats( 123, 5, bg_None );

     insert_a_player_offense_stats( expected1 );

     assertEquals( SQLITE_OK, player_offense_stats_t_update( db, expected2 ) );

     player_offense_stats_s *actual = get_a_player_offense_stats( expected1->player_id );

     comparePlayerOffenseStats( expected2, actual );

     sqlite3_exec( db, "delete from player_offense_stats_t", NULL, NULL, NULL );

     free( expected1 );
     free( expected2 );

     return NULL;
}

static char *player_offense_stats_t_delete__ShouldDeleteMatchingRecord_GivenThePlayerIdSeasonAndBowl()
{
     player_offense_stats_s *expected = buildPlayerOffenseStats( 123, 5, bg_None );

     insert_a_player_offense_stats( expected );

     assertEquals( SQLITE_OK, player_offense_stats_t_delete( db, expected ) );

     assertNull( get_a_player_offense_stats( expected->player_id ) );

     sqlite3_exec( db, "delete from player_offense_stats_t", NULL, NULL, NULL );

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
     run_test( player_offense_stats_t_create__ShouldInsertRecordsInThePlayerOffenseStatsTTable,          check_sqlite_error );
     run_test( player_offense_stats_t_create__ShouldGiveAnErrorIfRecordForSamePlayerSeasonAndBowlExists, check_sqlite_error );
     run_test( player_offense_stats_t_read__ShouldRetrieveMatchingRecord_GivenThePlayerIdSeasonAndBowl,  check_sqlite_error );
     run_test( player_offense_stats_t_read_by_player__ShouldRetrieveAllMatchingRecords_GivenThePlayerId, check_sqlite_error );
     run_test( player_offense_stats_t_update__ShouldModifyMatchingRecord_GivenThePlayerIdSeasonAndBowl,  check_sqlite_error );
     run_test( player_offense_stats_t_delete__ShouldDeleteMatchingRecord_GivenThePlayerIdSeasonAndBowl,  check_sqlite_error );
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
