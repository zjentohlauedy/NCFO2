#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "conversion.h"
#include "file_formats.h"
#include "save_state.h"


typedef struct
{
     char team_name [ 20 + 1 ];
     int  wins;
     int  losses;
     int  ties;
     int  scored;
     int  allowed;

} team_record_s;

typedef struct
{
     char          conference_name [ 20 + 1 ];
     team_record_s teams           [  6     ];

} conference_records_s;


static conference_records_s conferences [ 8 ] = { 0 };


static int compareTeams( const void *arg1, const void *arg2 )
{
     team_record_s *team1 = (team_record_s *)arg1;
     team_record_s *team2 = (team_record_s *)arg2;

     int games1 = team1->wins + team1->losses + team1->ties;
     int games2 = team2->wins + team2->losses + team2->ties;

     int pts_diff1 = team1->scored - team1->allowed;
     int pts_diff2 = team2->scored - team2->allowed;

     float win_pct1 = (games1 == 0) ? 0.0 : ((float)team1->wins / (float)games1);
     float win_pct2 = (games2 == 0) ? 0.0 : ((float)team2->wins / (float)games2);

     if ( win_pct1 != win_pct2 ) return (win_pct1 > win_pct2) ? -1 : 1;

     if ( pts_diff1 != pts_diff2 ) return (pts_diff1 > pts_diff2) ? -1 : 1;

     if ( team1->scored != team2->scored ) return (team1->scored > team2->scored) ? -1 : 1;

     return 0;
}

static void sortTeams( team_record_s *team_records )
{
     qsort( team_records, 6, sizeof(team_record_s), compareTeams );
}

static void processTeam( const team_s *team, team_record_s *team_record )
{
     position_e current_pos = pos_None;

     sprintf( team_record->team_name, "%s", team->location );

     if ( team->stats != NULL )
     {
          team_record->wins    = team->stats->wins;
          team_record->losses  = team->stats->losses;
          team_record->ties    = team->stats->ties;
          team_record->scored  = team->stats->points_scored;
          team_record->allowed = team->stats->points_allowed;
     }
}

static void processConference( const conference_s *conference, conference_records_s *conference_record )
{
     sprintf( conference_record->conference_name, "%s", conference->name );

     if ( conference->teams == NULL ) return;

     for ( int i = 0; conference->teams[i].team != NULL; ++i )
     {
          processTeam( conference->teams[i].team, &(conference_record->teams[i]) );
     }

     sortTeams( conference_record->teams );
}

static void printStandings( const organization_s *organization )
{
     if ( organization->conferences == NULL ) return;

     for ( int i = 0; organization->conferences[i].conference != NULL; ++i )
     {
          processConference( organization->conferences[i].conference, &(conferences[i]) );
     }

     for ( int i = 0; i < 4; ++i ) printf( "%-15s   W  L  T    ", conferences[i].conference_name );
     printf( "\n" );

     for ( int i = 0; i < 4; ++i ) printf( "-                 -  -  -    " );
     printf( "\n" );

     for ( int i = 0; i < 6; ++i )
     {
          for ( int j = 0; j < 4; ++j )
          {
               printf( "%-15s  %2d %2d %2d    ",
                       conferences[j].teams[i].team_name,
                       conferences[j].teams[i].wins,
                       conferences[j].teams[i].losses,
                       conferences[j].teams[i].ties );
          }

          printf( "\n" );
     }

     printf( "\n\n" );

     for ( int i = 4; i < 8; ++i ) printf( "%-15s   W  L  T    ", conferences[i].conference_name );
     printf( "\n" );

     for ( int i = 4; i < 8; ++i ) printf( "-                 -  -  -    " );
     printf( "\n" );

     for ( int i = 0; i < 6; ++i )
     {
          for ( int j = 4; j < 8; ++j )
          {
               printf( "%-15s  %2d %2d %2d    ",
                       conferences[j].teams[i].team_name,
                       conferences[j].teams[i].wins,
                       conferences[j].teams[i].losses,
                       conferences[j].teams[i].ties );
          }

          printf( "\n" );
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

     printStandings( organization );

     free( rom1 );
     free( rom2 );
     free( state_file1 );
     free( state_file2 );

     free_organization( organization );

     return EXIT_SUCCESS;
}
