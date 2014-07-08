#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "builders.h"
#include "file_formats.h"
#include "organization.h"
#include "unit_test.h"


static char *result;


static organization_s *get_test_org()
{
     organization_s            *organization         = buildOrganization( 1 );
     organization_conference_s *org_confs            = malloc( sizeof(organization_conference_s) * 6 );
     organization_conference_s  org_conf_sentinel    = ORGANIZATION_CONFERENCE_SENTINEL;
     conference_s              *conference1          = buildConference( 1 );
     conference_s              *conference2          = buildConference( 2 );
     conference_s              *conference3          = buildConference( 3 );
     conference_s              *conference4          = buildConference( 4 );
     conference_s              *conference5          = buildConference( 5 );
     conference_team_s         *conf_teams1          = malloc( sizeof(conference_team_s) * 2 );
     conference_team_s         *conf_teams5          = malloc( sizeof(conference_team_s) * 2 );
     conference_team_s          conf_team_sentinel   = CONFERENCE_TEAM_SENTINEL;
     team_s                    *team1                = buildTeam(  1 );
     team_s                    *team25               = buildTeam( 25 );
     team_player_s             *team_players         = malloc( sizeof(team_player_s) * 33);
     team_player_s              team_player_sentinel = TEAM_PLAYER_SENTINEL;
     player_s                  *player01              = buildPlayer(  1 );
     player_s                  *player02              = buildPlayer(  2 );
     player_s                  *player03              = buildPlayer(  3 );
     player_s                  *player04              = buildPlayer(  4 );
     player_s                  *player05              = buildPlayer(  5 );
     player_s                  *player06              = buildPlayer(  6 );
     player_s                  *player07              = buildPlayer(  7 );
     player_s                  *player08              = buildPlayer(  8 );
     player_s                  *player09              = buildPlayer(  9 );
     player_s                  *player10              = buildPlayer( 10 );
     player_s                  *player11              = buildPlayer( 11 );
     player_s                  *player12              = buildPlayer( 12 );
     player_s                  *player13              = buildPlayer( 13 );
     player_s                  *player14              = buildPlayer( 14 );
     player_s                  *player15              = buildPlayer( 15 );
     player_s                  *player16              = buildPlayer( 16 );
     player_s                  *player17              = buildPlayer( 17 );
     player_s                  *player18              = buildPlayer( 18 );
     player_s                  *player19              = buildPlayer( 19 );
     player_s                  *player20              = buildPlayer( 20 );
     player_s                  *player21              = buildPlayer( 21 );
     player_s                  *player22              = buildPlayer( 22 );
     player_s                  *player23              = buildPlayer( 23 );
     player_s                  *player24              = buildPlayer( 24 );
     player_s                  *player25              = buildPlayer( 25 );
     player_s                  *player26              = buildPlayer( 26 );
     player_s                  *player27              = buildPlayer( 27 );
     player_s                  *player28              = buildPlayer( 28 );
     player_s                  *player29              = buildPlayer( 29 );
     player_s                  *player30              = buildPlayer( 30 );
     player_s                  *player31              = buildPlayer( 31 );
     player_s                  *player32              = buildPlayer( 32 );

     buildIntoOrganizationConference( &org_confs[0], 1, 1 );

     buildIntoConferenceTeam( &conf_teams1[0], 1, 1 );

     buildIntoTeamPlayer( &team_players[0], 1, organization->season, 1 );
     buildIntoTeamPlayer( &team_players[1], 1, organization->season, 2 );

     player01->position = pos_Quarterback;
     player02->position = pos_Quarterback;
     player03->position = pos_Runningback;
     player04->position = pos_Runningback;
     player05->position = pos_Runningback;
     player06->position = pos_Runningback;
     player07->position = pos_WideReceiver;
     player08->position = pos_WideReceiver;
     player09->position = pos_WideReceiver;
     player10->position = pos_WideReceiver;
     player11->position = pos_WideReceiver;
     player12->position = pos_WideReceiver;
     player13->position = pos_TightEnd;
     player14->position = pos_TightEnd;
     player15->position = pos_OffensiveLine;
     player16->position = pos_OffensiveLine;
     player17->position = pos_OffensiveLine;
     player18->position = pos_OffensiveLine;
     player19->position = pos_OffensiveLine;
     player20->position = pos_DefensiveLine;
     player21->position = pos_DefensiveLine;
     player22->position = pos_DefensiveLine;
     player23->position = pos_Linebacker;
     player24->position = pos_Linebacker;
     player25->position = pos_Linebacker;
     player26->position = pos_Linebacker;
     player27->position = pos_Cornerback;
     player28->position = pos_Cornerback;
     player29->position = pos_Safety;
     player30->position = pos_Safety;
     player31->position = pos_Kicker;
     player32->position = pos_Punter;

     team_players[ 0].player = player01;
     team_players[ 1].player = player02;
     team_players[ 2].player = player03;
     team_players[ 3].player = player04;
     team_players[ 4].player = player05;
     team_players[ 5].player = player06;
     team_players[ 6].player = player07;
     team_players[ 7].player = player08;
     team_players[ 8].player = player09;
     team_players[ 9].player = player10;
     team_players[10].player = player11;
     team_players[11].player = player12;
     team_players[12].player = player13;
     team_players[13].player = player14;
     team_players[14].player = player15;
     team_players[15].player = player16;
     team_players[16].player = player17;
     team_players[17].player = player18;
     team_players[18].player = player19;
     team_players[19].player = player20;
     team_players[20].player = player21;
     team_players[21].player = player22;
     team_players[22].player = player23;
     team_players[23].player = player24;
     team_players[24].player = player25;
     team_players[25].player = player26;
     team_players[26].player = player27;
     team_players[27].player = player28;
     team_players[28].player = player29;
     team_players[29].player = player30;
     team_players[30].player = player31;
     team_players[31].player = player32;
     team_players[32]        = team_player_sentinel;

     team1 ->players = team_players;
     team25->players = team_players;

     conf_teams1[0].team = team1;
     conf_teams1[1]      = conf_team_sentinel;

     conference1->teams = conf_teams1;

     conf_teams5[0].team = team25;
     conf_teams5[1]      = conf_team_sentinel;

     conference5->teams = conf_teams5;

     org_confs[0].conference = conference1;
     org_confs[1].conference = conference2;
     org_confs[2].conference = conference3;
     org_confs[3].conference = conference4;
     org_confs[4].conference = conference5;
     org_confs[5]            = org_conf_sentinel;

     organization->conferences = org_confs;

     return organization;
}


static char *populateRoms_ShouldReturnTrueOnSuccess_GivenATsbRomAndOrganization()
{
     tsbrom_s        tsbrom1 = { 0 };
     tsbrom_s        tsbrom2 = { 0 };
     organization_s *org     = get_test_org();

     assertEquals( bl_True, populateRoms( &tsbrom1, &tsbrom2, org ) );

     return NULL;
}

static char *populateRoms_ShouldReturnAnErrorIfATeamIsShortPlayers_GivenATsbRomAndOrganization()
{
     tsbrom_s        tsbrom1 = { 0 };
     tsbrom_s        tsbrom2 = { 0 };
     organization_s *org     = get_test_org();

     player_s *player = org->conferences[0].conference->teams[0].team->players[3].player;

     player->position = pos_Quarterback;

     assertEquals( bl_False, populateRoms( &tsbrom1, &tsbrom2, org ) );

     return NULL;
}

static char *populateRoms_ShouldWriteThePlayerIdentifiersInThePlayerIdentifiersSection_GivenATsbRomAndOrganization()
{
     tsbrom_s        tsbrom1 = { 0 };
     tsbrom_s        tsbrom2 = { 0 };
     organization_s *org     = get_test_org();

     assertEquals( bl_True, populateRoms( &tsbrom1, &tsbrom2, org ) );

     player_s *player1 = org->conferences[0].conference->teams[0].team->players[0].player;
     player_s *player2 = org->conferences[0].conference->teams[0].team->players[1].player;

     char expected[100] = { 0 };

     int idx = 0;

     expected[idx++] = number2hex( player1->number );

     idx += sprintf( expected + idx, "%s%s", lowercase( player1->first_name ), uppercase( player1->last_name ) );

     expected[idx++] = number2hex( player2->number );

     idx += sprintf( expected + idx, "%s%s", lowercase( player2->first_name ), uppercase( player2->last_name ) );

     assertEqualsBfr( expected, tsbrom1.player_identifiers, idx );

     free_organization( org );

     return NULL;
}

static char *populateRoms_ShouldWriteTheOffsetsInThePlayerPointersSection_GivenATsbRomAndOrganization()
{
     tsbrom_s        tsbrom1 = { 0 };
     tsbrom_s        tsbrom2 = { 0 };
     organization_s *org     = get_test_org();

     assertEquals( bl_True, populateRoms( &tsbrom1, &tsbrom2, org ) );

     player_s *player1     = org->conferences[0].conference->teams[0].team->players[0].player;
     int       player1_len = strlen( player1->first_name ) + strlen( player1->last_name ) + 1; //+1 for the number

     assertEquals( 0x86ca,               pointer2int( &(tsbrom1.player_pointers[0][0]) ) );
     assertEquals( 0x86ca + player1_len, pointer2int( &(tsbrom1.player_pointers[0][1]) ) );

     free_organization( org );

     return NULL;
}

static char *populateRoms_ShouldWriteThePlayerRatingsInTheTeamPlayerRatingsSection_GivenATsbRomAndOrganization()
{
     tsbrom_s        tsbrom1 = { 0 };
     tsbrom_s        tsbrom2 = { 0 };
     organization_s *org     = get_test_org();

     player_s *player1 = org->conferences[0].conference->teams[0].team->players[0].player;
     player_s *player2 = org->conferences[0].conference->teams[0].team->players[1].player;

     player1->ratings = buildPlayerRatings( player1->player_id );
     player2->ratings = buildPlayerRatings( player2->player_id );

     player1->extra_ratings.quarterback = buildPlayerQuarterbackRatings( player1->player_id );
     player2->extra_ratings.quarterback = buildPlayerQuarterbackRatings( player2->player_id );

     assertEquals( bl_True, populateRoms( &tsbrom1, &tsbrom2, org ) );

     // populateRoms can change the player order so get them again...
     player1 = org->conferences[0].conference->teams[0].team->players[0].player;
     player2 = org->conferences[0].conference->teams[0].team->players[1].player;

     assertEquals( player1->face,                tsbrom1.team_player_ratings[0].qb1.player.face[0]            );
     assertEquals( player1->ratings->run_speed,  tsbrom1.team_player_ratings[0].qb1.player.ratings[0]  & 0x0f );
     assertEquals( player1->ratings->rush_power, tsbrom1.team_player_ratings[0].qb1.player.ratings[0] >>    4 );
     assertEquals( player1->ratings->max_speed,  tsbrom1.team_player_ratings[0].qb1.player.ratings[1] >>    4 );
     assertEquals( player1->ratings->hit_power,  tsbrom1.team_player_ratings[0].qb1.player.ratings[1]  & 0x0f );

     assertEquals( player1->extra_ratings.quarterback->pass_speed,       tsbrom1.team_player_ratings[0].qb1.qb_ratings[0] >>    4 );
     assertEquals( player1->extra_ratings.quarterback->pass_control,     tsbrom1.team_player_ratings[0].qb1.qb_ratings[0]  & 0x0f );
     assertEquals( player1->extra_ratings.quarterback->pass_accuracy,    tsbrom1.team_player_ratings[0].qb1.qb_ratings[1] >>    4 );
     assertEquals( player1->extra_ratings.quarterback->avoid_pass_block, tsbrom1.team_player_ratings[0].qb1.qb_ratings[1]  & 0x0f );

     assertEquals( player2->face,                tsbrom1.team_player_ratings[0].qb2.player.face[0]            );
     assertEquals( player2->ratings->run_speed,  tsbrom1.team_player_ratings[0].qb2.player.ratings[0]  & 0x0f );
     assertEquals( player2->ratings->rush_power, tsbrom1.team_player_ratings[0].qb2.player.ratings[0] >>    4 );
     assertEquals( player2->ratings->max_speed,  tsbrom1.team_player_ratings[0].qb2.player.ratings[1] >>    4 );
     assertEquals( player2->ratings->hit_power,  tsbrom1.team_player_ratings[0].qb2.player.ratings[1]  & 0x0f );

     assertEquals( player2->extra_ratings.quarterback->pass_speed,       tsbrom1.team_player_ratings[0].qb2.qb_ratings[0] >>    4 );
     assertEquals( player2->extra_ratings.quarterback->pass_control,     tsbrom1.team_player_ratings[0].qb2.qb_ratings[0]  & 0x0f );
     assertEquals( player2->extra_ratings.quarterback->pass_accuracy,    tsbrom1.team_player_ratings[0].qb2.qb_ratings[1] >>    4 );
     assertEquals( player2->extra_ratings.quarterback->avoid_pass_block, tsbrom1.team_player_ratings[0].qb2.qb_ratings[1]  & 0x0f );

     free_organization( org );

     return NULL;
}

static char *populateRoms_ShouldWriteTheKickAndPuntReturnersForEveryTeam()
{
     tsbrom_s        tsbrom1 = { 0 };
     tsbrom_s        tsbrom2 = { 0 };
     organization_s *org     = get_test_org();

     org->conferences[4].conference = NULL;

     player_s *kr = org->conferences[0].conference->teams[0].team->players[5].player;
     player_s *pr = org->conferences[0].conference->teams[0].team->players[9].player;

     kr->ratings = buildPlayerRatings( kr->player_id );
     pr->ratings = buildPlayerRatings( pr->player_id );

     kr->extra_ratings.offense = buildPlayerOffenseRatings( kr->player_id );
     pr->extra_ratings.offense = buildPlayerOffenseRatings( pr->player_id );

     kr->maturity                            =  4;
     kr->ratings->max_speed                  = 15;
     kr->ratings->run_speed                  =  1;
     kr->ratings->rush_power                 =  1;
     kr->extra_ratings.offense->ball_control = 15;

     pr->maturity                            =  4;
     pr->ratings->max_speed                  = 14;
     pr->ratings->run_speed                  = 15;
     pr->ratings->rush_power                 = 15;
     pr->extra_ratings.offense->ball_control = 15;

     assertEquals( bl_True, populateRoms( &tsbrom1, &tsbrom2, org ) );

     // Note: kr should be sorted to the top of RB section, and the same for pr in the WR section

     assertEquals( 0x26, tsbrom1.kick_and_punt_returners1[0] );
     assertEquals( 0x26, tsbrom1.kick_and_punt_returners2[0] );

     // No kr or pr should have been picked for the next team
     assertEquals( 0,    tsbrom1.kick_and_punt_returners1[1] );
     assertEquals( 0,    tsbrom1.kick_and_punt_returners2[1] );

     free_organization( org );

     return NULL;
}

static char *populateRoms_ShouldPopulateBothRoms_GivenTwoTsbRomsAndOrganization()
{
     tsbrom_s        tsbrom1 = { 0 };
     tsbrom_s        tsbrom2 = { 0 };
     organization_s *org     = get_test_org();

     player_s *player1 = org->conferences[4].conference->teams[0].team->players[0].player;
     player_s *player2 = org->conferences[4].conference->teams[0].team->players[1].player;

     player1->ratings = buildPlayerRatings( player1->player_id );
     player2->ratings = buildPlayerRatings( player2->player_id );

     player1->extra_ratings.quarterback = buildPlayerQuarterbackRatings( player1->player_id );
     player2->extra_ratings.quarterback = buildPlayerQuarterbackRatings( player2->player_id );

     assertEquals( bl_True, populateRoms( &tsbrom1, &tsbrom2, org ) );

     // populateRoms can change the player order so get them again...
     player1 = org->conferences[0].conference->teams[0].team->players[0].player;
     player2 = org->conferences[0].conference->teams[0].team->players[1].player;

     int player1_len = strlen( player1->first_name ) + strlen( player1->last_name ) + 1; //+1 for the number

     char expected[100] = { 0 };

     int idx = 0;

     expected[idx++] = number2hex( player1->number );

     idx += sprintf( expected + idx, "%s%s", lowercase( player1->first_name ), uppercase( player1->last_name ) );

     expected[idx++] = number2hex( player2->number );

     idx += sprintf( expected + idx, "%s%s", lowercase( player2->first_name ), uppercase( player2->last_name ) );

     assertEqualsBfr( expected, tsbrom2.player_identifiers, idx );

     assertEquals( 0x86ca,               pointer2int( &(tsbrom2.player_pointers[0][0]) ) );
     assertEquals( 0x86ca + player1_len, pointer2int( &(tsbrom2.player_pointers[0][1]) ) );

     free_organization( org );

     return NULL;
}

static char *populateRoms_ShouldAdjustPlayerRatingsBasedOnMaturity()
{
     tsbrom_s        tsbrom1 = { 0 };
     tsbrom_s        tsbrom2 = { 0 };
     organization_s *org     = get_test_org();

     org->season = 4;
     org->conferences[4].conference = NULL;

     // maturity 4
     player_s *player01 = org->conferences[0].conference->teams[0].team->players[0].player;
     player_s *player02 = org->conferences[0].conference->teams[0].team->players[1].player;
     player_s *player03 = org->conferences[0].conference->teams[0].team->players[2].player;
     player_s *player04 = org->conferences[0].conference->teams[0].team->players[3].player;

     player01->ratings             = buildPlayerRatings( player01->player_id );
     player01->freshman_season     = 4; // freshman
     player01->maturity            = 4;
     player01->ratings->run_speed  = 8;
     player01->ratings->rush_power = 0;

     player02->ratings             = buildPlayerRatings( player02->player_id );
     player02->freshman_season     = 3; // sophomore
     player02->maturity            = 4;
     player02->ratings->run_speed  = 8;
     player02->ratings->rush_power = 0;

     player03->ratings             = buildPlayerRatings( player03->player_id );
     player03->freshman_season     = 2; // junior
     player03->maturity            = 4;
     player03->ratings->run_speed  = 8;
     player03->ratings->rush_power = 0;

     player04->ratings             = buildPlayerRatings( player04->player_id );
     player04->freshman_season     = 1; // senior
     player04->maturity            = 4;
     player04->ratings->run_speed  = 8;
     player04->ratings->rush_power = 0;

     // maturity 3
     player_s *player05 = org->conferences[0].conference->teams[0].team->players[4].player;
     player_s *player06 = org->conferences[0].conference->teams[0].team->players[5].player;
     player_s *player07 = org->conferences[0].conference->teams[0].team->players[6].player;
     player_s *player08 = org->conferences[0].conference->teams[0].team->players[7].player;

     player05->ratings             = buildPlayerRatings( player05->player_id );
     player05->freshman_season     = 4; // freshman
     player05->maturity            = 3;
     player05->ratings->run_speed  = 8;
     player05->ratings->rush_power = 0;

     player06->ratings             = buildPlayerRatings( player06->player_id );
     player06->freshman_season     = 3; // sophomore
     player06->maturity            = 3;
     player06->ratings->run_speed  = 8;
     player06->ratings->rush_power = 0;

     player07->ratings             = buildPlayerRatings( player07->player_id );
     player07->freshman_season     = 2; // junior
     player07->maturity            = 3;
     player07->ratings->run_speed  = 8;
     player07->ratings->rush_power = 0;

     player08->ratings             = buildPlayerRatings( player08->player_id );
     player08->freshman_season     = 1; // senior
     player08->maturity            = 3;
     player08->ratings->run_speed  = 8;
     player08->ratings->rush_power = 0;

     // maturity 2
     player_s *player09 = org->conferences[0].conference->teams[0].team->players[ 8].player;
     player_s *player10 = org->conferences[0].conference->teams[0].team->players[ 9].player;
     player_s *player11 = org->conferences[0].conference->teams[0].team->players[10].player;
     player_s *player12 = org->conferences[0].conference->teams[0].team->players[11].player;

     player09->ratings             = buildPlayerRatings( player09->player_id );
     player09->freshman_season     = 4; // freshman
     player09->maturity            = 2;
     player09->ratings->run_speed  = 8;
     player09->ratings->rush_power = 0;

     player10->ratings             = buildPlayerRatings( player10->player_id );
     player10->freshman_season     = 3; // sophomore
     player10->maturity            = 2;
     player10->ratings->run_speed  = 8;
     player10->ratings->rush_power = 0;

     player11->ratings             = buildPlayerRatings( player11->player_id );
     player11->freshman_season     = 2; // junior
     player11->maturity            = 2;
     player11->ratings->run_speed  = 8;
     player11->ratings->rush_power = 0;

     player12->ratings             = buildPlayerRatings( player12->player_id );
     player12->freshman_season     = 1; // senior
     player12->maturity            = 2;
     player12->ratings->run_speed  = 8;
     player12->ratings->rush_power = 0;

     // maturity 1
     player_s *player13 = org->conferences[0].conference->teams[0].team->players[12].player;
     player_s *player14 = org->conferences[0].conference->teams[0].team->players[13].player;
     player_s *player15 = org->conferences[0].conference->teams[0].team->players[14].player;
     player_s *player16 = org->conferences[0].conference->teams[0].team->players[15].player;

     player13->ratings             = buildPlayerRatings( player13->player_id );
     player13->freshman_season     = 4; // freshman
     player13->maturity            = 1;
     player13->ratings->run_speed  = 8;
     player13->ratings->rush_power = 0;

     player14->ratings             = buildPlayerRatings( player14->player_id );
     player14->freshman_season     = 3; // sophomore
     player14->maturity            = 1;
     player14->ratings->run_speed  = 8;
     player14->ratings->rush_power = 0;

     player15->ratings             = buildPlayerRatings( player15->player_id );
     player15->freshman_season     = 2; // junior
     player15->maturity            = 1;
     player15->ratings->run_speed  = 8;
     player15->ratings->rush_power = 0;

     player16->ratings             = buildPlayerRatings( player16->player_id );
     player16->freshman_season     = 1; // senior
     player16->maturity            = 1;
     player16->ratings->run_speed  = 8;
     player16->ratings->rush_power = 0;

     assertEquals( bl_True, populateRoms( &tsbrom1, &tsbrom2, org ) );

     assertEquals( 8, player01->ratings->run_speed  );
     assertEquals( 0, player01->ratings->rush_power );

     assertEquals( 8, player02->ratings->run_speed  );
     assertEquals( 0, player02->ratings->rush_power );

     assertEquals( 8, player03->ratings->run_speed  );
     assertEquals( 0, player03->ratings->rush_power );

     assertEquals( 8, player04->ratings->run_speed  );
     assertEquals( 0, player04->ratings->rush_power );

     assertEquals( 7, player05->ratings->run_speed  );
     assertEquals( 0, player05->ratings->rush_power );

     assertEquals( 7, player06->ratings->run_speed  );
     assertEquals( 0, player06->ratings->rush_power );

     assertEquals( 8, player07->ratings->run_speed  );
     assertEquals( 0, player07->ratings->rush_power );

     assertEquals( 8, player08->ratings->run_speed  );
     assertEquals( 0, player08->ratings->rush_power );

     assertEquals( 6, player09->ratings->run_speed  );
     assertEquals( 0, player09->ratings->rush_power );

     assertEquals( 7, player10->ratings->run_speed  );
     assertEquals( 0, player10->ratings->rush_power );

     assertEquals( 7, player11->ratings->run_speed  );
     assertEquals( 0, player11->ratings->rush_power );

     assertEquals( 8, player12->ratings->run_speed  );
     assertEquals( 0, player12->ratings->rush_power );

     assertEquals( 5, player13->ratings->run_speed  );
     assertEquals( 0, player13->ratings->rush_power );

     assertEquals( 6, player14->ratings->run_speed  );
     assertEquals( 0, player14->ratings->rush_power );

     assertEquals( 7, player15->ratings->run_speed  );
     assertEquals( 0, player15->ratings->rush_power );

     assertEquals( 8, player16->ratings->run_speed  );
     assertEquals( 0, player16->ratings->rush_power );

     free_organization( org );

     return NULL;
}

static char *populateRoms_ShouldLimitPlayerNamesTo16Chars()
{
     tsbrom_s        tsbrom1 = { 0 };
     tsbrom_s        tsbrom2 = { 0 };
     organization_s *org     = get_test_org();

     player_s *player1 = org->conferences[4].conference->teams[0].team->players[0].player;
     player_s *player2 = org->conferences[4].conference->teams[0].team->players[1].player;

     /**/     player1->number    =  20;
     sprintf( player1->first_name, "Waldorff" );
     sprintf( player1->last_name,  "Johanneson" );
     /**/     player2->number    =  20;
     sprintf( player2->first_name, "Gregory" );
     sprintf( player2->last_name,  "Ivanistoyanovich" );

     assertEquals( bl_True, populateRoms( &tsbrom1, &tsbrom2, org ) );

     char *expected = " w.JOHANNESON g.IVANISTOYANOVI";

     assertEqualsBfr( expected, tsbrom1.player_identifiers, strlen(expected) );

     return NULL;
}

static void check_populate_roms_error()
{
     printf( "populate roms error: %s\n", getPopulateRomsError() );
}

static void run_all_tests()
{
     run_test( populateRoms_ShouldReturnTrueOnSuccess_GivenATsbRomAndOrganization,                                    check_populate_roms_error );
     run_test( populateRoms_ShouldReturnAnErrorIfATeamIsShortPlayers_GivenATsbRomAndOrganization,                     check_populate_roms_error );
     run_test( populateRoms_ShouldWriteThePlayerIdentifiersInThePlayerIdentifiersSection_GivenATsbRomAndOrganization, check_populate_roms_error );
     run_test( populateRoms_ShouldWriteTheOffsetsInThePlayerPointersSection_GivenATsbRomAndOrganization,              check_populate_roms_error );
     run_test( populateRoms_ShouldWriteThePlayerRatingsInTheTeamPlayerRatingsSection_GivenATsbRomAndOrganization,     check_populate_roms_error );
     run_test( populateRoms_ShouldWriteTheKickAndPuntReturnersForEveryTeam,                                           check_populate_roms_error );
     run_test( populateRoms_ShouldPopulateBothRoms_GivenTwoTsbRomsAndOrganization,                                    check_populate_roms_error );
     run_test( populateRoms_ShouldAdjustPlayerRatingsBasedOnMaturity,                                                 check_populate_roms_error );
     run_test( populateRoms_ShouldLimitPlayerNamesTo16Chars,                                                          check_populate_roms_error );
}


int main( int argc, char *argv[] )
{
     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
