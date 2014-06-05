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


static int load_player_offense_ratings_data( void *output, int cols, char *data[], char *names[] )
{
     static player_offense_ratings_s player_offense_ratings;

     player_offense_ratings_s **por = (player_offense_ratings_s **)output;

     if ( cols < 3 ) return SQLITE_ERROR;

     memset( &player_offense_ratings, '\0', sizeof(player_offense_ratings_s) );

     player_offense_ratings.player_id    = atoi( data[0] );
     player_offense_ratings.ball_control = atoi( data[1] );
     player_offense_ratings.receiving    = atoi( data[2] );

     *por = &player_offense_ratings;

     return SQLITE_OK;
}

static player_offense_ratings_s *get_a_player_offense_ratings( int player_id )
{
     player_offense_ratings_s *player_offense_ratings = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from player_offense_ratings_t where player_id = %d", player_id );

     sqlite3_exec( db, query, load_player_offense_ratings_data, &player_offense_ratings, NULL );

     return player_offense_ratings;
}

static void insert_a_player_offense_ratings( player_offense_ratings_s *player_offense_ratings )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into player_offense_ratings_t (player_id, ball_control, receiving)"
               "values (%d, %d, %d)",
               player_offense_ratings->player_id,
               player_offense_ratings->ball_control,
               player_offense_ratings->receiving    );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *player_offense_ratings_t_create__ShouldInsertRecordsInThePlayerOffenseRatingsTTable()
{
     player_offense_ratings_s expected = { 0 };

     buildIntoPlayerOffenseRatings( &expected, 123 );

     assertEquals( SQLITE_OK, player_offense_ratings_t_create( db, &expected ) );

     player_offense_ratings_s *actual = get_a_player_offense_ratings( expected.player_id );

     comparePlayerOffenseRatings( &expected, actual );

     sqlite3_exec( db, "delete from player_offense_ratings_t", NULL, NULL, NULL );

     return NULL;
}

static char *player_offense_ratings_t_create__ShouldGiveAnErrorIfPlayerIdAlreadyExists()
{
     player_offense_ratings_s expected = { 0 };

     buildIntoPlayerOffenseRatings( &expected, 123 );

     assertEquals( SQLITE_OK, player_offense_ratings_t_create( db, &expected ) );

     expected.ball_control  =  6;
     expected.receiving     =  2;

     assertEquals( SQLITE_CONSTRAINT, player_offense_ratings_t_create( db, &expected ) );

     sqlite3_exec( db, "delete from player_offense_ratings_t", NULL, NULL, NULL );

     return NULL;
}

static char *player_offense_ratings_t_read__ShouldRetrieveMatchingRecord_GivenThePlayerId()
{
     player_offense_ratings_s expected = { 0 };

     buildIntoPlayerOffenseRatings( &expected, 123 );

     insert_a_player_offense_ratings( &expected );

     player_offense_ratings_s actual = { 0 };

     actual.player_id = expected.player_id;

     assertEquals( SQLITE_OK, player_offense_ratings_t_read( db, &actual ) );

     comparePlayerOffenseRatings( &expected, &actual );

     sqlite3_exec( db, "delete from player_offense_ratings_t", NULL, NULL, NULL );

     return NULL;
}

static char *player_offense_ratings_t_update__ShouldModifyMatchingRecord_GivenThePlayerId()
{
     player_offense_ratings_s expected = { 0 };

     buildIntoPlayerOffenseRatings( &expected, 123 );

     insert_a_player_offense_ratings( &expected );

     expected.ball_control  =  6;
     expected.receiving     =  2;

     assertEquals( SQLITE_OK, player_offense_ratings_t_update( db, &expected ) );

     player_offense_ratings_s *actual = get_a_player_offense_ratings( expected.player_id );

     comparePlayerOffenseRatings( &expected, actual );

     sqlite3_exec( db, "delete from player_offense_ratings_t", NULL, NULL, NULL );

     return NULL;
}

static char *player_offense_ratings_t_delete__ShouldDeleteMatchingRecord_GivenThePlayerId()
{
     player_offense_ratings_s expected = { 0 };

     buildIntoPlayerOffenseRatings( &expected, 123 );

     insert_a_player_offense_ratings( &expected );

     assertEquals( SQLITE_OK, player_offense_ratings_t_delete( db, &expected ) );

     assertNull( get_a_player_offense_ratings( expected.player_id ) );

     sqlite3_exec( db, "delete from player_offense_ratings_t", NULL, NULL, NULL );

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
     run_test( player_offense_ratings_t_create__ShouldInsertRecordsInThePlayerOffenseRatingsTTable, check_sqlite_error );
     run_test( player_offense_ratings_t_create__ShouldGiveAnErrorIfPlayerIdAlreadyExists,           check_sqlite_error );
     run_test( player_offense_ratings_t_read__ShouldRetrieveMatchingRecord_GivenThePlayerId,        check_sqlite_error );
     run_test( player_offense_ratings_t_update__ShouldModifyMatchingRecord_GivenThePlayerId,        check_sqlite_error );
     run_test( player_offense_ratings_t_delete__ShouldDeleteMatchingRecord_GivenThePlayerId,        check_sqlite_error );
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
