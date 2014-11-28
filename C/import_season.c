#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include "conversion.h"
#include "file_formats.h"
#include "save_state.h"


int main( const int argc, const char *argv[] )
{
     const char              *rom1_filename = NULL;
     const char              *rom2_filename = NULL;
     const char              *ss1_filename  = NULL;
     const char              *ss2_filename  = NULL;
     const char              *db_filename   = NULL;

     unsigned char    *state_file1   = NULL;
     unsigned char    *state_file2   = NULL;
     nst_save_state_s *save_state1   = NULL;
     nst_save_state_s *save_state2   = NULL;
     tsbrom_s         *rom1          = NULL;
     tsbrom_s         *rom2          = NULL;
     organization_s   *organization  = NULL;
     sqlite3          *db            = NULL;
     int               season        = 0;


     if ( argc != 7 )
     {
          printf( "Usage: %s <database> <rom-file1> <save-state1> <rom-file2> <save-state2> <season>\n", argv[0] );

          return EXIT_SUCCESS;
     }

     db_filename   = argv[1];

     rom1_filename = argv[2];
     rom2_filename = argv[4];

     ss1_filename  = argv[3];
     ss2_filename  = argv[5];

     errno = 0;

     season = strtol( argv[6], NULL, 10 );

     if ( errno != 0 )
     {
          printf( "Unable to determine season: %s\n", strerror( errno ) );

          return EXIT_FAILURE;
     }

     if ( season < 1 )
     {
          printf( "Invalid season: <%s>\n", argv[6] );

          return EXIT_FAILURE;
     }

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

     if ( ss1_filename != NULL )
     {
          int len = 0;

          if ( (state_file1 = readNstSaveState( ss1_filename, &len )) == NULL )
          {
               printf( "Error reading Save State 1: %s\n", getFileUtilsError() );

               free( rom1 );
               free( rom2 );

               return EXIT_FAILURE;
          }

          if ( (save_state1 = getSaveStateStats( state_file1, len )) == NULL )
          {
               printf( "Error retrieving stats from Save State 1: %s\n", getFileUtilsError() );

               free( rom1 );
               free( rom2 );

               return EXIT_FAILURE;
          }
     }

     if ( ss2_filename != NULL )
     {
          int len = 0;

          if ( (state_file2 = readNstSaveState( ss2_filename, &len )) == NULL )
          {
               printf( "Error reading Save State 2: %s\n", getFileUtilsError() );

               free( rom1 );
               free( rom2 );
               free( state_file1 );

               return EXIT_FAILURE;
          }

          if ( (save_state2 = getSaveStateStats( state_file2, len )) == NULL )
          {
               printf( "Error retrieving stats from Save State 2: %s\n", getFileUtilsError() );

               free( rom1 );
               free( rom2 );
               free( state_file1 );

               return EXIT_FAILURE;
          }
     }

     if ( (organization = convertOrganization( rom1, save_state1, rom2, save_state2, season, bg_None )) == NULL )
     {
          printf( "Error converting rom and save state data: %s\n", getConvertOrganizationError() );

          free( rom1 );
          free( rom2 );
          free( state_file1 );
          free( state_file2 );

          return EXIT_FAILURE;
     }

     sqlite3_open( db_filename, &db );
     sqlite3_exec( db, "begin", NULL, NULL, NULL );

     if ( save_organization( db, organization ) != SQLITE_OK )
     {
          printf( "Unable to import season data.\n" );

          if ( sqlite3_errcode( db ) != 0 )
          {
               printf( "sqlite3 error message: %s\n", sqlite3_errmsg( db ) );
          }

          sqlite3_exec( db, "rollback", NULL, NULL, NULL );
          sqlite3_close( db );
     }
     else
     {
          sqlite3_exec( db, "commit", NULL, NULL, NULL );
          sqlite3_close( db );
     }

     free( rom1 );
     free( rom2 );
     free( state_file1 );
     free( state_file2 );

     free_organization( organization );

     return EXIT_SUCCESS;
}
