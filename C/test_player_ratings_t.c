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


static int load_player_ratings_data( void *output, int cols, char *data[], char *names[] )
{
     static player_ratings_s player_ratings;

     player_ratings_s **pr = (player_ratings_s **)output;

     if ( cols < 5 ) return SQLITE_ERROR;

     memset( &player_ratings, '\0', sizeof(player_ratings_s) );

     player_ratings.player_id  = atoi( data[0] );
     player_ratings.run_speed  = atoi( data[1] );
     player_ratings.rush_power = atoi( data[2] );
     player_ratings.max_speed  = atoi( data[3] );
     player_ratings.hit_power  = atoi( data[4] );

     *pr = &player_ratings;

     return SQLITE_OK;
}

static player_ratings_s *get_a_player_ratings( int player_id )
{
     player_ratings_s *player_ratings = NULL;

     char query[999+1];

     snprintf( query, sizeof(query), "select * from player_ratings_t where player_id = %d", player_id );

     sqlite3_exec( db, query, load_player_ratings_data, &player_ratings, NULL );

     return player_ratings;
}

static void insert_a_player_ratings( player_ratings_s *player_ratings )
{
     char query[999+1];


     snprintf( query, sizeof(query), "insert into player_ratings_t (player_id, run_speed, rush_power, max_speed, hit_power)"
               "values (%d, %d, %d, %d, %d)",
               player_ratings->player_id,
               player_ratings->run_speed,
               player_ratings->rush_power,
               player_ratings->max_speed,
               player_ratings->hit_power );

     sqlite3_exec( db, query, NULL, NULL, NULL );
}


static char *player_ratings_t_create__ShouldInsertRecordsInThePlayerRatingsTTable()
{
     player_ratings_s expected = { 0 };

     buildIntoPlayerRatings( &expected, 123 );

     assertEquals( SQLITE_OK, player_ratings_t_create( db, &expected ) );

     player_ratings_s *actual = get_a_player_ratings( expected.player_id );

     comparePlayerRatings( &expected, actual );

     sqlite3_exec( db, "delete from player_ratings_t", NULL, NULL, NULL );

     return NULL;
}

static char *player_ratings_t_create__ShouldGiveAnErrorIfPlayerRatingsIdAlreadyExists()
{
     player_ratings_s expected = { 0 };

     buildIntoPlayerRatings( &expected, 123 );

     assertEquals( SQLITE_OK, player_ratings_t_create( db, &expected ) );

     expected.run_speed   =  1;
     expected.rush_power  =  8;
     expected.max_speed   =  6;
     expected.hit_power   =  2;

     assertEquals( SQLITE_CONSTRAINT, player_ratings_t_create( db, &expected ) );

     sqlite3_exec( db, "delete from player_ratings_t", NULL, NULL, NULL );

     return NULL;
}

static char *player_ratings_t_read__ShouldRetrieveMatchingRecord_GivenThePlayerRatingsId()
{
     player_ratings_s expected = { 0 };

     buildIntoPlayerRatings( &expected, 123 );

     insert_a_player_ratings( &expected );

     player_ratings_s actual = { 0 };

     actual.player_id = expected.player_id;

     assertEquals( SQLITE_OK, player_ratings_t_read( db, &actual ) );

     comparePlayerRatings( &expected, &actual );

     sqlite3_exec( db, "delete from player_ratings_t", NULL, NULL, NULL );

     return NULL;
}

static char *player_ratings_t_update__ShouldModifyMatchingRecord_GivenThePlayerRatingsId()
{
     player_ratings_s expected = { 0 };

     buildIntoPlayerRatings( &expected, 123 );

     insert_a_player_ratings( &expected );

     expected.run_speed   =  1;
     expected.rush_power  =  8;
     expected.max_speed   =  6;
     expected.hit_power   =  2;

     assertEquals( SQLITE_OK, player_ratings_t_update( db, &expected ) );

     player_ratings_s *actual = get_a_player_ratings( expected.player_id );

     comparePlayerRatings( &expected, actual );

     sqlite3_exec( db, "delete from player_ratings_t", NULL, NULL, NULL );

     return NULL;
}

static char *player_ratings_t_delete__ShouldDeleteMatchingRecord_GivenThePlayerRatingsId()
{
     player_ratings_s expected = { 0 };

     buildIntoPlayerRatings( &expected, 123 );

     insert_a_player_ratings( &expected );

     assertEquals( SQLITE_OK, player_ratings_t_delete( db, &expected ) );

     assertNull( get_a_player_ratings( expected.player_id ) );

     sqlite3_exec( db, "delete from player_ratings_t", NULL, NULL, NULL );

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
     run_test( player_ratings_t_create__ShouldInsertRecordsInThePlayerRatingsTTable,        check_sqlite_error );
     run_test( player_ratings_t_create__ShouldGiveAnErrorIfPlayerRatingsIdAlreadyExists,    check_sqlite_error );
     run_test( player_ratings_t_read__ShouldRetrieveMatchingRecord_GivenThePlayerRatingsId, check_sqlite_error );
     run_test( player_ratings_t_update__ShouldModifyMatchingRecord_GivenThePlayerRatingsId, check_sqlite_error );
     run_test( player_ratings_t_delete__ShouldDeleteMatchingRecord_GivenThePlayerRatingsId, check_sqlite_error );
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
