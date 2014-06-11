#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "builders.h"
#include "compares.h"
#include "team.h"
#include "unit_test.h"


static team_player_s         team_player_sentinel        = TEAM_PLAYER_SENTINEL;
static team_accolade_s       team_accolade_sentinel      = TEAM_ACCOLADE_SENTINEL;
static team_stats_s          team_stats_sentinel         = TEAM_STATS_SENTINEL;
static team_offense_stats_s  team_offense_stats_sentinel = TEAM_OFFENSE_STATS_SENTINEL;
static team_defense_stats_s  team_defense_stats_sentinel = TEAM_DEFENSE_STATS_SENTINEL;
static team_kicking_stats_s  team_kicking_stats_sentinel = TEAM_KICKING_STATS_SENTINEL;

static sqlite3 *db;
static char    *result;


static char *get_team__ShouldReturnTheMatchingTeamObject_GivenATeamId()
{
     team_s *expected = buildTeam( 1 );

     assertEquals( SQLITE_OK, teams_t_create( db, expected ) );

     team_s *actual = get_team( db, expected->team_id );

     assertNotNull( actual );

     compareTeams( expected, actual );

     assertEquals( SQLITE_OK, teams_t_delete( db, expected ) );

     free_team( expected );
     free_team( actual   );

     return NULL;
}

static char *get_team__ShouldReturnTheMatchingTeamWithPlayers_GivenATeamId()
{
     team_s        *expected              = buildTeam( 1 );
     team_player_s *expected_team_player1 = buildTeamPlayer( 1, 1, 1 );
     team_player_s *expected_team_player2 = buildTeamPlayer( 1, 2, 2 );

     assertEquals( SQLITE_OK, teams_t_create(        db, expected              ) );
     assertEquals( SQLITE_OK, team_players_t_create( db, expected_team_player1 ) );
     assertEquals( SQLITE_OK, team_players_t_create( db, expected_team_player2 ) );

     team_s *actual = get_team( db, expected->team_id );

     assertNotNull( actual );

     team_player_s *actual_team_players = actual->players;

     assertNotNull( actual_team_players );

     compareTeamPlayers(  expected_team_player1, &actual_team_players[0] );
     compareTeamPlayers(  expected_team_player2, &actual_team_players[1] );
     compareTeamPlayers( &team_player_sentinel,  &actual_team_players[2] );

     assertEquals( SQLITE_OK, teams_t_delete(        db, expected              ) );
     assertEquals( SQLITE_OK, team_players_t_delete( db, expected_team_player1 ) );
     assertEquals( SQLITE_OK, team_players_t_delete( db, expected_team_player2 ) );

     free_team( expected );
     free_team( actual   );

     free( expected_team_player1 );
     free( expected_team_player2 );

     return NULL;
}

static char *get_team_for_season__ShouldReturnTheMatchingTeamWithPlayersForSeason_GivenATeamIdAndSeason()
{
     int            expected_season       = 1;
     team_s        *expected              = buildTeam( 1 );
     team_player_s *expected_team_player1 = buildTeamPlayer( 1, expected_season, 1 );
     team_player_s *expected_team_player2 = buildTeamPlayer( 1, expected_season, 2 );

     assertEquals( SQLITE_OK, teams_t_create(        db, expected              ) );
     assertEquals( SQLITE_OK, team_players_t_create( db, expected_team_player1 ) );
     assertEquals( SQLITE_OK, team_players_t_create( db, expected_team_player2 ) );

     team_s *actual = get_team_for_season( db, expected->team_id, expected_season );

     assertNotNull( actual );

     team_player_s *actual_team_players = actual->players;

     assertNotNull( actual_team_players );

     assertEquals( expected_season, actual_team_players[0].season );
     assertEquals( expected_season, actual_team_players[1].season );

     compareTeamPlayers(  expected_team_player1, &actual_team_players[0] );
     compareTeamPlayers(  expected_team_player2, &actual_team_players[1] );
     compareTeamPlayers( &team_player_sentinel,  &actual_team_players[2] );

     assertEquals( SQLITE_OK, teams_t_delete(        db, expected              ) );
     assertEquals( SQLITE_OK, team_players_t_delete( db, expected_team_player1 ) );
     assertEquals( SQLITE_OK, team_players_t_delete( db, expected_team_player2 ) );

     free_team( expected );
     free_team( actual   );

     free( expected_team_player1 );
     free( expected_team_player2 );

     return NULL;
}

static char *get_team_for_season__ShouldReturnOnlyMatchingTeamWithPlayersForSeason_GivenATeamIdAndSeason()
{
     int            expected_season       = 1;
     team_s        *expected              = buildTeam( 1 );
     team_player_s *expected_team_player1 = buildTeamPlayer( 1, expected_season,     1 );
     team_player_s *expected_team_player2 = buildTeamPlayer( 1, expected_season + 1, 2 );

     assertEquals( SQLITE_OK, teams_t_create(        db, expected              ) );
     assertEquals( SQLITE_OK, team_players_t_create( db, expected_team_player1 ) );
     assertEquals( SQLITE_OK, team_players_t_create( db, expected_team_player2 ) );

     team_s *actual = get_team_for_season( db, expected->team_id, expected_season );

     assertNotNull( actual );

     team_player_s *actual_team_players = actual->players;

     assertNotNull( actual_team_players );

     assertEquals( expected_season, actual_team_players[0].season );

     compareTeamPlayers(  expected_team_player1, &actual_team_players[0] );
     compareTeamPlayers( &team_player_sentinel,  &actual_team_players[1] );

     assertEquals( SQLITE_OK, teams_t_delete(        db, expected              ) );
     assertEquals( SQLITE_OK, team_players_t_delete( db, expected_team_player1 ) );
     assertEquals( SQLITE_OK, team_players_t_delete( db, expected_team_player2 ) );

     free_team( expected );
     free_team( actual   );

     free( expected_team_player1 );
     free( expected_team_player2 );

     return NULL;
}

static char *save_team__ShouldPersistTheAccoladesInTheDatabase_GivenATeamWithAccolades()
{
     team_s          expected              = { 0 };
     team_accolade_s expected_accolades[3] = { 0 };

     buildIntoTeam( &expected, 1 );
     buildIntoTeamAccolade( &expected_accolades[0], 1, 1, tacc_ConferenceChampions );
     buildIntoTeamAccolade( &expected_accolades[1], 1, 2, tacc_FiestaBowlChampions );

     expected_accolades[2] = team_accolade_sentinel;

     expected.accolades = expected_accolades;

     assertEquals( SQLITE_OK, save_team( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, team_accolades_t_read_by_team( db, expected.team_id, &list ) );

     assertEquals( 2, list.count );

     team_accolade_s *actual_accolades = list.data;

     assertNotNull( actual_accolades );

     compareTeamAccolades( &expected_accolades[0], &actual_accolades[0] );
     compareTeamAccolades( &expected_accolades[1], &actual_accolades[1] );

     assertEquals( SQLITE_OK, teams_t_delete(          db, &expected              ) );
     assertEquals( SQLITE_OK, team_accolades_t_delete( db, &expected_accolades[0] ) );
     assertEquals( SQLITE_OK, team_accolades_t_delete( db, &expected_accolades[1] ) );

     free( actual_accolades );

     return NULL;
}

static char *save_team__ShouldPersistTheTeamStatsInTheDatabase_GivenATeamWithTeamStats()
{
     team_s       expected          = { 0 };
     team_stats_s expected_stats[3] = { 0 };

     buildIntoTeam( &expected, 1 );
     buildIntoTeamStats( &expected_stats[0], 1, 1, bg_None );
     buildIntoTeamStats( &expected_stats[1], 1, 2, bg_None );

     expected_stats[2] = team_stats_sentinel;

     expected.stats = expected_stats;

     assertEquals( SQLITE_OK, save_team( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, team_stats_t_read_by_team( db, expected.team_id, &list ) );

     assertEquals( 2, list.count );

     team_stats_s *actual_stats = list.data;

     assertNotNull( actual_stats );

     compareTeamStats( &expected_stats[0], &actual_stats[0] );
     compareTeamStats( &expected_stats[1], &actual_stats[1] );

     assertEquals( SQLITE_OK, teams_t_delete(      db, &expected          ) );
     assertEquals( SQLITE_OK, team_stats_t_delete( db, &expected_stats[0] ) );
     assertEquals( SQLITE_OK, team_stats_t_delete( db, &expected_stats[1] ) );

     free( actual_stats );

     return NULL;
}

static char *save_team__ShouldPersistTheOffenseStatsInTheDatabase_GivenATeamWithOffenseStats()
{
     team_s               expected                  = { 0 };
     team_offense_stats_s expected_offense_stats[3] = { 0 };

     buildIntoTeam( &expected, 1 );
     buildIntoTeamOffenseStats( &expected_offense_stats[0], 1, 1, bg_None );
     buildIntoTeamOffenseStats( &expected_offense_stats[1], 1, 2, bg_None );

     expected_offense_stats[2] = team_offense_stats_sentinel;

     expected.offense_stats = expected_offense_stats;

     assertEquals( SQLITE_OK, save_team( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, team_offense_stats_t_read_by_team( db, expected.team_id, &list ) );

     assertEquals( 2, list.count );

     team_offense_stats_s *actual_offense_stats = list.data;

     assertNotNull( actual_offense_stats );

     compareTeamOffenseStats( &expected_offense_stats[0], &actual_offense_stats[0] );
     compareTeamOffenseStats( &expected_offense_stats[1], &actual_offense_stats[1] );

     assertEquals( SQLITE_OK, teams_t_delete(              db, &expected                  ) );
     assertEquals( SQLITE_OK, team_offense_stats_t_delete( db, &expected_offense_stats[0] ) );
     assertEquals( SQLITE_OK, team_offense_stats_t_delete( db, &expected_offense_stats[1] ) );

     free( actual_offense_stats );

     return NULL;
}

static char *save_team__ShouldPersistTheDefenseStatsInTheDatabase_GivenATeamWithDefenseStats()
{
     team_s               expected                  = { 0 };
     team_defense_stats_s expected_defense_stats[3] = { 0 };

     buildIntoTeam( &expected, 1 );
     buildIntoTeamDefenseStats( &expected_defense_stats[0], 1, 1, bg_None );
     buildIntoTeamDefenseStats( &expected_defense_stats[1], 1, 2, bg_None );

     expected_defense_stats[2] = team_defense_stats_sentinel;

     expected.defense_stats = expected_defense_stats;

     assertEquals( SQLITE_OK, save_team( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, team_defense_stats_t_read_by_team( db, expected.team_id, &list ) );

     assertEquals( 2, list.count );

     team_defense_stats_s *actual_defense_stats = list.data;

     assertNotNull( actual_defense_stats );

     compareTeamDefenseStats( &expected_defense_stats[0], &actual_defense_stats[0] );
     compareTeamDefenseStats( &expected_defense_stats[1], &actual_defense_stats[1] );

     assertEquals( SQLITE_OK, teams_t_delete(              db, &expected                  ) );
     assertEquals( SQLITE_OK, team_defense_stats_t_delete( db, &expected_defense_stats[0] ) );
     assertEquals( SQLITE_OK, team_defense_stats_t_delete( db, &expected_defense_stats[1] ) );

     free( actual_defense_stats );

     return NULL;
}

static char *save_team__ShouldPersistTheKickingStatsInTheDatabase_GivenATeamWithKickingStats()
{
     team_s               expected                  = { 0 };
     team_kicking_stats_s expected_kicking_stats[3] = { 0 };

     buildIntoTeam( &expected, 1 );
     buildIntoTeamKickingStats( &expected_kicking_stats[0], 1, 1, bg_None );
     buildIntoTeamKickingStats( &expected_kicking_stats[1], 1, 2, bg_None );

     expected_kicking_stats[2] = team_kicking_stats_sentinel;

     expected.kicking_stats = expected_kicking_stats;

     assertEquals( SQLITE_OK, save_team( db, &expected ) );

     data_list_s list = { 0 };

     assertEquals( SQLITE_OK, team_kicking_stats_t_read_by_team( db, expected.team_id, &list ) );

     assertEquals( 2, list.count );

     team_kicking_stats_s *actual_kicking_stats = list.data;

     assertNotNull( actual_kicking_stats );

     compareTeamKickingStats( &expected_kicking_stats[0], &actual_kicking_stats[0] );
     compareTeamKickingStats( &expected_kicking_stats[1], &actual_kicking_stats[1] );

     assertEquals( SQLITE_OK, teams_t_delete(              db, &expected                  ) );
     assertEquals( SQLITE_OK, team_kicking_stats_t_delete( db, &expected_kicking_stats[0] ) );
     assertEquals( SQLITE_OK, team_kicking_stats_t_delete( db, &expected_kicking_stats[1] ) );

     free( actual_kicking_stats );

     return NULL;
}

static char *save_team__ShouldUpdateRecordsIfTheyExistAndInsertIfTheyDont_GivenATeam()
{
     team_s               expected                  = { 0 };
     team_accolade_s      expected_accolades[3]     = { 0 };
     team_stats_s         expected_stats[3]         = { 0 };
     team_offense_stats_s expected_offense_stats[3] = { 0 };
     team_defense_stats_s expected_defense_stats[3] = { 0 };
     team_kicking_stats_s expected_kicking_stats[3] = { 0 };

     buildIntoTeam( &expected, 1 );

     buildIntoTeamAccolade( &expected_accolades[0], 1, 1, tacc_ConferenceChampions );
     buildIntoTeamAccolade( &expected_accolades[1], 1, 2, tacc_FiestaBowlChampions );
     expected_accolades[2] = team_accolade_sentinel;

     buildIntoTeamStats( &expected_stats[0], 1, 1, bg_None );
     buildIntoTeamStats( &expected_stats[1], 1, 2, bg_None );
     expected_stats[2] = team_stats_sentinel;

     buildIntoTeamOffenseStats( &expected_offense_stats[0], 1, 1, bg_None );
     buildIntoTeamOffenseStats( &expected_offense_stats[1], 1, 2, bg_None );
     expected_offense_stats[2] = team_offense_stats_sentinel;

     buildIntoTeamDefenseStats( &expected_defense_stats[0], 1, 1, bg_None );
     buildIntoTeamDefenseStats( &expected_defense_stats[1], 1, 2, bg_None );
     expected_defense_stats[2] = team_defense_stats_sentinel;

     buildIntoTeamKickingStats( &expected_kicking_stats[0], 1, 1, bg_None );
     buildIntoTeamKickingStats( &expected_kicking_stats[1], 1, 2, bg_None );
     expected_kicking_stats[2] = team_kicking_stats_sentinel;

     expected.accolades     = expected_accolades;
     expected.stats         = expected_stats;
     expected.offense_stats = expected_offense_stats;
     expected.defense_stats = expected_defense_stats;
     expected.kicking_stats = expected_kicking_stats;

     assertEquals( SQLITE_OK, team_accolades_t_create(     db, &expected_accolades[0]     ) );
     assertEquals( SQLITE_OK, team_stats_t_create(         db, &expected_stats[0]         ) );
     assertEquals( SQLITE_OK, team_offense_stats_t_create( db, &expected_offense_stats[0] ) );
     assertEquals( SQLITE_OK, team_defense_stats_t_create( db, &expected_defense_stats[0] ) );
     assertEquals( SQLITE_OK, team_kicking_stats_t_create( db, &expected_kicking_stats[0] ) );

     buildIntoTeamStats(        &expected_stats[0],         1, 1, bg_None );
     buildIntoTeamOffenseStats( &expected_offense_stats[0], 1, 1, bg_None );
     buildIntoTeamDefenseStats( &expected_defense_stats[0], 1, 1, bg_None );
     buildIntoTeamKickingStats( &expected_kicking_stats[0], 1, 1, bg_None );

     assertEquals( SQLITE_OK, save_team( db, &expected ) );

     data_list_s accolade_list = { 0 };

     assertEquals( SQLITE_OK, team_accolades_t_read_by_team( db, expected.team_id, &accolade_list ) );

     assertEquals( 2, accolade_list.count );

     team_accolade_s *actual_accolades = accolade_list.data;

     assertNotNull( actual_accolades );

     compareTeamAccolades( &expected_accolades[0], &actual_accolades[0] );
     compareTeamAccolades( &expected_accolades[1], &actual_accolades[1] );

     data_list_s stats_list = { 0 };

     assertEquals( SQLITE_OK, team_stats_t_read_by_team( db, expected.team_id, &stats_list ) );

     assertEquals( 2, stats_list.count );

     team_stats_s *actual_stats = stats_list.data;

     assertNotNull( actual_stats );

     compareTeamStats( &expected_stats[0], &actual_stats[0] );
     compareTeamStats( &expected_stats[1], &actual_stats[1] );

     data_list_s offense_stats_list = { 0 };

     assertEquals( SQLITE_OK, team_offense_stats_t_read_by_team( db, expected.team_id, &offense_stats_list ) );

     assertEquals( 2, offense_stats_list.count );

     team_offense_stats_s *actual_offense_stats = offense_stats_list.data;

     assertNotNull( actual_offense_stats );

     compareTeamOffenseStats( &expected_offense_stats[0], &actual_offense_stats[0] );
     compareTeamOffenseStats( &expected_offense_stats[1], &actual_offense_stats[1] );

     data_list_s defense_stats_list = { 0 };

     assertEquals( SQLITE_OK, team_defense_stats_t_read_by_team( db, expected.team_id, &defense_stats_list ) );

     assertEquals( 2, defense_stats_list.count );

     team_defense_stats_s *actual_defense_stats = defense_stats_list.data;

     assertNotNull( actual_defense_stats );

     compareTeamDefenseStats( &expected_defense_stats[0], &actual_defense_stats[0] );
     compareTeamDefenseStats( &expected_defense_stats[1], &actual_defense_stats[1] );

     data_list_s kicking_stats_list = { 0 };

     assertEquals( SQLITE_OK, team_kicking_stats_t_read_by_team( db, expected.team_id, &kicking_stats_list ) );

     assertEquals( 2, kicking_stats_list.count );

     team_kicking_stats_s *actual_kicking_stats = kicking_stats_list.data;

     assertNotNull( actual_kicking_stats );

     compareTeamKickingStats( &expected_kicking_stats[0], &actual_kicking_stats[0] );
     compareTeamKickingStats( &expected_kicking_stats[1], &actual_kicking_stats[1] );

     assertEquals( SQLITE_OK, teams_t_delete(              db, &expected              ) );
     assertEquals( SQLITE_OK, team_accolades_t_delete(     db, &expected_accolades[0] ) );
     assertEquals( SQLITE_OK, team_accolades_t_delete(     db, &expected_accolades[1] ) );
     assertEquals( SQLITE_OK, team_stats_t_delete(         db, &expected_stats[0] ) );
     assertEquals( SQLITE_OK, team_stats_t_delete(         db, &expected_stats[1] ) );
     assertEquals( SQLITE_OK, team_offense_stats_t_delete( db, &expected_offense_stats[0] ) );
     assertEquals( SQLITE_OK, team_offense_stats_t_delete( db, &expected_offense_stats[1] ) );
     assertEquals( SQLITE_OK, team_defense_stats_t_delete( db, &expected_defense_stats[0] ) );
     assertEquals( SQLITE_OK, team_defense_stats_t_delete( db, &expected_defense_stats[1] ) );
     assertEquals( SQLITE_OK, team_kicking_stats_t_delete( db, &expected_kicking_stats[0] ) );
     assertEquals( SQLITE_OK, team_kicking_stats_t_delete( db, &expected_kicking_stats[1] ) );

     free( actual_accolades     );
     free( actual_stats         );
     free( actual_offense_stats );
     free( actual_defense_stats );
     free( actual_kicking_stats );

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
     // get_team()
     run_test( get_team__ShouldReturnTheMatchingTeamObject_GivenATeamId,      check_sqlite_error );
     run_test( get_team__ShouldReturnTheMatchingTeamWithPlayers_GivenATeamId, check_sqlite_error );

     // get_team_by_season()
     run_test( get_team_for_season__ShouldReturnTheMatchingTeamWithPlayersForSeason_GivenATeamIdAndSeason,  check_sqlite_error );
     run_test( get_team_for_season__ShouldReturnOnlyMatchingTeamWithPlayersForSeason_GivenATeamIdAndSeason, check_sqlite_error );

     // save_team()
     run_test( save_team__ShouldPersistTheAccoladesInTheDatabase_GivenATeamWithAccolades,       check_sqlite_error );
     run_test( save_team__ShouldPersistTheTeamStatsInTheDatabase_GivenATeamWithTeamStats,       check_sqlite_error );
     run_test( save_team__ShouldPersistTheOffenseStatsInTheDatabase_GivenATeamWithOffenseStats, check_sqlite_error );
     run_test( save_team__ShouldPersistTheDefenseStatsInTheDatabase_GivenATeamWithDefenseStats, check_sqlite_error );
     run_test( save_team__ShouldPersistTheKickingStatsInTheDatabase_GivenATeamWithKickingStats, check_sqlite_error );
     run_test( save_team__ShouldUpdateRecordsIfTheyExistAndInsertIfTheyDont_GivenATeam,         check_sqlite_error );
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
