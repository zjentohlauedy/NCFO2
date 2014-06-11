#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "builders.h"
#include "compares.h"
#include "conference.h"
#include "unit_test.h"


static conference_team_s     conference_team_sentinel     = CONFERENCE_TEAM_SENTINEL;
static conference_stats_s    conference_stats_sentinel    = CONFERENCE_STATS_SENTINEL;
static conference_accolade_s conference_accolade_sentinel = CONFERENCE_ACCOLADE_SENTINEL;

static sqlite3 *db;
static char    *result;


static char *get_conference__ShouldReturnTheMatchingConferenceObject_GivenAConferenceId()
{
     conference_s *expected = buildConference( 1 );

     assertEquals( SQLITE_OK, conferences_t_create( db, expected ) );

     conference_s *actual = get_conference( db, expected->conference_id );

     compareConferences( expected, actual );

     assertEquals( SQLITE_OK, conferences_t_delete( db, expected ) );

     free_conference( actual );

     return NULL;
}

static char *get_conference__ShouldReturnTheMatchingConferenceWithTeams_GivenAConferenceId()
{
     conference_s      *expected                  = buildConference( 1 );
     conference_team_s *expected_conference_team1 = buildConferenceTeam( 1, 2 );
     conference_team_s *expected_conference_team2 = buildConferenceTeam( 1, 5 );

     assertEquals( SQLITE_OK, conferences_t_create( db, expected ) );

     assertEquals( SQLITE_OK, conference_teams_t_create( db, expected_conference_team1 ) );
     assertEquals( SQLITE_OK, conference_teams_t_create( db, expected_conference_team2 ) );

     conference_s *actual = get_conference( db, expected->conference_id );

     conference_team_s *actual_conference_teams = actual->teams;

     assertNotNull( actual_conference_teams );

     compareConferenceTeams(  expected_conference_team1, &actual_conference_teams[0] );
     compareConferenceTeams(  expected_conference_team2, &actual_conference_teams[1] );
     compareConferenceTeams( &conference_team_sentinel,  &actual_conference_teams[2] );

     assertEquals( SQLITE_OK, conferences_t_delete(      db, expected                  ) );
     assertEquals( SQLITE_OK, conference_teams_t_delete( db, expected_conference_team1 ) );
     assertEquals( SQLITE_OK, conference_teams_t_delete( db, expected_conference_team2 ) );

     free_conference( actual );

     free( expected_conference_team1 );
     free( expected_conference_team2 );

     return NULL;
}

static char *save_conference__ShouldPersistTheConferenceStatsInTheDatabase_GivenAConferenceWithConferenceStats()
{
     conference_s       expected          = { 0 };
     conference_stats_s expected_stats[3] = { 0 };

     buildIntoConference( &expected, 1 );

     buildIntoConferenceStats( &expected_stats[0], 1, 1, bg_None );
     buildIntoConferenceStats( &expected_stats[1], 1, 2, bg_None );

     expected_stats[2] = conference_stats_sentinel;

     expected.stats = expected_stats;

     assertEquals( SQLITE_OK, save_conference( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, conference_stats_t_read_by_conference( db, expected.conference_id, &list ) );

     assertEquals( 2, list.count );

     conference_stats_s *actual_conference_stats = list.data;

     assertNotNull( actual_conference_stats );

     compareConferenceStats( &expected_stats[0], &actual_conference_stats[0] );
     compareConferenceStats( &expected_stats[1], &actual_conference_stats[1] );

     assertEquals( SQLITE_OK, conferences_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, conference_stats_t_delete( db, &expected_stats[0] ) );
     assertEquals( SQLITE_OK, conference_stats_t_delete( db, &expected_stats[1] ) );

     free( actual_conference_stats );

     return NULL;
}

static char *save_conference__ShouldPersistTheAccoladesInTheDatabase_GivenAConferenceWithAccolades()
{
     conference_s          expected              = { 0 };
     conference_accolade_s expected_accolades[3] = { 0 };

     buildIntoConference( &expected, 1 );

     buildIntoConferenceAccolade( &expected_accolades[0], 1, 1, cacc_OrangeBowlChampions  );
     buildIntoConferenceAccolade( &expected_accolades[1], 1, 2, cacc_LibertyBowlChampions );

     expected_accolades[2] = conference_accolade_sentinel;

     expected.accolades = expected_accolades;

     assertEquals( SQLITE_OK, save_conference( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, conference_accolades_t_read_by_conference( db, expected.conference_id, &list ) );

     assertEquals( 2, list.count );

     conference_accolade_s *actual_conference_accolades = list.data;

     assertNotNull( actual_conference_accolades );

     compareConferenceAccolades( &expected_accolades[0], &actual_conference_accolades[0] );
     compareConferenceAccolades( &expected_accolades[1], &actual_conference_accolades[1] );

     assertEquals( SQLITE_OK, conferences_t_delete( db, &expected ) );
     assertEquals( SQLITE_OK, conference_accolades_t_delete( db, &expected_accolades[0] ) );
     assertEquals( SQLITE_OK, conference_accolades_t_delete( db, &expected_accolades[1] ) );

     free( actual_conference_accolades );

     return NULL;
}

static char *save_conference__ShouldUpdateRecordsIfTheyExistAndInsertIfTheyDont_GivenAConference()
{
     conference_s          expected              = { 0 };
     conference_stats_s    expected_stats[3]     = { 0 };
     conference_accolade_s expected_accolades[3] = { 0 };

     buildIntoConference( &expected, 1 );

     buildIntoConferenceStats( &expected_stats[0], 1, 1, bg_None );
     buildIntoConferenceStats( &expected_stats[1], 1, 2, bg_None );
     expected_stats[2] = conference_stats_sentinel;

     buildIntoConferenceAccolade( &expected_accolades[0], 1, 1, cacc_OrangeBowlChampions  );
     buildIntoConferenceAccolade( &expected_accolades[1], 1, 2, cacc_LibertyBowlChampions );
     expected_accolades[2] = conference_accolade_sentinel;

     assertEquals( SQLITE_OK, conferences_t_create(          db, &expected              ) );
     assertEquals( SQLITE_OK, conference_stats_t_create(     db, &expected_stats[0]     ) );
     assertEquals( SQLITE_OK, conference_accolades_t_create( db, &expected_accolades[0] ) );

     buildIntoConferenceStats( &expected_stats[0], 1, 1, bg_None );

     expected.stats     = expected_stats;
     expected.accolades = expected_accolades;

     assertEquals( SQLITE_OK, save_conference( db, &expected ) );

     data_list_s conference_stats_list = { 0 };

     assertEquals( SQLITE_OK, conference_stats_t_read_by_conference( db, expected.conference_id, &conference_stats_list ) );

     assertEquals( 2, conference_stats_list.count );

     conference_stats_s *actual_conference_stats = conference_stats_list.data;

     assertNotNull( actual_conference_stats );

     compareConferenceStats( &expected_stats[0], &actual_conference_stats[0] );
     compareConferenceStats( &expected_stats[1], &actual_conference_stats[1] );

     data_list_s conference_accolades_list = { 0 };

     assertEquals( SQLITE_OK, conference_accolades_t_read_by_conference( db, expected.conference_id, &conference_accolades_list ) );

     assertEquals( 2, conference_accolades_list.count );

     conference_accolade_s *actual_conference_accolades = conference_accolades_list.data;

     assertNotNull( actual_conference_accolades );

     compareConferenceAccolades( &expected_accolades[0], &actual_conference_accolades[0] );
     compareConferenceAccolades( &expected_accolades[1], &actual_conference_accolades[1] );

     assertEquals( SQLITE_OK, conferences_t_delete(          db, &expected              ) );
     assertEquals( SQLITE_OK, conference_stats_t_delete(     db, &expected_stats[0]     ) );
     assertEquals( SQLITE_OK, conference_stats_t_delete(     db, &expected_stats[1]     ) );
     assertEquals( SQLITE_OK, conference_accolades_t_delete( db, &expected_accolades[0] ) );
     assertEquals( SQLITE_OK, conference_accolades_t_delete( db, &expected_accolades[1] ) );

     free( actual_conference_stats     );
     free( actual_conference_accolades );

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
     // get_conference()
     run_test( get_conference__ShouldReturnTheMatchingConferenceObject_GivenAConferenceId,    check_sqlite_error );
     run_test( get_conference__ShouldReturnTheMatchingConferenceWithTeams_GivenAConferenceId, check_sqlite_error );

     // save_conference()
     run_test( save_conference__ShouldPersistTheConferenceStatsInTheDatabase_GivenAConferenceWithConferenceStats, check_sqlite_error );
     run_test( save_conference__ShouldPersistTheAccoladesInTheDatabase_GivenAConferenceWithAccolades,             check_sqlite_error );
     run_test( save_conference__ShouldUpdateRecordsIfTheyExistAndInsertIfTheyDont_GivenAConference,               check_sqlite_error );
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


     run_all_tests();

     show_test_results();


     sqlite3_exec( db, "rollback", NULL, NULL, NULL );
     sqlite3_close( db );

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
