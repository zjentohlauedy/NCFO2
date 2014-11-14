#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_formats.h"


static char *map_name( char *name )
{
     if ( strcmp( name, "North_Carolina" ) == 0 ) return "N. Carolina";
     if ( strcmp( name, "South_Carolina" ) == 0 ) return "S. Carolina";

     for ( int i = 0; i < strlen(name); ++i )
     {
          if ( name[i] == '_' ) name[i] = ' ';
     }

     return name;
}

static int findTeamIdx( tsbrom_s *rom, char *team )
{
     char *team_loc = uppercase( team );
     int   team_len = strlen(team_loc);

     for ( int team_idx = 0; team_idx < 28; ++team_idx )
     {
          int offset = pointer2int( &(rom->team_loc_pointers[team_idx]) ) - 0xbcf0;

          if ( memcmp( rom->team_conference_names + offset, team_loc, team_len ) == 0 ) return team_idx;
     }

     printf( "Unable to find team <%s> in rom as <%s>\n", team, team_loc );

     return -1;
}

static boolean_e setPlayoffUniforms( tsbrom_s *rom, char *road, char *home )
{
     int       road_idx = -1;
     int       home_idx = -1;

     road_idx = findTeamIdx( rom, road );
     home_idx = findTeamIdx( rom, home );

     if ( road_idx < 0  ||  home_idx < 0 ) return bl_False;

     rom->field_uniforms[road_idx].use_flags[0] = 0;
     rom->field_uniforms[road_idx].use_flags[1] = 0;
     rom->field_uniforms[road_idx].use_flags[2] = 0;
     rom->field_uniforms[road_idx].use_flags[3] = 0;

     rom->cutscene_uniforms[road_idx].use_flags[0] = 0;
     rom->cutscene_uniforms[road_idx].use_flags[1] = 0;
     rom->cutscene_uniforms[road_idx].use_flags[2] = 0;
     rom->cutscene_uniforms[road_idx].use_flags[3] = 0;

     rom->field_uniforms[home_idx].use_flags[0] = 0;
     rom->field_uniforms[home_idx].use_flags[1] = 0;
     rom->field_uniforms[home_idx].use_flags[2] = 0;
     rom->field_uniforms[home_idx].use_flags[3] = 0;

     rom->cutscene_uniforms[home_idx].use_flags[0] = 0;
     rom->cutscene_uniforms[home_idx].use_flags[1] = 0;
     rom->cutscene_uniforms[home_idx].use_flags[2] = 0;
     rom->cutscene_uniforms[home_idx].use_flags[3] = 0;


     if ( rom->field_uniforms[home_idx].home[0] == rom->field_uniforms[road_idx].home[0] &&
          rom->field_uniforms[home_idx].home[2] == rom->field_uniforms[road_idx].home[2]    )
     {
          printf( "%s switching to road uniforms.\n", road );

          rom->field_uniforms[road_idx].use_flags[0] = 0xff;
          rom->field_uniforms[road_idx].use_flags[1] = 0xff;
          rom->field_uniforms[road_idx].use_flags[2] = 0xff;
          rom->field_uniforms[road_idx].use_flags[3] = 0xfc;

          rom->cutscene_uniforms[road_idx].use_flags[0] = 0xff;
          rom->cutscene_uniforms[road_idx].use_flags[1] = 0xff;
          rom->cutscene_uniforms[road_idx].use_flags[2] = 0xff;
          rom->cutscene_uniforms[road_idx].use_flags[3] = 0xfc;
     }

     return bl_True;
}

int main( const int argc, const char const *argv[] )
{
     const char     *rom_filename    = NULL;
     const char     *rom_output_file = NULL;
     /**/  tsbrom_s *rom             = NULL;
     /**/  char     *games           = NULL;
     /**/  char      home [30]        = { 0 };
     /**/  char      road [30]        = { 0 };


     if ( argc != 3 )
     {
          printf( "Usage: %s <input playoff rom> <output playoff rom>\n", argv[0] );

          return EXIT_SUCCESS;
     }

     rom_filename    = argv[1];
     rom_output_file = argv[2];

     if ( (rom = readTsbRom( rom_filename )) == NULL )
     {
          printf( "Error reading Playoff Rom: %s\n", getFileUtilsError() );

          return EXIT_FAILURE;
     }

     if ( (games  = getenv( "GAMES" )) == NULL )
     {
          printf( "Environment variable GAMES not found.\n" );

          return EXIT_FAILURE;
     }

     while ( sscanf( games, "%s %s ", road, home ) != EOF )
     {
          if ( ! setPlayoffUniforms( rom, map_name( road ), map_name( home ) ) )
          {
               printf( "Unknown team in game %s @ %s\n", road, home );

               free( rom );

               return EXIT_FAILURE;
          }

          games += strlen(road) + strlen(home) + 2;
     }

     if ( ! writeTsbRom( rom_output_file, rom ) )
     {
          printf( "Unable to write to file %s: %s\n", rom_output_file, getFileUtilsError() );

          free( rom );

          return EXIT_FAILURE;
     }

     free( rom );

     return EXIT_SUCCESS;
}
