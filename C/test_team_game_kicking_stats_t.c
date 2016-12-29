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


static int load_team_game_kicking_stats_data( void *output, int cols, char *data[], char *names[] )
{
     static team_game_kicking_stats_s team_game_kicking_stats;

     team_game_kicking_stats_s **tks = (team_game_kicking_stats_s **)output;

     if ( cols < 16 ) return SQLITE_ERROR;

     memset( &team_game_kicking_stats, '\0', sizeof(team_game_kicking_stats_s) );

     team_game_kicking_stats.team_id                = atoi( data[ 0] );
     team_game_kicking_stats.season                 = atoi( data[ 1] );
     team_game_kicking_stats.week                   = atoi( data[ 2] );
     team_game_kicking_stats.game                   = atoi( data[ 3] );
     team_game_kicking_stats.extra_point_attempts   = atoi( data[ 4] );
     team_game_kicking_stats.extra_points_made      = atoi( data[ 5] );
     team_game_kicking_stats.field_goal_attempts    = atoi( data[ 6] );
     team_game_kicking_stats.field_goals_made       = atoi( data[ 7] );
     team_game_kicking_stats.punts                  = atoi( data[ 8] );
     team_game_kicking_stats.punt_yards             = atoi( data[ 9] );
     team_game_kicking_stats.kick_returns           = atoi( data[10] );
     team_game_kicking_stats.kick_return_yards      = atoi( data[11] );
     team_game_kicking_stats.kick_return_touchdowns = atoi( data[12] );
     team_game_kicking_stats.punt_returns           = atoi( data[13] );
     team_game_kicking_stats.punt_return_yards      = atoi( data[14] );
     team_game_kicking_stats.punt_return_touchdowns = atoi( data[15] );

     *tks = &team_game_kicking_stats;

     return SQLITE_OK;
}

static team_game_kicking_stats_s *get_a_team_game_kicking_stats( int team_id )
{
     team_game_kicking_stats_s *team_game_kicking_stats = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from team_game_kicking_stats_t where team_id = %d", team_id );

     sqlite3_exec( db, query, load_team_game_kicking_stats_data, &team_game_kicking_stats, NULL );

     return team_game_kicking_stats;
}


static void insert_a_team_game_kicking_stats( team_game_kicking_stats_s *team_game_kicking_stats )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into team_game_kicking_stats_t (team_id, season, week, game, extra_point_attempts, extra_points_made, field_goal_attempts, field_goals_made, punts, punt_yards, kick_returns, kick_return_yards, kick_return_touchdowns, punt_returns, punt_return_yards, punt_return_touchdowns)"
               "values (%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)",
               team_game_kicking_stats->team_id,
               team_game_kicking_stats->season,
               team_game_kicking_stats->week,
               team_game_kicking_stats->game,
               team_game_kicking_stats->extra_point_attempts,
               team_game_kicking_stats->extra_points_made,
               team_game_kicking_stats->field_goal_attempts,
               team_game_kicking_stats->field_goals_made,
               team_game_kicking_stats->punts,
               team_game_kicking_stats->punt_yards,
               team_game_kicking_stats->kick_returns,
               team_game_kicking_stats->kick_return_yards,
               team_game_kicking_stats->kick_return_touchdowns,
               team_game_kicking_stats->punt_returns,
               team_game_kicking_stats->punt_return_yards,
               team_game_kicking_stats->punt_return_touchdowns );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *team_game_kicking_stats_t_create__ShouldInsertRecordsInTheTeamGameKickingStatsTTable()
{
     team_game_kicking_stats_s *expected = buildTeamGameKickingStats( 123, 5, 7, 12 );

     assertEquals( SQLITE_OK, team_game_kicking_stats_t_create( db, expected ) );

     team_game_kicking_stats_s *actual = get_a_team_game_kicking_stats( expected->team_id );

     compareTeamGameKickingStats( expected, actual );

     sqlite3_exec( db, "delete from team_game_kicking_stats_t", NULL, NULL, NULL );

     free( expected );

     return NULL;
}

static char *team_game_kicking_stats_t_create__ShouldGiveAnErrorIfRecordForSameTeamSeasonWeekAndGameExists()
{
     team_game_kicking_stats_s *expected1 = buildTeamGameKickingStats( 123, 5, 7, 12 );
     team_game_kicking_stats_s *expected2 = buildTeamGameKickingStats( 123, 5, 7, 12 );

     assertEquals( SQLITE_OK,         team_game_kicking_stats_t_create( db, expected1 ) );
     assertEquals( SQLITE_CONSTRAINT, team_game_kicking_stats_t_create( db, expected2 ) );

     sqlite3_exec( db, "delete from team_game_kicking_stats_t", NULL, NULL, NULL );

     free( expected1 );
     free( expected2 );

     return NULL;
}

static char *team_game_kicking_stats_t_read__ShouldRetrieveMatchingRecord_GivenTheTeamIdSeasonWeekAndGame()
{
     team_game_kicking_stats_s *expected = buildTeamGameKickingStats( 123, 5, 7, 12 );

     insert_a_team_game_kicking_stats( expected );

     team_game_kicking_stats_s actual = { 0 };

     actual.team_id = expected->team_id;
     actual.season  = expected->season;
     actual.week    = expected->week;
     actual.game    = expected->game;

     assertEquals( SQLITE_OK, team_game_kicking_stats_t_read( db, &actual ) );

     compareTeamGameKickingStats( expected, &actual );

     sqlite3_exec( db, "delete from team_game_kicking_stats_t", NULL, NULL, NULL );

     free( expected );

     return NULL;
}

static char *team_game_kicking_stats_t_read_by_team__ShouldRetrieveAllMatchingRecords_GivenTheTeamId()
{
     team_game_kicking_stats_s *expected1 = buildTeamGameKickingStats( 123, 5, 7, 12 );
     team_game_kicking_stats_s *expected2 = buildTeamGameKickingStats( 123, 8, 3, 18 );

     insert_a_team_game_kicking_stats( expected1 );
     insert_a_team_game_kicking_stats( expected2 );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, team_game_kicking_stats_t_read_by_team( db, expected1->team_id, &list ) );

     assertEquals( 2, list.count );

     team_game_kicking_stats_s *actual = list.data;

     assertNotNull( actual );

     compareTeamGameKickingStats( expected1, &actual[0] );
     compareTeamGameKickingStats( expected2, &actual[1] );

     free( expected1 );
     free( expected2 );
     free( actual );

     sqlite3_exec( db, "delete from team_game_kicking_stats_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_game_kicking_stats_t_update__ShouldModifyMatchingRecord_GivenTheTeamIdSeasonWeekAndGame()
{
     team_game_kicking_stats_s *expected1 = buildTeamGameKickingStats( 123, 5, 7, 12 );
     team_game_kicking_stats_s *expected2 = buildTeamGameKickingStats( 123, 5, 7, 12 );

     insert_a_team_game_kicking_stats( expected1 );

     assertEquals( SQLITE_OK, team_game_kicking_stats_t_update( db, expected2 ) );

     team_game_kicking_stats_s *actual = get_a_team_game_kicking_stats( expected1->team_id );

     compareTeamGameKickingStats( expected2, actual );

     sqlite3_exec( db, "delete from team_game_kicking_stats_t", NULL, NULL, NULL );

     free( expected1 );
     free( expected2 );

     return NULL;
}

static char *team_game_kicking_stats_t_delete__ShouldDeleteMatchingRecord_GivenTheTeamIdSeasonWeekAndGame()
{
     team_game_kicking_stats_s *expected = buildTeamGameKickingStats( 123, 5, 7, 12 );

     insert_a_team_game_kicking_stats( expected );

     assertEquals( SQLITE_OK, team_game_kicking_stats_t_delete( db, expected ) );

     assertNull( get_a_team_game_kicking_stats( expected->team_id ) );

     sqlite3_exec( db, "delete from team_game_kicking_stats_t", NULL, NULL, NULL );

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
     run_test( team_game_kicking_stats_t_create__ShouldInsertRecordsInTheTeamGameKickingStatsTTable,          check_sqlite_error );
     run_test( team_game_kicking_stats_t_create__ShouldGiveAnErrorIfRecordForSameTeamSeasonWeekAndGameExists, check_sqlite_error );
     run_test( team_game_kicking_stats_t_read__ShouldRetrieveMatchingRecord_GivenTheTeamIdSeasonWeekAndGame,  check_sqlite_error );
     run_test( team_game_kicking_stats_t_read_by_team__ShouldRetrieveAllMatchingRecords_GivenTheTeamId,       check_sqlite_error );
     run_test( team_game_kicking_stats_t_update__ShouldModifyMatchingRecord_GivenTheTeamIdSeasonWeekAndGame,  check_sqlite_error );
     run_test( team_game_kicking_stats_t_delete__ShouldDeleteMatchingRecord_GivenTheTeamIdSeasonWeekAndGame,  check_sqlite_error );
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
