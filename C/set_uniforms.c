#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_formats.h"

static const char *teams[] = {

     "Delaware",
     "Maryland",
     "New_Jersey",
     "Pennsylvania",
     "Virginia",
     "West_Virginia",
     "Illinois",
     "Michigan",
     "Minnesota",
     "New_York",
     "Ohio",
     "Wisconsin",
     "Arkansas",
     "Indiana",
     "Kansas",
     "Kentucky",
     "Missouri",
     "Tennessee",
     "Connecticut",
     "Maine",
     "Massachusetts",
     "New_Hampshire",
     "Rhode_Island",
     "Vermont",
     "Iowa",
     "Montana",
     "Nebraska",
     "North_Dakota",
     "South_Dakota",
     "Wyoming",
     "California",
     "Idaho",
     "Nevada",
     "Oregon",
     "Utah",
     "Washington",
     "Alabama",
     "Florida",
     "Georgia",
     "Mississippi",
     "North_Carolina",
     "South_Carolina",
     "Arizona",
     "Colorado",
     "Louisiana",
     "New_Mexico",
     "Oklahoma",
     "Texas"
};

static boolean_e setUniforms( tsbrom_s *rom1, tsbrom_s *rom2, char *road, char *home )
{
     tsbrom_s *rom      = rom1;
     int       home_idx = -1;
     int       road_idx = -1;

     for ( int i = 0; i < 48; ++i )
     {
          if ( home_idx < 0  &&  strcmp( home, teams[i] ) == 0 ) home_idx = i;
          if ( road_idx < 0  &&  strcmp( road, teams[i] ) == 0 ) road_idx = i;
     }

     if ( home_idx < 0  ||  road_idx < 0 ) return bl_False;

     if ( home_idx >= 24 )
     {
          rom = rom2;

          home_idx %= 24;
          road_idx %= 24;
     }

     rom->field_uniforms[home_idx].use_flags[0] = 0;
     rom->field_uniforms[home_idx].use_flags[1] = 0;
     rom->field_uniforms[home_idx].use_flags[2] = 0;
     rom->field_uniforms[home_idx].use_flags[3] = 0;

     rom->field_uniforms[road_idx].use_flags[0] = 0xff;
     rom->field_uniforms[road_idx].use_flags[1] = 0xff;
     rom->field_uniforms[road_idx].use_flags[2] = 0xff;
     rom->field_uniforms[road_idx].use_flags[3] = 0xfc;

     rom->cutscene_uniforms[home_idx].use_flags[0] = 0;
     rom->cutscene_uniforms[home_idx].use_flags[1] = 0;
     rom->cutscene_uniforms[home_idx].use_flags[2] = 0;
     rom->cutscene_uniforms[home_idx].use_flags[3] = 0;

     rom->cutscene_uniforms[road_idx].use_flags[0] = 0xff;
     rom->cutscene_uniforms[road_idx].use_flags[1] = 0xff;
     rom->cutscene_uniforms[road_idx].use_flags[2] = 0xff;
     rom->cutscene_uniforms[road_idx].use_flags[3] = 0xfc;

     return bl_True;
}

int main( const int argc, const char const *argv[] )
{
     const char     *rom1_filename    = NULL;
     const char     *rom2_filename    = NULL;
     const char     *rom1_output_file = NULL;
     const char     *rom2_output_file = NULL;
     /**/  tsbrom_s *rom1             = NULL;
     /**/  tsbrom_s *rom2             = NULL;
     /**/  char     *games            = NULL;
     /**/  char      home [30]        = { 0 };
     /**/  char      road [30]        = { 0 };


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

     if ( (games  = getenv( "GAMES" )) == NULL )
     {
          printf( "Environment variable GAMES not found.\n" );

          return EXIT_FAILURE;
     }

     while ( sscanf( games, "%s %s ", road, home ) != EOF )
     {
          if ( ! setUniforms( rom1, rom2, road, home ) )
          {
               printf( "Unknown team in game %s @ %s\n", road, home );

               return EXIT_FAILURE;
          }

          games += strlen(road) + strlen(home) + 2;
     }

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
