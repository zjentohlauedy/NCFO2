#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <errno.h>
#include <ctype.h>
#include "conversion.h"
#include "file_formats.h"
#include "save_state.h"


static int importTeam( sqlite3 *db, team_s *team )
{
     int rc;

     if ( team->players != NULL )
     {
          for ( int i = 0; team->players[i].player != NULL; ++i )
          {
               if ( (rc = save_player( db, team->players[i].player )) != SQLITE_OK ) return rc;
          }
     }

     if ( (rc = save_team( db, team )) != SQLITE_OK ) return rc;

     return SQLITE_OK;
}

int main( const int argc, const char const *argv[] )
{
     static nst_save_state_s  empty_save_state = { 0 };
     const  char             *rom_filename     = NULL;
     const  char             *ss_filename      = NULL;
     const  char             *db_filename      = NULL;

     organization_s    org            = { 0 };
     tsbrom_s         *rom            = NULL;
     nst_save_state_s *save_state     = NULL;
     unsigned char    *ram            = NULL;
     unsigned char    *state_file     = NULL;
     bowl_game_s      *bowl_game      = NULL;
     int               save_state_len = 0;
     sqlite3          *db             = NULL;
     bowl_game_e       bowl           = bg_None;


     if ( argc != 5 )
     {
          printf( "Usage: %s <database> <rom_file> <save_state_file> <bowl>\n", argv[0] );

          return EXIT_SUCCESS;
     }

     db_filename  = argv[1];

     rom_filename = argv[2];
     ss_filename  = argv[3];

     errno = 0;

     bowl = strtol( argv[4], NULL, 10 );

     if ( errno != 0 )
     {
          printf( "Unable to determine bowl: %s\n", strerror( errno ) );

          return EXIT_FAILURE;
     }

     if ( bowl <= bg_None  ||  bowl > bg_NCFOChampionship )
     {
          printf( "Invalid bowl: <%s>\n", argv[4] );

          return EXIT_FAILURE;
     }

     if ( (rom = readTsbRom( rom_filename )) == NULL )
     {
          printf( "Error reading rom: %s\n", getFileUtilsError() );

          return EXIT_FAILURE;
     }

     if ( (state_file = readNstSaveState( ss_filename, &save_state_len )) == NULL )
     {
          printf( "Error reading Save State: %s\n", getFileUtilsError() );

          free( rom );

          return EXIT_FAILURE;
     }

     if ( (save_state = getSaveStateStats( state_file, save_state_len )) == NULL )
     {
          printf( "Error retrieving stats from Save State: %s\n", getFileUtilsError() );

          free( rom        );
          free( state_file );

          return EXIT_FAILURE;
     }

     if ( (ram = getSaveStateRam( state_file, save_state_len )) == NULL )
     {
          printf( "Error retrieving RAM from Save State: %s\n", getFileUtilsError() );

          free( rom        );
          free( state_file );

          return EXIT_FAILURE;
     }

     copyScores( save_state, ram );

     sqlite3_open( db_filename, &db );
     sqlite3_exec( db, "begin", NULL, NULL, NULL );

     org.organization_id = 1;

     if ( organizations_t_read( db, &org ) != SQLITE_OK )
     {
          printf( "Error retrieving organization.\n" );

          if ( sqlite3_errcode( db ) != 0 )
          {
               printf( "sqlite3 error message: %s\n", sqlite3_errmsg( db ) );
          }

          sqlite3_exec( db, "rollback", NULL, NULL, NULL );
          sqlite3_close( db );

          free( rom        );
          free( state_file );

          freeBowlGame( bowl_game );

          return EXIT_FAILURE;
     }

     if ( (bowl_game = convertBowlGame( rom, save_state, org.season, bowl )) == NULL )
     {
          printf( "Error converting bowl game: %s\n", getConvertBowlGameError() );

          sqlite3_exec( db, "rollback", NULL, NULL, NULL );
          sqlite3_close( db );

          free( rom        );
          free( state_file );

          return EXIT_FAILURE;
     }

     if ( importTeam( db, bowl_game->road_team ) != SQLITE_OK )
     {
          printf( "Unable to import data for road team.\n" );

          if ( sqlite3_errcode( db ) != 0 )
          {
               printf( "sqlite3 error message: %s\n", sqlite3_errmsg( db ) );
          }

          sqlite3_exec( db, "rollback", NULL, NULL, NULL );
          sqlite3_close( db );

          free( rom        );
          free( state_file );

          freeBowlGame( bowl_game );

          return EXIT_FAILURE;
     }

     if ( importTeam( db, bowl_game->home_team ) != SQLITE_OK )
     {
          printf( "Unable to import data for home team.\n" );

          if ( sqlite3_errcode( db ) != 0 )
          {
               printf( "sqlite3 error message: %s\n", sqlite3_errmsg( db ) );
          }

          sqlite3_exec( db, "rollback", NULL, NULL, NULL );
          sqlite3_close( db );

          free( rom        );
          free( state_file );

          freeBowlGame( bowl_game );

          return EXIT_FAILURE;
     }

     sqlite3_exec( db, "commit", NULL, NULL, NULL );
     sqlite3_close( db );

     free( rom        );
     free( state_file );

     freeBowlGame( bowl_game );

     return EXIT_SUCCESS;
}
