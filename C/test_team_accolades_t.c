#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "builders.h"
#include "compares.h"
#include "team.h"
#include "unit_test.h"


static sqlite3 *db;


static int load_team_accolade_data( void *output, int cols, char *data[], char *names[] )
{
     static team_accolade_s team_accolade;

     team_accolade_s **ta = (team_accolade_s **)output;

     if ( cols < 3 ) return SQLITE_ERROR;

     memset( &team_accolade, '\0', sizeof(team_accolade_s) );

     team_accolade.team_id  = atoi( data[0] );
     team_accolade.season   = atoi( data[1] );
     team_accolade.accolade = atoi( data[2] );

     *ta = &team_accolade;

     return SQLITE_OK;
}

static team_accolade_s *get_a_team_accolade( int team_id )
{
     team_accolade_s *team_accolade = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from team_accolades_t where team_id = %d", team_id );

     sqlite3_exec( db, query, load_team_accolade_data, &team_accolade, NULL );

     return team_accolade;
}

static void insert_a_team_accolade( team_accolade_s *team_accolade )
{
     char query[999+1];

     snprintf( query, sizeof(query), "insert into team_accolades_t (team_id, season, accolade) values (%d, %d, %d)",
               team_accolade->team_id,
               team_accolade->season,
               team_accolade->accolade );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}

static char *team_accolades_t_create__ShouldInsertRecordsInTheTeamAccoladesTTable()
{
     team_accolade_s expected = { 0 };

     buildIntoTeamAccolade( &expected, 123, 5, tacc_LibertyBowlChampions );

     assertEquals( SQLITE_OK, team_accolades_t_create( db, &expected ) );

     team_accolade_s *actual = get_a_team_accolade( expected.team_id );

     compareTeamAccolades( &expected, actual );

     sqlite3_exec( db, "delete from team_accolades_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_accolades_t_create__ShouldGiveAnErrorIfDuplicateRecordIsInserted()
{
     team_accolade_s expected = { 0 };

     buildIntoTeamAccolade( &expected, 123, 5, tacc_LibertyBowlChampions );

     assertEquals( SQLITE_OK,         team_accolades_t_create( db, &expected ) );
     assertEquals( SQLITE_CONSTRAINT, team_accolades_t_create( db, &expected ) );

     sqlite3_exec( db, "delete from team_accolades_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_accolades_t_read_by_team__ShouldRetrieveMatchingRecord_GivenTheTeamId()
{
     team_accolade_s expected = { 0 };

     buildIntoTeamAccolade( &expected, 123, 5, tacc_LibertyBowlChampions );

     insert_a_team_accolade( &expected );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, team_accolades_t_read_by_team( db, expected.team_id, &list ) );

     assertEquals( 1, list.count );

     team_accolade_s *actual = list.data;

     compareTeamAccolades( &expected, actual );

     free( actual );

     sqlite3_exec( db, "delete from team_accolades_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_accolades_t_read_by_team__ShouldRetrieveAllMatchingRecords_GivenTheTeamId()
{
     team_accolade_s expected1 = { 0 };
     team_accolade_s expected2 = { 0 };

     buildIntoTeamAccolade( &expected1, 123, 5, tacc_LibertyBowlChampions );
     buildIntoTeamAccolade( &expected2, 123, 6, tacc_NCFOChampions        );

     insert_a_team_accolade( &expected1 );
     insert_a_team_accolade( &expected2 );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, team_accolades_t_read_by_team( db, expected1.team_id, &list ) );

     assertEquals( 2, list.count );

     team_accolade_s *actual = list.data;

     compareTeamAccolades( &expected1, &actual[0] );
     compareTeamAccolades( &expected2, &actual[1] );

     free( actual );

     sqlite3_exec( db, "delete from team_accolades_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_accolades_t_delete__ShouldDeleteMatchingRecord_GivenATeamAccolade()
{
     team_accolade_s expected = { 0 };

     buildIntoTeamAccolade( &expected, 123, 5, tacc_LibertyBowlChampions );

     insert_a_team_accolade( &expected );

     assertEquals( SQLITE_OK, team_accolades_t_delete( db, &expected ) );

     assertNull( get_a_team_accolade( expected.team_id ) );

     sqlite3_exec( db, "delete from team_accolades_t", NULL, NULL, NULL );

     return NULL;
}

static char *team_accolades_t_delete__ShouldOnlyDeleteMatchingRecord_GivenATeamAccolade()
{
     team_accolade_s expected1 = { 0 };
     team_accolade_s expected2 = { 0 };

     buildIntoTeamAccolade( &expected1, 123, 5, tacc_LibertyBowlChampions );
     buildIntoTeamAccolade( &expected2, 123, 5, tacc_OrangeBowlChampions  );

     insert_a_team_accolade( &expected1 );
     insert_a_team_accolade( &expected2 );

     assertEquals( SQLITE_OK, team_accolades_t_delete( db, &expected2 ) );

     team_accolade_s *actual = get_a_team_accolade( expected1.team_id );

     compareTeamAccolades( &expected1, actual );

     sqlite3_exec( db, "delete from team_accolades_t", NULL, NULL, NULL );

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
     run_test( team_accolades_t_create__ShouldInsertRecordsInTheTeamAccoladesTTable,           check_sqlite_error );
     run_test( team_accolades_t_create__ShouldGiveAnErrorIfDuplicateRecordIsInserted,          check_sqlite_error );
     run_test( team_accolades_t_read_by_team__ShouldRetrieveMatchingRecord_GivenTheTeamId,     check_sqlite_error );
     run_test( team_accolades_t_read_by_team__ShouldRetrieveAllMatchingRecords_GivenTheTeamId, check_sqlite_error );
     run_test( team_accolades_t_delete__ShouldDeleteMatchingRecord_GivenATeamAccolade,         check_sqlite_error );
     run_test( team_accolades_t_delete__ShouldOnlyDeleteMatchingRecord_GivenATeamAccolade,     check_sqlite_error );
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
