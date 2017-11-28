#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/param.h>
#include "file_formats.h"
#include "save_state.h"


#define  NUM_PLAYOFF_TEAMS                8

#define  PLAYERS_PER_TEAM                 30
#define  TEAM_CONF_NAMES_START_OFFSET     0xbcf0
#define  PLAYER_IDENTIFIERS_START_OFFSET  0x86ca


static const struct {
     char *top;
     char *bottom;
} big_numbers[] =
{
     { "\x90\x91", "\x92\x93" },
     { "\xAE\xA8", "\xC6\xC7" },
     { "\x9C\xCD", "\x82\x83" },
     { "\x9C\xCD", "\x9E\xCF" },
     { "\xB0\xB1", "\xB2\xB3" },
     { "\xC0\xAC", "\x9F\xCF" },
     { "\xD0\x9D", "\xD2\xCF" },
     { "\xD4\xD5", "\xDA\xDB" },
     { "\xCC\xCD", "\xCE\xCF" },
     { "\xCC\xD1", "\x9E\xD3" }
};


static void *allocations[10]  = { 0 };
static  int   allocation_count =   0;

static void freeAllocations( void )
{
     for ( int i = 0; i < allocation_count; ++i )
     {
          if ( allocations[i] != NULL ) free( allocations[i] );
     }
}

static void registerAllocation( void *allocation )
{
     allocations[allocation_count] = allocation;

     allocation_count++;
}


static int player_identifier_offset = 0;


static int parsePlayerList( const char *player_list_str, int *player_list )
{
     char *s     = strndup( player_list_str, 1024 );
     char *begin = s;
     char *end   = NULL;
     int   count = 0;

     while ( (end = strchr( begin, ',' )) != NULL )
     {
          *end = '\0';

          player_list[count] = atoi( begin );

          count++;

          if ( count >= PLAYERS_PER_TEAM ) break;

          begin = end + 1;
     }
     while ( 0 );

     if ( count < PLAYERS_PER_TEAM )
     {
          player_list[count] = atoi( begin );

          count++;
     }

     free( s );

     return count;
}

static void copySeason( void *output_rom, void *output_state, void *source_rom )
{
     char season_str[10] = { 0 };

     memcpy( season_str, source_rom + 0x1e129, 2 );

     memcpy( output_rom + 0x1e129, season_str, 2 );
     memcpy( output_rom + 0x1e28b, season_str, 2 );
     memcpy( output_rom + 0x1e2be, season_str, 2 );
     memcpy( output_rom + 0x1e378, season_str, 2 );
     memcpy( output_rom + 0x1f89c, season_str, 2 );

     memcpy( output_state + 0xb7b, season_str, 2 );
     memcpy( output_state + 0xf73, season_str, 2 );

     int tens = season_str[0] - '0';
     int ones = season_str[1] - '0';

     memcpy( output_rom + 0x16db6, big_numbers[tens].top,    2 );
     memcpy( output_rom + 0x16db8, big_numbers[ones].top,    2 );
     memcpy( output_rom + 0x16dba, big_numbers[tens].bottom, 2 );
     memcpy( output_rom + 0x16dbc, big_numbers[ones].bottom, 2 );

     memcpy( output_state + 0xab1, big_numbers[tens].top,    2 );
     memcpy( output_state + 0xab3, big_numbers[ones].top,    2 );
     memcpy( output_state + 0xad1, big_numbers[tens].bottom, 2 );
     memcpy( output_state + 0xad3, big_numbers[ones].bottom, 2 );
}

static void insertTeams( tsbrom_s *output_rom )
{
     const char *team1_abbr = "AME.";
     const char *team2_abbr = "NAT.";

     const char *team1_name = "AMERICAN";
     const char *team2_name = "NATIONAL";

     int start  = TEAM_CONF_NAMES_START_OFFSET;
     int offset = 0;


     output_rom->team_ids[0] = 49;
     output_rom->team_ids[1] = 50;


     int2pointer( start + offset, &(output_rom->team_abbr_pointers[0]) );

     memcpy( output_rom->team_conference_names + offset, team1_abbr, strlen(team1_abbr) );

     offset += strlen(team1_abbr);

     int2pointer( start + offset, &(output_rom->team_abbr_pointers[1]) );

     memcpy( output_rom->team_conference_names + offset, team2_abbr, strlen(team2_abbr) );

     offset += strlen(team2_abbr);

     for ( int i = 2; i < 28; ++i )
     {
          int2pointer( start + offset, &(output_rom->team_abbr_pointers[i]) );

          memcpy( output_rom->team_conference_names + offset, " ", 1 );

          offset++;
     }

     int2pointer( start + offset, &(output_rom->conference_abbr_pointers[0]) );

     memcpy( output_rom->team_conference_names + offset, team1_abbr, strlen(team1_abbr) );

     offset += strlen(team1_abbr);

     int2pointer( start + offset, &(output_rom->conference_abbr_pointers[1]) );

     memcpy( output_rom->team_conference_names + offset, team2_abbr, strlen(team2_abbr) );

     offset += strlen(team2_abbr);

     for ( int i = 2; i < 4; ++i )
     {
          int2pointer( start + offset, &(output_rom->conference_abbr_pointers[i]) );

          memcpy( output_rom->team_conference_names + offset, " ", 1 );

          offset++;
     }

     int2pointer( start + offset, &(output_rom->team_loc_pointers[0]) );

     memcpy( output_rom->team_conference_names + offset, team1_name, strlen(team1_name) );

     offset += strlen(team1_name);

     int2pointer( start + offset, &(output_rom->team_loc_pointers[1]) );

     memcpy( output_rom->team_conference_names + offset, team2_name, strlen(team2_name) );

     offset += strlen(team2_name);

     for ( int i = 2; i < 28; ++i )
     {
          int2pointer( start + offset, &(output_rom->team_loc_pointers[i]) );

          memcpy( output_rom->team_conference_names + offset, " ", 1 );

          offset++;
     }

     int2pointer( start + offset, &(output_rom->conference_loc_pointers[0]) );

     memcpy( output_rom->team_conference_names + offset, team1_name, strlen(team1_name) );

     offset += strlen(team1_name);

     int2pointer( start + offset, &(output_rom->conference_loc_pointers[1]) );

     memcpy( output_rom->team_conference_names + offset, team2_name, strlen(team2_name) );

     offset += strlen(team2_name);

     for ( int i = 2; i < 4; ++i )
     {
          int2pointer( start + offset, &(output_rom->conference_loc_pointers[i]) );

          memcpy( output_rom->team_conference_names + offset, " ", 1 );

          offset++;
     }

     int2pointer( start + offset, &(output_rom->team_name_pointers[0]) );

     memcpy( output_rom->team_conference_names + offset, team1_name, strlen(team1_name) );

     offset += strlen(team1_name);

     int2pointer( start + offset, &(output_rom->team_name_pointers[1]) );

     memcpy( output_rom->team_conference_names + offset, team2_name, strlen(team2_name) );

     offset += strlen(team2_name);

     for ( int i = 2; i < 28; ++i )
     {
          int2pointer( start + offset, &(output_rom->team_name_pointers[i]) );

          memcpy( output_rom->team_conference_names + offset, " ", 1 );

          offset++;
     }

     int2pointer( start + offset, &(output_rom->conference_name_pointers[0]) );

     memcpy( output_rom->team_conference_names + offset, team1_name, strlen(team1_name) );

     offset += strlen(team1_name);

     int2pointer( start + offset, &(output_rom->conference_name_pointers[1]) );

     memcpy( output_rom->team_conference_names + offset, team2_name, strlen(team2_name) );

     offset += strlen(team2_name);

     for ( int i = 2; i < 4; ++i )
     {
          int2pointer( start + offset, &(output_rom->conference_name_pointers[i]) );

          memcpy( output_rom->team_conference_names + offset, " ", 1 );

          offset++;
     }

     int2pointer( start + offset, &(output_rom->down_name_pointers[0]) ); memcpy( output_rom->team_conference_names + offset, "1ST", 3 ); offset += 3;
     int2pointer( start + offset, &(output_rom->down_name_pointers[1]) ); memcpy( output_rom->team_conference_names + offset, "2ND", 3 ); offset += 3;
     int2pointer( start + offset, &(output_rom->down_name_pointers[2]) ); memcpy( output_rom->team_conference_names + offset, "3RD", 3 ); offset += 3;
     int2pointer( start + offset, &(output_rom->down_name_pointers[3]) ); memcpy( output_rom->team_conference_names + offset, "4TH", 3 ); offset += 3;

     int2pointer( start + offset, &(output_rom->unknown_pointers1[0]) ); memcpy( output_rom->team_conference_names + offset, "\xd6\xd7\xb8", 3 ); offset += 3;
     int2pointer( start + offset, &(output_rom->unknown_pointers1[1]) ); memcpy( output_rom->team_conference_names + offset, "\xd4\xd5\xd7", 3 ); offset += 3;
     int2pointer( start + offset, &(output_rom->unknown_pointers1[2]) ); memcpy( output_rom->team_conference_names + offset, "\xd8\xd9\xda", 3 ); offset += 3;
     int2pointer( start + offset, &(output_rom->unknown_pointers1[3]) ); memcpy( output_rom->team_conference_names + offset, "\xdb\xdc\xdd", 3 ); offset += 3;

     int2pointer( start + offset, &(output_rom->misc_name_pointers[ 0]) ); memcpy( output_rom->team_conference_names + offset, "OFFENSE",   7 ); offset += 7;
     int2pointer( start + offset, &(output_rom->misc_name_pointers[ 1]) ); memcpy( output_rom->team_conference_names + offset, "DEFENSE",   7 ); offset += 7;

     int2pointer( start + offset, &(output_rom->misc_name_pointers[ 2]) ); memcpy( output_rom->team_conference_names + offset, "AMERICAN", 8 ); offset += 8;
     int2pointer( start + offset, &(output_rom->misc_name_pointers[ 3]) ); memcpy( output_rom->team_conference_names + offset, "NATIONAL", 8 ); offset += 8;
     int2pointer( start + offset, &(output_rom->misc_name_pointers[ 4]) ); memcpy( output_rom->team_conference_names + offset, " ",        1 ); offset += 1;
     int2pointer( start + offset, &(output_rom->misc_name_pointers[ 5]) ); memcpy( output_rom->team_conference_names + offset, " ",        1 ); offset += 1;
     int2pointer( start + offset, &(output_rom->misc_name_pointers[ 6]) ); memcpy( output_rom->team_conference_names + offset, " ",        1 ); offset += 1;
     int2pointer( start + offset, &(output_rom->misc_name_pointers[ 7]) ); memcpy( output_rom->team_conference_names + offset, " ",        1 ); offset += 1;

     int2pointer( start + offset, &(output_rom->misc_name_pointers[ 8]) ); memcpy( output_rom->team_conference_names + offset, "MAN",       3 ); offset += 3;
     int2pointer( start + offset, &(output_rom->misc_name_pointers[ 9]) ); memcpy( output_rom->team_conference_names + offset, "COA",       3 ); offset += 3;
     int2pointer( start + offset, &(output_rom->misc_name_pointers[10]) ); memcpy( output_rom->team_conference_names + offset, "COM",       3 ); offset += 3;
     int2pointer( start + offset, &(output_rom->misc_name_pointers[11]) ); memcpy( output_rom->team_conference_names + offset, "SKP",       3 ); offset += 3;

     int2pointer( start + offset, &(output_rom->misc_name_pointers[12]) ); memcpy( output_rom->team_conference_names + offset, "NEW ENGLAND", 11 ); offset += 11;
     int2pointer( start + offset, &(output_rom->misc_name_pointers[13]) ); memcpy( output_rom->team_conference_names + offset, "MIDWEST",      7 ); offset +=  7;

     int2pointer( start + offset, &(output_rom->misc_name_pointers[14]) );
     int2pointer( start + offset, &(output_rom->misc_name_pointers[15]) );
}

static position_e getPosition( const int index )
{
     switch ( index )
     {
     case  0: return pos_Quarterback;
     case  1: return pos_Quarterback;
     case  2: return pos_Runningback;
     case  3: return pos_Runningback;
     case  4: return pos_Runningback;
     case  5: return pos_Runningback;
     case  6: return pos_WideReceiver;
     case  7: return pos_WideReceiver;
     case  8: return pos_WideReceiver;
     case  9: return pos_WideReceiver;
     case 10: return pos_TightEnd;
     case 11: return pos_TightEnd;
     case 12: return pos_OffensiveLine;
     case 13: return pos_OffensiveLine;
     case 14: return pos_OffensiveLine;
     case 15: return pos_OffensiveLine;
     case 16: return pos_OffensiveLine;
     case 17: return pos_DefensiveLine;
     case 18: return pos_DefensiveLine;
     case 19: return pos_DefensiveLine;
     case 20: return pos_Linebacker;
     case 21: return pos_Linebacker;
     case 22: return pos_Linebacker;
     case 23: return pos_Linebacker;
     case 24: return pos_Cornerback;
     case 25: return pos_Cornerback;
     case 26: return pos_Safety;
     case 27: return pos_Safety;
     case 28: return pos_Kicker;
     case 29: return pos_Punter;
     }

     return pos_None;
}

static int get_player_idx( const tsbrom_s *rom, const int target_player_id )
{
     for ( int i = 0; i < 28; ++i )
     {
          for ( int j = 0; j < 30; ++j )
          {
               int player_id = pointer2int( &(rom->player_ids[i][j]) );

               if ( player_id == target_player_id ) return (i * 30) + j;
          }
     }

     return 0;
}

static void fillRemainingPlayers( tsbrom_s *output_rom, const int team_idx )
{
     for ( int i = team_idx; i < 28; ++i )
     {
          for ( int j = 0; j < 30; ++j )
          {
               int2pointer( PLAYER_IDENTIFIERS_START_OFFSET + player_identifier_offset, &(output_rom->player_pointers[i][j]) );

               memcpy( output_rom->player_identifiers + player_identifier_offset, "\0aA", 3 );

               player_identifier_offset += 3;
          }
     }
}

static void copyPlayers(
     const tsbrom_s         *source_rom,
     const nst_save_state_s *source_stats,
     /**/  tsbrom_s         *output_rom,
     /**/  nst_save_state_s *output_stats,
     const int               team_idx,
     const int              *player_ids )
{
     for ( int i = 0; i < PLAYERS_PER_TEAM; ++i )
     {
          int result = get_player_idx( source_rom, player_ids[i] );

          if ( result == 0 )
          {
               printf( "Cannot find player in source rom with ID: %d\n", player_ids[i] );

               continue;
          }

          position_e position = getPosition( i );

          if ( position == pos_None )
          {
               printf( "Unknown position for player at index %d\n", i );

               continue;
          }

          int2pointer( player_ids[i], &(output_rom->player_ids[team_idx][i]) );

          int source_team_idx   = result / 30;
          int source_player_idx = result % 30;

          int source_player_offset = pointer2int( &(source_rom->player_pointers[source_team_idx][source_player_idx    ]) ) - PLAYER_IDENTIFIERS_START_OFFSET;
          int source_player_end    = pointer2int( &(source_rom->player_pointers[source_team_idx][source_player_idx + 1]) ) - PLAYER_IDENTIFIERS_START_OFFSET;

          int len = source_player_end - source_player_offset;

          int2pointer( PLAYER_IDENTIFIERS_START_OFFSET +  player_identifier_offset, &(output_rom->player_pointers[team_idx][i]) );

          memcpy( output_rom->player_identifiers + player_identifier_offset, source_rom->player_identifiers + source_player_offset, len );

          player_identifier_offset += len;

          const nst_players_s *s_stats = NULL;
          /**/  nst_players_s *o_stats = NULL;

          if   ( source_team_idx < 9 ) s_stats = source_stats->stats1[source_team_idx    ].player_stats;
          else                         s_stats = source_stats->stats2[source_team_idx - 9].player_stats;

          if   ( team_idx < 9 ) o_stats = output_stats->stats1[team_idx    ].player_stats;
          else                  o_stats = output_stats->stats2[team_idx - 9].player_stats;


          if ( position == pos_Quarterback )
          {
               int source_qb_idx = source_player_idx;
               int qb_idx        = i;

               output_rom->team_player_ratings[team_idx].quarterback[qb_idx] = source_rom->team_player_ratings[source_team_idx].quarterback[source_qb_idx];

               output_rom->sim_data[team_idx].quarterbacks[qb_idx][0] = source_rom->sim_data[source_team_idx].quarterbacks[source_qb_idx][0];
               output_rom->sim_data[team_idx].quarterbacks[qb_idx][1] = source_rom->sim_data[source_team_idx].quarterbacks[source_qb_idx][1];

               o_stats->quarterback[qb_idx] = s_stats->quarterback[source_qb_idx];
          }
          else if ( position == pos_Runningback  ||
                    position == pos_WideReceiver ||
                    position == pos_TightEnd        )
          {
               int source_off_idx = source_player_idx - 2;
               int off_idx        =                 i - 2;

               output_rom->team_player_ratings[team_idx].offense[off_idx] = source_rom->team_player_ratings[source_team_idx].offense[source_off_idx];

               output_rom->sim_data[team_idx].offense[off_idx][0] = source_rom->sim_data[source_team_idx].offense[source_off_idx][0];
               output_rom->sim_data[team_idx].offense[off_idx][1] = source_rom->sim_data[source_team_idx].offense[source_off_idx][1];

               o_stats->offense[off_idx] = s_stats->offense[source_off_idx];
          }
          else if ( position == pos_OffensiveLine )
          {
               int source_ol_idx = source_player_idx - 12;
               int ol_idx        =                 i - 12;

               output_rom->team_player_ratings[team_idx].linesmen[ol_idx] = source_rom->team_player_ratings[source_team_idx].linesmen[source_ol_idx];
          }
          else if ( position == pos_DefensiveLine ||
                    position == pos_Linebacker    ||
                    position == pos_Cornerback    ||
                    position == pos_Safety           )
          {
               int source_def_idx = source_player_idx - 17;
               int def_idx        =                 i - 17;

               output_rom->team_player_ratings[team_idx].defense[def_idx] = source_rom->team_player_ratings[source_team_idx].defense[source_def_idx];

               output_rom->sim_data[team_idx].defense_pass_rush[def_idx] = source_rom->sim_data[source_team_idx].defense_pass_rush[source_def_idx];
               output_rom->sim_data[team_idx].defense_coverage [def_idx] = source_rom->sim_data[source_team_idx].defense_coverage [source_def_idx];

               o_stats->defense[def_idx] = s_stats->defense[source_def_idx];
          }
          else if ( position == pos_Kicker ||
                    position == pos_Punter    )
          {
               int source_ki_idx = source_player_idx - 28;
               int ki_idx        =                 i - 28;

               output_rom->team_player_ratings[team_idx].kickers[ki_idx] = source_rom->team_player_ratings[source_team_idx].kickers[source_ki_idx];

               if ( position == pos_Kicker )
               {
                    unsigned char kick_ability = source_rom->sim_data[source_team_idx].kicking[source_ki_idx] & 0xf0;

                    output_rom->sim_data[team_idx].kicking[ki_idx] &= 0x0f;
                    output_rom->sim_data[team_idx].kicking[ki_idx] |= kick_ability;

                    o_stats->kicker[0] = s_stats->kicker[0];
               }
               else
               {
                    unsigned char kick_ability = source_rom->sim_data[source_team_idx].kicking[source_ki_idx] & 0x0f;

                    output_rom->sim_data[team_idx].kicking[ki_idx] &= 0xf0;
                    output_rom->sim_data[team_idx].kicking[ki_idx] |= kick_ability;

                    o_stats->punter[0] = s_stats->punter[0];
               }
          }
     }

     int2pointer( PLAYER_IDENTIFIERS_START_OFFSET +  player_identifier_offset, &(output_rom->player_pointers[team_idx][30]) );
}

static void initializeData( tsbrom_s *rom )
{
     memset( rom->player_ids,               0xff, sizeof(rom->player_ids)               );
     memset( rom->player_pointers,          0xff, sizeof(rom->player_pointers)          );
     memset( rom->player_identifiers,       0xff, sizeof(rom->player_identifiers)       );
     memset( rom->team_abbr_pointers,       0xff, sizeof(rom->team_abbr_pointers)       );
     memset( rom->conference_abbr_pointers, 0xff, sizeof(rom->conference_abbr_pointers) );
     memset( rom->team_loc_pointers,        0xff, sizeof(rom->team_loc_pointers)        );
     memset( rom->conference_loc_pointers,  0xff, sizeof(rom->conference_loc_pointers)  );
     memset( rom->team_name_pointers,       0xff, sizeof(rom->team_name_pointers)       );
     memset( rom->conference_name_pointers, 0xff, sizeof(rom->conference_name_pointers) );
     memset( rom->down_name_pointers,       0xff, sizeof(rom->down_name_pointers)       );
     memset( rom->unknown_pointers1,        0xff, sizeof(rom->unknown_pointers1)        );
     memset( rom->misc_name_pointers,       0xff, sizeof(rom->misc_name_pointers)       );
     memset( rom->team_conference_names,    0xff, sizeof(rom->team_conference_names)    );

     memset( rom->team_player_ratings,      0,    sizeof(rom->team_player_ratings)      );
     memset( rom->sim_data,                 0,    sizeof(rom->sim_data)                 );

     memset( rom->kick_and_punt_returners1, 0,    sizeof(rom->kick_and_punt_returners1) );
     memset( rom->kick_and_punt_returners2, 0,    sizeof(rom->kick_and_punt_returners2) );
}

int main( const int argc, const char const *argv[] )
{
     /**/  tsbrom_s         *source_rom1                     = NULL;
     /**/  tsbrom_s         *source_rom2                     = NULL;
     /**/  tsbrom_s         *output_rom                      = NULL;
     /**/  nst_save_state_s *source_stats1                   = NULL;
     /**/  nst_save_state_s *source_stats2                   = NULL;
     /**/  nst_save_state_s *output_stats                    = NULL;
     const char             *source_path                     = NULL;
     const char             *template_path                   = NULL;
     const char             *output_path                     = NULL;
     const char             *player_list_filename            = NULL;
     /**/  FILE             *player_list_file                = NULL;
     /**/  int               team1_players[PLAYERS_PER_TEAM] = { 0 };
     /**/  int               team2_players[PLAYERS_PER_TEAM] = { 0 };
     /**/  char              filename_buffer[PATH_MAX + 1]   = { 0 };
     /**/  char              player_list_buffer[999 + 1]     = { 0 };
     /**/  unsigned char    *save_state                      = NULL;
     /**/  int               save_state_len                  = 0;

     if ( argc != 5 )
     {
          printf( "Usage: %s <source path> <template path> <output path> <player list file>\n", argv[0] );
          printf( "       Where player list file is a csv file of player IDs with 2 lines, one for each team.\n" );

          return EXIT_SUCCESS;
     }

     source_path          = argv[1];
     template_path        = argv[2];
     output_path          = argv[3];
     player_list_filename = argv[4];

     sprintf( filename_buffer, "%s/ncfo1.nes", source_path );

     if ( (source_rom1 = readTsbRom( filename_buffer )) == NULL )
     {
          printf( "Unable to load source rom1: %s\n", getFileUtilsError() );

          freeAllocations();

          return EXIT_FAILURE;
     }

     registerAllocation( source_rom1 );

     sprintf( filename_buffer, "%s/ncfo2.nes", source_path );

     if ( (source_rom2 = readTsbRom( filename_buffer )) == NULL )
     {
          printf( "Unable to load source rom2: %s\n", getFileUtilsError() );

          freeAllocations();

          return EXIT_FAILURE;
     }

     registerAllocation( source_rom2 );

     sprintf( filename_buffer, "%s/ncfo1.nst", source_path );

     if ( (save_state = readNstSaveState( filename_buffer, &save_state_len )) == NULL )
     {
          printf( "Unable to load source save state 1: %s\n", getFileUtilsError() );

          freeAllocations();

          return EXIT_FAILURE;
     }

     registerAllocation( save_state );

     if ( (source_stats1 = getSaveStateStats( save_state, save_state_len )) == NULL )
     {
          printf( "Unable to load source stats 1: %s\n", getFileUtilsError() );

          freeAllocations();

          return EXIT_FAILURE;
     }

     sprintf( filename_buffer, "%s/ncfo2.nst", source_path );

     if ( (save_state = readNstSaveState( filename_buffer, &save_state_len )) == NULL )
     {
          printf( "Unable to load source save state 2: %s\n", getFileUtilsError() );

          freeAllocations();

          return EXIT_FAILURE;
     }

     registerAllocation( save_state );

     if ( (source_stats2 = getSaveStateStats( save_state, save_state_len )) == NULL )
     {
          printf( "Unable to load source stats 2: %s\n", getFileUtilsError() );

          freeAllocations();

          return EXIT_FAILURE;
     }

     sprintf( filename_buffer, "%s/aab_template.nes", template_path );

     if ( (output_rom = readTsbRom( filename_buffer )) == NULL )
     {
          printf( "Unable to load template rom: %s\n", getFileUtilsError() );

          freeAllocations();

          return EXIT_FAILURE;
     }

     registerAllocation( output_rom );

     sprintf( filename_buffer, "%s/aab_template.nst", template_path );

     if ( (save_state = readNstSaveState( filename_buffer, &save_state_len )) == NULL )
     {
          printf( "Unable to load template save state: %s\n", getFileUtilsError() );

          freeAllocations();

          return EXIT_FAILURE;
     }

     registerAllocation( save_state );

     if ( (output_stats = getSaveStateStats( save_state, save_state_len )) == NULL )
     {
          printf( "Unable to load output stats: %s\n", getFileUtilsError() );

          freeAllocations();

          return EXIT_FAILURE;
     }

     if ( (player_list_file = fopen( player_list_filename, "r" )) == NULL )
     {
          printf( "Unable to load players file: %s\n", strerror( errno ) );

          freeAllocations();

          return EXIT_FAILURE;
     }

     if ( parsePlayerList( fgets( player_list_buffer, sizeof(player_list_buffer), player_list_file ), team1_players ) < PLAYERS_PER_TEAM )
     {
          printf( "Must have at least %d players in team 1 player list <%s>\n", PLAYERS_PER_TEAM, player_list_filename );

          freeAllocations();

          return EXIT_FAILURE;
     }

     if ( parsePlayerList( fgets( player_list_buffer, sizeof(player_list_buffer), player_list_file ), team2_players ) < PLAYERS_PER_TEAM )
     {
          printf( "Must have at least %d players in team 2 player list <%s>\n", PLAYERS_PER_TEAM, player_list_filename );

          freeAllocations();

          return EXIT_FAILURE;
     }

     fclose( player_list_file );

     initializeData( output_rom );

     copyPlayers( source_rom1, source_stats1, output_rom, output_stats, 0, team1_players );
     copyPlayers( source_rom2, source_stats2, output_rom, output_stats, 1, team2_players );

     fillRemainingPlayers( output_rom, 2 );

     insertTeams( output_rom );

     copySeason( output_rom, save_state, source_rom1 );

     sprintf( filename_buffer, "%s/all_americans.nes", output_path );

     if ( ! writeTsbRom( filename_buffer, output_rom ) )
     {
          printf( "Unable to save output rom: %s\n", getFileUtilsError() );

          freeAllocations();

          return EXIT_FAILURE;
     }

     sprintf( filename_buffer, "%s/all_americans.nst", output_path );

     if ( ! writeNstSaveState( filename_buffer, save_state, save_state_len ) )
     {
          printf( "Unable to save output stats: %s\n", getFileUtilsError() );

          freeAllocations();

          return EXIT_FAILURE;
     }

     freeAllocations();

     return EXIT_SUCCESS;
}
