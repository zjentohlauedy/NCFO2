#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "builders.h"
#include "compares.h"
#include "player.h"
#include "unit_test.h"


static sqlite3 *db;


static int load_player_accolade_data( void *output, int cols, char *data[], char *names[] )
{
     static player_accolade_s player_accolade;

     player_accolade_s **ta = (player_accolade_s **)output;

     if ( cols < 3 ) return SQLITE_ERROR;

     memset( &player_accolade, '\0', sizeof(player_accolade_s) );

     player_accolade.player_id = atoi( data[0] );
     player_accolade.season    = atoi( data[1] );
     player_accolade.accolade  = atoi( data[2] );

     *ta = &player_accolade;

     return SQLITE_OK;
}

static player_accolade_s *get_a_player_accolade( int player_id )
{
     player_accolade_s *player_accolade = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from player_accolades_t where player_id = %d", player_id );

     sqlite3_exec( db, query, load_player_accolade_data, &player_accolade, NULL );

     return player_accolade;
}

static void insert_a_player_accolade( player_accolade_s *player_accolade )
{
     char query[999+1];

     snprintf( query, sizeof(query), "insert into player_accolades_t (player_id, season, accolade) values (%d, %d, %d)",
               player_accolade->player_id,
               player_accolade->season,
               player_accolade->accolade );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}

static char *player_accolades_t_create__ShouldInsertRecordsInThePlayerAccoladesTTable()
{
     player_accolade_s expected = { 0 };

     buildIntoPlayerAccolade( &expected, 123, 5, tacc_LibertyBowlChampions );

     assertEquals( SQLITE_OK, player_accolades_t_create( db, &expected ) );

     player_accolade_s *actual = get_a_player_accolade( expected.player_id );

     comparePlayerAccolades( &expected, actual );

     sqlite3_exec( db, "delete from player_accolades_t", NULL, NULL, NULL );

     return NULL;
}

static char *player_accolades_t_create__ShouldGiveAnErrorIfDuplicateRecordIsInserted()
{
     player_accolade_s expected = { 0 };

     buildIntoPlayerAccolade( &expected, 123, 5, tacc_LibertyBowlChampions );

     assertEquals( SQLITE_OK,         player_accolades_t_create( db, &expected ) );
     assertEquals( SQLITE_CONSTRAINT, player_accolades_t_create( db, &expected ) );

     sqlite3_exec( db, "delete from player_accolades_t", NULL, NULL, NULL );

     return NULL;
}

static char *player_accolades_t_read_by_player__ShouldRetrieveMatchingRecord_GivenThePlayerId()
{
     player_accolade_s expected = { 0 };

     buildIntoPlayerAccolade( &expected, 123, 5, tacc_LibertyBowlChampions );

     insert_a_player_accolade( &expected );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, player_accolades_t_read_by_player( db, expected.player_id, &list ) );

     assertEquals( 1, list.count );

     player_accolade_s *actual = list.data;

     comparePlayerAccolades( &expected, actual );

     free( actual );

     sqlite3_exec( db, "delete from player_accolades_t", NULL, NULL, NULL );

     return NULL;
}

static char *player_accolades_t_read_by_player__ShouldRetrieveAllMatchingRecords_GivenThePlayerId()
{
     player_accolade_s expected1 = { 0 };
     player_accolade_s expected2 = { 0 };

     buildIntoPlayerAccolade( &expected1, 123, 5, tacc_LibertyBowlChampions );
     buildIntoPlayerAccolade( &expected2, 123, 6, tacc_NCFOChampions        );

     insert_a_player_accolade( &expected1 );
     insert_a_player_accolade( &expected2 );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, player_accolades_t_read_by_player( db, expected1.player_id, &list ) );

     assertEquals( 2, list.count );

     player_accolade_s *actual = list.data;

     comparePlayerAccolades( &expected1, &actual[0] );
     comparePlayerAccolades( &expected2, &actual[1] );

     free( actual );

     sqlite3_exec( db, "delete from player_accolades_t", NULL, NULL, NULL );

     return NULL;
}

static char *player_accolades_t_delete__ShouldDeleteMatchingRecord_GivenAPlayerAccolade()
{
     player_accolade_s expected = { 0 };

     buildIntoPlayerAccolade( &expected, 123, 5, tacc_LibertyBowlChampions );

     insert_a_player_accolade( &expected );

     assertEquals( SQLITE_OK, player_accolades_t_delete( db, &expected ) );

     assertNull( get_a_player_accolade( expected.player_id ) );

     sqlite3_exec( db, "delete from player_accolades_t", NULL, NULL, NULL );

     return NULL;
}

static char *player_accolades_t_delete__ShouldOnlyDeleteMatchingRecord_GivenAPlayerAccolade()
{
     player_accolade_s expected1 = { 0 };
     player_accolade_s expected2 = { 0 };

     buildIntoPlayerAccolade( &expected1, 123, 5, tacc_LibertyBowlChampions );
     buildIntoPlayerAccolade( &expected2, 123, 5, tacc_OrangeBowlChampions  );

     insert_a_player_accolade( &expected1 );
     insert_a_player_accolade( &expected2 );

     assertEquals( SQLITE_OK, player_accolades_t_delete( db, &expected2 ) );

     player_accolade_s *actual = get_a_player_accolade( expected1.player_id );

     comparePlayerAccolades( &expected1, actual );

     sqlite3_exec( db, "delete from player_accolades_t", NULL, NULL, NULL );

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
     run_test( player_accolades_t_create__ShouldInsertRecordsInThePlayerAccoladesTTable,             check_sqlite_error );
     run_test( player_accolades_t_create__ShouldGiveAnErrorIfDuplicateRecordIsInserted,              check_sqlite_error );
     run_test( player_accolades_t_read_by_player__ShouldRetrieveMatchingRecord_GivenThePlayerId,     check_sqlite_error );
     run_test( player_accolades_t_read_by_player__ShouldRetrieveAllMatchingRecords_GivenThePlayerId, check_sqlite_error );
     run_test( player_accolades_t_delete__ShouldDeleteMatchingRecord_GivenAPlayerAccolade,           check_sqlite_error );
     run_test( player_accolades_t_delete__ShouldOnlyDeleteMatchingRecord_GivenAPlayerAccolade,       check_sqlite_error );
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
