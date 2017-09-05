#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include "file_formats.h"


#define  NUM_TEAMS                        24
#define  NUM_TEAM_SLOTS                   28
#define  TEAM_CONF_NAMES_START_OFFSET     0xbcf0


static void *allocations[10]  = { 0 };
static  int  allocation_count =   0;

static char *team_names[] = {
     "BLUE HENS",
     "TERRAPINS",
     "S. KNIGHTS",
     "N. LIONS",
     "CAVALIERS",
     "MNTNEERS",
     "F. ILLINI",
     "WOLVERINES",
     "G. GOPHERS",
     "ORANGEMEN",
     "BUCKEYES",
     "BADGERS",
     "RAZORBACKS",
     "HOOSIERS",
     "JAYHAWKS",
     "WILDCATS",
     "TIGERS",
     "VOLUNTEERS",
     "HUSKIES",
     "B. BEARS",
     "MINUTEMEN",
     "BIG GREEN",
     "RAMS",
     "CATAMOUNTS"
};


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

static void replaceTeamNames( tsbrom_s *source_rom )
{
     int start  = TEAM_CONF_NAMES_START_OFFSET;
     int offset = pointer2int( &(source_rom->team_name_pointers[0]) ) - TEAM_CONF_NAMES_START_OFFSET;

     for ( int i = 0; i < NUM_TEAMS; ++i )
     {
          int2pointer( start + offset, &(source_rom->team_name_pointers[i]) );

          memcpy( source_rom->team_conference_names + offset, team_names[i], strlen(team_names[i]) );

          offset += strlen(team_names[i]);
     }

     for ( int i = NUM_TEAMS; i < NUM_TEAM_SLOTS; ++i )
     {
          int2pointer( start + offset, &(source_rom->team_name_pointers[i]) );

          memcpy( source_rom->team_conference_names + offset, " ", 1 );

          offset++;
     }

     for ( int i = 0; i < 4; ++i )
     {
          int2pointer( start + offset, &(source_rom->conference_name_pointers[i]) );

          memcpy( source_rom->team_conference_names + offset, " ", 1 );

          offset++;
     }

     int2pointer( start + offset, &(source_rom->down_name_pointers[0]) ); memcpy( source_rom->team_conference_names + offset, "1ST", 3 ); offset += 3;
     int2pointer( start + offset, &(source_rom->down_name_pointers[1]) ); memcpy( source_rom->team_conference_names + offset, "2ND", 3 ); offset += 3;
     int2pointer( start + offset, &(source_rom->down_name_pointers[2]) ); memcpy( source_rom->team_conference_names + offset, "3RD", 3 ); offset += 3;
     int2pointer( start + offset, &(source_rom->down_name_pointers[3]) ); memcpy( source_rom->team_conference_names + offset, "4TH", 3 ); offset += 3;

     int2pointer( start + offset, &(source_rom->unknown_pointers1[0]) ); memcpy( source_rom->team_conference_names + offset, "\xd6\xd7\xb8", 3 ); offset += 3;
     int2pointer( start + offset, &(source_rom->unknown_pointers1[1]) ); memcpy( source_rom->team_conference_names + offset, "\xd4\xd5\xd7", 3 ); offset += 3;
     int2pointer( start + offset, &(source_rom->unknown_pointers1[2]) ); memcpy( source_rom->team_conference_names + offset, "\xd8\xd9\xda", 3 ); offset += 3;
     int2pointer( start + offset, &(source_rom->unknown_pointers1[3]) ); memcpy( source_rom->team_conference_names + offset, "\xdb\xdc\xdd", 3 ); offset += 3;

     int2pointer( start + offset, &(source_rom->misc_name_pointers[ 0]) ); memcpy( source_rom->team_conference_names + offset, "OFFENSE",   7 ); offset += 7;
     int2pointer( start + offset, &(source_rom->misc_name_pointers[ 1]) ); memcpy( source_rom->team_conference_names + offset, "DEFENSE",   7 ); offset += 7;

     int2pointer( start + offset, &(source_rom->misc_name_pointers[ 2]) ); memcpy( source_rom->team_conference_names + offset, "NORTH",       5 ); offset +=  5;
     int2pointer( start + offset, &(source_rom->misc_name_pointers[ 3]) ); memcpy( source_rom->team_conference_names + offset, "SOUTHEAST",   9 ); offset +=  9;
     int2pointer( start + offset, &(source_rom->misc_name_pointers[ 4]) ); memcpy( source_rom->team_conference_names + offset, "GREAT LAKE", 10 ); offset += 10;
     int2pointer( start + offset, &(source_rom->misc_name_pointers[ 5]) ); memcpy( source_rom->team_conference_names + offset, "PACIFIC",     7 ); offset +=  7;
     int2pointer( start + offset, &(source_rom->misc_name_pointers[ 6]) ); memcpy( source_rom->team_conference_names + offset, "ATLANTIC",    8 ); offset +=  8;
     int2pointer( start + offset, &(source_rom->misc_name_pointers[ 7]) ); memcpy( source_rom->team_conference_names + offset, "SOUTHWEST",   9 ); offset +=  9;

     int2pointer( start + offset, &(source_rom->misc_name_pointers[ 8]) ); memcpy( source_rom->team_conference_names + offset, "MAN",       3 ); offset += 3;
     int2pointer( start + offset, &(source_rom->misc_name_pointers[ 9]) ); memcpy( source_rom->team_conference_names + offset, "COA",       3 ); offset += 3;
     int2pointer( start + offset, &(source_rom->misc_name_pointers[10]) ); memcpy( source_rom->team_conference_names + offset, "COM",       3 ); offset += 3;
     int2pointer( start + offset, &(source_rom->misc_name_pointers[11]) ); memcpy( source_rom->team_conference_names + offset, "SKP",       3 ); offset += 3;

     int2pointer( start + offset, &(source_rom->misc_name_pointers[12]) ); memcpy( source_rom->team_conference_names + offset, "NEW ENGLAND", 11 ); offset += 11;
     int2pointer( start + offset, &(source_rom->misc_name_pointers[13]) ); memcpy( source_rom->team_conference_names + offset, "MIDWEST",      7 ); offset +=  7;

     int2pointer( start + offset, &(source_rom->misc_name_pointers[14]) );
     int2pointer( start + offset, &(source_rom->misc_name_pointers[15]) );
}

static void printTeamNames( tsbrom_s *source_rom )
{
     int start  = TEAM_CONF_NAMES_START_OFFSET;
     int offset = 0;

     for ( int i = 0; i < NUM_TEAMS; ++i )
     {
          int start_offset = pointer2int( &(source_rom->team_name_pointers[i    ]) ) - TEAM_CONF_NAMES_START_OFFSET;
          int end_offset   = pointer2int( &(source_rom->team_name_pointers[i + 1]) ) - TEAM_CONF_NAMES_START_OFFSET;

          printf( "%.*s\n", end_offset - start_offset, source_rom->team_conference_names + start_offset );
     }
}

int main( const int argc, const char const *argv[] )
{
     /**/  tsbrom_s         *source_rom    = NULL;
     /**/  tsbrom_s         *output_rom    = NULL;
     const char             *source_path   = NULL;
     const char             *output_path   = NULL;

     if ( argc != 3 )
     {
          printf( "Usage: %s <source rom> <output rom>\n", argv[0] );

          return EXIT_SUCCESS;
     }

     source_path   = argv[1];
     output_path   = argv[2];

     if ( (source_rom = readTsbRom( source_path )) == NULL )
     {
          printf( "Unable to load source rom: %s\n", getFileUtilsError() );

          freeAllocations();

          return EXIT_FAILURE;
     }

     registerAllocation( source_rom );

     printf( "Before:\n" );
     printTeamNames( source_rom );

     replaceTeamNames( source_rom );

     printf( "After:\n" );
     printTeamNames( source_rom );

     output_rom = source_rom;

     if ( ! writeTsbRom( output_path, output_rom ) )
     {
          printf( "Unable to save output rom: %s\n", getFileUtilsError() );

          freeAllocations();

          return EXIT_FAILURE;
     }

     freeAllocations();

     return EXIT_SUCCESS;
}
