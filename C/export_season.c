#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <time.h>
#include "file_formats.h"
#include "schedule.h"


static void injectSeason( void *rom, int season )
{
     char season_str[10] = { 0 };

     sprintf( season_str, "%02d", season );

     memcpy( rom + 0x1e129, season_str, 2 );
     memcpy( rom + 0x1e28b, season_str, 2 );
     memcpy( rom + 0x1e2be, season_str, 2 );
     memcpy( rom + 0x1e378, season_str, 2 );
     memcpy( rom + 0x1f89c, season_str, 2 );
}

int main( const int argc, const char const *argv[] )
{
     const char           *rom1_filename;
     const char           *rom2_filename;
     const char           *db_filename;
     /**/  organization_s *organization;
     /**/  schedule_s     *schedule;
     /**/  tsbrom_s       *rom1;
     /**/  tsbrom_s       *rom2;
     /**/  sqlite3        *db;
     /**/  char            rom1_output_file     [FILENAME_MAX + 1] = { 0 };
     /**/  char            rom2_output_file     [FILENAME_MAX + 1] = { 0 };
     /**/  char            schedule_output_file [FILENAME_MAX + 1] = { 0 };

     if ( argc < 4 )
     {
          printf( "Usage %s <rom1> <rom2> <database> <output path>\n", argv[0] );

          return EXIT_SUCCESS;
     }

     rom1_filename = argv[1];
     rom2_filename = argv[2];
     db_filename   = argv[3];

     sprintf( rom1_output_file,     "%s/ncfo1.nes",    argv[4] );
     sprintf( rom2_output_file,     "%s/ncfo2.nes",    argv[4] );
     sprintf( schedule_output_file, "%s/schedule.csv", argv[4] );

     srand( time( NULL ) );

     if ( (rom1 = readTsbRom( rom1_filename )) == NULL )
     {
          printf( "Unable to open rom 1: %s\n", getFileUtilsError() );

          return EXIT_FAILURE;
     }

     if ( (rom2 = readTsbRom( rom2_filename )) == NULL )
     {
          printf( "Unable to open rom 2: %s\n", getFileUtilsError() );

          free( rom1 );

          return EXIT_FAILURE;
     }

     sqlite3_open( db_filename, &db );

     if ( (organization = get_organization( db, 1 )) == NULL )
     {
          printf( "Unable to retrieve organization data from database.\n" );

          free( rom1 );
          free( rom2 );

          sqlite3_close( db );

          return EXIT_FAILURE;
     }

     if ( ! populateRoms( rom1, rom2, organization ) )
     {
          printf( "Error populating roms: %s\n", getPopulateRomsError() );

          free( rom1 );
          free( rom2 );
          free_organization( organization );

          sqlite3_close( db );

          return EXIT_FAILURE;
     }

     injectSeason( rom1, organization->season );
     injectSeason( rom2, organization->season );

     if ( (schedule = generateSchedule( rom1, rom2 )) == NULL )
     {
          printf( "Error generating schedule: %s\n", getGenerateScheduleError() );

          free( rom1 );
          free( rom2 );
          free_organization( organization );

          sqlite3_close( db );

          return EXIT_FAILURE;
     }

     printf( "Creating TSB Rom File %s\n", rom1_output_file );

     if ( ! writeTsbRom( rom1_output_file, rom1 ) )
     {
          printf( "Unable to write to file %s: %s\n", rom1_output_file, getFileUtilsError() );

          free( rom1 );
          free( rom2 );
          freeSchedule( schedule );
          free_organization( organization );

          sqlite3_close( db );

          return EXIT_FAILURE;
     }

     printf( "Creating TSB Rom File %s\n", rom2_output_file );

     if ( ! writeTsbRom( rom2_output_file, rom2 ) )
     {
          printf( "Unable to write to file %s: %s\n", rom2_output_file, getFileUtilsError() );

          free( rom1 );
          free( rom2 );
          freeSchedule( schedule );
          free_organization( organization );

          sqlite3_close( db );

          return EXIT_FAILURE;
     }

     printf( "Creating Schedule File %s\n", schedule_output_file );

     if ( ! writeSchedule( schedule_output_file, schedule ) )
     {
          printf( "Unable to write to file %s: %s\n", schedule_output_file, getFileUtilsError() );

          free( rom1 );
          free( rom2 );
          freeSchedule( schedule );
          free_organization( organization );

          sqlite3_close( db );

          return EXIT_FAILURE;
     }

     free( rom1 );
     free( rom2 );
     freeSchedule( schedule );
     free_organization( organization );

     sqlite3_close( db );

     return EXIT_SUCCESS;
}
