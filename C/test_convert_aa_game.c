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
     save_state->current_week[0] = 17;
     save_state->game_of_week[0] =  4;

     save_state->playoffs[4].match.road[0] = 5;
     save_state->playoffs[4].match.home[0] = 2;

     save_state->playoffs[4].road_score[0] = 0x27;
     save_state->playoffs[4].home_score[0] = 0x14;

     for ( int i = 0; i < 2; ++i )
     {
          save_state->road_team[0].quarterback[i].pass_attempts[0]    = 18;
          save_state->road_team[0].quarterback[i].pass_completions[0] = 12;
          save_state->road_team[0].quarterback[i].pass_touchdowns[0]  =  2;
          save_state->road_team[0].quarterback[i].interceptions[0]    =  1;
          save_state->road_team[0].quarterback[i].rush_attempts[0]    =  5;
          save_state->road_team[0].quarterback[i].rush_touchdowns[0]  =  1;

          int2word( 284, save_state->road_team[0].quarterback[i].pass_yards );
          int2word(  13, save_state->road_team[0].quarterback[i].rush_yards );

          save_state->home_team[0].quarterback[i].pass_attempts[0]    = 18;
          save_state->home_team[0].quarterback[i].pass_completions[0] = 12;
          save_state->home_team[0].quarterback[i].pass_touchdowns[0]  =  2;
          save_state->home_team[0].quarterback[i].interceptions[0]    =  1;
          save_state->home_team[0].quarterback[i].rush_attempts[0]    =  5;
          save_state->home_team[0].quarterback[i].rush_touchdowns[0]  =  1;

          int2word( 284, save_state->home_team[0].quarterback[i].pass_yards );
          int2word(  13, save_state->home_team[0].quarterback[i].rush_yards );
     }

     for ( int i = 0; i < 10; ++i )
     {
          save_state->road_team[0].offense[i].receptions[0]           = 5;
          save_state->road_team[0].offense[i].receiving_touchdowns[0] = 3;
          save_state->road_team[0].offense[i].kick_returns[0]         = 2;
          save_state->road_team[0].offense[i].kr_touchdowns[0]        = 1;
          save_state->road_team[0].offense[i].punt_returns[0]         = 4;
          save_state->road_team[0].offense[i].pr_touchdowns[0]        = 1;
          save_state->road_team[0].offense[i].rush_attempts[0]        = 8;
          save_state->road_team[0].offense[i].rush_touchdowns[0]      = 2;

          int2word( 84, save_state->road_team[0].offense[i].receiving_yards );
          int2word( 35, save_state->road_team[0].offense[i].kr_yards        );
          int2word( 46, save_state->road_team[0].offense[i].pr_yards        );
          int2word( 73, save_state->road_team[0].offense[i].rush_yards      );

          save_state->home_team[0].offense[i].receptions[0]           = 5;
          save_state->home_team[0].offense[i].receiving_touchdowns[0] = 3;
          save_state->home_team[0].offense[i].kick_returns[0]         = 2;
          save_state->home_team[0].offense[i].kr_touchdowns[0]        = 1;
          save_state->home_team[0].offense[i].punt_returns[0]         = 4;
          save_state->home_team[0].offense[i].pr_touchdowns[0]        = 1;
          save_state->home_team[0].offense[i].rush_attempts[0]        = 8;
          save_state->home_team[0].offense[i].rush_touchdowns[0]      = 2;

          int2word( 84, save_state->home_team[0].offense[i].receiving_yards );
          int2word( 35, save_state->home_team[0].offense[i].kr_yards        );
          int2word( 46, save_state->home_team[0].offense[i].pr_yards        );
          int2word( 73, save_state->home_team[0].offense[i].rush_yards      );
     }

     for ( int i = 0; i < 11; ++i )
     {
          save_state->road_team[0].defense[i].sacks[0]             = 3;
          save_state->road_team[0].defense[i].interceptions[0]     = 2;
          save_state->road_team[0].defense[i].return_touchdowns[0] = 1;

          int2word( 20, save_state->road_team[0].defense[i].return_yards );

          save_state->home_team[0].defense[i].sacks[0]             = 3;
          save_state->home_team[0].defense[i].interceptions[0]     = 2;
          save_state->home_team[0].defense[i].return_touchdowns[0] = 1;

          int2word( 20, save_state->home_team[0].defense[i].return_yards );
     }

     save_state->road_team[0].kicker[0].xp_attempts[0] = 5;
     save_state->road_team[0].kicker[0].xp_made[0]     = 4;
     save_state->road_team[0].kicker[0].fg_attempts[0] = 3;
     save_state->road_team[0].kicker[0].fg_made[0]     = 2;

     save_state->home_team[0].kicker[0].xp_attempts[0] = 5;
     save_state->home_team[0].kicker[0].xp_made[0]     = 4;
     save_state->home_team[0].kicker[0].fg_attempts[0] = 3;
     save_state->home_team[0].kicker[0].fg_made[0]     = 2;

     save_state->road_team[0].punter[0].punts[0] = 5;

     int2word( 123, save_state->road_team[0].punter[0].punt_yards );

     save_state->home_team[0].punter[0].punts[0] = 5;

     int2word( 123, save_state->home_team[0].punter[0].punt_yards );
}

static void setupRam( unsigned char *ram )
{
     ram[ROAD_TEAM_RAM_OFFSET] = 0x1c;
     ram[HOME_TEAM_RAM_OFFSET] = 0x1d;

     ram[ROAD_TEAM_SCORE_OFFSET] = 0x24;
     ram[HOME_TEAM_SCORE_OFFSET] = 0x21;
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
          rom->team_ids[i] = i + 1;

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
               int2pointer( 456, &(rom->player_ids[i][j]) );

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

static char *convertAAGame_ShouldReturnABowlGameObject_GivenTsbRomAndSaveState()
{
     tsbrom_s          rom        = { 0 };
     nst_save_state_s  save_state = { 0 };
     unsigned char     ram[2049]  = { 0 };

     setupRom( &rom );
     setupRam(  ram );
     setupSaveState( &save_state );

     bowl_game_s *bowl_game = convertAAGame( &rom, ram, &save_state, 0, bg_None );

     assertNotNull( bowl_game            );
     assertNotNull( bowl_game->road_team );
     assertNotNull( bowl_game->home_team );

     freeBowlGame( bowl_game );

     return NULL;
}

static char *convertAAGame_ShouldReturnBowlGameWithTeamsThatPlayed_GivenTsbRomAndSaveState()
{
     tsbrom_s          rom        = { 0 };
     nst_save_state_s  save_state = { 0 };
     unsigned char     ram[2049]  = { 0 };

     setupRom( &rom );
     setupRam(  ram );
     setupSaveState( &save_state );

     bowl_game_s *bowl_game = convertAAGame( &rom, ram, &save_state, 0, bg_None );

     assertNotNull( bowl_game            );
     assertNotNull( bowl_game->road_team );
     assertNotNull( bowl_game->home_team );

     int expected_road_team_id = ram[ROAD_TEAM_RAM_OFFSET] - 27;
     int expected_home_team_id = ram[HOME_TEAM_RAM_OFFSET] - 27;

     assertEquals( expected_road_team_id, bowl_game->road_team->team_id );
     assertEquals( expected_home_team_id, bowl_game->home_team->team_id );

     freeBowlGame( bowl_game );

     return NULL;
}

static char *convertAAGame_ShouldReturnTeamsWithStatsFromGame_GivenTsbRomAndSaveState()
{
     tsbrom_s          rom        = { 0 };
     nst_save_state_s  save_state = { 0 };
     unsigned char     ram[2049]  = { 0 };

     setupRom( &rom );
     setupRam(  ram );
     setupSaveState( &save_state );

     bowl_game_s *bowl_game = convertAAGame( &rom, ram, &save_state, 5, bg_RoseBowl );

     assertNotNull( bowl_game                   );
     assertNotNull( bowl_game->road_team        );
     assertNotNull( bowl_game->home_team        );
     assertNotNull( bowl_game->road_team->stats );
     assertNotNull( bowl_game->home_team->stats );

     int expected_road_team_id = ram[ROAD_TEAM_RAM_OFFSET] - 27;
     int expected_home_team_id = ram[HOME_TEAM_RAM_OFFSET] - 27;

     int expected_road_score = hex2number( ram[ROAD_TEAM_SCORE_OFFSET] );
     int expected_home_score = hex2number( ram[HOME_TEAM_SCORE_OFFSET] );

     team_stats_s *road_stats = bowl_game->road_team->stats;
     team_stats_s *home_stats = bowl_game->home_team->stats;

     assertEquals( expected_road_team_id, road_stats->team_id        );
     assertEquals( 5,                     road_stats->season         );
     assertEquals( bg_RoseBowl,           road_stats->bowl_game      );
     assertEquals( 1,                     road_stats->wins           );
     assertEquals( 0,                     road_stats->losses         );
     assertEquals( 0,                     road_stats->ties           );
     assertEquals( 0,                     road_stats->home_wins      );
     assertEquals( 0,                     road_stats->home_losses    );
     assertEquals( 0,                     road_stats->home_ties      );
     assertEquals( 0,                     road_stats->road_wins      );
     assertEquals( 0,                     road_stats->road_losses    );
     assertEquals( 0,                     road_stats->road_ties      );
     assertEquals( expected_road_score,   road_stats->points_scored  );
     assertEquals( expected_home_score,   road_stats->points_allowed );

     assertEquals( expected_home_team_id, home_stats->team_id        );
     assertEquals( 5,                     home_stats->season         );
     assertEquals( bg_RoseBowl,           home_stats->bowl_game      );
     assertEquals( 0,                     home_stats->wins           );
     assertEquals( 1,                     home_stats->losses         );
     assertEquals( 0,                     home_stats->ties           );
     assertEquals( 0,                     home_stats->home_wins      );
     assertEquals( 0,                     home_stats->home_losses    );
     assertEquals( 0,                     home_stats->home_ties      );
     assertEquals( 0,                     home_stats->road_wins      );
     assertEquals( 0,                     home_stats->road_losses    );
     assertEquals( 0,                     home_stats->road_ties      );
     assertEquals( expected_home_score,   home_stats->points_scored  );
     assertEquals( expected_road_score,   home_stats->points_allowed );

     return NULL;
}

static char *convertAAGame_ShouldReturnTeamsWithOffenseStatsFromGame_GivenTsbRomAndSaveState()
{
     tsbrom_s          rom        = { 0 };
     nst_save_state_s  save_state = { 0 };
     unsigned char     ram[2049]  = { 0 };

     setupRom( &rom );
     setupRam(  ram );
     setupSaveState( &save_state );

     bowl_game_s *bowl_game = convertAAGame( &rom, ram, &save_state, 5, bg_RoseBowl );

     assertNotNull( bowl_game                           );
     assertNotNull( bowl_game->road_team                );
     assertNotNull( bowl_game->home_team                );
     assertNotNull( bowl_game->road_team->offense_stats );
     assertNotNull( bowl_game->home_team->offense_stats );

     int expected_road_team_id = ram[ROAD_TEAM_RAM_OFFSET] - 27;
     int expected_home_team_id = ram[HOME_TEAM_RAM_OFFSET] - 27;

     team_offense_stats_s *road_stats = bowl_game->road_team->offense_stats;
     team_offense_stats_s *home_stats = bowl_game->home_team->offense_stats;

     assertEquals( expected_road_team_id, road_stats->team_id         );
     assertEquals( 5,                     road_stats->season          );
     assertEquals( bg_RoseBowl,           road_stats->bowl_game       );
     assertEquals( 36,                    road_stats->pass_attempts   );
     assertEquals( 24,                    road_stats->completions     );
     assertEquals( 2,                     road_stats->interceptions   );
     assertEquals( 568,                   road_stats->pass_yards      );
     assertEquals( 4,                     road_stats->pass_touchdowns );
     assertEquals( 90,                    road_stats->rush_attempts   );
     assertEquals( 756,                   road_stats->rush_yards      );
     assertEquals( 22,                    road_stats->rush_touchdowns );

     assertEquals( expected_home_team_id, home_stats->team_id         );
     assertEquals( 5,                     home_stats->season          );
     assertEquals( bg_RoseBowl,           home_stats->bowl_game       );
     assertEquals( 36,                    home_stats->pass_attempts   );
     assertEquals( 24,                    home_stats->completions     );
     assertEquals( 2,                     home_stats->interceptions   );
     assertEquals( 568,                   home_stats->pass_yards      );
     assertEquals( 4,                     home_stats->pass_touchdowns );
     assertEquals( 90,                    home_stats->rush_attempts   );
     assertEquals( 756,                   home_stats->rush_yards      );
     assertEquals( 22,                    home_stats->rush_touchdowns );

     return NULL;
}

static char *convertAAGame_ShouldReturnTeamsWithDefenseStatsFromGame_GivenTsbRomAndSaveState()
{
     tsbrom_s          rom        = { 0 };
     nst_save_state_s  save_state = { 0 };
     unsigned char     ram[2049]  = { 0 };

     setupRom( &rom );
     setupRam(  ram );
     setupSaveState( &save_state );

     bowl_game_s *bowl_game = convertAAGame( &rom, ram, &save_state, 5, bg_RoseBowl );

     assertNotNull( bowl_game                           );
     assertNotNull( bowl_game->road_team                );
     assertNotNull( bowl_game->home_team                );
     assertNotNull( bowl_game->road_team->defense_stats );
     assertNotNull( bowl_game->home_team->defense_stats );

     int expected_road_team_id = ram[ROAD_TEAM_RAM_OFFSET] - 27;
     int expected_home_team_id = ram[HOME_TEAM_RAM_OFFSET] - 27;

     team_defense_stats_s *road_stats = bowl_game->road_team->defense_stats;
     team_defense_stats_s *home_stats = bowl_game->home_team->defense_stats;

     assertEquals( expected_road_team_id, road_stats->team_id           );
     assertEquals( 5,                     road_stats->season            );
     assertEquals( bg_RoseBowl,           road_stats->bowl_game         );
     assertEquals(  33,                   road_stats->sacks             );
     assertEquals(  22,                   road_stats->interceptions     );
     assertEquals( 220,                   road_stats->return_yards      );
     assertEquals(  11,                   road_stats->return_touchdowns );

     assertEquals( expected_home_team_id, home_stats->team_id           );
     assertEquals( 5,                     home_stats->season            );
     assertEquals( bg_RoseBowl,           home_stats->bowl_game         );
     assertEquals(  33,                   home_stats->sacks             );
     assertEquals(  22,                   home_stats->interceptions     );
     assertEquals( 220,                   home_stats->return_yards      );
     assertEquals(  11,                   home_stats->return_touchdowns );

     return NULL;
}

static char *convertAAGame_ShouldReturnTeamsWithKickingStatsFromGame_GivenTsbRomAndSaveState()
{
     tsbrom_s          rom        = { 0 };
     nst_save_state_s  save_state = { 0 };
     unsigned char     ram[2049]  = { 0 };

     setupRom( &rom );
     setupRam(  ram );
     setupSaveState( &save_state );

     bowl_game_s *bowl_game = convertAAGame( &rom, ram, &save_state, 5, bg_RoseBowl );

     assertNotNull( bowl_game                           );
     assertNotNull( bowl_game->road_team                );
     assertNotNull( bowl_game->home_team                );
     assertNotNull( bowl_game->road_team->kicking_stats );
     assertNotNull( bowl_game->home_team->kicking_stats );

     int expected_road_team_id = ram[ROAD_TEAM_RAM_OFFSET] - 27;
     int expected_home_team_id = ram[HOME_TEAM_RAM_OFFSET] - 27;

     team_kicking_stats_s *road_stats = bowl_game->road_team->kicking_stats;
     team_kicking_stats_s *home_stats = bowl_game->home_team->kicking_stats;

     assertEquals( expected_road_team_id, road_stats->team_id                );
     assertEquals( 5,                     road_stats->season                 );
     assertEquals( bg_RoseBowl,           road_stats->bowl_game              );
     assertEquals(   5,                   road_stats->extra_point_attempts   );
     assertEquals(   4,                   road_stats->extra_points_made      );
     assertEquals(   3,                   road_stats->field_goal_attempts    );
     assertEquals(   2,                   road_stats->field_goals_made       );
     assertEquals(   5,                   road_stats->punts                  );
     assertEquals( 123,                   road_stats->punt_yards             );
     assertEquals(  20,                   road_stats->kick_returns           );
     assertEquals( 350,                   road_stats->kick_return_yards      );
     assertEquals(  10,                   road_stats->kick_return_touchdowns );
     assertEquals(  40,                   road_stats->punt_returns           );
     assertEquals( 460,                   road_stats->punt_return_yards      );
     assertEquals(  10,                   road_stats->punt_return_touchdowns );

     assertEquals( expected_home_team_id, home_stats->team_id                );
     assertEquals( 5,                     home_stats->season                 );
     assertEquals( bg_RoseBowl,           home_stats->bowl_game              );
     assertEquals(   5,                   road_stats->extra_point_attempts   );
     assertEquals(   4,                   road_stats->extra_points_made      );
     assertEquals(   3,                   road_stats->field_goal_attempts    );
     assertEquals(   2,                   road_stats->field_goals_made       );
     assertEquals(   5,                   road_stats->punts                  );
     assertEquals( 123,                   road_stats->punt_yards             );
     assertEquals(  20,                   road_stats->kick_returns           );
     assertEquals( 350,                   road_stats->kick_return_yards      );
     assertEquals(  10,                   road_stats->kick_return_touchdowns );
     assertEquals(  40,                   road_stats->punt_returns           );
     assertEquals( 460,                   road_stats->punt_return_yards      );
     assertEquals(  10,                   road_stats->punt_return_touchdowns );

     return NULL;
}

static char *check_quarterback( player_s *player )
{
     assertNotNull( player->stats.offense );

     assertEquals(  18, player->stats.offense->pass_attempts        );
     assertEquals(  12, player->stats.offense->completions          );
     assertEquals(   1, player->stats.offense->interceptions        );
     assertEquals( 284, player->stats.offense->pass_yards           );
     assertEquals(   2, player->stats.offense->pass_touchdowns      );
     assertEquals(   5, player->stats.offense->rush_attempts        );
     assertEquals(  13, player->stats.offense->rush_yards           );
     assertEquals(   1, player->stats.offense->rush_touchdowns      );
     assertEquals(   0, player->stats.offense->receptions           );
     assertEquals(   0, player->stats.offense->receiving_yards      );
     assertEquals(   0, player->stats.offense->receiving_touchdowns );

     return NULL;
}

static char *check_offense( player_s *player )
{
     assertNotNull( player->stats.offense );

     assertEquals(  0, player->stats.offense->pass_attempts        );
     assertEquals(  0, player->stats.offense->completions          );
     assertEquals(  0, player->stats.offense->interceptions        );
     assertEquals(  0, player->stats.offense->pass_yards           );
     assertEquals(  0, player->stats.offense->pass_touchdowns      );
     assertEquals(  8, player->stats.offense->rush_attempts        );
     assertEquals( 73, player->stats.offense->rush_yards           );
     assertEquals(  2, player->stats.offense->rush_touchdowns      );
     assertEquals(  5, player->stats.offense->receptions           );
     assertEquals( 84, player->stats.offense->receiving_yards      );
     assertEquals(  3, player->stats.offense->receiving_touchdowns );

     assertNotNull( player->stats.returns );

     assertEquals(  2, player->stats.returns->kick_returns           );
     assertEquals( 35, player->stats.returns->kick_return_yards      );
     assertEquals(  1, player->stats.returns->kick_return_touchdowns );
     assertEquals(  4, player->stats.returns->punt_returns           );
     assertEquals( 46, player->stats.returns->punt_return_yards      );
     assertEquals(  1, player->stats.returns->punt_return_touchdowns );

     return NULL;
}

static char *check_defense( player_s *player )
{
     assertNotNull( player->stats.defense );

     assertEquals(  3, player->stats.defense->sacks             );
     assertEquals(  2, player->stats.defense->interceptions     );
     assertEquals( 20, player->stats.defense->return_yards      );
     assertEquals(  1, player->stats.defense->return_touchdowns );

     return NULL;
}

static char *check_kicker( player_s *player )
{
     assertNotNull( player->stats.kicking );

     assertEquals( 5, player->stats.kicking->extra_point_attempts );
     assertEquals( 4, player->stats.kicking->extra_points_made    );
     assertEquals( 3, player->stats.kicking->field_goal_attempts  );
     assertEquals( 2, player->stats.kicking->field_goals_made     );
     assertEquals( 0, player->stats.kicking->punts                );
     assertEquals( 0, player->stats.kicking->punt_yards           );

     return NULL;
}

static char *check_punter( player_s *player )
{
     assertNotNull( player->stats.kicking );

     assertEquals(   0, player->stats.kicking->extra_point_attempts );
     assertEquals(   0, player->stats.kicking->extra_points_made    );
     assertEquals(   0, player->stats.kicking->field_goal_attempts  );
     assertEquals(   0, player->stats.kicking->field_goals_made     );
     assertEquals(   5, player->stats.kicking->punts                );
     assertEquals( 123, player->stats.kicking->punt_yards           );

     return NULL;
}

static char *convertAAGame_ShouldReturnTeamsWithPlayerStatsFromGame_GivenTsbRomAndSaveState()
{
     tsbrom_s          rom        = { 0 };
     nst_save_state_s  save_state = { 0 };
     unsigned char     ram[2049]  = { 0 };

     setupRom( &rom );
     setupRam(  ram );
     setupSaveState( &save_state );

     bowl_game_s *bowl_game = convertAAGame( &rom, ram, &save_state, 0, bg_None );

     assertNotNull( bowl_game                     );
     assertNotNull( bowl_game->road_team          );
     assertNotNull( bowl_game->home_team          );
     assertNotNull( bowl_game->road_team->players );
     assertNotNull( bowl_game->home_team->players );

     team_player_s *road_players = bowl_game->road_team->players;

     for ( int i = 0; road_players[i].player != NULL; ++i )
     {
          player_s *player = road_players[i].player;
          char     *retval;

          if      ( i <=  1 ) { if ( (retval = check_quarterback( player )) != NULL ) return retval; }
          else if ( i <= 11 ) { if ( (retval = check_offense(     player )) != NULL ) return retval; }
          else if ( i <= 16 ) {}// offensive line - no stats
          else if ( i <= 27 ) { if ( (retval = check_defense(     player )) != NULL ) return retval; }
          else if ( i == 28 ) { if ( (retval = check_kicker(      player )) != NULL ) return retval; }
          else                { if ( (retval = check_punter(      player )) != NULL ) return retval; }
     }

     team_player_s *home_players = bowl_game->home_team->players;

     for ( int i = 0; home_players[i].player != NULL; ++i )
     {
          player_s *player = home_players[i].player;
          char     *retval;

          if      ( i <=  1 ) { if ( (retval = check_quarterback( player )) != NULL ) return retval; }
          else if ( i <= 11 ) { if ( (retval = check_offense(     player )) != NULL ) return retval; }
          else if ( i <= 16 ) {}// offensive line - no stats
          else if ( i <= 27 ) { if ( (retval = check_defense(     player )) != NULL ) return retval; }
          else if ( i == 28 ) { if ( (retval = check_kicker(      player )) != NULL ) return retval; }
          else                { if ( (retval = check_punter(      player )) != NULL ) return retval; }
     }

     freeBowlGame( bowl_game );

     return NULL;
}

static void check_convert_aa_game_error()
{
     printf( "convert bowl game error: %s\n", getConvertAAGameError() );
}

static void run_all_tests()
{
     run_test( convertAAGame_ShouldReturnABowlGameObject_GivenTsbRomAndSaveState,               check_convert_aa_game_error );
     run_test( convertAAGame_ShouldReturnBowlGameWithTeamsThatPlayed_GivenTsbRomAndSaveState,   check_convert_aa_game_error );
     run_test( convertAAGame_ShouldReturnTeamsWithStatsFromGame_GivenTsbRomAndSaveState,        check_convert_aa_game_error );
     run_test( convertAAGame_ShouldReturnTeamsWithOffenseStatsFromGame_GivenTsbRomAndSaveState, check_convert_aa_game_error );
     run_test( convertAAGame_ShouldReturnTeamsWithDefenseStatsFromGame_GivenTsbRomAndSaveState, check_convert_aa_game_error );
     run_test( convertAAGame_ShouldReturnTeamsWithKickingStatsFromGame_GivenTsbRomAndSaveState, check_convert_aa_game_error );
     run_test( convertAAGame_ShouldReturnTeamsWithPlayerStatsFromGame_GivenTsbRomAndSaveState,  check_convert_aa_game_error );
}

int main( int argc, char *argv[] )
{
     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
