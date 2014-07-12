#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_formats.h"
#include "schedule.h"

#define SWAP( A, B )  do { int x = A; A = B; B = x; } while ( 0 )


static char error_message[999 + 1] = { 0 };


static void clearErrorMessage( void )
{
     error_message[0] = '\0';
}

char *getGenerateScheduleError( void )
{
     return error_message;
}


void freeSchedule( schedule_s *schedule )
{
     if ( schedule->weeks != NULL )
     {
          for ( int i = 0; i < 10; ++i )
          {
               if ( schedule->weeks[i].games != NULL ) free( schedule->weeks[i].games );
          }

          free( schedule->weeks );
     }

     free( schedule );
}

static void shuffle( int list[], const int length )
{
     for( int i = length; i > 1; --i )
     {
          int n = rand() % (i - 1);
          int x;

          x           = list[n    ];
          list[n    ] = list[i - 1];
          list[i - 1] = x;
     }

}

static void rotate( int list[], const int length )
{
     int x = list[length - 1];

     for ( int i = length - 1; i > 0; --i )
     {
          list[i] = list[i - 1];
     }

     list[0] = x;
}

static void shuffleGames( game_s *games )
{
     for ( int i = 12; i > 1; --i )
     {
          int n = rand() % (i - 1);

          game_s game;

          game         = games[i - 1];
          games[i - 1] = games[n    ];
          games[n    ] = game;
     }
}

static void shuffleWeeksByConference( schedule_s *schedule )
{
     for ( int i = 0; i < 12; i += 3 )
     {
          for ( int j = 10; j > 1; --j )
          {
               int n = rand() % (j - 1);

               game_s games[3];

               memcpy(                          games,     &(schedule->weeks[n    ].games[i]), sizeof(game_s) * 3 );
               memcpy( &(schedule->weeks[n    ].games[i]), &(schedule->weeks[j - 1].games[i]), sizeof(game_s) * 3 );
               memcpy( &(schedule->weeks[j - 1].games[i]),                          games,     sizeof(game_s) * 3 );
          }
     }
}

static boolean_e swapGameLocation( const int road_team, const int home_team, schedule_s *schedule, const int week )
{
     for ( int i = week - 1; i >= 0; --i )
     {
          game_s *games = schedule->weeks[i].games;

          for ( int j = 0; games[j].home >= 0; ++j )
          {
               // Last game scheduled between these teams the location is different so no swap needed
               if ( games[j].road == home_team  &&  games[j].home == road_team ) return bl_False;

               // Last game scheduled between these teams was the same location, so do the swap
               if ( games[j].road == road_team  &&  games[j].home == home_team ) return bl_True;
          }
     }

     // Teams did not play before so no need to swap
     return bl_False;
}

static void scheduleWeek( const int *teams, schedule_s *schedule, const int week )
{
     for ( int i = 0; i < 24; i += 6 )
     {
          for ( int j = 0; j < 3; ++j )
          {
               int match = (i / 2) + j;
               int road  = i + j;
               int home  = i + 5 - j;

               if ( swapGameLocation( teams[road], teams[home], schedule, week ) ) SWAP( road, home );

               schedule->weeks[week].games[match].road = teams[road];
               schedule->weeks[week].games[match].home = teams[home];
          }
     }
}

void mergeSchedules( schedule_s *dest, schedule_s *src )
{
     for ( int i = 0; i < 10; ++i )
     {
          for ( int j = 0; j < 12; ++j )
          {
               dest->weeks[i].games[j + 12].road = src->weeks[i].games[j].road + 24;
               dest->weeks[i].games[j + 12].home = src->weeks[i].games[j].home + 24;
          }
     }
}

static void injectSchedule( tsbrom_s *rom, const schedule_s *schedule )
{
     memset( rom->schedule[0].ncfo.filler, 0xff, sizeof(rom->schedule[0].ncfo.filler) );

     for ( int i = 0; i < 10; ++i ) rom->schedule[0].ncfo.games_per_week[i] = 12;
     for ( int i = 0; i <  7; ++i ) rom->schedule[0].ncfo.unused_weeks[i]   =  0;

     rom->schedule[0].ncfo.separator[0] = 0xff;

     int pointer = 0x89cb;

     for ( int i = 0; schedule->weeks[i].games != NULL; ++i )
     {
          int2pointer( pointer, &(rom->schedule[0].ncfo.week_pointers[i]) );

          game_s *games = schedule->weeks[i].games;

          for ( int j = 0; games[j].home >= 0; ++j )
          {
               rom->schedule[0].ncfo.games[i][j].road[0] = games[j].road;
               rom->schedule[0].ncfo.games[i][j].home[0] = games[j].home;
          }

          pointer += 24;
     }

     for ( int i = 10; i < 17; ++i ) int2pointer( pointer, &(rom->schedule[0].ncfo.week_pointers[i]) );
}

schedule_s *buildRomSchedule( tsbrom_s *rom )
{
     schedule_s *schedule = NULL;

     if ( (schedule = malloc( sizeof(schedule_s) )) == NULL )
     {
          sprintf( error_message, "Cannot allocate memory for schedule" );

          return NULL;
     }

     if ( (schedule->weeks = malloc( sizeof(week_s) * (10 + 1) )) == NULL )
     {
          sprintf( error_message, "Cannot allocate memory for weeks" );

          freeSchedule( schedule );

          return NULL;
     }

     for ( int i = 0; i < 10; ++i )
     {
          if ( (schedule->weeks[i].games = malloc( sizeof(game_s) * (24 + 1) )) == NULL )
          {
               sprintf( error_message, "Cannot allocate memory for games for week %d", i + 1 );

               freeSchedule( schedule );

               return NULL;
          }

          for ( int j = 0; j < 24 + 1; ++j ) schedule->weeks[i].games[j].home = -1;
     }

     schedule->weeks[10].games = NULL;

     int teams[24];

     for ( int i = 0; i < 24; ++i ) teams[i] = i;

     // Shuffle the teams in each conference to add some randomness
     shuffle( &teams[ 0], 6 );
     shuffle( &teams[ 6], 6 );
     shuffle( &teams[12], 6 );
     shuffle( &teams[18], 6 );

     for ( int i = 0; i < 10; ++i )
     {
          scheduleWeek( teams, schedule, i );

          // Rotate each conference individually (first team in each conf. stays put)
          rotate( &teams[ 0], 6 - 1 );
          rotate( &teams[ 6], 6 - 1 );
          rotate( &teams[12], 6 - 1 );
          rotate( &teams[18], 6 - 1 );
     }

     shuffleWeeksByConference( schedule );

     for ( int i = 0; i < 10; ++i ) shuffleGames( schedule->weeks[i].games );

     if ( rom != NULL ) injectSchedule( rom, schedule );

     return schedule;
}

schedule_s *generateSchedule( tsbrom_s *rom1, tsbrom_s *rom2 )
{
     schedule_s *rom1_schedule = buildRomSchedule( rom1 );
     schedule_s *rom2_schedule = buildRomSchedule( rom2 );

     mergeSchedules( rom1_schedule, rom2_schedule );

     return rom1_schedule;
}
