#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include "file_formats.h"
#include "save_state.h"


#define  NUM_PLAYOFF_TEAMS                8
#define  TEAM_CONF_NAMES_START_OFFSET     0xbcf0
#define  PLAYER_IDENTIFIERS_START_OFFSET  0x86ca


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


static int parseTeamList( const char *team_list_str, int *team_list )
{
     char *s     = strndup( team_list_str, 64 );
     char *begin = s;
     char *end   = NULL;
     int   count = 0;

     while ( (end = strchr( begin, ',' )) != NULL )
     {
          *end = '\0';

          team_list[count] = atoi( begin );

          count++;

          if ( count >= NUM_PLAYOFF_TEAMS ) break;

          begin = end + 1;
     }
     while ( 0 );

     if ( count < NUM_PLAYOFF_TEAMS )
     {
          team_list[count] = atoi( begin );

          count++;
     }

     free( s );

     return count;
}

static void copyTeamIdentifiers( tsbrom_s *output_rom, tsbrom_s *source_rom1, tsbrom_s *source_rom2, int *team_list )
{
     int start  = TEAM_CONF_NAMES_START_OFFSET;
     int offset = 0;

     for ( int i = 0; i < NUM_PLAYOFF_TEAMS; ++i )
     {
          tsbrom_s *source_rom = (team_list[i] > 24) ? source_rom2 : source_rom1;
          int team_idx = (team_list[i] - 1) % 24;

          int start_offset = pointer2int( &(source_rom->team_abbr_pointers[team_idx    ]) ) - TEAM_CONF_NAMES_START_OFFSET;
          int end_offset   = pointer2int( &(source_rom->team_abbr_pointers[team_idx + 1]) ) - TEAM_CONF_NAMES_START_OFFSET;

          int2pointer( start + offset, &(output_rom->team_abbr_pointers[i]) );

          memcpy( output_rom->team_conference_names + offset, source_rom->team_conference_names + start_offset, end_offset - start_offset );

          offset += (end_offset - start_offset);
     }

     for ( int i = NUM_PLAYOFF_TEAMS; i < 28; ++i )
     {
          int2pointer( start + offset, &(output_rom->team_abbr_pointers[i]) );

          memcpy( output_rom->team_conference_names + offset, " ", 1 );

          offset++;
     }

     for ( int i = 0; i < 4; ++i )
     {
          int2pointer( start + offset, &(output_rom->conference_abbr_pointers[i]) );

          memcpy( output_rom->team_conference_names + offset, " ", 1 );

          offset++;
     }

     for ( int i = 0; i < NUM_PLAYOFF_TEAMS; ++i )
     {
          tsbrom_s *source_rom = (team_list[i] > 24) ? source_rom2 : source_rom1;
          int team_idx = (team_list[i] - 1) % 24;

          int start_offset = pointer2int( &(source_rom->team_loc_pointers[team_idx    ]) ) - TEAM_CONF_NAMES_START_OFFSET;
          int end_offset   = pointer2int( &(source_rom->team_loc_pointers[team_idx + 1]) ) - TEAM_CONF_NAMES_START_OFFSET;

          int2pointer( start + offset, &(output_rom->team_loc_pointers[i]) );

          memcpy( output_rom->team_conference_names + offset, source_rom->team_conference_names + start_offset, end_offset - start_offset );

          offset += (end_offset - start_offset);
     }

     for ( int i = NUM_PLAYOFF_TEAMS; i < 28; ++i )
     {
          int2pointer( start + offset, &(output_rom->team_loc_pointers[i]) );

          memcpy( output_rom->team_conference_names + offset, " ", 1 );

          offset++;
     }

     for ( int i = 0; i < 4; ++i )
     {
          int2pointer( start + offset, &(output_rom->conference_loc_pointers[i]) );

          memcpy( output_rom->team_conference_names + offset, " ", 1 );

          offset++;
     }

     for ( int i = 0; i < NUM_PLAYOFF_TEAMS; ++i )
     {
          tsbrom_s *source_rom = (team_list[i] > 24) ? source_rom2 : source_rom1;
          int team_idx = (team_list[i] - 1) % 24;

          int start_offset = pointer2int( &(source_rom->team_name_pointers[team_idx    ]) ) - TEAM_CONF_NAMES_START_OFFSET;
          int end_offset   = pointer2int( &(source_rom->team_name_pointers[team_idx + 1]) ) - TEAM_CONF_NAMES_START_OFFSET;

          int2pointer( start + offset, &(output_rom->team_name_pointers[i]) );

          memcpy( output_rom->team_conference_names + offset, source_rom->team_conference_names + start_offset, end_offset - start_offset );

          offset += (end_offset - start_offset);
     }

     for ( int i = NUM_PLAYOFF_TEAMS; i < 28; ++i )
     {
          int2pointer( start + offset, &(output_rom->team_name_pointers[i]) );

          memcpy( output_rom->team_conference_names + offset, " ", 1 );

          offset++;
     }

     for ( int i = 0; i < 4; ++i )
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

     int2pointer( start + offset, &(output_rom->misc_name_pointers[ 2]) ); memcpy( output_rom->team_conference_names + offset, "NORTH",       5 ); offset +=  5;
     int2pointer( start + offset, &(output_rom->misc_name_pointers[ 3]) ); memcpy( output_rom->team_conference_names + offset, "SOUTHEAST",   9 ); offset +=  9;
     int2pointer( start + offset, &(output_rom->misc_name_pointers[ 4]) ); memcpy( output_rom->team_conference_names + offset, "GREAT LAKE", 10 ); offset += 10;
     int2pointer( start + offset, &(output_rom->misc_name_pointers[ 5]) ); memcpy( output_rom->team_conference_names + offset, "PACIFIC",     7 ); offset +=  7;
     int2pointer( start + offset, &(output_rom->misc_name_pointers[ 6]) ); memcpy( output_rom->team_conference_names + offset, "ATLANTIC",    8 ); offset +=  8;
     int2pointer( start + offset, &(output_rom->misc_name_pointers[ 7]) ); memcpy( output_rom->team_conference_names + offset, "SOUTHWEST",   9 ); offset +=  9;

     int2pointer( start + offset, &(output_rom->misc_name_pointers[ 8]) ); memcpy( output_rom->team_conference_names + offset, "MAN",       3 ); offset += 3;
     int2pointer( start + offset, &(output_rom->misc_name_pointers[ 9]) ); memcpy( output_rom->team_conference_names + offset, "COA",       3 ); offset += 3;
     int2pointer( start + offset, &(output_rom->misc_name_pointers[10]) ); memcpy( output_rom->team_conference_names + offset, "COM",       3 ); offset += 3;
     int2pointer( start + offset, &(output_rom->misc_name_pointers[11]) ); memcpy( output_rom->team_conference_names + offset, "SKP",       3 ); offset += 3;

     int2pointer( start + offset, &(output_rom->misc_name_pointers[12]) ); memcpy( output_rom->team_conference_names + offset, "NEW ENGLAND", 11 ); offset += 11;
     int2pointer( start + offset, &(output_rom->misc_name_pointers[13]) ); memcpy( output_rom->team_conference_names + offset, "MIDWEST",      7 ); offset +=  7;

     int2pointer( start + offset, &(output_rom->misc_name_pointers[14]) );
     int2pointer( start + offset, &(output_rom->misc_name_pointers[15]) );
}

static void copyTeamPlayerData( tsbrom_s *output_rom, tsbrom_s *source_rom1, tsbrom_s *source_rom2, int *team_list )
{
     for ( int i = 0; i < NUM_PLAYOFF_TEAMS; ++i )
     {
          tsbrom_s *source_rom = (team_list[i] > 24) ? source_rom2 : source_rom1;
          int team_idx = (team_list[i] - 1) % 24;

          output_rom->team_ids                 [i] = source_rom->team_ids                 [team_idx];
          output_rom->helmet_colors            [i] = source_rom->helmet_colors            [team_idx];
          output_rom->helmet_types             [i] = source_rom->helmet_types             [team_idx];
          output_rom->mini_helmets             [i] = source_rom->mini_helmets             [team_idx];
          output_rom->field_uniforms           [i] = source_rom->field_uniforms           [team_idx];
          output_rom->cutscene_uniforms        [i] = source_rom->cutscene_uniforms        [team_idx];
          output_rom->cutscene_uniforms2       [i] = source_rom->cutscene_uniforms2       [team_idx];
          output_rom->cutscene_uniforms3       [i] = source_rom->cutscene_uniforms3       [team_idx];
          output_rom->team_background_colors   [i] = source_rom->team_background_colors   [team_idx];
          output_rom->team_player_ratings      [i] = source_rom->team_player_ratings      [team_idx];
          output_rom->sim_data                 [i] = source_rom->sim_data                 [team_idx];
          output_rom->formations1              [i] = source_rom->formations1              [team_idx];
          output_rom->formations2              [i] = source_rom->formations2              [team_idx];
          output_rom->offensive_preference     [i] = source_rom->offensive_preference     [team_idx];
          output_rom->default_playbooks        [i] = source_rom->default_playbooks        [team_idx];
          output_rom->kick_and_punt_returners1 [i] = source_rom->kick_and_punt_returners1 [team_idx];
          output_rom->kick_and_punt_returners2 [i] = source_rom->kick_and_punt_returners2 [team_idx];
     }
}

static void copyPlayerIdentifiers( tsbrom_s *output_rom, tsbrom_s *source_rom1, tsbrom_s *source_rom2, int *team_list )
{
     int start  = PLAYER_IDENTIFIERS_START_OFFSET;
     int offset = 0;

     for ( int i = 0; i < NUM_PLAYOFF_TEAMS; ++i )
     {
          tsbrom_s *source_rom = (team_list[i] > 24) ? source_rom2 : source_rom1;
          int team_idx = (team_list[i] - 1) % 24;

          for ( int j = 0; j < 30; ++j )
          {
               int start_offset = pointer2int( &(source_rom->player_pointers[team_idx][j    ]) ) - start;
               int end_offset   = pointer2int( &(source_rom->player_pointers[team_idx][j + 1]) ) - start;

               output_rom->player_ids[i][j] = source_rom->player_ids[team_idx][j];

               int2pointer( start + offset, &(output_rom->player_pointers[i][j]) );

               memcpy( output_rom->player_identifiers + offset, source_rom->player_identifiers + start_offset, end_offset - start_offset );

               offset += (end_offset - start_offset);
          }
     }

     for ( int i = NUM_PLAYOFF_TEAMS; i < 28; ++i )
     {
          for ( int j = 0; j < 30; ++j )
          {
               int2pointer( start + offset, &(output_rom->player_pointers[i][j]) );

               memcpy( output_rom->player_identifiers + offset, "\0aA", 3 );

               offset += 3;
          }
     }
}

static void copyTeams( tsbrom_s *output_rom, tsbrom_s *source_rom1, tsbrom_s *source_rom2, int *team_list )
{
     for ( int i = 0; i < NUM_PLAYOFF_TEAMS; ++i )
     {
          tsbrom_s *source_rom = (team_list[i] > 24) ? source_rom2 : source_rom1;
          int team_idx = (team_list[i] - 1) % 24;

          int start_offset = pointer2int( &(source_rom->team_abbr_pointers[team_idx    ]) ) - TEAM_CONF_NAMES_START_OFFSET;
          int end_offset   = pointer2int( &(source_rom->team_abbr_pointers[team_idx + 1]) ) - TEAM_CONF_NAMES_START_OFFSET;

          printf( "Copying: %.*s ", end_offset - start_offset, source_rom->team_conference_names + start_offset );

          start_offset = pointer2int( &(source_rom->team_loc_pointers[team_idx    ]) ) - TEAM_CONF_NAMES_START_OFFSET;
          end_offset   = pointer2int( &(source_rom->team_loc_pointers[team_idx + 1]) ) - TEAM_CONF_NAMES_START_OFFSET;

          printf( "%.*s ", end_offset - start_offset, source_rom->team_conference_names + start_offset );

          start_offset = pointer2int( &(source_rom->team_name_pointers[team_idx    ]) ) - TEAM_CONF_NAMES_START_OFFSET;
          end_offset   = pointer2int( &(source_rom->team_name_pointers[team_idx + 1]) ) - TEAM_CONF_NAMES_START_OFFSET;

          printf( "%.*s\n", end_offset - start_offset, source_rom->team_conference_names + start_offset );
     }

     copyTeamIdentifiers(   output_rom, source_rom1, source_rom2, team_list );
     copyPlayerIdentifiers( output_rom, source_rom1, source_rom2, team_list );
     copyTeamPlayerData(    output_rom, source_rom1, source_rom2, team_list );
}

static void copyStats( nst_save_state_s *output_stats, nst_save_state_s *source_stats1, nst_save_state_s *source_stats2, int *team_list )
{
     for ( int i = 0; i < NUM_PLAYOFF_TEAMS; ++i )
     {
          nst_save_state_s *source_stats = (team_list[i] > 24) ? source_stats2 : source_stats1;
          int team_idx = (team_list[i] - 1) % 24;
          nst_stats_s *stats = NULL;

          if   ( team_idx < 9 ) { stats = source_stats->stats1;                }
          else                  { stats = source_stats->stats2; team_idx -= 9; }

          output_stats->stats1[i] = stats[team_idx];
     }
}

static void initializeData( tsbrom_s *rom )
{
     memset( rom->team_ids,                 0xff, sizeof(rom->team_ids)                 );
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

     memset( rom->helmet_colors,            0, sizeof(rom->helmet_colors)            );
     memset( rom->helmet_types,             0, sizeof(rom->helmet_types)             );
     memset( rom->mini_helmets,             0, sizeof(rom->mini_helmets)             );
     memset( rom->field_uniforms,           0, sizeof(rom->field_uniforms)           );
     memset( rom->cutscene_uniforms,        0, sizeof(rom->cutscene_uniforms)        );
     memset( rom->team_background_colors,   0, sizeof(rom->team_background_colors)   );
     memset( rom->team_player_ratings,      0, sizeof(rom->team_player_ratings)      );
     memset( rom->sim_data,                 0, sizeof(rom->sim_data)                 );
     memset( rom->formations1,              0, sizeof(rom->formations1)              );
     memset( rom->formations2,              0, sizeof(rom->formations2)              );
     memset( rom->offensive_preference,     0, sizeof(rom->offensive_preference)     );
     memset( rom->default_playbooks,        0, sizeof(rom->default_playbooks)        );
     memset( rom->kick_and_punt_returners1, 0, sizeof(rom->kick_and_punt_returners1) );
     memset( rom->kick_and_punt_returners2, 0, sizeof(rom->kick_and_punt_returners2) );
}

int main( const int argc, const char const *argv[] )
{
     /**/  tsbrom_s         *source_rom1   = NULL;
     /**/  tsbrom_s         *source_rom2   = NULL;
     /**/  tsbrom_s         *output_rom    = NULL;
     /**/  nst_save_state_s *source_stats1 = NULL;
     /**/  nst_save_state_s *source_stats2 = NULL;
     /**/  nst_save_state_s *output_stats  = NULL;
     const char             *source_path   = NULL;
     const char             *template_path = NULL;
     const char             *output_path   = NULL;
     const char             *team_list_str = NULL;
     /**/  char              filename_buffer[PATH_MAX + 1] = { 0 };
     /**/  int               team_list[NUM_PLAYOFF_TEAMS]  = { 0 };
     /**/  unsigned char    *save_state    = NULL;
     /**/  int               save_state_len = 0;

     if ( argc != 5 )
     {
          printf( "Usage: %s <source path> <template path> <output path> <team list>\n", argv[0] );
          printf( "       Where team list is a csv string of team IDs.\n" );

          return EXIT_SUCCESS;
     }

     source_path   = argv[1];
     template_path = argv[2];
     output_path   = argv[3];
     team_list_str = argv[4];

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

     sprintf( filename_buffer, "%s/playoff_template.nes", template_path );

     if ( (output_rom = readTsbRom( filename_buffer )) == NULL )
     {
          printf( "Unable to load template rom: %s\n", getFileUtilsError() );

          freeAllocations();

          return EXIT_FAILURE;
     }

     registerAllocation( output_rom );

     sprintf( filename_buffer, "%s/playoff_template.nst", template_path );

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

     if ( parseTeamList( team_list_str, team_list ) < NUM_PLAYOFF_TEAMS )
     {
          printf( "Must have at least %d teams in team list <%s>\n", NUM_PLAYOFF_TEAMS, team_list_str );

          freeAllocations();

          return EXIT_FAILURE;
     }


     initializeData( output_rom );

     copyTeams( output_rom,   source_rom1,   source_rom2,   team_list );
     copyStats( output_stats, source_stats1, source_stats2, team_list );


     sprintf( filename_buffer, "%s/playoffs.nes", output_path );

     if ( ! writeTsbRom( filename_buffer, output_rom ) )
     {
          printf( "Unable to save output rom: %s\n", getFileUtilsError() );

          freeAllocations();

          return EXIT_FAILURE;
     }

     sprintf( filename_buffer, "%s/playoffs.nst", output_path );

     if ( ! writeNstSaveState( filename_buffer, save_state, save_state_len ) )
     {
          printf( "Unable to save output stats: %s\n", getFileUtilsError() );

          freeAllocations();

          return EXIT_FAILURE;
     }

     freeAllocations();

     return EXIT_SUCCESS;
}
