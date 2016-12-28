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


static int load_team_game_offense_stats_data( void *output, int cols, char *data[], char *names[] )
{
     static team_game_offense_stats_s team_game_offense_stats;

     team_game_offense_stats_s **tos = (team_game_offense_stats_s **)output;

     if ( cols < 12 ) return SQLITE_ERROR;

     memset( &team_game_offense_stats, '\0', sizeof(team_game_offense_stats_s) );

     team_game_offense_stats.team_id         = atoi( data[ 0] );
     team_game_offense_stats.season          = atoi( data[ 1] );
     team_game_offense_stats.week            = atoi( data[ 2] );
     team_game_offense_stats.game            = atoi( data[ 3] );
     team_game_offense_stats.pass_attempts   = atoi( data[ 4] );
     team_game_offense_stats.completions     = atoi( data[ 5] );
     team_game_offense_stats.interceptions   = atoi( data[ 6] );
     team_game_offense_stats.pass_yards      = atoi( data[ 7] );
     team_game_offense_stats.pass_touchdowns = atoi( data[ 8] );
     team_game_offense_stats.rush_attempts   = atoi( data[ 9] );
     team_game_offense_stats.rush_yards      = atoi( data[10] );
     team_game_offense_stats.rush_touchdowns = atoi( data[11] );

     *tos = &team_game_offense_stats;

     return SQLITE_OK;
}

static team_game_offense_stats_s *get_a_team_game_offense_stats( int team_id )
{
     team_game_offense_stats_s *team_game_offense_stats = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from team_game_offense_stats_t where team_id = %d", team_id );

     sqlite3_exec( db, query, load_team_game_offense_stats_data, &team_game_offense_stats, NULL );

     return team_game_offense_stats;
}


static void insert_a_team_game_offense_stats( team_game_offense_stats_s *team_game_offense_stats )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into team_game_offense_stats_t (team_id, season, week, game, pass_attempts, completions, interceptions, pass_yards, pass_touchdowns, rush_attempts, rush_yards, rush_touchdowns)"
               "values (%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)",
               team_game_offense_stats->team_id,
               team_game_offense_stats->season,
               team_game_offense_stats->week,
               team_game_offense_stats->game,
               team_game_offense_stats->pass_attempts,
               team_game_offense_stats->completions,
               team_game_offense_stats->interceptions,
               team_game_offense_stats->pass_yards,
               team_game_offense_stats->pass_touchdowns,
               team_game_offense_stats->rush_attempts,
               team_game_offense_stats->rush_yards,
               team_game_offense_stats->rush_touchdowns );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *team_game_offense_stats_t_create__ShouldInsertRecordsInTheTeamGameOffenseStatsTTable()
{
     team_game_offense_stats_s *expected = buildTeamGameOffenseStats( 123, 5, 4, 6 );

     assertEquals( SQLITE_OK, team_game_offense_stats_t_create( db, expected ) );

     team_game_offense_stats_s *actual = get_a_team_game_offense_stats( expected->team_id );

     compareTeamGameOffenseStats( expected, actual );

     sqlite3_exec( db, "delete from team_game_offense_stats_t", NULL, NULL, NULL );

     free( expected );

     return NULL;
}

static char *team_game_offense_stats_t_create__ShouldGiveAnErrorIfRecordForSameTeamSeasonWeekAndGameExists()
{
     team_game_offense_stats_s *expected1 = buildTeamGameOffenseStats( 123, 5, 4, 6 );
     team_game_offense_stats_s *expected2 = buildTeamGameOffenseStats( 123, 5, 4, 6 );

     assertEquals( SQLITE_OK,         team_game_offense_stats_t_create( db, expected1 ) );
     assertEquals( SQLITE_CONSTRAINT, team_game_offense_stats_t_create( db, expected2 ) );

     sqlite3_exec( db, "delete from team_game_offense_stats_t", NULL, NULL, NULL );

     free( expected1 );
     free( expected2 );

     return NULL;
}

static char *team_game_offense_stats_t_read__ShouldRetrieveMatchingRecord_GivenTheTeamIdSeasonWeekAndGame()
{
     team_game_offense_stats_s *expected = buildTeamGameOffenseStats( 123, 5, 4, 6 );

     insert_a_team_game_offense_stats( expected );

     team_game_offense_stats_s actual = { 0 };

     actual.team_id   = expected->team_id;
     actual.season    = expected->season;
     actual.week      = expected->week;
     actual.game      = expected->game;

     assertEquals( SQLITE_OK, team_game_offense_stats_t_read( db, &actual ) );

     compareTeamGameOffenseStats( expected, &actual );

     sqlite3_exec( db, "delete from team_game_offense_stats_t", NULL, NULL, NULL );

     free( expected );

     return NULL;
}

static char *team_game_offense_stats_t_read_by_team__ShouldRetrieveAllMatchingRecords_GivenTheTeamId()
{
     team_game_offense_stats_s *expected1 = buildTeamGameOffenseStats( 123, 5, 4, 6 );
     team_game_offense_stats_s *expected2 = buildTeamGameOffenseStats( 123, 8, 3, 9 );

     insert_a_team_game_offense_stats( expected1 );
     insert_a_team_game_offense_stats( expected2 );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, team_game_offense_stats_t_read_by_team( db, expected1->team_id, &list ) );

     assertEquals( 2, list.count );

     team_game_offense_stats_s *actual = list.data;

     assertNotNull( actual );

     compareTeamGameOffenseStats( expected1, &actual[0] );
     compareTeamGameOffenseStats( expected2, &actual[1] );

     free( expected1 );
     free( expected2 );
     free( actual );

     sqlite3_exec( db, "delete from team_game_offense_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_game_offense_stats_t_update__ShouldModifyMatchingRecord_GivenTheTeamIdSeasonWeekAndGame()
{
     team_game_offense_stats_s *expected1 = buildTeamGameOffenseStats( 123, 5, 4, 6 );
     team_game_offense_stats_s *expected2 = buildTeamGameOffenseStats( 123, 5, 4, 6 );

     insert_a_team_game_offense_stats( expected1 );

     assertEquals( SQLITE_OK, team_game_offense_stats_t_update( db, expected2 ) );

     team_game_offense_stats_s *actual = get_a_team_game_offense_stats( expected1->team_id );

     compareTeamGameOffenseStats( expected2, actual );

     sqlite3_exec( db, "delete from team_game_offense_stats_t", NULL, NULL, NULL );

     free( expected1 );
     free( expected2 );

     return NULL;
}

static char *team_game_offense_stats_t_delete__ShouldDeleteMatchingRecord_GivenTheTeamIdSeasonWeekAndGame()
{
     team_game_offense_stats_s *expected = buildTeamGameOffenseStats( 123, 5, 4, 6 );

     insert_a_team_game_offense_stats( expected );

     assertEquals( SQLITE_OK, team_game_offense_stats_t_delete( db, expected ) );

     assertNull( get_a_team_game_offense_stats( expected->team_id ) );

     sqlite3_exec( db, "delete from team_game_offense_stats_t", NULL, NULL, NULL );

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
     run_test( team_game_offense_stats_t_create__ShouldInsertRecordsInTheTeamGameOffenseStatsTTable,          check_sqlite_error );
     run_test( team_game_offense_stats_t_create__ShouldGiveAnErrorIfRecordForSameTeamSeasonWeekAndGameExists, check_sqlite_error );
     run_test( team_game_offense_stats_t_read__ShouldRetrieveMatchingRecord_GivenTheTeamIdSeasonWeekAndGame,  check_sqlite_error );
     run_test( team_game_offense_stats_t_read_by_team__ShouldRetrieveAllMatchingRecords_GivenTheTeamId,       check_sqlite_error );
     run_test( team_game_offense_stats_t_update__ShouldModifyMatchingRecord_GivenTheTeamIdSeasonWeekAndGame,  check_sqlite_error );
     run_test( team_game_offense_stats_t_delete__ShouldDeleteMatchingRecord_GivenTheTeamIdSeasonWeekAndGame,  check_sqlite_error );
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
