#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "builders.h"
#include "compares.h"
#include "conference.h"
#include "unit_test.h"


static sqlite3 *db;


static int load_conference_team_data( void *output, int cols, char *data[], char *names[] )
{
     static conference_team_s conference_team;

     conference_team_s **dt = (conference_team_s **)output;

     if ( cols < 2 ) return SQLITE_ERROR;

     memset( &conference_team, '\0', sizeof(conference_team_s) );

     conference_team.conference_id = atoi( data[0] );
     conference_team.team_id       = atoi( data[1] );

     *dt = &conference_team;

     return SQLITE_OK;
}

static conference_team_s *get_a_conference_team( int conference_id )
{
     conference_team_s *conference_team = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from conference_teams_t where conference_id = %d", conference_id );

     sqlite3_exec( db, query, load_conference_team_data, &conference_team, NULL );

     return conference_team;
}

static void insert_a_conference_team( conference_team_s *conference_team )
{
     char query[999+1];

     snprintf( query, sizeof(query), "insert into conference_teams_t (conference_id, team_id) values (%d, %d)",
               conference_team->conference_id,
               conference_team->team_id );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *conference_teams_t_create__ShouldInsertRecordsInTheConferenceTeamsTTable()
{
     conference_team_s expected = { 0 };

     buildIntoConferenceTeam( &expected, 8, 123 );

     assertEquals( SQLITE_OK, conference_teams_t_create( db, &expected) );

     conference_team_s *actual = get_a_conference_team( expected.conference_id );

     compareConferenceTeams( &expected, actual );

     sqlite3_exec( db, "delete from conference_teams_t", NULL, NULL, NULL );

     return NULL;
}

static char *conference_teams_t_create__ShouldGiveAnErrorIfDuplicateRecordIsInserted()
{
     conference_team_s expected = { 0 };

     buildIntoConferenceTeam( &expected, 8, 123 );

     assertEquals( SQLITE_OK,         conference_teams_t_create( db, &expected) );
     assertEquals( SQLITE_CONSTRAINT, conference_teams_t_create( db, &expected) );

     sqlite3_exec( db, "delete from conference_teams_t", NULL, NULL, NULL );

     return NULL;
}

static char *conference_teams_t_read_by_conference__ShouldRetrieveMatchingRecord_GivenTheConferenceId()
{
     conference_team_s expected = { 0 };

     buildIntoConferenceTeam( &expected, 8, 123 );

     insert_a_conference_team( &expected );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, conference_teams_t_read_by_conference( db, expected.conference_id, &list) );

     assertEquals( 1, list.count );

     conference_team_s *actual   = list.data;

     compareConferenceTeams( &expected, &actual[0] );

     free( actual );

     sqlite3_exec( db, "delete from conference_teams_t", NULL, NULL, NULL );

     return NULL;
}

static char *conference_teams_t_read_by_conference__ShouldRetrieveAllMatchingRecords_GivenTheConferenceId()
{
     conference_team_s expected1 = { 0 };
     conference_team_s expected2 = { 0 };

     buildIntoConferenceTeam( &expected1, 8, 123 );
     buildIntoConferenceTeam( &expected2, 8, 321 );

     insert_a_conference_team( &expected1 );
     insert_a_conference_team( &expected2 );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, conference_teams_t_read_by_conference( db, expected1.conference_id, &list) );

     assertEquals( 2, list.count );

     conference_team_s *actual   = list.data;

     compareConferenceTeams( &expected1, &actual[0] );
     compareConferenceTeams( &expected2, &actual[1] );

     free( actual );

     sqlite3_exec( db, "delete from conference_teams_t", NULL, NULL, NULL );

     return NULL;
}

static char *conference_teams_t_delete__ShouldDeleteMatchingRecord_GivenAConferenceTeam()
{
     conference_team_s expected = { 0 };

     buildIntoConferenceTeam( &expected, 8, 123 );

     insert_a_conference_team( &expected );

     assertEquals( SQLITE_OK, conference_teams_t_delete( db, &expected) );

     assertNull( get_a_conference_team( expected.conference_id ) );

     sqlite3_exec( db, "delete from conference_teams_t", NULL, NULL, NULL );

     return NULL;
}

static char *conference_teams_t_delete__ShouldOnlyDeleteMatchingRecord_GivenAConferenceTeam()
{
     conference_team_s expected1 = { 0 };
     conference_team_s expected2 = { 0 };

     buildIntoConferenceTeam( &expected1, 8, 123 );
     buildIntoConferenceTeam( &expected2, 8, 321 );

     insert_a_conference_team( &expected1 );
     insert_a_conference_team( &expected2 );

     assertEquals( SQLITE_OK, conference_teams_t_delete( db, &expected2) );

     conference_team_s *actual = get_a_conference_team( expected1.conference_id );

     compareConferenceTeams( &expected1, actual );

     sqlite3_exec( db, "delete from conference_teams_t", NULL, NULL, NULL );

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
     run_test( conference_teams_t_create__ShouldInsertRecordsInTheConferenceTeamsTTable,                     check_sqlite_error );
     run_test( conference_teams_t_create__ShouldGiveAnErrorIfDuplicateRecordIsInserted,                      check_sqlite_error );
     run_test( conference_teams_t_read_by_conference__ShouldRetrieveMatchingRecord_GivenTheConferenceId,     check_sqlite_error );
     run_test( conference_teams_t_read_by_conference__ShouldRetrieveAllMatchingRecords_GivenTheConferenceId, check_sqlite_error );
     run_test( conference_teams_t_delete__ShouldDeleteMatchingRecord_GivenAConferenceTeam,                   check_sqlite_error );
     run_test( conference_teams_t_delete__ShouldOnlyDeleteMatchingRecord_GivenAConferenceTeam,               check_sqlite_error );
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
