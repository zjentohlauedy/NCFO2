#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "builders.h"
#include "compares.h"
#include "player.h"
#include "unit_test.h"


static player_accolade_s      player_accolade_sentinel      = PLAYER_ACCOLADE_SENTINEL;
static player_offense_stats_s player_offense_stats_sentinel = PLAYER_OFFENSE_STATS_SENTINEL;
static player_defense_stats_s player_defense_stats_sentinel = PLAYER_DEFENSE_STATS_SENTINEL;
static player_kicking_stats_s player_kicking_stats_sentinel = PLAYER_KICKING_STATS_SENTINEL;
static player_returns_stats_s player_returns_stats_sentinel = PLAYER_RETURNS_STATS_SENTINEL;


static sqlite3 *db;
static char    *result;


static char *get_player__ShouldReturnTheMatchingPlayerObject_GivenAPlayerId()
{
     player_s *expected = buildPlayer( 1 );

     assertEquals( SQLITE_OK, players_t_create( db, expected ) );

     player_s *actual = get_player( db, expected->player_id );

     assertNotNull( actual );

     comparePlayers( expected, actual );

     assertEquals( SQLITE_OK, players_t_delete( db, expected ) );

     free_player( expected );
     free_player( actual   );

     return NULL;
}

static char *get_player__ShouldReturnTheMatchingPlayerWithRatings_GivenAPlayerId()
{
     player_s         *expected         = buildPlayer( 1 );
     player_ratings_s *expected_ratings = buildPlayerRatings( 1 );

     assertEquals( SQLITE_OK, players_t_create(        db, expected         ) );
     assertEquals( SQLITE_OK, player_ratings_t_create( db, expected_ratings ) );

     player_s *actual = get_player( db, expected->player_id );

     assertNotNull( actual );

     player_ratings_s *actual_ratings = actual->ratings;

     assertNotNull( actual_ratings );

     comparePlayerRatings(  expected_ratings, actual_ratings );

     assertEquals( SQLITE_OK, players_t_delete(        db, expected         ) );
     assertEquals( SQLITE_OK, player_ratings_t_delete( db, expected_ratings ) );

     free_player( expected );
     free_player( actual   );

     free( expected_ratings );

     return NULL;
}

static char *get_player__ShouldReturnTheMatchingPlayerWithQuarterbackRatings_GivenAPlayerId()
{
     player_s                     *expected                     = buildPlayer( 1 );
     player_quarterback_ratings_s *expected_quarterback_ratings = buildPlayerQuarterbackRatings( 1 );

     expected->position = pos_Quarterback;

     assertEquals( SQLITE_OK, players_t_create(                    db, expected                     ) );
     assertEquals( SQLITE_OK, player_quarterback_ratings_t_create( db, expected_quarterback_ratings ) );

     player_s *actual = get_player( db, expected->player_id );

     assertNotNull( actual );

     player_quarterback_ratings_s *actual_quarterback_ratings = actual->extra_ratings.quarterback;

     assertNotNull( actual_quarterback_ratings );

     comparePlayerQuarterbackRatings( expected_quarterback_ratings, actual_quarterback_ratings );

     assertEquals( SQLITE_OK, players_t_delete(                    db, expected                     ) );
     assertEquals( SQLITE_OK, player_quarterback_ratings_t_delete( db, expected_quarterback_ratings ) );

     free_player( expected );
     free_player( actual   );

     free( expected_quarterback_ratings );

     return NULL;
}

static char *get_player__ShouldReturnTheMatchingPlayerWithOffenseRatings_GivenAPlayerId()
{
     player_s                 *expected                 = buildPlayer( 1 );
     player_offense_ratings_s *expected_offense_ratings = buildPlayerOffenseRatings( 1 );

     expected->position = pos_Runningback;

     assertEquals( SQLITE_OK, players_t_create(                db, expected                 ) );
     assertEquals( SQLITE_OK, player_offense_ratings_t_create( db, expected_offense_ratings ) );

     player_s *actual = get_player( db, expected->player_id );

     assertNotNull( actual );

     player_offense_ratings_s *actual_offense_ratings = actual->extra_ratings.offense;

     assertNotNull( actual_offense_ratings );

     comparePlayerOffenseRatings( expected_offense_ratings, actual_offense_ratings );

     assertEquals( SQLITE_OK, players_t_delete(                db, expected                 ) );
     assertEquals( SQLITE_OK, player_offense_ratings_t_delete( db, expected_offense_ratings ) );

     free_player( expected );
     free_player( actual   );

     free( expected_offense_ratings );

     return NULL;
}

static char *get_player__ShouldReturnTheMatchingPlayerWithDefenseRatings_GivenAPlayerId()
{
     player_s                 *expected                 = buildPlayer( 1 );
     player_defense_ratings_s *expected_defense_ratings = buildPlayerDefenseRatings( 1 );

     expected->position = pos_Linebacker;

     assertEquals( SQLITE_OK, players_t_create(                db, expected                 ) );
     assertEquals( SQLITE_OK, player_defense_ratings_t_create( db, expected_defense_ratings ) );

     player_s *actual = get_player( db, expected->player_id );

     assertNotNull( actual );

     player_defense_ratings_s *actual_defense_ratings = actual->extra_ratings.defense;

     assertNotNull( actual_defense_ratings );

     comparePlayerDefenseRatings( expected_defense_ratings, actual_defense_ratings );

     assertEquals( SQLITE_OK, players_t_delete(                db, expected                 ) );
     assertEquals( SQLITE_OK, player_defense_ratings_t_delete( db, expected_defense_ratings ) );

     free_player( expected );
     free_player( actual   );

     free( expected_defense_ratings );

     return NULL;
}

static char *get_player__ShouldReturnTheMatchingPlayerWithKickingRatings_GivenAPlayerId()
{
     player_s                 *expected                 = buildPlayer( 1 );
     player_kicking_ratings_s *expected_kicking_ratings = buildPlayerKickingRatings( 1 );

     expected->position = pos_Kicker;

     assertEquals( SQLITE_OK, players_t_create(                db, expected                 ) );
     assertEquals( SQLITE_OK, player_kicking_ratings_t_create( db, expected_kicking_ratings ) );

     player_s *actual = get_player( db, expected->player_id );

     assertNotNull( actual );

     player_kicking_ratings_s *actual_kicking_ratings = actual->extra_ratings.kicking;

     assertNotNull( actual_kicking_ratings );

     comparePlayerKickingRatings( expected_kicking_ratings, actual_kicking_ratings );

     assertEquals( SQLITE_OK, players_t_delete(                db, expected                 ) );
     assertEquals( SQLITE_OK, player_kicking_ratings_t_delete( db, expected_kicking_ratings ) );

     free_player( expected );
     free_player( actual   );

     free( expected_kicking_ratings );

     return NULL;
}

static char *save_player__ShouldSavePlayerAccoladesToTheDatabase_GivenAPlayerWithAccolades()
{
     player_s          expected              = { 0 };
     player_accolade_s expected_accolades[3] = { 0 };

     buildIntoPlayer( &expected, 1 );

     buildIntoPlayerAccolade( &expected_accolades[0], 1, 1, pacc_All_American );
     buildIntoPlayerAccolade( &expected_accolades[1], 1, 2, pacc_All_American );

     expected_accolades[2] = player_accolade_sentinel;

     expected.accolades = expected_accolades;

     assertEquals( SQLITE_OK, save_player( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, player_accolades_t_read_by_player( db, expected.player_id, &list ) );

     assertEquals( 2, list.count );

     player_accolade_s *actual_accolades = list.data;

     assertNotNull( actual_accolades );

     comparePlayerAccolades( &expected_accolades[0], &actual_accolades[0] );
     comparePlayerAccolades( &expected_accolades[1], &actual_accolades[1] );

     assertEquals( SQLITE_OK, players_t_delete(          db, &expected              ) );
     assertEquals( SQLITE_OK, player_accolades_t_delete( db, &expected_accolades[0] ) );
     assertEquals( SQLITE_OK, player_accolades_t_delete( db, &expected_accolades[1] ) );

     free( actual_accolades );

     return NULL;
}

static char *save_player__ShouldSaveOffenseStatsToTheDatabase_GivenAPlayerWithOffenseStats()
{
     player_s               expected                  = { 0 };
     player_offense_stats_s expected_offense_stats[3] = { 0 };

     buildIntoPlayer( &expected, 1 );

     buildIntoPlayerOffenseStats( &expected_offense_stats[0], 1, 1, bg_None );
     buildIntoPlayerOffenseStats( &expected_offense_stats[1], 1, 2, bg_None );

     expected_offense_stats[2] = player_offense_stats_sentinel;

     expected.stats.offense = expected_offense_stats;

     assertEquals( SQLITE_OK, save_player( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, player_offense_stats_t_read_by_player( db, expected.player_id, &list ) );

     assertEquals( 2, list.count );

     player_offense_stats_s *actual_offense_stats = list.data;

     assertNotNull( actual_offense_stats );

     comparePlayerOffenseStats( &expected_offense_stats[0], &actual_offense_stats[0] );
     comparePlayerOffenseStats( &expected_offense_stats[1], &actual_offense_stats[1] );

     assertEquals( SQLITE_OK, players_t_delete(              db, &expected                  ) );
     assertEquals( SQLITE_OK, player_offense_stats_t_delete( db, &expected_offense_stats[0] ) );
     assertEquals( SQLITE_OK, player_offense_stats_t_delete( db, &expected_offense_stats[1] ) );

     free( actual_offense_stats );

     return NULL;
}

static char *save_player__ShouldSaveDefenseStatsToTheDatabase_GivenAPlayerWithDefenseStats()
{
     player_s               expected                  = { 0 };
     player_defense_stats_s expected_defense_stats[3] = { 0 };

     buildIntoPlayer( &expected, 1 );

     buildIntoPlayerDefenseStats( &expected_defense_stats[0], 1, 1, bg_None );
     buildIntoPlayerDefenseStats( &expected_defense_stats[1], 1, 2, bg_None );

     expected_defense_stats[2] = player_defense_stats_sentinel;

     expected.stats.defense = expected_defense_stats;

     assertEquals( SQLITE_OK, save_player( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, player_defense_stats_t_read_by_player( db, expected.player_id, &list ) );

     assertEquals( 2, list.count );

     player_defense_stats_s *actual_defense_stats = list.data;

     assertNotNull( actual_defense_stats );

     comparePlayerDefenseStats( &expected_defense_stats[0], &actual_defense_stats[0] );
     comparePlayerDefenseStats( &expected_defense_stats[1], &actual_defense_stats[1] );

     assertEquals( SQLITE_OK, players_t_delete(              db, &expected                  ) );
     assertEquals( SQLITE_OK, player_defense_stats_t_delete( db, &expected_defense_stats[0] ) );
     assertEquals( SQLITE_OK, player_defense_stats_t_delete( db, &expected_defense_stats[1] ) );

     free( actual_defense_stats );

     return NULL;
}

static char *save_player__ShouldSaveKickingStatsToTheDatabase_GivenAPlayerWithKickingStats()
{
     player_s               expected                  = { 0 };
     player_kicking_stats_s expected_kicking_stats[3] = { 0 };

     buildIntoPlayer( &expected, 1 );

     buildIntoPlayerKickingStats( &expected_kicking_stats[0], 1, 1, bg_None );
     buildIntoPlayerKickingStats( &expected_kicking_stats[1], 1, 2, bg_None );

     expected_kicking_stats[2] = player_kicking_stats_sentinel;

     expected.stats.kicking = expected_kicking_stats;

     assertEquals( SQLITE_OK, save_player( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, player_kicking_stats_t_read_by_player( db, expected.player_id, &list ) );

     assertEquals( 2, list.count );

     player_kicking_stats_s *actual_kicking_stats = list.data;

     assertNotNull( actual_kicking_stats );

     comparePlayerKickingStats( &expected_kicking_stats[0], &actual_kicking_stats[0] );
     comparePlayerKickingStats( &expected_kicking_stats[1], &actual_kicking_stats[1] );

     assertEquals( SQLITE_OK, players_t_delete(              db, &expected                  ) );
     assertEquals( SQLITE_OK, player_kicking_stats_t_delete( db, &expected_kicking_stats[0] ) );
     assertEquals( SQLITE_OK, player_kicking_stats_t_delete( db, &expected_kicking_stats[1] ) );

     free( actual_kicking_stats );

     return NULL;
}

static char *save_player__ShouldSaveReturnsStatsToTheDatabase_GivenAPlayerWithReturnsStats()
{
     player_s               expected                  = { 0 };
     player_returns_stats_s expected_returns_stats[3] = { 0 };

     buildIntoPlayer( &expected, 1 );

     buildIntoPlayerReturnsStats( &expected_returns_stats[0], 1, 1, bg_None );
     buildIntoPlayerReturnsStats( &expected_returns_stats[1], 1, 2, bg_None );

     expected_returns_stats[2] = player_returns_stats_sentinel;

     expected.stats.returns = expected_returns_stats;

     assertEquals( SQLITE_OK, save_player( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, player_returns_stats_t_read_by_player( db, expected.player_id, &list ) );

     assertEquals( 2, list.count );

     player_returns_stats_s *actual_returns_stats = list.data;

     assertNotNull( actual_returns_stats );

     comparePlayerReturnsStats( &expected_returns_stats[0], &actual_returns_stats[0] );
     comparePlayerReturnsStats( &expected_returns_stats[1], &actual_returns_stats[1] );

     assertEquals( SQLITE_OK, players_t_delete(              db, &expected                  ) );
     assertEquals( SQLITE_OK, player_returns_stats_t_delete( db, &expected_returns_stats[0] ) );
     assertEquals( SQLITE_OK, player_returns_stats_t_delete( db, &expected_returns_stats[1] ) );

     free( actual_returns_stats );

     return NULL;
}

static char *save_player__ShouldUpdateRecordsIfTheyExistAndInsertIfTheyDont_GivenAPlayer()
{
     player_s               expected                  = { 0 };
     player_accolade_s      expected_accolades[3]     = { 0 };
     player_offense_stats_s expected_offense_stats[3] = { 0 };
     player_defense_stats_s expected_defense_stats[3] = { 0 };
     player_kicking_stats_s expected_kicking_stats[3] = { 0 };
     player_returns_stats_s expected_returns_stats[3] = { 0 };

     buildIntoPlayer( &expected, 1 );

     buildIntoPlayerAccolade( &expected_accolades[0], 1, 1, pacc_All_American );
     buildIntoPlayerAccolade( &expected_accolades[1], 1, 2, pacc_All_American );

     buildIntoPlayerOffenseStats( &expected_offense_stats[0], 1, 1, bg_None );
     buildIntoPlayerOffenseStats( &expected_offense_stats[1], 1, 2, bg_None );

     buildIntoPlayerDefenseStats( &expected_defense_stats[0], 1, 1, bg_None );
     buildIntoPlayerDefenseStats( &expected_defense_stats[1], 1, 2, bg_None );

     buildIntoPlayerKickingStats( &expected_kicking_stats[0], 1, 1, bg_None );
     buildIntoPlayerKickingStats( &expected_kicking_stats[1], 1, 2, bg_None );

     buildIntoPlayerReturnsStats( &expected_returns_stats[0], 1, 1, bg_None );
     buildIntoPlayerReturnsStats( &expected_returns_stats[1], 1, 2, bg_None );

     expected_accolades[2]     = player_accolade_sentinel;
     expected_offense_stats[2] = player_offense_stats_sentinel;
     expected_defense_stats[2] = player_defense_stats_sentinel;
     expected_kicking_stats[2] = player_kicking_stats_sentinel;
     expected_returns_stats[2] = player_returns_stats_sentinel;

     expected.accolades     = expected_accolades;
     expected.stats.offense = expected_offense_stats;
     expected.stats.defense = expected_defense_stats;
     expected.stats.kicking = expected_kicking_stats;
     expected.stats.returns = expected_returns_stats;

     assertEquals( SQLITE_OK, player_accolades_t_create(     db, &expected_accolades[0]     ) );
     assertEquals( SQLITE_OK, player_offense_stats_t_create( db, &expected_offense_stats[0] ) );
     assertEquals( SQLITE_OK, player_defense_stats_t_create( db, &expected_defense_stats[0] ) );
     assertEquals( SQLITE_OK, player_kicking_stats_t_create( db, &expected_kicking_stats[0] ) );
     assertEquals( SQLITE_OK, player_returns_stats_t_create( db, &expected_returns_stats[0] ) );

     buildIntoPlayerOffenseStats( &expected_offense_stats[0], 1, 1, bg_None );
     buildIntoPlayerDefenseStats( &expected_defense_stats[0], 1, 1, bg_None );
     buildIntoPlayerKickingStats( &expected_kicking_stats[0], 1, 1, bg_None );
     buildIntoPlayerReturnsStats( &expected_returns_stats[0], 1, 1, bg_None );

     assertEquals( SQLITE_OK, save_player( db, &expected ) );

     data_list_s accolade_list = { 0 };

     assertEquals( SQLITE_OK, player_accolades_t_read_by_player( db, expected.player_id, &accolade_list ) );

     assertEquals( 2, accolade_list.count );

     player_accolade_s *actual_accolades = accolade_list.data;

     assertNotNull( actual_accolades );

     comparePlayerAccolades( &expected_accolades[0], &actual_accolades[0] );
     comparePlayerAccolades( &expected_accolades[1], &actual_accolades[1] );

     data_list_s offense_stats_list = { 0 };

     assertEquals( SQLITE_OK, player_offense_stats_t_read_by_player( db, expected.player_id, &offense_stats_list ) );

     assertEquals( 2, offense_stats_list.count );

     player_offense_stats_s *actual_offense_stats = offense_stats_list.data;

     assertNotNull( actual_offense_stats );

     comparePlayerOffenseStats( &expected_offense_stats[0], &actual_offense_stats[0] );
     comparePlayerOffenseStats( &expected_offense_stats[1], &actual_offense_stats[1] );

     data_list_s defense_stats_list = { 0 };

     assertEquals( SQLITE_OK, player_defense_stats_t_read_by_player( db, expected.player_id, &defense_stats_list ) );

     assertEquals( 2, defense_stats_list.count );

     player_defense_stats_s *actual_defense_stats = defense_stats_list.data;

     assertNotNull( actual_defense_stats );

     comparePlayerDefenseStats( &expected_defense_stats[0], &actual_defense_stats[0] );
     comparePlayerDefenseStats( &expected_defense_stats[1], &actual_defense_stats[1] );

     data_list_s kicking_stats_list = { 0 };

     assertEquals( SQLITE_OK, player_kicking_stats_t_read_by_player( db, expected.player_id, &kicking_stats_list ) );

     assertEquals( 2, kicking_stats_list.count );

     player_kicking_stats_s *actual_kicking_stats = kicking_stats_list.data;

     assertNotNull( actual_kicking_stats );

     comparePlayerKickingStats( &expected_kicking_stats[0], &actual_kicking_stats[0] );
     comparePlayerKickingStats( &expected_kicking_stats[1], &actual_kicking_stats[1] );

     data_list_s returns_stats_list = { 0 };

     assertEquals( SQLITE_OK, player_returns_stats_t_read_by_player( db, expected.player_id, &returns_stats_list ) );

     assertEquals( 2, returns_stats_list.count );

     player_returns_stats_s *actual_returns_stats = returns_stats_list.data;

     assertNotNull( actual_returns_stats );

     comparePlayerReturnsStats( &expected_returns_stats[0], &actual_returns_stats[0] );
     comparePlayerReturnsStats( &expected_returns_stats[1], &actual_returns_stats[1] );

     assertEquals( SQLITE_OK, players_t_delete(              db, &expected                  ) );
     assertEquals( SQLITE_OK, player_accolades_t_delete(     db, &expected_accolades[0]     ) );
     assertEquals( SQLITE_OK, player_accolades_t_delete(     db, &expected_accolades[1]     ) );
     assertEquals( SQLITE_OK, player_offense_stats_t_delete( db, &expected_offense_stats[0] ) );
     assertEquals( SQLITE_OK, player_offense_stats_t_delete( db, &expected_offense_stats[1] ) );
     assertEquals( SQLITE_OK, player_defense_stats_t_delete( db, &expected_defense_stats[0] ) );
     assertEquals( SQLITE_OK, player_defense_stats_t_delete( db, &expected_defense_stats[1] ) );
     assertEquals( SQLITE_OK, player_kicking_stats_t_delete( db, &expected_kicking_stats[0] ) );
     assertEquals( SQLITE_OK, player_kicking_stats_t_delete( db, &expected_kicking_stats[1] ) );
     assertEquals( SQLITE_OK, player_returns_stats_t_delete( db, &expected_returns_stats[0] ) );
     assertEquals( SQLITE_OK, player_returns_stats_t_delete( db, &expected_returns_stats[1] ) );

     free( actual_accolades     );
     free( actual_offense_stats );
     free( actual_defense_stats );
     free( actual_kicking_stats );
     free( actual_returns_stats );

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
     // get_player()
     run_test( get_player__ShouldReturnTheMatchingPlayerObject_GivenAPlayerId,                 check_sqlite_error );
     run_test( get_player__ShouldReturnTheMatchingPlayerWithRatings_GivenAPlayerId,            check_sqlite_error );
     run_test( get_player__ShouldReturnTheMatchingPlayerWithQuarterbackRatings_GivenAPlayerId, check_sqlite_error );
     run_test( get_player__ShouldReturnTheMatchingPlayerWithOffenseRatings_GivenAPlayerId,     check_sqlite_error );
     run_test( get_player__ShouldReturnTheMatchingPlayerWithDefenseRatings_GivenAPlayerId,     check_sqlite_error );
     run_test( get_player__ShouldReturnTheMatchingPlayerWithKickingRatings_GivenAPlayerId,     check_sqlite_error );

     // save_player()
     run_test( save_player__ShouldSavePlayerAccoladesToTheDatabase_GivenAPlayerWithAccolades, check_sqlite_error );
     run_test( save_player__ShouldSaveOffenseStatsToTheDatabase_GivenAPlayerWithOffenseStats, check_sqlite_error );
     run_test( save_player__ShouldSaveDefenseStatsToTheDatabase_GivenAPlayerWithDefenseStats, check_sqlite_error );
     run_test( save_player__ShouldSaveKickingStatsToTheDatabase_GivenAPlayerWithKickingStats, check_sqlite_error );
     run_test( save_player__ShouldSaveReturnsStatsToTheDatabase_GivenAPlayerWithReturnsStats, check_sqlite_error );
     run_test( save_player__ShouldUpdateRecordsIfTheyExistAndInsertIfTheyDont_GivenAPlayer,   check_sqlite_error );
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
