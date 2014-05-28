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


static int load_player_data( void *output, int cols, char *data[], char *names[] )
{
     static player_s player;

     player_s **p = (player_s **)output;

     if ( cols < 8 ) return SQLITE_ERROR;

     memset( &player, '\0', sizeof(player_s) );

     /**/    player.player_id        = atoi( data[0] );
     strcpy( player.first_name,              data[1] );
     strcpy( player.last_name,               data[2] );
     /**/    player.face             = atoi( data[3] );
     /**/    player.position         = atoi( data[4] );
     /**/    player.number           = atoi( data[5] );
     /**/    player.freshman_season  = atoi( data[6] );
     /**/    player.maturity         = atoi( data[7] );

     *p = &player;

     return SQLITE_OK;
}

static player_s *get_a_player( int player_id )
{
     player_s *player = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from players_t where player_id = %d", player_id );

     sqlite3_exec( db, query, load_player_data, &player, NULL );

     return player;
}

static void insert_a_player( player_s *player )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into players_t (player_id, first_name, last_name, face, position, number, freshman_season, maturity)"
               "values (%d, '%s', '%s', %d, %d, %d, %d, %d)",
               player->player_id,
               player->first_name,
               player->last_name,
               player->face,
               player->position,
               player->number,
               player->freshman_season,
               player->maturity );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *players_t_create__ShouldInsertRecordsInThePlayersTTable()
{
     player_s expected = { 0 };

     buildIntoPlayer( &expected, 123 );

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );

     player_s *actual = get_a_player( expected.player_id );

     comparePlayers( &expected, actual );

     sqlite3_exec( db, "delete from players_t", NULL, NULL, NULL );

     return NULL;
}

static char *players_t_create__ShouldGiveAnErrorIfPlayerIdAlreadyExists()
{
     player_s expected = { 0 };

     buildIntoPlayer( &expected, 123 );

     assertEquals( SQLITE_OK, players_t_create( db, &expected ) );

     strcpy( expected.first_name,        "SomethingElse"   );
     strcpy( expected.last_name,         "AnotherName"     );
     /**/    expected.face             =  999;
     /**/    expected.position         =  241;
     /**/    expected.number           =   88;
     /**/    expected.freshman_season  =   6;
     /**/    expected.maturity         =   2;

     assertEquals( SQLITE_CONSTRAINT, players_t_create( db, &expected ) );

     sqlite3_exec( db, "delete from players_t", NULL, NULL, NULL );

     return NULL;
}

static char *players_t_read__ShouldRetrieveMatchingRecord_GivenThePlayerId()
{
     player_s expected = { 0 };

     buildIntoPlayer( &expected, 123 );

     insert_a_player( &expected );

     player_s actual = { 0 };

     actual.player_id = expected.player_id;

     assertEquals( SQLITE_OK, players_t_read( db, &actual ) );

     comparePlayers( &expected, &actual );

     sqlite3_exec( db, "delete from players_t", NULL, NULL, NULL );

     return NULL;
}

static char *players_t_update__ShouldModifyMatchingRecord_GivenThePlayerId()
{
     player_s expected = { 0 };

     buildIntoPlayer( &expected, 123 );

     insert_a_player( &expected );

     strcpy( expected.first_name,        "SomethingElse"   );
     strcpy( expected.last_name,         "AnotherName"     );
     /**/    expected.face             =  999;
     /**/    expected.position         =  241;
     /**/    expected.number           =   88;
     /**/    expected.freshman_season  =   6;
     /**/    expected.maturity         =   2;

     assertEquals( SQLITE_OK, players_t_update( db, &expected ) );

     player_s *actual = get_a_player( expected.player_id );

     comparePlayers( &expected, actual );

     sqlite3_exec( db, "delete from players_t", NULL, NULL, NULL );

     return NULL;
}

static char *players_t_delete__ShouldDeleteMatchingRecord_GivenThePlayerId()
{
     player_s expected = { 0 };

     buildIntoPlayer( &expected, 123 );

     insert_a_player( &expected );

     assertEquals( SQLITE_OK, players_t_delete( db, &expected ) );

     assertNull( get_a_player( expected.player_id ) );

     sqlite3_exec( db, "delete from players_t", NULL, NULL, NULL );

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
     run_test( players_t_create__ShouldInsertRecordsInThePlayersTTable,       check_sqlite_error );
     run_test( players_t_create__ShouldGiveAnErrorIfPlayerIdAlreadyExists,    check_sqlite_error );
     run_test( players_t_read__ShouldRetrieveMatchingRecord_GivenThePlayerId, check_sqlite_error );
     run_test( players_t_update__ShouldModifyMatchingRecord_GivenThePlayerId, check_sqlite_error );
     run_test( players_t_delete__ShouldDeleteMatchingRecord_GivenThePlayerId, check_sqlite_error );
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
