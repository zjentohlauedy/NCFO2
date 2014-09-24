#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "conversion.h"
#include "file_formats.h"
#include "save_state.h"


static char *getDisplayPosition( const position_e position )
{
     switch ( position )
     {
     case pos_None:          return "";
     case pos_Quarterback:   return "QB";
     case pos_Runningback:   return "RB";
     case pos_WideReceiver:  return "WR";
     case pos_TightEnd:      return "TE";
     case pos_OffensiveLine: return "OL";
     case pos_DefensiveLine: return "DL";
     case pos_Linebacker:    return "LB";
     case pos_Cornerback:    return "CB";
     case pos_Safety:        return "S";
     case pos_Kicker:        return "K";
     case pos_Punter:        return "P";
     default:                return "XX";
     }
}


static void printPlayer( const player_s *player )
{
     char display_name[30 + 1] = { 0 };

     sprintf( display_name, "%s, %s", player->last_name, player->first_name );

     printf( "%-2s %2d %-20s", getDisplayPosition( player->position ), player->number, display_name );

     printf( "\n" );
}

static void printTeam( const team_s *team )
{
     char      display_name[30 + 1] = { 0 };
     boolean_e print_team           = bl_True;

     if ( team->players == NULL ) return;

     for ( int i = 0; team->players[i].player != NULL; ++i )
     {
          const player_s *player = team->players[i].player;

          if ( player->injuries == 0 ) continue;

          if ( print_team )
          {
               printf( "%s %s:\n", team->location, team->name );

               print_team = bl_False;
          }

          sprintf( display_name, "%s, %s", player->last_name, player->first_name );

          printf( "%-2s %2d %-20s", getDisplayPosition( player->position ), player->number, display_name );

          printf( "  %d days", player->injuries );

          printf( "\n" );
     }

     if ( ! print_team ) printf( "\n" );
}

static void printConference( const conference_s *conference )
{
     if ( conference->teams == NULL ) return;

     for ( int i = 0; conference->teams[i].team != NULL; ++i )
     {
          printTeam( conference->teams[i].team );
     }
}

static void printRosters( const organization_s *organization )
{
     if ( organization->conferences == NULL ) return;

     for ( int i = 0; organization->conferences[i].conference != NULL; ++i )
     {
          printConference( organization->conferences[i].conference );
     }
}

int main( const int argc, const char *argv[] )
{
     static nst_save_state_s  empty_save_state = { 0 };
     const char              *rom1_filename = NULL;
     const char              *rom2_filename = NULL;
     const char              *ss1_filename  = NULL;
     const char              *ss2_filename  = NULL;

     unsigned char    *state_file1   = NULL;
     unsigned char    *state_file2   = NULL;
     nst_save_state_s *save_state1   = NULL;
     nst_save_state_s *save_state2   = NULL;
     tsbrom_s         *rom1          = NULL;
     tsbrom_s         *rom2          = NULL;
     organization_s   *organization  = NULL;


     if ( argc == 3 )
     {
          rom1_filename = argv[1];
          rom2_filename = argv[2];

          save_state1 = &empty_save_state;
          save_state2 = &empty_save_state;
     }
     else if ( argc == 5 )
     {
          rom1_filename = argv[1];
          rom2_filename = argv[3];

          ss1_filename  = argv[2];
          ss2_filename  = argv[4];
     }
     else
     {
          printf( "Usage: %s <rom-file1> [save-state1] <rom-file2> [save-state2]\n", argv[0] );

          return EXIT_SUCCESS;
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

     if ( (organization = convertOrganization( rom1, save_state1, rom2, save_state2, 0, bg_None )) == NULL )
     {
          printf( "Error converting rom and save state data: %s\n", getConvertOrganizationError() );

          free( rom1 );
          free( rom2 );
          free( state_file1 );
          free( state_file2 );

          return EXIT_FAILURE;
     }

     printRosters( organization );

     free( rom1 );
     free( rom2 );
     free( state_file1 );
     free( state_file2 );

     free_organization( organization );

     return EXIT_SUCCESS;
}
