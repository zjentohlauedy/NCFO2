#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "builders.h"
#include "compares.h"
#include "conversion.h"
#include "organization.h"
#include "unit_test.h"


static sqlite3 *db;
static char    *result;


static void setupSaveState( nst_save_state_s *save_state )
{
     for ( int i = 0; i < 28; ++i )
     {
          int save_state_idx = (i < 9) ? i : i - 9;

          nst_stats_s *stats = (i < 9) ? &(save_state->stats1[save_state_idx]) : &(save_state->stats2[save_state_idx]);

          nst_teams_s *team_stats = stats->team_stats;

          team_stats->wins[0]   = 6;
          team_stats->losses[0] = 3;
          team_stats->ties[0]   = 1;

          int2word( 321,  team_stats->points_scored      );
          int2word( 123,  team_stats->points_allowed     );
          int2word( 2321, team_stats->pass_yards_allowed );
          int2word( 1321, team_stats->rush_yards_allowed );

          nst_players_s *player_stats = stats->player_stats;

          for ( int j = 0; j < 2; ++j )
          {
               player_stats->quarterback[j].pass_attempts             [0] = 0x8c;
               player_stats->quarterback[j].pass_completions          [0] = 0x1a;
               player_stats->quarterback[j].pass_attempts_modifier    [0] = 0x02 | 0xa4;
               player_stats->quarterback[j].pass_completions_modifier [0] = 0x02 | 0x90;
               player_stats->quarterback[j].pass_yards                [0] = 0x3e;
               player_stats->quarterback[j].rush_attempts             [0] = 0xa4;
               player_stats->quarterback[j].rush_yards                [0] = 0x5C;
               player_stats->quarterback[j].yards_modifiers           [0] = 0x06 | 0xa0;
               player_stats->quarterback[j].rush_touchdowns           [0] = 0x94;
          }

          for ( int j = 0; j < 10; ++j )
          {
               player_stats->offense[j].receptions           [0] = 0xad;
               player_stats->offense[j].receiving_yards      [0] = 0xff;
               player_stats->offense[j].pr_yards_modifier    [0] = 0x02 | 0x9c;
               player_stats->offense[j].kick_returns         [0] = 0xd0;
               player_stats->offense[j].kr_yards             [0] = 0x73;
               player_stats->offense[j].kr_yards_modifier    [0] = 0x06 | 0xa8;
               player_stats->offense[j].rush_yards_modifier1 [0] = 0x01 | 0x8c;
               player_stats->offense[j].pr_yards             [0] = 0xfb;
               player_stats->offense[j].rec_yards_modifier   [0] = 0x0d | 0xc0;
               player_stats->offense[j].rush_attempts        [0] = 0xf5;
               player_stats->offense[j].rush_yards           [0] = 0x79;
               player_stats->offense[j].rush_yards_modifier2 [0] = 0x02 | 0xb4;
          }

          for ( int j = 0; j < 11; ++j )
          {
               player_stats->defense[j].sacks         [0] = 0x01 | 0xc0;
               player_stats->defense[j].interceptions [0] = 0x01 | 0x0c | 0x80;
               player_stats->defense[j].ir_yards      [0] = 0x25;
          }

          // kicker
          player_stats->kicker[0].xp_attempts [0] = 32;
          player_stats->kicker[0].xp_made     [0] = 31;
          player_stats->kicker[0].fg_attempts [0] = 24;
          player_stats->kicker[0].fg_made     [0] = 16;

          // punter
          player_stats->punter[0].punts               [0] = 47;
          player_stats->punter[0].punt_yards          [0] = 0x8c;
          player_stats->punter[0].punt_yards_modifier [0] = 0x0c;

          stats->injuries.qb1 = 0;
          stats->injuries.qb2 = 0;
          stats->injuries.rb1 = 1;
          stats->injuries.rb2 = 1;
          stats->injuries.rb3 = 1;
          stats->injuries.rb4 = 1;
          stats->injuries.wr1 = 2;
          stats->injuries.wr2 = 2;
          stats->injuries.wr3 = 2;
          stats->injuries.wr4 = 2;
          stats->injuries.te1 = 3;
          stats->injuries.te2 = 3;

          stats->conditions.qb1 = nc_Bad;
          stats->conditions.qb2 = nc_Bad;
          stats->conditions.rb1 = nc_Average;
          stats->conditions.rb2 = nc_Average;
          stats->conditions.rb3 = nc_Average;
          stats->conditions.rb4 = nc_Average;
          stats->conditions.wr1 = nc_Good;
          stats->conditions.wr2 = nc_Good;
          stats->conditions.wr3 = nc_Good;
          stats->conditions.wr4 = nc_Good;
          stats->conditions.te1 = nc_Excellent;
          stats->conditions.te2 = nc_Excellent;
          stats->conditions.ol1 = nc_Bad;
          stats->conditions.ol2 = nc_Bad;
          stats->conditions.ol3 = nc_Bad;
          stats->conditions.ol4 = nc_Bad;
          stats->conditions.ol5 = nc_Bad;
          stats->conditions.dl1 = nc_Average;
          stats->conditions.dl2 = nc_Average;
          stats->conditions.dl3 = nc_Average;
          stats->conditions.lb1 = nc_Good;
          stats->conditions.lb2 = nc_Good;
          stats->conditions.lb3 = nc_Good;
          stats->conditions.lb4 = nc_Good;
          stats->conditions.cb1 = nc_Excellent;
          stats->conditions.cb2 = nc_Excellent;
          stats->conditions.s1  = nc_Bad;
          stats->conditions.s2  = nc_Bad;
          stats->conditions.k   = nc_Average;
          stats->conditions.p   = nc_Good;
          stats->conditions.end = 5;
     }
}

static void setupRom( tsbrom_s *rom )
{
     int team_base   = 0xbcf0;
     int team_offset = 0;

     char *abbr = "UXZ";
     char *locn = "XANADU ZERO";
     char *name = "UTOPIANS";

     for ( int i = 0; i < 28; ++i )
     {
          int2pointer( team_base + team_offset, &(rom->team_abbr_pointers[i]) );

          memcpy( rom->team_conference_names + team_offset, abbr, strlen(abbr) );

          team_offset += strlen(abbr);
     }

     for ( int i = 0; i < 28; ++i )
     {
          int2pointer( team_base + team_offset, &(rom->team_loc_pointers[i]) );

          memcpy( rom->team_conference_names + team_offset, locn, strlen(locn) );

          team_offset += strlen(locn);
     }

     for ( int i = 0; i < 28; ++i )
     {
          int2pointer( team_base + team_offset, &(rom->team_name_pointers[i]) );

          memcpy( rom->team_conference_names + team_offset, name, strlen(name) );

          team_offset += strlen(name);
     }

     for ( int i = 0; i < 28; ++i )
     {
          rom->offensive_preference[i] = 3;

          rom->formations1[i] = 1;
          rom->formations2[i] = 1;
     }

     int player_base   = 0x86ca;
     int player_offset = 0;

     int   number = 0x55;
     char *first_name = "joe";
     char *last_name  = "COOL";

     for ( int i = 0; i < 28; ++i )
     {
          for ( int j = 0; j < 30; ++j )
          {
               int2pointer( player_base + player_offset, &(rom->player_pointers[i][j]) );

               rom->player_identifiers[player_offset++] = number;

               memcpy( rom->player_identifiers + player_offset, first_name, strlen(first_name) );

               player_offset += strlen(first_name);

               memcpy( rom->player_identifiers + player_offset, last_name, strlen(last_name) );

               player_offset += strlen(last_name);
          }

          rom->sim_data[i].team[0] = 0xab;

          for ( int j = 0; j < 2; ++j )
          {
               rom->team_player_ratings[i].quarterback[j].player.ratings[0] = 0x12;
               rom->team_player_ratings[i].quarterback[j].player.ratings[1] = 0x34;
               rom->team_player_ratings[i].quarterback[j].qb_ratings[0]     = 0x56;
               rom->team_player_ratings[i].quarterback[j].qb_ratings[1]     = 0x78;

               rom->sim_data[i].quarterbacks[j][0] = 0x9a;
               rom->sim_data[i].quarterbacks[j][1] = 0x0b;
          }

          for ( int j = 0; j < 10; ++j )
          {
               rom->team_player_ratings[i].offense[j].player.ratings[0] = 0x12;
               rom->team_player_ratings[i].offense[j].player.ratings[1] = 0x34;
               rom->team_player_ratings[i].offense[j].off_ratings[0]    = 0x56;

               rom->sim_data[i].offense[j][0] = 0x78;
               rom->sim_data[i].offense[j][1] = 0x9a;
          }

          for ( int j = 0; j < 5; ++j )
          {
               rom->team_player_ratings[i].linesmen[j].ratings[0] = 0x12;
               rom->team_player_ratings[i].linesmen[j].ratings[1] = 0x34;
          }

          for ( int j = 0; j < 11; ++j )
          {
               rom->team_player_ratings[i].defense[j].player.ratings[0] = 0x12;
               rom->team_player_ratings[i].defense[j].player.ratings[1] = 0x34;
               rom->team_player_ratings[i].defense[j].def_ratings[0]    = 0x56;

               rom->sim_data[i].defense_pass_rush[j] = 123;
               rom->sim_data[i].defense_coverage [j] = 210;
          }

          for ( int j = 0; j < 2; ++j )
          {
               rom->team_player_ratings[i].kickers[j].player.ratings[0] = 0x12;
               rom->team_player_ratings[i].kickers[j].player.ratings[1] = 0x34;
               rom->team_player_ratings[i].kickers[j].kick_ratings[0]   = 0x56;
          }

          rom->sim_data[i].kicking[0] = 0x78;
     }

     int2pointer( player_base + player_offset, rom->end_player_pointers );
}


static char *convertOrganization_ShouldReturnAnOrganizationObject_GivenTsbRomsAndSaveStates()
{
     tsbrom_s          rom1        = { 0 };
     tsbrom_s          rom2        = { 0 };
     nst_save_state_s  save_state1 = { 0 };
     nst_save_state_s  save_state2 = { 0 };

     setupRom( &rom1 );
     setupRom( &rom2 );

     organization_s *org = convertOrganization( &rom1, &save_state1, &rom2, &save_state2, 0, bg_None );

     assertNotNull( org );

     assertEqualsInt( 1,                                        org->organization_id );
     assertEqualsStr( "National College Football Organization", org->name            );
     assertEqualsStr( "NCFO",                                   org->abbreviation    );
     assertEqualsInt( 1,                                        org->season          );

     free_organization( org );

     return NULL;
}

static char *convertOrganization_ShouldReturnAnOrganizationObjectWithConferences()
{
     tsbrom_s          rom1        = { 0 };
     tsbrom_s          rom2        = { 0 };
     nst_save_state_s  save_state1 = { 0 };
     nst_save_state_s  save_state2 = { 0 };

     setupRom( &rom1 );
     setupRom( &rom2 );

     organization_s *org = convertOrganization( &rom1, &save_state1, &rom2, &save_state2, 0, bg_None );

     assertNotNull( org              );
     assertNotNull( org->conferences );

     for ( int i = 0; i < 8; ++i )
     {
          assertNotNull( org->conferences[i].conference );

          assertEquals( 1,     org->conferences[i].organization_id           );
          assertEquals( i + 1, org->conferences[i].conference_id             );
          assertEquals( i + 1, org->conferences[i].conference->conference_id );
     }

     assertEquals( -1, org->conferences[8].organization_id );
     assertEquals( -1, org->conferences[8].conference_id   );

     assertEqualsStr( "New England", org->conferences[0].conference->name );
     assertEqualsStr( "Atlantic",    org->conferences[1].conference->name );
     assertEqualsStr( "Southeast",   org->conferences[2].conference->name );
     assertEqualsStr( "Great Lake",  org->conferences[3].conference->name );
     assertEqualsStr( "Southwest",   org->conferences[4].conference->name );
     assertEqualsStr( "Northwest",   org->conferences[5].conference->name );
     assertEqualsStr( "Midwest",     org->conferences[6].conference->name );
     assertEqualsStr( "South",       org->conferences[7].conference->name );

     free_organization( org );

     return NULL;
}

static char *convertOrganization_ShouldReturnAnOrganizationObjectWithTeams()
{
     tsbrom_s          rom1        = { 0 };
     tsbrom_s          rom2        = { 0 };
     nst_save_state_s  save_state1 = { 0 };
     nst_save_state_s  save_state2 = { 0 };

     setupRom( &rom1 );
     setupRom( &rom2 );

     organization_s *org = convertOrganization( &rom1, &save_state1, &rom2, &save_state2, 0, bg_None );

     assertNotNull( org                                   );
     assertNotNull( org->conferences                      );
     assertNotNull( org->conferences[0].conference        );
     assertNotNull( org->conferences[0].conference->teams );

     conference_team_s *teams = org->conferences[0].conference->teams;

     for ( int i = 0; i < 6; ++i )
     {
          assertNotNull( teams[i].team );

          assertEqualsInt( 1,              teams[i].conference_id              );
          assertEqualsInt( i + 1,          teams[i].team_id                    );
          assertEqualsInt( i + 1,          teams[i].team->team_id              );
          assertEqualsBfr( "UXZ",          teams[i].team->abbreviation,      3 );
          assertEqualsBfr( "Xanadu Zero",  teams[i].team->location,         11 );
          assertEqualsBfr( "Utopians",     teams[i].team->name,              8 );

          assertEqualsInt( 10,             teams[i].team->sim_offense          );
          assertEqualsInt( 11,             teams[i].team->sim_defense          );

          assertEqualsInt( form_Four_Wide, teams[i].team->offensive_formation  );
          assertEqualsInt( pref_HeavyPass, teams[i].team->offensive_preference );
     }

     assertEquals( -1, teams[6].conference_id );
     assertEquals( -1, teams[6].team_id       );

     free_organization( org );

     return NULL;
}

static char *convertOrganization_ShouldReturnAnOrganizationObjectWithTeamsWithStats()
{
     tsbrom_s          rom1        = { 0 };
     tsbrom_s          rom2        = { 0 };
     nst_save_state_s  save_state1 = { 0 };
     nst_save_state_s  save_state2 = { 0 };

     setupRom( &rom1 );
     setupRom( &rom2 );

     setupSaveState( &save_state1 );
     setupSaveState( &save_state2 );

     organization_s *org = convertOrganization( &rom1, &save_state1, &rom2, &save_state2, 3, bg_OrangeBowl );

     assertNotNull( org                                   );
     assertNotNull( org->conferences                      );
     assertNotNull( org->conferences[0].conference        );
     assertNotNull( org->conferences[0].conference->teams );

     conference_team_s *teams = org->conferences[0].conference->teams;

     for ( int i = 0; i < 6; ++i )
     {
          assertNotNull( teams[i].team );
          assertNotNull( teams[i].team->stats );

          team_stats_s *stats = teams[i].team->stats;

          assertEquals( i + 1,         stats->team_id        );
          assertEquals( 3,             stats->season         );
          assertEquals( bg_OrangeBowl, stats->bowl_game      );
          assertEquals( 6,             stats->wins           );
          assertEquals( 3,             stats->losses         );
          assertEquals( 1,             stats->ties           );
          assertEquals( 0,             stats->home_wins      );
          assertEquals( 0,             stats->home_losses    );
          assertEquals( 0,             stats->home_ties      );
          assertEquals( 0,             stats->road_wins      );
          assertEquals( 0,             stats->road_losses    );
          assertEquals( 0,             stats->road_ties      );
          assertEquals( 321,           stats->points_scored  );
          assertEquals( 123,           stats->points_allowed );
     }

     free_organization( org );

     return NULL;
}

static char *convertOrganization_ShouldReturnAnOrganizationObjectWithTeamsWithOffenseStats()
{
     tsbrom_s          rom1        = { 0 };
     tsbrom_s          rom2        = { 0 };
     nst_save_state_s  save_state1 = { 0 };
     nst_save_state_s  save_state2 = { 0 };

     setupRom( &rom1 );
     setupRom( &rom2 );

     setupSaveState( &save_state1 );
     setupSaveState( &save_state2 );

     organization_s *org = convertOrganization( &rom1, &save_state1, &rom2, &save_state2, 3, bg_OrangeBowl );

     assertNotNull( org                                   );
     assertNotNull( org->conferences                      );
     assertNotNull( org->conferences[0].conference        );
     assertNotNull( org->conferences[0].conference->teams );

     conference_team_s *teams = org->conferences[0].conference->teams;

     for ( int i = 0; i < 6; ++i )
     {
          assertNotNull( teams[i].team );
          assertNotNull( teams[i].team->offense_stats );

          team_offense_stats_s *offense = teams[i].team->offense_stats;

          assertEquals(         i + 1, offense->team_id         );
          assertEquals(             3, offense->season          );
          assertEquals( bg_OrangeBowl, offense->bowl_game       );
          assertEquals(          1304, offense->pass_attempts   );
          assertEquals(          1076, offense->completions     );
          assertEquals(            72, offense->interceptions   );
          assertEquals(         10364, offense->pass_yards      );
          assertEquals(            82, offense->pass_touchdowns );
          assertEquals(          2778, offense->rush_attempts   );
          assertEquals(         27506, offense->rush_yards      );
          assertEquals(           524, offense->rush_touchdowns );
     }

     free_organization( org );

     return NULL;
}

static char *convertOrganization_ShouldReturnAnOrganizationObjectWithTeamsWithDefenseStats()
{
     tsbrom_s          rom1        = { 0 };
     tsbrom_s          rom2        = { 0 };
     nst_save_state_s  save_state1 = { 0 };
     nst_save_state_s  save_state2 = { 0 };

     setupRom( &rom1 );
     setupRom( &rom2 );

     setupSaveState( &save_state1 );
     setupSaveState( &save_state2 );

     organization_s *org = convertOrganization( &rom1, &save_state1, &rom2, &save_state2, 3, bg_OrangeBowl );

     assertNotNull( org                                   );
     assertNotNull( org->conferences                      );
     assertNotNull( org->conferences[0].conference        );
     assertNotNull( org->conferences[0].conference->teams );

     conference_team_s *teams = org->conferences[0].conference->teams;

     for ( int i = 0; i < 6; ++i )
     {
          assertNotNull( teams[i].team );
          assertNotNull( teams[i].team->defense_stats );

          team_defense_stats_s *defense = teams[i].team->defense_stats;

          assertEquals(         i + 1, defense->team_id           );
          assertEquals(             3, defense->season            );
          assertEquals( bg_OrangeBowl, defense->bowl_game         );
          assertEquals(          1056, defense->sacks             );
          assertEquals(           264, defense->interceptions     );
          assertEquals(          3223, defense->return_yards      );
          assertEquals(            66, defense->return_touchdowns );
     }

     free_organization( org );

     return NULL;
}

static char *convertOrganization_ShouldReturnAnOrganizationObjectWithTeamsWithKickingStats()
{
     tsbrom_s          rom1        = { 0 };
     tsbrom_s          rom2        = { 0 };
     nst_save_state_s  save_state1 = { 0 };
     nst_save_state_s  save_state2 = { 0 };

     setupRom( &rom1 );
     setupRom( &rom2 );

     setupSaveState( &save_state1 );
     setupSaveState( &save_state2 );

     organization_s *org = convertOrganization( &rom1, &save_state1, &rom2, &save_state2, 3, bg_OrangeBowl );

     assertNotNull( org                                   );
     assertNotNull( org->conferences                      );
     assertNotNull( org->conferences[0].conference        );
     assertNotNull( org->conferences[0].conference->teams );

     conference_team_s *teams = org->conferences[0].conference->teams;

     for ( int i = 0; i < 6; ++i )
     {
          assertNotNull( teams[i].team );
          assertNotNull( teams[i].team->kicking_stats );

          team_kicking_stats_s *kicking = teams[i].team->kicking_stats;

          assertEquals(         i + 1, kicking->team_id                );
          assertEquals(             3, kicking->season                 );
          assertEquals( bg_OrangeBowl, kicking->bowl_game              );
          assertEquals(            32, kicking->extra_point_attempts   );
          assertEquals(            31, kicking->extra_points_made      );
          assertEquals(            24, kicking->field_goal_attempts    );
          assertEquals(            16, kicking->field_goals_made       );
          assertEquals(            47, kicking->punts                  );
          assertEquals(          3212, kicking->punt_yards             );
          assertEquals(          1040, kicking->kick_returns           );
          assertEquals(         16510, kicking->kick_return_yards      );
          assertEquals(           210, kicking->kick_return_touchdowns );
          assertEquals(           350, kicking->punt_returns           );
          assertEquals(          7630, kicking->punt_return_yards      );
          assertEquals(           120, kicking->punt_return_touchdowns );
     }

     free_organization( org );

     return NULL;
}

static char *convertOrganization_ShouldReturnAnOrganizationObjectWithPlayers()
{
     tsbrom_s          rom1        = { 0 };
     tsbrom_s          rom2        = { 0 };
     nst_save_state_s  save_state1 = { 0 };
     nst_save_state_s  save_state2 = { 0 };

     setupRom( &rom1 );
     setupRom( &rom2 );

     setupSaveState( &save_state1 );
     setupSaveState( &save_state2 );

     for ( int i = 0; i < 28; ++i )
     {
          rom1.formations1[i] = 0;
          rom1.formations2[i] = 0;
          rom2.formations1[i] = 0;
          rom2.formations2[i] = 0;
     }

     organization_s *org = convertOrganization( &rom1, &save_state1, &rom2, &save_state2, 0, bg_None );

     assertNotNull( org                                                    );
     assertNotNull( org->conferences                                       );
     assertNotNull( org->conferences[0].conference                         );
     assertNotNull( org->conferences[0].conference->teams                  );
     assertNotNull( org->conferences[0].conference->teams[0].team          );
     assertNotNull( org->conferences[0].conference->teams[0].team->players );

     team_player_s *players = org->conferences[0].conference->teams[0].team->players;

     for ( int i = 0; i < 30; ++i )
     {
          assertNotNull( players[i].player );

          assertEqualsInt( 1,      players[i].team_id            );
          assertEqualsInt( i + 1,  players[i].player_id          );
          assertEqualsInt( i + 1,  players[i].player->player_id  );
          assertEqualsInt( 55,     players[i].player->number     );
          assertEqualsStr( "Joe",  players[i].player->first_name );
          assertEqualsStr( "Cool", players[i].player->last_name  );

          switch ( i )
          {
          case  0: assertEquals( cnd_Bad,       players[i].player->condition ); assertEquals( 0, players[i].player->injuries ); break;
          case  1: assertEquals( cnd_Bad,       players[i].player->condition ); assertEquals( 0, players[i].player->injuries ); break;
          case  2: assertEquals( cnd_Average,   players[i].player->condition ); assertEquals( 1, players[i].player->injuries ); break;
          case  3: assertEquals( cnd_Average,   players[i].player->condition ); assertEquals( 1, players[i].player->injuries ); break;
          case  4: assertEquals( cnd_Average,   players[i].player->condition ); assertEquals( 1, players[i].player->injuries ); break;
          case  5: assertEquals( cnd_Average,   players[i].player->condition ); assertEquals( 1, players[i].player->injuries ); break;
          case  6: assertEquals( cnd_Good,      players[i].player->condition ); assertEquals( 2, players[i].player->injuries ); break;
          case  7: assertEquals( cnd_Good,      players[i].player->condition ); assertEquals( 2, players[i].player->injuries ); break;
          case  8: assertEquals( cnd_Good,      players[i].player->condition ); assertEquals( 2, players[i].player->injuries ); break;
          case  9: assertEquals( cnd_Good,      players[i].player->condition ); assertEquals( 2, players[i].player->injuries ); break;
          case 10: assertEquals( cnd_Excellent, players[i].player->condition ); assertEquals( 3, players[i].player->injuries ); break;
          case 11: assertEquals( cnd_Excellent, players[i].player->condition ); assertEquals( 3, players[i].player->injuries ); break;
          case 12: assertEquals( cnd_Bad,       players[i].player->condition );                                                 break;
          case 13: assertEquals( cnd_Bad,       players[i].player->condition );                                                 break;
          case 14: assertEquals( cnd_Bad,       players[i].player->condition );                                                 break;
          case 15: assertEquals( cnd_Bad,       players[i].player->condition );                                                 break;
          case 16: assertEquals( cnd_Bad,       players[i].player->condition );                                                 break;
          case 17: assertEquals( cnd_Average,   players[i].player->condition );                                                 break;
          case 18: assertEquals( cnd_Average,   players[i].player->condition );                                                 break;
          case 19: assertEquals( cnd_Average,   players[i].player->condition );                                                 break;
          case 20: assertEquals( cnd_Good,      players[i].player->condition );                                                 break;
          case 21: assertEquals( cnd_Good,      players[i].player->condition );                                                 break;
          case 22: assertEquals( cnd_Good,      players[i].player->condition );                                                 break;
          case 23: assertEquals( cnd_Good,      players[i].player->condition );                                                 break;
          case 24: assertEquals( cnd_Excellent, players[i].player->condition );                                                 break;
          case 25: assertEquals( cnd_Excellent, players[i].player->condition );                                                 break;
          case 26: assertEquals( cnd_Bad,       players[i].player->condition );                                                 break;
          case 27: assertEquals( cnd_Bad,       players[i].player->condition );                                                 break;
          case 28: assertEquals( cnd_Average,   players[i].player->condition );                                                 break;
          case 29: assertEquals( cnd_Good,      players[i].player->condition );                                                 break;
          }

          switch ( i )
          {
          case  0:
          case  1: assertEquals( pos_Quarterback,   players[i].player->position ); break;
          case  2:
          case  3:
          case  4:
          case  5: assertEquals( pos_Runningback,   players[i].player->position ); break;
          case  6:
          case  7:
          case  8:
          case  9: assertEquals( pos_WideReceiver,  players[i].player->position ); break;
          case 10:
          case 11: assertEquals( pos_TightEnd,      players[i].player->position ); break;
          case 12:
          case 13:
          case 14:
          case 15:
          case 16: assertEquals( pos_OffensiveLine, players[i].player->position ); break;
          case 17:
          case 18:
          case 19: assertEquals( pos_DefensiveLine, players[i].player->position ); break;
          case 20:
          case 21:
          case 22:
          case 23: assertEquals( pos_Linebacker,    players[i].player->position ); break;
          case 24:
          case 25: assertEquals( pos_Cornerback,    players[i].player->position ); break;
          case 26:
          case 27: assertEquals( pos_Safety,        players[i].player->position ); break;
          case 28: assertEquals( pos_Kicker,        players[i].player->position ); break;
          case 29: assertEquals( pos_Punter,        players[i].player->position ); break;
          }
     }

     assertEquals( -1, players[30].team_id   );
     assertEquals( -1, players[30].player_id );

     free_organization( org );

     return NULL;
}

static char *convertOrganization_ShouldReturnAnOrganizationObjectWithPlayersWithRatings()
{
     tsbrom_s          rom1        = { 0 };
     tsbrom_s          rom2        = { 0 };
     nst_save_state_s  save_state1 = { 0 };
     nst_save_state_s  save_state2 = { 0 };

     setupRom( &rom1 );
     setupRom( &rom2 );

     organization_s *org = convertOrganization( &rom1, &save_state1, &rom2, &save_state2, 0, bg_None );

     assertNotNull( org                                                    );
     assertNotNull( org->conferences                                       );
     assertNotNull( org->conferences[0].conference                         );
     assertNotNull( org->conferences[0].conference->teams                  );
     assertNotNull( org->conferences[0].conference->teams[0].team          );
     assertNotNull( org->conferences[0].conference->teams[0].team->players );

     team_player_s *players = org->conferences[0].conference->teams[0].team->players;

     for ( int i = 0; i < 30; ++i )
     {
          assertNotNull( players[i].player          );
          assertNotNull( players[i].player->ratings );

          player_ratings_s *ratings = players[i].player->ratings;

          assertEquals( 2, ratings->run_speed  );
          assertEquals( 1, ratings->rush_power );
          assertEquals( 3, ratings->max_speed  );
          assertEquals( 4, ratings->hit_power  );

          if ( players[i].player->position == pos_Quarterback )
          {
               player_quarterback_ratings_s *qb_ratings = players[i].player->extra_ratings.quarterback;

               assertNotNull( qb_ratings );

               assertEquals(  5, qb_ratings->pass_speed       );
               assertEquals(  6, qb_ratings->pass_control     );
               assertEquals(  7, qb_ratings->pass_accuracy    );
               assertEquals(  8, qb_ratings->avoid_pass_block );

               assertEquals(  9, qb_ratings->sim_rush         );
               assertEquals( 10, qb_ratings->sim_pass         );
               assertEquals( 11, qb_ratings->sim_pocket       );
          }
          else if ( players[i].player->position == pos_Runningback  ||
                    players[i].player->position == pos_WideReceiver ||
                    players[i].player->position == pos_TightEnd        )
          {
               player_offense_ratings_s *off_ratings = players[i].player->extra_ratings.offense;

               assertNotNull( off_ratings );

               assertEquals(  5, off_ratings->ball_control );
               assertEquals(  6, off_ratings->receiving    );

               assertEquals(  7, off_ratings->sim_rush     );
               assertEquals(  8, off_ratings->sim_catch    );
               assertEquals(  9, off_ratings->sim_yards    );
               assertEquals( 10, off_ratings->sim_target   );
          }
          else if ( players[i].player->position == pos_DefensiveLine ||
                    players[i].player->position == pos_Linebacker    ||
                    players[i].player->position == pos_Cornerback    ||
                    players[i].player->position == pos_Safety           )
          {
               player_defense_ratings_s *def_ratings = players[i].player->extra_ratings.defense;

               assertNotNull( def_ratings );

               assertEquals(   5, def_ratings->interceptions );
               assertEquals(   6, def_ratings->quickness     );

               assertEquals( 123, def_ratings->sim_pass_rush );
               assertEquals( 210, def_ratings->sim_coverage  );
          }
          else if ( players[i].player->position == pos_Kicker ||
                    players[i].player->position == pos_Punter    )
          {
               player_kicking_ratings_s *kick_ratings = players[i].player->extra_ratings.kicking;

               assertNotNull( kick_ratings );

               assertEquals( 5, kick_ratings->kicking_ability  );
               assertEquals( 6, kick_ratings->avoid_kick_block );

               if   ( players[i].player->position == pos_Kicker ) assertEquals( 7, kick_ratings->sim_kicking );
               else                                               assertEquals( 8, kick_ratings->sim_kicking );
          }
     }

     free_organization( org );

     return NULL;
}

static char *convertOrganization_ShouldReturnAnOrganizationObjectWithPlayersWithStats()
{
     tsbrom_s          rom1        = { 0 };
     tsbrom_s          rom2        = { 0 };
     nst_save_state_s  save_state1 = { 0 };
     nst_save_state_s  save_state2 = { 0 };

     setupRom( &rom1 );
     setupRom( &rom2 );

     setupSaveState( &save_state1 );
     setupSaveState( &save_state2 );

     organization_s *org = convertOrganization( &rom1, &save_state1, &rom2, &save_state2, 5, bg_SugarBowl );

     assertNotNull( org                                                    );
     assertNotNull( org->conferences                                       );
     assertNotNull( org->conferences[0].conference                         );
     assertNotNull( org->conferences[0].conference->teams                  );
     assertNotNull( org->conferences[0].conference->teams[0].team          );
     assertNotNull( org->conferences[0].conference->teams[0].team->players );

     team_player_s *players = org->conferences[0].conference->teams[0].team->players;

     for ( int i = 0; i < 30; ++i )
     {
          assertNotNull( players[i].player );

          if ( players[i].player->position == pos_Quarterback )
          {
               assertNotNull( players[i].player->stats.offense );
               assertNull(    players[i].player->stats.defense );
               assertNull(    players[i].player->stats.kicking );
               assertNull(    players[i].player->stats.returns );

               player_offense_stats_s *off_stats = players[i].player->stats.offense;

               assertEquals(        i + 1, off_stats->player_id            );
               assertEquals(            5, off_stats->season               );
               assertEquals( bg_SugarBowl, off_stats->bowl_game            );
               assertEquals(          652, off_stats->pass_attempts        );
               assertEquals(          538, off_stats->completions          );
               assertEquals(           36, off_stats->interceptions        );
               assertEquals(         5182, off_stats->pass_yards           );
               assertEquals(           41, off_stats->pass_touchdowns      );
               assertEquals(          164, off_stats->rush_attempts        );
               assertEquals(         1628, off_stats->rush_yards           );
               assertEquals(           37, off_stats->rush_touchdowns      );
               assertEquals(            0, off_stats->receptions           );
               assertEquals(            0, off_stats->receiving_yards      );
               assertEquals(            0, off_stats->receiving_touchdowns );
          }
          else if ( players[i].player->position == pos_Runningback  ||
                    players[i].player->position == pos_WideReceiver ||
                    players[i].player->position == pos_TightEnd        )
          {
               assertNotNull( players[i].player->stats.offense );
               assertNull(    players[i].player->stats.defense );
               assertNull(    players[i].player->stats.kicking );
               assertNotNull( players[i].player->stats.returns );

               player_offense_stats_s *off_stats = players[i].player->stats.offense;

               assertEquals(        i + 1, off_stats->player_id            );
               assertEquals(            5, off_stats->season               );
               assertEquals( bg_SugarBowl, off_stats->bowl_game            );
               assertEquals(            0, off_stats->pass_attempts        );
               assertEquals(            0, off_stats->completions          );
               assertEquals(            0, off_stats->interceptions        );
               assertEquals(            0, off_stats->pass_yards           );
               assertEquals(            0, off_stats->pass_touchdowns      );
               assertEquals(          245, off_stats->rush_attempts        );
               assertEquals(         2425, off_stats->rush_yards           );
               assertEquals(           45, off_stats->rush_touchdowns      );
               assertEquals(          173, off_stats->receptions           );
               assertEquals(         3583, off_stats->receiving_yards      );
               assertEquals(           39, off_stats->receiving_touchdowns );

               player_returns_stats_s *ret_stats = players[i].player->stats.returns;

               assertEquals(        i + 1, ret_stats->player_id              );
               assertEquals(            5, ret_stats->season                 );
               assertEquals( bg_SugarBowl, ret_stats->bowl_game              );
               assertEquals(          104, ret_stats->kick_returns           );
               assertEquals(         1651, ret_stats->kick_return_yards      );
               assertEquals(           21, ret_stats->kick_return_touchdowns );
               assertEquals(           35, ret_stats->punt_returns           );
               assertEquals(          763, ret_stats->punt_return_yards      );
               assertEquals(           12, ret_stats->punt_return_touchdowns );
          }
          else if ( players[i].player->position == pos_DefensiveLine ||
                    players[i].player->position == pos_Linebacker    ||
                    players[i].player->position == pos_Cornerback    ||
                    players[i].player->position == pos_Safety           )
          {
               assertNull(    players[i].player->stats.offense );
               assertNotNull( players[i].player->stats.defense );
               assertNull(    players[i].player->stats.kicking );
               assertNull(    players[i].player->stats.returns );

               player_defense_stats_s *def_stats = players[i].player->stats.defense;

               assertEquals(        i + 1, def_stats->player_id         );
               assertEquals(            5, def_stats->season            );
               assertEquals( bg_SugarBowl, def_stats->bowl_game         );
               assertEquals(           96, def_stats->sacks             );
               assertEquals(           24, def_stats->interceptions     );
               assertEquals(          293, def_stats->return_yards      );
               assertEquals(            6, def_stats->return_touchdowns );
          }
          else if ( players[i].player->position == pos_Kicker )
          {
               assertNull(    players[i].player->stats.offense );
               assertNull(    players[i].player->stats.defense );
               assertNotNull( players[i].player->stats.kicking );
               assertNull(    players[i].player->stats.returns );

               player_kicking_stats_s *kick_stats = players[i].player->stats.kicking;

               assertEquals(        i + 1, kick_stats->player_id            );
               assertEquals(            5, kick_stats->season               );
               assertEquals( bg_SugarBowl, kick_stats->bowl_game            );
               assertEquals(           32, kick_stats->extra_point_attempts );
               assertEquals(           31, kick_stats->extra_points_made    );
               assertEquals(           24, kick_stats->field_goal_attempts  );
               assertEquals(           16, kick_stats->field_goals_made     );
               assertEquals(            0, kick_stats->punts                );
               assertEquals(            0, kick_stats->punt_yards           );
          }
          else if ( players[i].player->position == pos_Punter )
          {
               assertNull(    players[i].player->stats.offense );
               assertNull(    players[i].player->stats.defense );
               assertNotNull( players[i].player->stats.kicking );
               assertNull(    players[i].player->stats.returns );

               player_kicking_stats_s *kick_stats = players[i].player->stats.kicking;

               assertEquals(        i + 1, kick_stats->player_id            );
               assertEquals(            5, kick_stats->season               );
               assertEquals( bg_SugarBowl, kick_stats->bowl_game            );
               assertEquals(            0, kick_stats->extra_point_attempts );
               assertEquals(            0, kick_stats->extra_points_made    );
               assertEquals(            0, kick_stats->field_goal_attempts  );
               assertEquals(            0, kick_stats->field_goals_made     );
               assertEquals(           47, kick_stats->punts                );
               assertEquals(         3212, kick_stats->punt_yards           );
          }
     }

     free_organization( org );

     return NULL;
}

static void check_generate_org_error()
{
     printf( "convert organization error: %s\n", getConvertOrganizationError() );
}

static void run_all_tests()
{
     run_test( convertOrganization_ShouldReturnAnOrganizationObject_GivenTsbRomsAndSaveStates, check_generate_org_error );
     run_test( convertOrganization_ShouldReturnAnOrganizationObjectWithConferences,            check_generate_org_error );
     run_test( convertOrganization_ShouldReturnAnOrganizationObjectWithTeams,                  check_generate_org_error );
     run_test( convertOrganization_ShouldReturnAnOrganizationObjectWithTeamsWithStats,         check_generate_org_error );
     run_test( convertOrganization_ShouldReturnAnOrganizationObjectWithTeamsWithOffenseStats,  check_generate_org_error );
     run_test( convertOrganization_ShouldReturnAnOrganizationObjectWithTeamsWithDefenseStats,  check_generate_org_error );
     run_test( convertOrganization_ShouldReturnAnOrganizationObjectWithTeamsWithKickingStats,  check_generate_org_error );
     run_test( convertOrganization_ShouldReturnAnOrganizationObjectWithPlayers,                check_generate_org_error );
     run_test( convertOrganization_ShouldReturnAnOrganizationObjectWithPlayersWithRatings,     check_generate_org_error );
     run_test( convertOrganization_ShouldReturnAnOrganizationObjectWithPlayersWithStats,       check_generate_org_error );
}

int main( int argc, char *argv[] )
{
     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
