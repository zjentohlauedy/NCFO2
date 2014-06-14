#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "builders.h"
#include "compares.h"
#include "organization.h"
#include "unit_test.h"


static sqlite3 *db;
static char    *result;


static char *get_organization__ShouldReturnTheMatchingOrganizationObject_GivenAnOrganizationId()
{
     organization_s *expected = buildOrganization( 1 );

     assertEquals( SQLITE_OK, organizations_t_create( db, expected ) );

     organization_s *actual = get_organization( db, expected->organization_id );

     assertNotNull( actual );

     compareOrganizations( expected, actual );

     assertEquals( SQLITE_OK, organizations_t_delete( db, expected ) );

     free_organization( expected );
     free_organization( actual   );

     return NULL;
}

static char *get_organization__ShouldRetrieveConferencesFromDatabase_GivenASeason()
{
     organization_s            *expected             = buildOrganization( 1 );
     organization_conference_s *expected_org_conf1   = buildOrganizationConference( 1, 1 );
     organization_conference_s *expected_org_conf2   = buildOrganizationConference( 1, 2 );
     conference_s              *expected_conference1 = buildConference( 1 );
     conference_s              *expected_conference2 = buildConference( 2 );

     assertEquals( SQLITE_OK, organizations_t_create(            db, expected             ) );
     assertEquals( SQLITE_OK, organization_conferences_t_create( db, expected_org_conf1   ) );
     assertEquals( SQLITE_OK, organization_conferences_t_create( db, expected_org_conf2   ) );
     assertEquals( SQLITE_OK, conferences_t_create(              db, expected_conference1 ) );
     assertEquals( SQLITE_OK, conferences_t_create(              db, expected_conference2 ) );

     organization_s *organization = get_organization( db, 1 );

     assertNotNull( organization              );
     assertNotNull( organization->conferences );

     assertNotNull( organization->conferences[0].conference );
     assertNotNull( organization->conferences[1].conference );

     compareConferences( expected_conference1, organization->conferences[0].conference );
     compareConferences( expected_conference2, organization->conferences[1].conference );

     assertNull( organization->conferences[2].conference );

     assertEquals( SQLITE_OK, organizations_t_delete(            db, expected             ) );
     assertEquals( SQLITE_OK, organization_conferences_t_delete( db, expected_org_conf1   ) );
     assertEquals( SQLITE_OK, organization_conferences_t_delete( db, expected_org_conf2   ) );
     assertEquals( SQLITE_OK, conferences_t_delete(              db, expected_conference1 ) );
     assertEquals( SQLITE_OK, conferences_t_delete(              db, expected_conference2 ) );

     free_organization( organization         );
     free_conference(   expected_conference1 );
     free_conference(   expected_conference2 );

     free( expected_org_conf1 );
     free( expected_org_conf2 );

     return NULL;
}

static char *get_organization__ShouldRetrieveConferencesWithTeamsFromDatabase_GivenASeason()
{
     organization_s            *expected            = buildOrganization( 1 );
     organization_conference_s *expected_org_conf   = buildOrganizationConference( 1, 1 );
     conference_s              *expected_conference = buildConference( 1 );
     conference_team_s         *expected_conf_team1 = buildConferenceTeam( 1, 1 );
     conference_team_s         *expected_conf_team2 = buildConferenceTeam( 1, 2 );
     team_s                    *expected_team1      = buildTeam( 1 );
     team_s                    *expected_team2      = buildTeam( 2 );

     assertEquals( SQLITE_OK, organizations_t_create(            db, expected            ) );
     assertEquals( SQLITE_OK, organization_conferences_t_create( db, expected_org_conf   ) );
     assertEquals( SQLITE_OK, conferences_t_create(              db, expected_conference ) );
     assertEquals( SQLITE_OK, conference_teams_t_create(         db, expected_conf_team1 ) );
     assertEquals( SQLITE_OK, conference_teams_t_create(         db, expected_conf_team2 ) );
     assertEquals( SQLITE_OK, teams_t_create(                    db, expected_team1      ) );
     assertEquals( SQLITE_OK, teams_t_create(                    db, expected_team2      ) );

     organization_s *organization = get_organization( db, 1 );

     assertNotNull( organization              );
     assertNotNull( organization->conferences );
     assertNotNull( organization->conferences[0].conference );
     assertNotNull( organization->conferences[0].conference->teams );

     conference_team_s *conference_teams = organization->conferences[0].conference->teams;

     assertNotNull( conference_teams[0].team );
     assertNotNull( conference_teams[1].team );

     assertEquals( expected_conference->conference_id, conference_teams[0].conference_id );
     assertEquals( expected_team1->team_id,            conference_teams[0].team_id       );
     assertEquals( expected_conference->conference_id, conference_teams[1].conference_id );
     assertEquals( expected_team2->team_id,            conference_teams[1].team_id       );

     compareTeams( expected_team1,      conference_teams[0].team );
     compareTeams( expected_team2,      conference_teams[1].team );

     conference_team_s conf_team_sentinel = CONFERENCE_TEAM_SENTINEL;

     compareConferenceTeams( &conf_team_sentinel, &conference_teams[2] );

     assertEquals( SQLITE_OK, organizations_t_delete(            db, expected            ) );
     assertEquals( SQLITE_OK, organization_conferences_t_delete( db, expected_org_conf   ) );
     assertEquals( SQLITE_OK, conferences_t_delete(              db, expected_conference ) );
     assertEquals( SQLITE_OK, conference_teams_t_delete(         db, expected_conf_team1 ) );
     assertEquals( SQLITE_OK, conference_teams_t_delete(         db, expected_conf_team2 ) );
     assertEquals( SQLITE_OK, teams_t_delete(                    db, expected_team1      ) );
     assertEquals( SQLITE_OK, teams_t_delete(                    db, expected_team2      ) );

     free_organization( organization        );
     free_conference(   expected_conference );
     free_team(         expected_team1      );
     free_team(         expected_team2      );

     free( expected_org_conf   );
     free( expected_conf_team1 );
     free( expected_conf_team2 );

     return NULL;
}

static char *get_organization__ShouldRetrieveConferencesWithTeamsAndPlayersFromDatabase_GivenASeason()
{
     organization_s            *expected              = buildOrganization( 1 );
     organization_conference_s *expected_org_conf     = buildOrganizationConference( 1, 1 );
     conference_s              *expected_conference   = buildConference( 1 );
     conference_team_s         *expected_conf_team    = buildConferenceTeam( 1, 1 );
     team_s                    *expected_team         = buildTeam( 1 );
     team_player_s             *expected_team_player1 = buildTeamPlayer( 1, expected->season, 1 );
     team_player_s             *expected_team_player2 = buildTeamPlayer( 1, expected->season, 2 );
     player_s                  *expected_player1      = buildPlayer( 1 );
     player_s                  *expected_player2      = buildPlayer( 2 );

     assertEquals( SQLITE_OK, organizations_t_create(            db, expected              ) );
     assertEquals( SQLITE_OK, organization_conferences_t_create( db, expected_org_conf     ) );
     assertEquals( SQLITE_OK, conferences_t_create(              db, expected_conference   ) );
     assertEquals( SQLITE_OK, conference_teams_t_create(         db, expected_conf_team    ) );
     assertEquals( SQLITE_OK, teams_t_create(                    db, expected_team         ) );
     assertEquals( SQLITE_OK, team_players_t_create(             db, expected_team_player1 ) );
     assertEquals( SQLITE_OK, team_players_t_create(             db, expected_team_player2 ) );
     assertEquals( SQLITE_OK, players_t_create(                  db, expected_player1      ) );
     assertEquals( SQLITE_OK, players_t_create(                  db, expected_player2      ) );

     organization_s *organization = get_organization( db, 1 );

     assertNotNull( organization              );
     assertNotNull( organization->conferences );
     assertNotNull( organization->conferences[0].conference );
     assertNotNull( organization->conferences[0].conference->teams );
     assertNotNull( organization->conferences[0].conference->teams[0].team );
     assertNotNull( organization->conferences[0].conference->teams[0].team->players );

     team_player_s *team_players = organization->conferences[0].conference->teams[0].team->players;

     assertNotNull( team_players[0].player );
     assertNotNull( team_players[1].player );

     assertEquals( expected_team->team_id,      team_players[0].team_id   );
     assertEquals( expected_player1->player_id, team_players[0].player_id );
     assertEquals( expected_team->team_id,      team_players[1].team_id   );
     assertEquals( expected_player2->player_id, team_players[1].player_id );

     comparePlayers( expected_player1, team_players[0].player );
     comparePlayers( expected_player2, team_players[1].player );

     team_player_s team_player_sentinel = TEAM_PLAYER_SENTINEL;

     compareTeamPlayers( &team_player_sentinel, &team_players[2] );

     assertEquals( SQLITE_OK, organizations_t_delete(            db, expected              ) );
     assertEquals( SQLITE_OK, organization_conferences_t_delete( db, expected_org_conf     ) );
     assertEquals( SQLITE_OK, conferences_t_delete(              db, expected_conference   ) );
     assertEquals( SQLITE_OK, conference_teams_t_delete(         db, expected_conf_team    ) );
     assertEquals( SQLITE_OK, teams_t_delete(                    db, expected_team         ) );
     assertEquals( SQLITE_OK, team_players_t_delete(             db, expected_team_player1 ) );
     assertEquals( SQLITE_OK, team_players_t_delete(             db, expected_team_player2 ) );
     assertEquals( SQLITE_OK, players_t_delete(                  db, expected_player1      ) );
     assertEquals( SQLITE_OK, players_t_delete(                  db, expected_player2      ) );

     free_organization( organization        );
     free_conference(   expected_conference );
     free_team(         expected_team       );
     free_player(       expected_player1    );
     free_player(       expected_player2    );

     free( expected_org_conf  );
     free( expected_conf_team );
     free( expected_team_player1 );
     free( expected_team_player2 );

     return NULL;
}

static char *save_organization__ShouldSaveConferenceStatsToDatabase_GivenA_organizationWithConferences()
{
     organization_s            *expected                  = buildOrganization( 1 );
     organization_conference_s  expected_org_confs[3]     = { 0 };
     organization_conference_s  org_conf_sentinel         = ORGANIZATION_CONFERENCE_SENTINEL;
     conference_s              *expected_conference1      = buildConference( 1 );
     conference_s              *expected_conference2      = buildConference( 2 );
     conference_stats_s         conference_stats1[2]      = { 0 };
     conference_stats_s         conference_stats2[2]      = { 0 };
     conference_stats_s         conference_stats_sentinel = CONFERENCE_STATS_SENTINEL;

     buildIntoOrganizationConference( &expected_org_confs[0], 1, 1 );
     buildIntoOrganizationConference( &expected_org_confs[1], 1, 2 );

     buildIntoConferenceStats( &conference_stats1[0], 1, expected->season, bg_None );
     buildIntoConferenceStats( &conference_stats2[0], 2, expected->season, bg_None );

     conference_stats1[1] = conference_stats_sentinel;
     conference_stats2[1] = conference_stats_sentinel;

     expected_conference1->stats = conference_stats1;
     expected_conference2->stats = conference_stats2;

     expected_org_confs[0].conference = expected_conference1;
     expected_org_confs[1].conference = expected_conference2;
     expected_org_confs[2]            = org_conf_sentinel;

     expected->conferences = expected_org_confs;

     assertEquals( SQLITE_OK, save_organization( db, expected ) );

     conference_stats_s actual1 = { 0 };
     conference_stats_s actual2 = { 0 };

     actual1.conference_id = conference_stats1[0].conference_id;
     actual1.season        = conference_stats1[0].season;
     actual1.bowl_game     = conference_stats1[0].bowl_game;

     actual2.conference_id = conference_stats2[0].conference_id;
     actual2.season        = conference_stats2[0].season;
     actual2.bowl_game     = conference_stats2[0].bowl_game;

     assertEquals( SQLITE_OK, conference_stats_t_read( db, &actual1 ) );
     assertEquals( SQLITE_OK, conference_stats_t_read( db, &actual2 ) );

     compareConferenceStats( &conference_stats1[0], &actual1 );
     compareConferenceStats( &conference_stats2[0], &actual2 );

     assertEquals( SQLITE_OK, conference_stats_t_delete( db, &conference_stats1[0] ) );
     assertEquals( SQLITE_OK, conference_stats_t_delete( db, &conference_stats2[0] ) );

     free( expected             );
     free( expected_conference1 );
     free( expected_conference2 );

     return NULL;
}

static char *save_organization__ShouldSaveTeamStatsToDatabase_GivenAnOrganizationWithConferenceTeams()
{
     organization_s            *expected                  = buildOrganization( 1 );
     organization_conference_s  expected_org_confs[2]     = { 0 };
     organization_conference_s  org_conf_sentinel         = ORGANIZATION_CONFERENCE_SENTINEL;
     conference_s              *expected_conference       = buildConference( 1 );
     conference_team_s          expected_conf_teams[3]    = { 0 };
     conference_team_s          conf_team_sentinel        = CONFERENCE_TEAM_SENTINEL;
     team_s                    *expected_team1            = buildTeam( 1 );
     team_s                    *expected_team2            = buildTeam( 2 );
     team_stats_s               expected_team_stats1[2]   = { 0 };
     team_stats_s               expected_team_stats2[2]   = { 0 };
     team_stats_s               team_stats_sentinel       = TEAM_STATS_SENTINEL;

     buildIntoOrganizationConference( &expected_org_confs[0], 1, 1 );

     buildIntoConferenceTeam( &expected_conf_teams[0], 1, 1 );
     buildIntoConferenceTeam( &expected_conf_teams[1], 1, 2 );

     buildIntoTeamStats( &expected_team_stats1[0], 1, expected->season, bg_None );
     buildIntoTeamStats( &expected_team_stats2[0], 2, expected->season, bg_None );

     expected_team_stats1[1] = team_stats_sentinel;
     expected_team_stats2[1] = team_stats_sentinel;

     expected_team1->stats = expected_team_stats1;
     expected_team2->stats = expected_team_stats2;

     expected_conf_teams[0].team = expected_team1;
     expected_conf_teams[1].team = expected_team2;
     expected_conf_teams[2]      = conf_team_sentinel;

     expected_conference->teams = expected_conf_teams;

     expected_org_confs[0].conference = expected_conference;
     expected_org_confs[1]            = org_conf_sentinel;

     expected->conferences = expected_org_confs;

     assertEquals( SQLITE_OK, save_organization( db, expected ) );

     team_stats_s actual1 = { 0 };
     team_stats_s actual2 = { 0 };

     actual1.team_id   = expected_team_stats1[0].team_id;
     actual1.season    = expected_team_stats1[0].season;
     actual1.bowl_game = expected_team_stats1[0].bowl_game;

     actual2.team_id   = expected_team_stats2[0].team_id;
     actual2.season    = expected_team_stats2[0].season;
     actual2.bowl_game = expected_team_stats2[0].bowl_game;

     assertEquals( SQLITE_OK, team_stats_t_read( db, &actual1 ) );
     assertEquals( SQLITE_OK, team_stats_t_read( db, &actual2 ) );

     compareTeamStats( &expected_team_stats1[0], &actual1 );
     compareTeamStats( &expected_team_stats2[0], &actual2 );

     assertEquals( SQLITE_OK, team_stats_t_delete( db, &expected_team_stats1[0] ) );
     assertEquals( SQLITE_OK, team_stats_t_delete( db, &expected_team_stats2[0] ) );

     free( expected            );
     free( expected_conference );
     free( expected_team1      );
     free( expected_team2      );

     return NULL;
}

static char *save_organization__ShouldSavePlayerStatsToDatabase_GivenAnOrganizationWithConferenceTeamsWithPlayers()
{
     organization_s            *expected                      = buildOrganization( 1 );
     organization_conference_s  expected_org_confs[2]         = { 0 };
     organization_conference_s  org_conf_sentinel             = ORGANIZATION_CONFERENCE_SENTINEL;
     conference_s              *expected_conference           = buildConference( 1 );
     conference_team_s          expected_conf_teams[2]        = { 0 };
     conference_team_s          conf_team_sentinel            = CONFERENCE_TEAM_SENTINEL;
     team_s                    *expected_team                 = buildTeam( 1 );
     team_player_s              expected_team_players[3]      = { 0 };
     team_player_s              team_player_sentinel          = TEAM_PLAYER_SENTINEL;
     player_s                  *expected_player1              = buildPlayer( 1 );
     player_s                  *expected_player2              = buildPlayer( 2 );
     player_offense_stats_s     expected_offense_stats1[2]    = { 0 };
     player_offense_stats_s     expected_offense_stats2[2]    = { 0 };
     player_offense_stats_s     player_offense_stats_sentinel = PLAYER_OFFENSE_STATS_SENTINEL;

     buildIntoOrganizationConference( &expected_org_confs[0], 1, 1 );

     buildIntoConferenceTeam( &expected_conf_teams[0], 1, 1 );

     buildIntoTeamPlayer( &expected_team_players[0], 1, expected->season, 1 );
     buildIntoTeamPlayer( &expected_team_players[1], 1, expected->season, 2 );

     buildIntoPlayerOffenseStats( &expected_offense_stats1[0], 1, expected->season, bg_None );
     buildIntoPlayerOffenseStats( &expected_offense_stats2[0], 2, expected->season, bg_None );

     expected_offense_stats1[1] = player_offense_stats_sentinel;
     expected_offense_stats2[1] = player_offense_stats_sentinel;

     expected_player1->stats.offense = expected_offense_stats1;
     expected_player2->stats.offense = expected_offense_stats2;

     expected_team_players[0].player = expected_player1;
     expected_team_players[1].player = expected_player2;
     expected_team_players[2]        = team_player_sentinel;

     expected_team->players = expected_team_players;

     expected_conf_teams[0].team = expected_team;
     expected_conf_teams[1]      = conf_team_sentinel;

     expected_conference->teams = expected_conf_teams;

     expected_org_confs[0].conference = expected_conference;
     expected_org_confs[1]            = org_conf_sentinel;

     expected->conferences = expected_org_confs;

     assertEquals( SQLITE_OK, save_organization( db, expected ) );

     player_offense_stats_s actual1 = { 0 };
     player_offense_stats_s actual2 = { 0 };

     actual1.player_id = expected_offense_stats1[0].player_id;
     actual1.season    = expected_offense_stats1[0].season;
     actual1.bowl_game = expected_offense_stats1[0].bowl_game;

     actual2.player_id = expected_offense_stats2[0].player_id;
     actual2.season    = expected_offense_stats2[0].season;
     actual2.bowl_game = expected_offense_stats2[0].bowl_game;

     assertEquals( SQLITE_OK, player_offense_stats_t_read( db, &actual1 ) );
     assertEquals( SQLITE_OK, player_offense_stats_t_read( db, &actual2 ) );

     comparePlayerOffenseStats( &expected_offense_stats1[0], &actual1 );
     comparePlayerOffenseStats( &expected_offense_stats2[0], &actual2 );

     assertEquals( SQLITE_OK, player_offense_stats_t_delete( db, &expected_offense_stats1[0] ) );
     assertEquals( SQLITE_OK, player_offense_stats_t_delete( db, &expected_offense_stats2[0] ) );

     free( expected            );
     free( expected_conference );
     free( expected_team       );
     free( expected_player1    );
     free( expected_player2    );

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
     run_test( get_organization__ShouldReturnTheMatchingOrganizationObject_GivenAnOrganizationId,       check_sqlite_error );
     run_test( get_organization__ShouldRetrieveConferencesFromDatabase_GivenASeason,                    check_sqlite_error );
     run_test( get_organization__ShouldRetrieveConferencesWithTeamsFromDatabase_GivenASeason,           check_sqlite_error );
     run_test( get_organization__ShouldRetrieveConferencesWithTeamsAndPlayersFromDatabase_GivenASeason, check_sqlite_error );

     run_test( save_organization__ShouldSaveConferenceStatsToDatabase_GivenA_organizationWithConferences,            check_sqlite_error );
     run_test( save_organization__ShouldSaveTeamStatsToDatabase_GivenAnOrganizationWithConferenceTeams,              check_sqlite_error );
     run_test( save_organization__ShouldSavePlayerStatsToDatabase_GivenAnOrganizationWithConferenceTeamsWithPlayers, check_sqlite_error );
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
