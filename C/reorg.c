#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_formats.h"

static const int idx_map[] = { 6, 7, 8, 23, 10, 11, 18, 21, 38, 9, 22, 41, 42, 19, 37, 20, 45, 17, 0, 1, 2, 3, 4, 5, 36, 31, 32, 39, 40, 35, 25, 30, 27, 33, 29, 34, 12, 13, 14, 44, 15, 16, 24, 26, 43, 28, 46, 47 };

static const struct { char *name; char *location; char *abbrev; } teams1[] = {

     { "BLUE HENS",  "DELAWARE",      "UDE" },
     { "TERRAPINS",  "MARYLAND",      "UMD" },
     { "S. KNIGHTS", "NEW JERSEY",    "UNJ" },
     { "N. LIONS",   "PENNSYLVANIA",  "UPA" },
     { "CAVALIERS",  "VIRGINIA",      "UVA" },
     { "MNTNEERS",   "WEST VIRGINIA", "UWV" },
     { "F. ILLINI",  "ILLINOIS",      "UIL" },
     { "WOLVERINES", "MICHIGAN",      "UMI" },
     { "G. GOPHERS", "MINNESOTA",     "UMN" },
     { "ORANGEMEN",  "NEW YORK",      "UNY" },
     { "BUCKEYES",   "OHIO",          "UOH" },
     { "BADGERS",    "WISCONSIN",     "UWI" },
     { "RAZORBACKS", "ARKANSAS",      "UAR" },
     { "HOOSIERS",   "INDIANA",       "UIN" },
     { "JAYHAWKS",   "KANSAS",        "UKS" },
     { "WILDCATS",   "KENTUCKY",      "UKY" },
     { "TIGERS",     "MISSOURI",      "UMO" },
     { "VOLUNTEERS", "TENNESSEE",     "UTN" },
     { "HUSKIES",    "CONNECTICUT",   "UCT" },
     { "B. BEARS",   "MAINE",         "UME" },
     { "MINUTEMEN",  "MASSACHUSETTS", "UMA" },
     { "BIG GREEN",  "NEW HAMPSHIRE", "UNH" },
     { "RAMS",       "RHODE ISLAND",  "URI" },
     { "CATAMOUNTS", "VERMONT",       "UVT" }
};

static const struct { char *name; char *location; char *abbrev; } teams2[] = {

     { "HAWKEYES",   "IOWA",          "UIA" },
     { "GRIZZLIES",  "MONTANA",       "UMT" },
     { "C. HUSKERS", "NEBRASKA",      "UNE" },
     { "F. SIOUX",   "NORTH DAKOTA",  "UND" },
     { "COYOTES",    "SOUTH DAKOTA",  "USD" },
     { "COWBOYS",    "WYOMING",       "UWY" },
     { "G. BEARS",   "CALIFORNIA",    "UCA" },
     { "VANDALS",    "IDAHO",         "UID" },
     { "WOLF PACK",  "NEVADA",        "UNV" },
     { "DUCKS",      "OREGON",        "UOR" },
     { "UTES",       "UTAH",          "UUT" },
     { "COUGARS",    "WASHINGTON",    "UWA" },
     { "C. TIDE",    "ALABAMA",       "UAL" },
     { "GATORS",     "FLORIDA",       "UFL" },
     { "BULLDOGS",   "GEORGIA",       "UGA" },
     { "REBELS",     "MISSISSIPPI",   "UMS" },
     { "TARHEELS",   "N. CAROLINA",   "UNC" },
     { "GAMECOCKS",  "S. CAROLINA",   "USC" },
     { "SUN DEVILS", "ARIZONA",       "UAZ" },
     { "BUFFALOS",   "COLORADO",      "UCO" },
     { "R. CAJUNS",  "LOUISIANA",     "ULA" },
     { "LOBOS",      "NEW MEXICO",    "UNM" },
     { "SOONERS",    "OKLAHOMA",      "UOK" },
     { "LONGHORNS",  "TEXAS",         "UTX" }
};

static void reorderBigHelmetData( tsbrom_s *rom1, tsbrom_s *rom2 )
{
     helmet_colors_s  source      [48] = { 0 };
     unsigned char    source_types[48] = { 0 };

     for ( int i = 0; i < 24; ++i )
     {
          source[i     ] = rom1->helmet_colors[i];
          source[i + 24] = rom2->helmet_colors[i];

          source_types[i     ] = rom1->helmet_types[i];
          source_types[i + 24] = rom2->helmet_types[i];
     }

     for ( int i = 0; i < 48; ++i )
     {
          tsbrom_s *rom = (i < 24) ? rom1 : rom2;

          rom->helmet_colors[i % 24] = source      [idx_map[i]];
          rom->helmet_types [i % 24] = source_types[idx_map[i]];
     }
}

static void reorderMiniHelmetData( tsbrom_s *rom1, tsbrom_s *rom2 )
{
     mini_helmet_s  source[48] = { 0 };

     for ( int i = 0; i < 24; ++i )
     {
          source[i     ] = rom1->mini_helmets[i];
          source[i + 24] = rom2->mini_helmets[i];
     }

     for ( int i = 0; i < 48; ++i )
     {
          tsbrom_s *rom = (i < 24) ? rom1 : rom2;

          rom->mini_helmets[i % 24] = source[idx_map[i]];
     }
}

static void reorderUniformData( tsbrom_s *rom1, tsbrom_s *rom2 )
{
     tsb_uniforms_s   field_source    [48] = { 0 };
     tsb_uniforms2_s  cutscene_source [48] = { 0 };

     for ( int i = 0; i < 24; ++i )
     {
          field_source[i     ] = rom1->field_uniforms[i];
          field_source[i + 24] = rom2->field_uniforms[i];

          cutscene_source[i     ] = rom1->cutscene_uniforms[i];
          cutscene_source[i + 24] = rom2->cutscene_uniforms[i];
     }

     for ( int i = 0; i < 48; ++i )
     {
          tsbrom_s *rom = (i < 24) ? rom1 : rom2;

          rom->field_uniforms   [i % 24] = field_source   [idx_map[i]];
          rom->cutscene_uniforms[i % 24] = cutscene_source[idx_map[i]];
     }
}

static void reorderBackgroundColors( tsbrom_s *rom1, tsbrom_s *rom2 )
{
     unsigned char  source[48] = { 0 };

     for ( int i = 0; i < 24; ++i )
     {
          source[i     ] = rom1->team_background_colors[i];
          source[i + 24] = rom2->team_background_colors[i];
     }

     for ( int i = 0; i < 48; ++i )
     {
          tsbrom_s *rom = (i < 24) ? rom1 : rom2;

          rom->team_background_colors[i % 24] = source[idx_map[i]];
     }
}

static void updateTeamInfo1( tsbrom_s *rom )
{
     const int start   = 0xbcf0;
     /**/  int offset  = 0;

     memset( rom->team_conference_names, 0xff, sizeof(rom->team_conference_names) );

     for ( int i = 0; i < 24; ++i )
     {
          int2pointer( start + offset, &(rom->team_abbr_pointers[i]) );

          memcpy( rom->team_conference_names + offset, teams1[i].abbrev, strlen(teams1[i].abbrev) ); offset += strlen(teams1[i].abbrev);

          *(rom->team_conference_names + offset) = ' '; offset++;
     }

     int2pointer( start + offset, &(rom->team_abbr_pointers[24]) );
     int2pointer( start + offset, &(rom->team_abbr_pointers[25]) );
     int2pointer( start + offset, &(rom->team_abbr_pointers[26]) );
     int2pointer( start + offset, &(rom->team_abbr_pointers[27]) );

     for ( int i = 0; i < 4; ++i )
     {
          int2pointer( start + offset, &(rom->conference_abbr_pointers[i]) );

          *(rom->team_conference_names + offset) = ' '; offset++;
     }

     for ( int i = 0; i < 24; ++i )
     {
          int2pointer( start + offset, &(rom->team_loc_pointers[i]) );

          memcpy( rom->team_conference_names + offset, teams1[i].location, strlen(teams1[i].location) ); offset += strlen(teams1[i].location);
     }

     int2pointer( start + offset, &(rom->team_loc_pointers[24]) );
     int2pointer( start + offset, &(rom->team_loc_pointers[25]) );
     int2pointer( start + offset, &(rom->team_loc_pointers[26]) );
     int2pointer( start + offset, &(rom->team_loc_pointers[27]) );

     for ( int i = 0; i < 4; ++i ) int2pointer( start + offset, &(rom->conference_loc_pointers[i]) );

     for ( int i = 0; i < 24; ++i )
     {
          int2pointer( start + offset, &(rom->team_name_pointers[i]) );

          memcpy( rom->team_conference_names + offset, teams1[i].name, strlen(teams1[i].name) ); offset += strlen(teams1[i].name);
     }

     int2pointer( start + offset, &(rom->team_name_pointers[24]) );
     int2pointer( start + offset, &(rom->team_name_pointers[25]) );
     int2pointer( start + offset, &(rom->team_name_pointers[26]) );
     int2pointer( start + offset, &(rom->team_name_pointers[27]) );

     for ( int i = 0; i < 4; ++i ) int2pointer( start + offset, &(rom->conference_name_pointers[i]) );

     int2pointer( start + offset, &(rom->down_name_pointers[0]) ); memcpy( rom->team_conference_names + offset, "1ST", 3 ); offset += 3;
     int2pointer( start + offset, &(rom->down_name_pointers[1]) ); memcpy( rom->team_conference_names + offset, "2ND", 3 ); offset += 3;
     int2pointer( start + offset, &(rom->down_name_pointers[2]) ); memcpy( rom->team_conference_names + offset, "3RD", 3 ); offset += 3;
     int2pointer( start + offset, &(rom->down_name_pointers[3]) ); memcpy( rom->team_conference_names + offset, "4TH", 3 ); offset += 3;

     int2pointer( start + offset, &(rom->unknown_pointers1[0]) ); memcpy( rom->team_conference_names + offset, "\xd6\xd7\xb8", 3 ); offset += 3;
     int2pointer( start + offset, &(rom->unknown_pointers1[1]) ); memcpy( rom->team_conference_names + offset, "\xd4\xd5\xd7", 3 ); offset += 3;
     int2pointer( start + offset, &(rom->unknown_pointers1[2]) ); memcpy( rom->team_conference_names + offset, "\xd8\xd9\xda", 3 ); offset += 3;
     int2pointer( start + offset, &(rom->unknown_pointers1[3]) ); memcpy( rom->team_conference_names + offset, "\xdb\xdc\xdd", 3 ); offset += 3;

     int2pointer( start + offset, &(rom->misc_name_pointers[ 0]) ); memcpy( rom->team_conference_names + offset, "OFFENSE",      7 ); offset +=  7;
     int2pointer( start + offset, &(rom->misc_name_pointers[ 1]) ); memcpy( rom->team_conference_names + offset, "DEFENSE",      7 ); offset +=  7;
     int2pointer( start + offset, &(rom->misc_name_pointers[ 2]) ); memcpy( rom->team_conference_names + offset, "ATLANTIC",     8 ); offset +=  8;
     int2pointer( start + offset, &(rom->misc_name_pointers[ 3]) ); memcpy( rom->team_conference_names + offset, "GREAT LAKE",  10 ); offset += 10;
     int2pointer( start + offset, &(rom->misc_name_pointers[ 4]) ); memcpy( rom->team_conference_names + offset, "MIDWEST",      7 ); offset +=  7;
     int2pointer( start + offset, &(rom->misc_name_pointers[ 5]) ); memcpy( rom->team_conference_names + offset, "NEW ENGLAND", 11 ); offset += 11;
     int2pointer( start + offset, &(rom->misc_name_pointers[ 6]) ); memcpy( rom->team_conference_names + offset, "AFC",          3 ); offset +=  3;
     int2pointer( start + offset, &(rom->misc_name_pointers[ 7]) ); memcpy( rom->team_conference_names + offset, "NFC",          3 ); offset +=  3;
     int2pointer( start + offset, &(rom->misc_name_pointers[ 8]) ); memcpy( rom->team_conference_names + offset, "MAN",          3 ); offset +=  3;
     int2pointer( start + offset, &(rom->misc_name_pointers[ 9]) ); memcpy( rom->team_conference_names + offset, "COA",          3 ); offset +=  3;
     int2pointer( start + offset, &(rom->misc_name_pointers[10]) ); memcpy( rom->team_conference_names + offset, "COM",          3 ); offset +=  3;
     int2pointer( start + offset, &(rom->misc_name_pointers[11]) ); memcpy( rom->team_conference_names + offset, "SKP",          3 ); offset +=  3;

     int2pointer( start + offset, &(rom->misc_name_pointers[12]) );
     int2pointer( start + offset, &(rom->misc_name_pointers[13]) );
     int2pointer( start + offset, &(rom->misc_name_pointers[14]) );
     int2pointer( start + offset, &(rom->misc_name_pointers[15]) );
}

static void updateTeamInfo2( tsbrom_s *rom )
{
     const int start   = 0xbcf0;
     /**/  int offset  = 0;

     memset( rom->team_conference_names, 0xff, sizeof(rom->team_conference_names) );

     for ( int i = 0; i < 24; ++i )
     {
          int2pointer( start + offset, &(rom->team_abbr_pointers[i]) );

          memcpy( rom->team_conference_names + offset, teams2[i].abbrev, strlen(teams2[i].abbrev) ); offset += strlen(teams2[i].abbrev);

          *(rom->team_conference_names + offset) = ' '; offset++;
     }

     int2pointer( start + offset, &(rom->team_abbr_pointers[24]) );
     int2pointer( start + offset, &(rom->team_abbr_pointers[25]) );
     int2pointer( start + offset, &(rom->team_abbr_pointers[26]) );
     int2pointer( start + offset, &(rom->team_abbr_pointers[27]) );

     for ( int i = 0; i < 4; ++i )
     {
          int2pointer( start + offset, &(rom->conference_abbr_pointers[i]) );

          *(rom->team_conference_names + offset) = ' '; offset++;
     }

     for ( int i = 0; i < 24; ++i )
     {
          int2pointer( start + offset, &(rom->team_loc_pointers[i]) );

          memcpy( rom->team_conference_names + offset, teams2[i].location, strlen(teams2[i].location) ); offset += strlen(teams2[i].location);
     }

     int2pointer( start + offset, &(rom->team_loc_pointers[24]) );
     int2pointer( start + offset, &(rom->team_loc_pointers[25]) );
     int2pointer( start + offset, &(rom->team_loc_pointers[26]) );
     int2pointer( start + offset, &(rom->team_loc_pointers[27]) );

     for ( int i = 0; i < 4; ++i ) int2pointer( start + offset, &(rom->conference_loc_pointers[i]) );

     for ( int i = 0; i < 24; ++i )
     {
          int2pointer( start + offset, &(rom->team_name_pointers[i]) );

          memcpy( rom->team_conference_names + offset, teams2[i].name, strlen(teams2[i].name) ); offset += strlen(teams2[i].name);
     }

     int2pointer( start + offset, &(rom->team_name_pointers[24]) );
     int2pointer( start + offset, &(rom->team_name_pointers[25]) );
     int2pointer( start + offset, &(rom->team_name_pointers[26]) );
     int2pointer( start + offset, &(rom->team_name_pointers[27]) );

     for ( int i = 0; i < 4; ++i ) int2pointer( start + offset, &(rom->conference_name_pointers[i]) );

     int2pointer( start + offset, &(rom->down_name_pointers[0]) ); memcpy( rom->team_conference_names + offset, "1ST", 3 ); offset += 3;
     int2pointer( start + offset, &(rom->down_name_pointers[1]) ); memcpy( rom->team_conference_names + offset, "2ND", 3 ); offset += 3;
     int2pointer( start + offset, &(rom->down_name_pointers[2]) ); memcpy( rom->team_conference_names + offset, "3RD", 3 ); offset += 3;
     int2pointer( start + offset, &(rom->down_name_pointers[3]) ); memcpy( rom->team_conference_names + offset, "4TH", 3 ); offset += 3;

     int2pointer( start + offset, &(rom->unknown_pointers1[0]) ); memcpy( rom->team_conference_names + offset, "\xd6\xd7\xb8", 3 ); offset += 3;
     int2pointer( start + offset, &(rom->unknown_pointers1[1]) ); memcpy( rom->team_conference_names + offset, "\xd4\xd5\xd7", 3 ); offset += 3;
     int2pointer( start + offset, &(rom->unknown_pointers1[2]) ); memcpy( rom->team_conference_names + offset, "\xd8\xd9\xda", 3 ); offset += 3;
     int2pointer( start + offset, &(rom->unknown_pointers1[3]) ); memcpy( rom->team_conference_names + offset, "\xdb\xdc\xdd", 3 ); offset += 3;

     int2pointer( start + offset, &(rom->misc_name_pointers[ 0]) ); memcpy( rom->team_conference_names + offset, "OFFENSE",   7 ); offset += 7;
     int2pointer( start + offset, &(rom->misc_name_pointers[ 1]) ); memcpy( rom->team_conference_names + offset, "DEFENSE",   7 ); offset += 7;
     int2pointer( start + offset, &(rom->misc_name_pointers[ 2]) ); memcpy( rom->team_conference_names + offset, "NORTH",     5 ); offset += 5;
     int2pointer( start + offset, &(rom->misc_name_pointers[ 3]) ); memcpy( rom->team_conference_names + offset, "PACIFIC",   7 ); offset += 7;
     int2pointer( start + offset, &(rom->misc_name_pointers[ 4]) ); memcpy( rom->team_conference_names + offset, "SOUTHEAST", 9 ); offset += 9;
     int2pointer( start + offset, &(rom->misc_name_pointers[ 5]) ); memcpy( rom->team_conference_names + offset, "SOUTHWEST", 9 ); offset += 9;
     int2pointer( start + offset, &(rom->misc_name_pointers[ 6]) ); memcpy( rom->team_conference_names + offset, "AFC",       3 ); offset += 3;
     int2pointer( start + offset, &(rom->misc_name_pointers[ 7]) ); memcpy( rom->team_conference_names + offset, "NFC",       3 ); offset += 3;
     int2pointer( start + offset, &(rom->misc_name_pointers[ 8]) ); memcpy( rom->team_conference_names + offset, "MAN",       3 ); offset += 3;
     int2pointer( start + offset, &(rom->misc_name_pointers[ 9]) ); memcpy( rom->team_conference_names + offset, "COA",       3 ); offset += 3;
     int2pointer( start + offset, &(rom->misc_name_pointers[10]) ); memcpy( rom->team_conference_names + offset, "COM",       3 ); offset += 3;
     int2pointer( start + offset, &(rom->misc_name_pointers[11]) ); memcpy( rom->team_conference_names + offset, "SKP",       3 ); offset += 3;

     int2pointer( start + offset, &(rom->misc_name_pointers[12]) );
     int2pointer( start + offset, &(rom->misc_name_pointers[13]) );
     int2pointer( start + offset, &(rom->misc_name_pointers[14]) );
     int2pointer( start + offset, &(rom->misc_name_pointers[15]) );
}

int main( const int argc, const char const *argv[] )
{
     const char     *rom1_filename    = NULL;
     const char     *rom2_filename    = NULL;
     const char     *rom1_output_file = NULL;
     const char     *rom2_output_file = NULL;
     /**/  tsbrom_s *rom1             = NULL;
     /**/  tsbrom_s *rom2             = NULL;

     if ( argc != 5 )
     {
          printf( "Usage: %s <input rom 1> <input rom 2> <output rom 1> <output rom 2>\n", argv[0] );

          return EXIT_SUCCESS;
     }

     rom1_filename = argv[1];
     rom2_filename = argv[2];

     rom1_output_file = argv[3];
     rom2_output_file = argv[4];

     if ( (rom1 = readTsbRom( rom1_filename )) == NULL )
     {
          printf( "Error reading Rom 1: %s\n", getFileUtilsError() );

          return EXIT_FAILURE;
     }

     if ( (rom2 = readTsbRom( rom2_filename )) == NULL )
     {
          printf( "Error reading Rom 2: %s\n", getFileUtilsError() );

          free( rom1 );

          return EXIT_FAILURE;
     }

     printf( "Reorganizing...\n" );

     reorderBigHelmetData( rom1, rom2 );

     reorderMiniHelmetData( rom1, rom2 );

     reorderUniformData( rom1, rom2 );

     reorderBackgroundColors( rom1, rom2 );

     updateTeamInfo1( rom1 );
     updateTeamInfo2( rom2 );

     if ( ! writeTsbRom( rom1_output_file, rom1 ) )
     {
          printf( "Unable to write to file %s: %s\n", rom1_output_file, getFileUtilsError() );

          free( rom1 );
          free( rom2 );

          return EXIT_FAILURE;
     }

     if ( ! writeTsbRom( rom2_output_file, rom2 ) )
     {
          printf( "Unable to write to file %s: %s\n", rom2_output_file, getFileUtilsError() );

          free( rom1 );
          free( rom2 );

          return EXIT_FAILURE;
     }

     free( rom1 );
     free( rom2 );

     return EXIT_SUCCESS;
}
