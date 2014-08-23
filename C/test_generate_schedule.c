#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "file_formats.h"
#include "schedule.h"
#include "unit_test.h"


static char *result;


static char *generateSchedule_ShouldReturnASchedule()
{
     assertNotNull( generateSchedule( NULL, NULL ) );

     return NULL;
}

static char *generateSchedule_ShouldReturnAScheduleWith10Weeks()
{
     schedule_s *schedule = generateSchedule( NULL, NULL );

     assertNotNull( schedule );

     assertNotNull( schedule->weeks );

     int count;

     for ( count = 0; schedule->weeks[count].games != NULL; ++count );

     assertEquals( 10, count );

     freeSchedule( schedule );

     return NULL;
}

static char *generateSchedule_ShouldReturnAScheduleWith24GamesPerWeek()
{
     schedule_s *schedule = generateSchedule( NULL, NULL );

     assertNotNull( schedule );

     assertNotNull( schedule->weeks );

     for ( int i = 0; schedule->weeks[i].games != NULL; ++i )
     {
          game_s *games = schedule->weeks[i].games;

          int count;

          for ( count = 0; games[count].home >= 0; ++count);

          assertEquals( 24, count );
     }

     freeSchedule( schedule );

     return NULL;
}

static char *generateSchedule_ShouldReturnAScheduleWith48TeamsPlayingPerWeek()
{
     schedule_s *schedule = generateSchedule( NULL, NULL );

     assertNotNull( schedule );

     assertNotNull( schedule->weeks );

     for ( int i = 0; schedule->weeks[i].games != NULL; ++i )
     {
          game_s *games = schedule->weeks[i].games;

          int teams[48] = { 0 };

          for ( int j = 0; games[j].home >= 0; ++j )
          {
               teams[games[j].road]++;
               teams[games[j].home]++;
          }

          for ( int j = 0; j < 48; ++j ) assertEquals( 1, teams[j] );
     }

     freeSchedule( schedule );

     return NULL;
}

static char *generateSchedule_ShouldReturnAScheduleWhereEveryTeamPlays10Games()
{
     schedule_s *schedule = generateSchedule( NULL, NULL );

     assertNotNull( schedule );

     assertNotNull( schedule->weeks );

     int teams[48] = { 0 };

     for ( int i = 0; schedule->weeks[i].games != NULL; ++i )
     {
          game_s *games = schedule->weeks[i].games;

          for ( int j = 0; games[j].home >= 0; ++j )
          {
               teams[games[j].road]++;
               teams[games[j].home]++;
          }
     }

     for ( int j = 0; j < 48; ++j ) assertEquals( 10, teams[j] );

     freeSchedule( schedule );

     return NULL;
}

static char *generateSchedule_ShouldReturnAScheduleWhereEveryTeamPlays5HomeGames()
{
     schedule_s *schedule = generateSchedule( NULL, NULL );

     assertNotNull( schedule );

     assertNotNull( schedule->weeks );

     int teams[48] = { 0 };

     for ( int i = 0; schedule->weeks[i].games != NULL; ++i )
     {
          game_s *games = schedule->weeks[i].games;

          for ( int j = 0; games[j].home >= 0; ++j )
          {
               teams[games[j].home]++;
          }
     }

     for ( int j = 0; j < 48; ++j ) assertEquals( 5, teams[j] );

     freeSchedule( schedule );

     return NULL;
}

static char *generateSchedule_ShouldReturnAScheduleWhereEveryTeamPlays5RoadGames()
{
     schedule_s *schedule = generateSchedule( NULL, NULL );

     assertNotNull( schedule );

     assertNotNull( schedule->weeks );

     int teams[48] = { 0 };

     for ( int i = 0; schedule->weeks[i].games != NULL; ++i )
     {
          game_s *games = schedule->weeks[i].games;

          for ( int j = 0; games[j].home >= 0; ++j )
          {
               teams[games[j].road]++;
          }
     }

     for ( int j = 0; j < 48; ++j ) assertEquals( 5, teams[j] );

     freeSchedule( schedule );

     return NULL;
}

static char *generateSchedule_ShouldReturnAScheduleWhereEveryTeamPlays5DifferentTeamsTwice()
{
     schedule_s *schedule = generateSchedule( NULL, NULL );

     assertNotNull( schedule );

     assertNotNull( schedule->weeks );

     for ( int i = 0; i < 48; ++i )
     {
          struct { int team; int count; } opponents[5] = { { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 }, { -1, 0 } };

          for ( int j = 0; schedule->weeks[j].games != NULL; ++j )
          {
               game_s *games = schedule->weeks[j].games;

               for ( int k = 0; games[k].home >= 0; ++k )
               {
                    if ( games[k].road == i )
                    {
                         for ( int l = 0; l < 5; ++l )
                         {
                              if ( opponents[l].team == games[k].home )
                              {
                                   opponents[l].count++;

                                   break;
                              }

                              if ( opponents[l].team == -1 )
                              {
                                   opponents[l].team = games[k].home;
                                   opponents[l].count++;

                                   break;
                              }
                         }
                    }

                    if ( games[k].home == i )
                    {
                         for ( int l = 0; l < 5; ++l )
                         {
                              if ( opponents[l].team == games[k].road )
                              {
                                   opponents[l].count++;

                                   break;
                              }

                              if ( opponents[l].team == -1 )
                              {
                                   opponents[l].team = games[k].road;
                                   opponents[l].count++;

                                   break;
                              }
                         }
                    }
               }
          }

          for ( int j = 0; j < 5; ++j )
          {
               assertEquals( 2, opponents[j].count );
          }
     }

     freeSchedule( schedule );

     return NULL;
}

static char *generateSchedule_ShouldReturnAScheduleWhereOnlyPlayOtherTeamsInTheSameConference()
{
     schedule_s *schedule = generateSchedule( NULL, NULL );

     assertNotNull( schedule );

     assertNotNull( schedule->weeks );

     for ( int i = 0; schedule->weeks[i].games != NULL; ++i )
     {
          game_s *games = schedule->weeks[i].games;

          for ( int j = 0; games[j].home >= 0; ++j )
          {
               if ( games[j].home >=  0  &&  games[j].home <=  5 ) assertEquals( bl_True, (games[j].road >=  0  &&  games[j].road <=  5) );
               if ( games[j].home >=  6  &&  games[j].home <= 11 ) assertEquals( bl_True, (games[j].road >=  6  &&  games[j].road <= 11) );
               if ( games[j].home >= 12  &&  games[j].home <= 17 ) assertEquals( bl_True, (games[j].road >= 12  &&  games[j].road <= 17) );
               if ( games[j].home >= 18  &&  games[j].home <= 23 ) assertEquals( bl_True, (games[j].road >= 18  &&  games[j].road <= 23) );
               if ( games[j].home >= 24  &&  games[j].home <= 29 ) assertEquals( bl_True, (games[j].road >= 24  &&  games[j].road <= 29) );
               if ( games[j].home >= 30  &&  games[j].home <= 35 ) assertEquals( bl_True, (games[j].road >= 30  &&  games[j].road <= 35) );
               if ( games[j].home >= 36  &&  games[j].home <= 41 ) assertEquals( bl_True, (games[j].road >= 36  &&  games[j].road <= 41) );
               if ( games[j].home >= 42  &&  games[j].home <= 47 ) assertEquals( bl_True, (games[j].road >= 42  &&  games[j].road <= 47) );
          }
     }

     freeSchedule( schedule );

     return NULL;
}

static char *generateSchedule_ShouldUpdateTheRomsWithTheGeneratedSchedule_GivenTwoRoms()
{
     tsbrom_s tsbrom1 = { 0 };
     tsbrom_s tsbrom2 = { 0 };

     schedule_s *schedule = generateSchedule( &tsbrom1, &tsbrom2 );

     assertNotNull( schedule );

     assertNotNull( schedule->weeks );

     for ( int i = 0; i < 10; ++i )
     {
          int this = pointer2int( &(tsbrom1.schedule[0].ncfo.week_pointers[i    ]) );
          int next = pointer2int( &(tsbrom1.schedule[0].ncfo.week_pointers[i + 1]) );

          assertEquals( 24, next - this );

          this = pointer2int( &(tsbrom2.schedule[0].ncfo.week_pointers[i    ]) );
          next = pointer2int( &(tsbrom2.schedule[0].ncfo.week_pointers[i + 1]) );

          assertEquals( 24, next - this );
     }

     // 12 games per week
     for ( int i = 0; i < 10; ++i )
     {
          assertEquals( 12, tsbrom1.schedule[0].ncfo.games_per_week[i] );
          assertEquals( 12, tsbrom2.schedule[0].ncfo.games_per_week[i] );
     }

     // unused week 1 is set to 1
     assertEquals( 1, tsbrom1.schedule[0].ncfo.unused_weeks[0] );
     assertEquals( 1, tsbrom2.schedule[0].ncfo.unused_weeks[0] );

     // rest of unused weeks are zeroed out
     for ( int i = 1; i < 7; ++i )
     {
          assertEquals( 0, tsbrom1.schedule[0].ncfo.unused_weeks[i] );
          assertEquals( 0, tsbrom2.schedule[0].ncfo.unused_weeks[i] );
     }

     assertEquals( 0xff, tsbrom1.schedule[0].ncfo.separator[0] );
     assertEquals( 0xff, tsbrom2.schedule[0].ncfo.separator[0] );

     for ( int i = 0; schedule->weeks[i].games != NULL; ++i )
     {
          game_s *games = schedule->weeks[i].games;

          for ( int j = 0; games[j].home >= 0; ++j )
          {
               if ( j < 12 )
               {
                    assertEquals( games[j].road, tsbrom1.schedule[0].ncfo.games[i][j].road[0] );
                    assertEquals( games[j].home, tsbrom1.schedule[0].ncfo.games[i][j].home[0] );
               }
               else
               {
                    assertEquals( games[j].road, tsbrom2.schedule[0].ncfo.games[i][j - 12].road[0] + 24 );
                    assertEquals( games[j].home, tsbrom2.schedule[0].ncfo.games[i][j - 12].home[0] + 24 );
               }
          }
     }

     freeSchedule( schedule );

     return NULL;
}

static char *generateSchedule_ShouldReturnAScheduleWith24TVGamesSelected()
{
     schedule_s *schedule = generateSchedule( NULL, NULL );

     assertNotNull( schedule );

     assertNotNull( schedule->weeks );

     int count = 0;

     for ( int i = 0; schedule->weeks[i].games != NULL; ++i )
     {
          game_s *games = schedule->weeks[i].games;

          for ( int j = 0; games[j].home >= 0; ++j )
          {
               if ( games[j].on_tv ) count++;
          }
     }

     assertEquals( 24, count );

     freeSchedule( schedule );

     return NULL;
}

static char *generateSchedule_ShouldSelectTVGamesForEveryTeam()
{
     schedule_s *schedule = generateSchedule( NULL, NULL );

     assertNotNull( schedule );

     assertNotNull( schedule->weeks );

     boolean_e teams_on_tv[48] = { 0 };

     for ( int i = 0; schedule->weeks[i].games != NULL; ++i )
     {
          game_s *games = schedule->weeks[i].games;

          for ( int j = 0; games[j].home >= 0; ++j )
          {
               if ( games[j].on_tv )
               {
                    teams_on_tv[games[j].road] = bl_True;
                    teams_on_tv[games[j].home] = bl_True;
               }
          }
     }

     for ( int i = 0; i < 48; ++i )
     {
          assertEquals( bl_True, teams_on_tv[i] );
     }

     freeSchedule( schedule );

     return NULL;
}

static void check_generate_schedule_error()
{
     printf( "generate schedule error: %s\n", getGenerateScheduleError() );
}

static void run_all_tests()
{
     run_test( generateSchedule_ShouldReturnASchedule,                                           check_generate_schedule_error );
     run_test( generateSchedule_ShouldReturnAScheduleWith10Weeks,                                check_generate_schedule_error );
     run_test( generateSchedule_ShouldReturnAScheduleWith24GamesPerWeek,                         check_generate_schedule_error );
     run_test( generateSchedule_ShouldReturnAScheduleWith48TeamsPlayingPerWeek,                  check_generate_schedule_error );
     run_test( generateSchedule_ShouldReturnAScheduleWhereEveryTeamPlays10Games,                 check_generate_schedule_error );
     run_test( generateSchedule_ShouldReturnAScheduleWhereEveryTeamPlays5HomeGames,              check_generate_schedule_error );
     run_test( generateSchedule_ShouldReturnAScheduleWhereEveryTeamPlays5RoadGames,              check_generate_schedule_error );
     run_test( generateSchedule_ShouldReturnAScheduleWhereEveryTeamPlays5DifferentTeamsTwice,    check_generate_schedule_error );
     run_test( generateSchedule_ShouldReturnAScheduleWhereOnlyPlayOtherTeamsInTheSameConference, check_generate_schedule_error );

     run_test( generateSchedule_ShouldUpdateTheRomsWithTheGeneratedSchedule_GivenTwoRoms,        check_generate_schedule_error );

     run_test( generateSchedule_ShouldReturnAScheduleWith24TVGamesSelected,                      check_generate_schedule_error );
     run_test( generateSchedule_ShouldSelectTVGamesForEveryTeam,                                 check_generate_schedule_error );
}


int main( int argc, char *argv[] )
{
     run_all_tests();

     show_test_results();

     return (tests_pass()) ? EXIT_SUCCESS : EXIT_FAILURE;
}
