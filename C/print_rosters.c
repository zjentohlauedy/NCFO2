#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "conversion.h"
#include "file_formats.h"
#include "save_state.h"

typedef enum
{
     ps_Ratings,
     ps_Stats

} printstyle_e;

static printstyle_e  print_style;


static int getDisplayRating( const int rating )
{
     static int values[] = { 6, 13, 19, 25, 31, 38, 44, 50, 56, 63, 69, 75, 81, 88, 94, 100 };

     return values[rating];
}

static int getPlayerAcceleration( const player_s *player )
{
     if ( player->ratings == NULL ) return 0;

     return (player->ratings->run_speed > player->ratings->rush_power) ? player->ratings->run_speed : player->ratings->rush_power;
}

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

static char *getDisplayPreference( const preference_e preference )
{
     switch ( preference )
     {
     case pref_BalancedRush: return "Balanced Rush";
     case pref_HeavyRush:    return "Heavy Rush";
     case pref_BalancedPass: return "Balanced Pass";
     case pref_HeavyPass:    return "Heavy Pass";
     }

     return "Unknown";
}

static char *getDisplayFormation( const formation_e formation )
{
     switch ( formation )
     {
     case form_Two_Back:   return "2RB/2WR/1TE";
     case form_Three_Wide: return "1RB/3WR/1TE";
     case form_Four_Wide:  return "1RB/4WR";
     }

     return "Unknown";
}


static void printHeader( const position_e position )
{
     if ( print_style == ps_Ratings )
     {
          switch ( position )
          {
          case pos_Quarterback:   printf( "      Quarterbacks           RS   RP   MS   HP   PS   PC   PA  APB   Score   RA  PA  PK\n" ); break;
               //                          QB  5 CIDADESRF, q.          31   25   13   38   38   50   38   44       0    2   6   1

          case pos_Runningback:   printf( "      Runningbacks           RS   RP   MS   HP   BC   RC   Score   RA  CA  YD  TG\n" ); break;
               //                          RB 34 UWKDDIY, mzpezp        44   50   69   31   50   50       0    8   8   7   8

          case pos_WideReceiver:  printf( "      Wide Receivers         RS   RP   MS   HP   BC   RC   Score   RA  CA  YD  TG\n" ); break;
               //                          WR 89 BBDWCGGAFOF, sdvdv     69   50   69   56   69   50       0    1   8   9   8

          case pos_TightEnd:      printf( "      Tight Ends             RS   RP   MS   HP   BC   RC   Score   RA  CA  YD  TG\n" ); break;
               //                          TE 85 FEVSHLGWWWQ, u.        38   25   50   50   44   50       0    1   7   7   7

          case pos_OffensiveLine: printf( "      Offensive Linesmen     RS   RP   MS   HP   Score\n" ); break;
               //                          OL 74 FCJNYMOWOGC, goqse     31   44   25   50       0

          case pos_DefensiveLine: printf( "      Defensive Linesmen     RS   RP   MS   HP   IN   QU   Score    PR   CV\n" ); break;
               //                          DL 95 JBIAI, rcot            63   25   44   69   44   38       0    34    6

          case pos_Linebacker:    printf( "      Linebackers            RS   RP   MS   HP   IN   QU   Score    PR   CV\n" ); break;
               //                          LB 50 VSQWDYSIFRI, c.        38   19   50   44   44   19       0    19   13

          case pos_Cornerback:    printf( "      Cornerbacks            RS   RP   MS   HP   IN   QU   Score    PR   CV\n" ); break;
               //                          CB 20 PXFKVKAG, j.           25   19   69   25   63   38       0     1   33

          case pos_Safety:        printf( "      Safeties               RS   RP   MS   HP   IN   QU   Score    PR   CV\n" ); break;
               //                          S  49 YOTW, rnxos            38   56   38   56   69   56       0     1   33

          case pos_Kicker:        printf( "      Kickers                RS   RP   MS   HP   KA  AKB   Score    A\n" ); break;
               //                          K  13 CAJEZGMHZOFH, t.       50   38   31   44   44   44       0    6

          case pos_Punter:        printf( "      Punters                RS   RP   MS   HP   KA  AKB   Score    A\n" ); break;
               //                          P  12 UVHX, nsdfo            19   19   19   19   25    6       0    3
          }
     }
     else if ( print_style == ps_Stats )
     {
          switch ( position )
          {
          case pos_Quarterback:   printf( "      Quarterbacks          ATT  CMP    PCT   YDS   AVG  TD INT     QBR   ATT   YDS  TD\n" ); break;
               //                          QB  5 CIDADESRF, q.          12    7  000.0   168  00.0   3   0  000.00     1     9   0

          case pos_Runningback:   printf( "      Runningbacks          ATT   YDS   AVG  TD   REC   YDS   AVG  TD\n" ); break;
               //                          RB 34 UWKDDIY, mzpezp         9    48  00.0   1     2    58  00.0   0

          case pos_WideReceiver:  printf( "      Wide Receivers        ATT   YDS   AVG  TD   REC   YDS   AVG  TD\n" ); break;
               //                          WR 89 BBDWCGGAFOF, sdvdv      0     0  00.0   0     0     0  00.0   0

          case pos_TightEnd:      printf( "      Tight Ends            ATT   YDS   AVG  TD   REC   YDS   AVG  TD\n" ); break;
               //                          TE 85 FEVSHLGWWWQ, u.         0     0  00.0   0     1    20  00.0   1

          case pos_OffensiveLine: printf( "      Offensive Linesmen\n" ); break;
               //                          OL 74 FCJNYMOWOGC, goqse

          case pos_DefensiveLine: printf( "      Defensive Linesmen     S INT  YDS   AVG  TD\n" ); break;
               //                          DL 95 JBIAI, rcot            1   0    0  00.0   0

          case pos_Linebacker:    printf( "      Linebackers            S INT  YDS   AVG  TD\n" ); break;
               //                          LB 50 VSQWDYSIFRI, c.        0   0    0  00.0   0

          case pos_Cornerback:    printf( "      Cornerbacks            S INT  YDS   AVG  TD\n" ); break;
               //                          CB 20 PXFKVKAG, j.           0   0    0  00.0   0

          case pos_Safety:        printf( "      Safeties               S INT  YDS   AVG  TD\n" ); break;
               //                          S  49 YOTW, rnxos            0   0    0  00.0   0

          case pos_Kicker:        printf( "      Kickers              XPA XPM    PCT FGA FGM    PCT  PTS    P   YDS   AVG\n" ); break;
               //                          K  13 CAJEZGMHZOFH, t.       4   4  000.0   0   0  000.0    0    0     0  00.0

          case pos_Punter:        printf( "      Punters              XPA XPM    PCT FGA FGM    PCT  PTS    P   YDS   AVG\n" ); break;
               //                          P  12 UVHX, nsdfo            0   0  000.0   0   0  000.0    0    2   109  00.0
          }
     }
}

static float calcQuarterbackRating( const player_s *player )
{
     if ( player->stats.offense == NULL ) return 0.0;

     float qbr = 0.0;

     player_offense_stats_s *offense = player->stats.offense;

     float completion_pct       = (offense->pass_attempts == 0) ? 0.0 : ((float)offense->completions / (float)offense->pass_attempts * 100.0);

     qbr  = (float)offense->pass_yards;
     qbr += (2.0 * powf( (float)offense->pass_touchdowns, 2.0 )) + (5.0 * (float)offense->pass_touchdowns) + 1.0;
     qbr -= (2.0 * powf( (float)offense->interceptions,   2.0 )) + (5.0 * (float)offense->interceptions)   + 1.0;
     qbr += powf( completion_pct, (completion_pct / 100.0) ) * 3.0 / 2.0;

     return (qbr / 5.0);
}

static int calcPlayerScore( const player_s *player )
{
     if ( player->ratings == NULL ) return 0;

     int acceleration = getPlayerAcceleration( player );
     int score        = 0;

     switch ( player->position )
     {
     case pos_Quarterback:
          if ( player->extra_ratings.quarterback == NULL ) break;

          score  = player->extra_ratings.quarterback->pass_control     * 8;
          score += player->      ratings->            max_speed        * 5;
          score += player->extra_ratings.quarterback->pass_speed       * 3;
          score += player->      ratings->            hit_power;
          score += player->extra_ratings.quarterback->pass_accuracy;
          score += player->extra_ratings.quarterback->avoid_pass_block;
          score +=                                    acceleration;

          break;
     case pos_Runningback:
          if ( player->extra_ratings.offense == NULL ) break;

          score  = player->      ratings->        max_speed    * 8;
          score += player->      ratings->        hit_power    * 5;
          score += player->extra_ratings.offense->receiving    * 3;
          score += player->extra_ratings.offense->ball_control * 2;
          score +=                                acceleration * 2;

          break;
     case pos_WideReceiver:
          if ( player->extra_ratings.offense == NULL ) break;

          score  = player->extra_ratings.offense->receiving    * 8;
          score += player->      ratings->        max_speed    * 6;
          score += player->extra_ratings.offense->ball_control * 2;
          score += player->      ratings->        hit_power    * 2;
          score +=                                acceleration * 2;

          break;
     case pos_TightEnd:
          if ( player->extra_ratings.offense == NULL ) break;

          score  = player->      ratings->        hit_power    * 7;
          score += player->extra_ratings.offense->receiving    * 7;
          score += player->      ratings->        max_speed    * 3;
          score += player->extra_ratings.offense->ball_control * 2;
          score +=                                acceleration;

          break;
     case pos_OffensiveLine:
          score  = player-> ratings-> hit_power    * 13;
          score += player-> ratings-> max_speed    *  5;
          score +=                    acceleration *  2;

          break;
     case pos_DefensiveLine:
          if ( player->extra_ratings.defense == NULL ) break;

          score  = player->      ratings->        hit_power     * 11;
          score += player->      ratings->        max_speed     *  5;
          score += player->extra_ratings.defense->interceptions;
          score += player->extra_ratings.defense->quickness;
          score +=                                acceleration  *  2;

          break;
     case pos_Linebacker:
          if ( player->extra_ratings.defense == NULL ) break;

          score  = player->      ratings->        hit_power     * 8;
          score += player->      ratings->        max_speed     * 5;
          score += player->extra_ratings.defense->interceptions * 3;
          score += player->extra_ratings.defense->quickness;
          score +=                                acceleration  *  2;

          break;
     case pos_Cornerback:
          if ( player->extra_ratings.defense == NULL ) break;

          score  = player->      ratings->        max_speed     * 8;
          score += player->extra_ratings.defense->interceptions * 5;
          score += player->      ratings->        hit_power     * 2;
          score += player->extra_ratings.defense->quickness;
          score +=                                acceleration  * 3;

          break;
     case pos_Safety:
          if ( player->extra_ratings.defense == NULL ) break;

          score  = player->      ratings->        max_speed     * 7;
          score += player->extra_ratings.defense->interceptions * 6;
          score += player->      ratings->        hit_power     * 3;
          score += player->extra_ratings.defense->quickness;
          score +=                                acceleration  * 2;

          break;
     case pos_Kicker:
     case pos_Punter:
          if ( player->extra_ratings.kicking == NULL ) break;

          score  = player->extra_ratings.kicking->kicking_ability  * 15;
          score += player->      ratings->        max_speed        *  3;
          score += player->extra_ratings.kicking->avoid_kick_block;
          score += player->      ratings->        hit_power;

          break;
     default:
          score = 0;
     }

     return score;
}


static void printPlayer( const player_s *player )
{
     char display_name[30 + 1] = { 0 };

     sprintf( display_name, "%s, %s", player->last_name, player->first_name );

     printf( "%-2s %2d %-20s", getDisplayPosition( player->position ), player->number, display_name );

     if ( print_style == ps_Ratings )
     {
          if ( player->ratings != NULL )
          {
               printf( "  %3d  %3d  %3d  %3d",
                       getDisplayRating( player->ratings->run_speed  ),
                       getDisplayRating( player->ratings->rush_power ),
                       getDisplayRating( player->ratings->max_speed  ),
                       getDisplayRating( player->ratings->hit_power  ) );
          }

          switch ( player->position )
          {
          case pos_Quarterback:
               if ( player->extra_ratings.quarterback != NULL )
               {
                    player_quarterback_ratings_s *quarterback = player->extra_ratings.quarterback;

                    printf( "  %3d  %3d  %3d  %3d",
                            getDisplayRating( quarterback->pass_speed       ),
                            getDisplayRating( quarterback->pass_control     ),
                            getDisplayRating( quarterback->pass_accuracy    ),
                            getDisplayRating( quarterback->avoid_pass_block ) );

                    printf( "     %3d", calcPlayerScore( player ) );

                    printf( "   %2d  %2d  %2d", quarterback->sim_rush, quarterback->sim_pass, quarterback->sim_pocket );
               }

               break;

          case pos_Runningback:
          case pos_WideReceiver:
          case pos_TightEnd:
               if ( player->extra_ratings.offense != NULL )
               {
                    player_offense_ratings_s *offense = player->extra_ratings.offense;

                    printf( "  %3d  %3d",
                            getDisplayRating( offense->ball_control ),
                            getDisplayRating( offense->receiving    ) );

                    printf( "     %3d", calcPlayerScore( player ) );

                    printf( "   %2d  %2d  %2d  %2d", offense->sim_rush, offense->sim_catch, offense->sim_yards, offense->sim_target );
               }

               break;

          case pos_OffensiveLine:
               if ( player->ratings != NULL )
               {
                    printf( "     %3d", calcPlayerScore( player ) );
               }

               break;

          case pos_DefensiveLine:
          case pos_Linebacker:
          case pos_Cornerback:
          case pos_Safety:
               if ( player->extra_ratings.defense != NULL )
               {
                    player_defense_ratings_s *defense = player->extra_ratings.defense;

                    printf( "  %3d  %3d",
                            getDisplayRating( defense->interceptions ),
                            getDisplayRating( defense->quickness     ) );

                    printf( "     %3d", calcPlayerScore( player ) );

                    printf( "   %3d  %3d", defense->sim_pass_rush, defense->sim_coverage );
               }

               break;

          case pos_Kicker:
          case pos_Punter:
               if ( player->extra_ratings.kicking != NULL )
               {
                    player_kicking_ratings_s *kicking = player->extra_ratings.kicking;

                    printf( "  %3d  %3d",
                            getDisplayRating( kicking->kicking_ability  ),
                            getDisplayRating( kicking->avoid_kick_block ) );

                    printf( "     %3d", calcPlayerScore( player ) );

                    printf( "   %2d", kicking->sim_kicking );
               }

               break;
          }
     }
     else if ( print_style == ps_Stats )
     {
          switch ( player->position )
          {
          case pos_Quarterback:
               if ( player->stats.offense != NULL )
               {
                    player_offense_stats_s *offense = player->stats.offense;

                    float completion_pct       = (offense->pass_attempts == 0) ? 0.0 : ((float)offense->completions / (float)offense->pass_attempts * 100.0);
                    float yards_per_completion = (offense->completions   == 0) ? 0.0 : ((float)offense->pass_yards  / (float)offense->completions);

                    printf( "  %3d  %3d", offense->pass_attempts, offense->completions );

                    printf( "  %5.1f", completion_pct );

                    printf( "  %4d", offense->pass_yards );

                    printf( "  %4.1f", yards_per_completion );

                    printf( "  %2d  %2d", offense->pass_touchdowns, offense->interceptions );

                    printf( "  %6.2f", calcQuarterbackRating( player ) );

                    printf( "   %3d  %4d  %2d", offense->rush_attempts, offense->rush_yards, offense->rush_touchdowns );
               }

               break;

          case pos_Runningback:
          case pos_WideReceiver:
          case pos_TightEnd:
               if ( player->stats.offense != NULL )
               {
                    player_offense_stats_s *offense = player->stats.offense;

                    float yards_per_carry = (offense->rush_attempts == 0) ? 0.0 : ((float)offense->rush_yards      / (float)offense->rush_attempts);
                    float yards_per_catch = (offense->receptions    == 0) ? 0.0 : ((float)offense->receiving_yards / (float)offense->receptions);

                    printf( "  %3d  %4d", offense->rush_attempts, offense->rush_yards );
                    printf( "  %4.1f", yards_per_carry );
                    printf( "  %2d", offense->rush_touchdowns );
                    printf( "   %3d  %4d", offense->receptions, offense->receiving_yards );
                    printf( "  %4.1f", yards_per_catch );
                    printf( "  %2d", offense->receiving_touchdowns );
               }

               break;

          case pos_DefensiveLine:
          case pos_Linebacker:
          case pos_Cornerback:
          case pos_Safety:
               if ( player->stats.defense != NULL )
               {
                    player_defense_stats_s *defense = player->stats.defense;

                    float yards_per_return = (defense->interceptions == 0) ? 0.0 : ((float)defense->return_yards / (float)defense->interceptions);

                    printf( "  %2d  %2d  %3d", defense->sacks, defense->interceptions, defense->return_yards );
                    printf( "  %4.1f", yards_per_return );
                    printf( "  %2d", defense->return_touchdowns );
               }

               break;

          case pos_Kicker:
          case pos_Punter:
               if ( player->stats.kicking != NULL )
               {
                    player_kicking_stats_s *kicking = player->stats.kicking;

                    float extra_point_pct = (kicking->extra_point_attempts == 0) ? 0.0 : ((float)kicking->extra_points_made / (float)kicking->extra_point_attempts * 100.0);
                    float field_goal_pct  = (kicking->field_goal_attempts  == 0) ? 0.0 : ((float)kicking->field_goals_made  / (float)kicking->field_goal_attempts  * 100.0);
                    float yards_per_punt  = (kicking->punts                == 0) ? 0.0 : ((float)kicking->punt_yards        / (float)kicking->punts);

                    int points = (kicking->field_goals_made * 3) + kicking->extra_points_made;

                    printf( "  %2d  %2d", kicking->extra_point_attempts, kicking->extra_points_made );
                    printf( "  %5.1f", extra_point_pct );
                    printf( "  %2d  %2d", kicking->field_goal_attempts, kicking->field_goals_made );
                    printf( "  %5.1f", field_goal_pct );
                    printf( "  %3d", points );
                    printf( "   %2d  %4d", kicking->punts, kicking->punt_yards );
                    printf( "  %4.1f", yards_per_punt );
               }

               break;
          }
     }

     printf( "\n" );
}

static int comparePlayersByPositionAndStats( const void *arg1, const void *arg2 )
{
     const player_s *player1 = ((team_player_s *)arg1)->player;
     const player_s *player2 = ((team_player_s *)arg2)->player;

     if ( player1 == NULL )
     {
          if ( player2 == NULL ) return 0;

          return 1;
     }

     if ( player2 == NULL ) return -1;

     if ( player1->position != player2->position ) return player1->position - player2->position;

     switch ( player1->position )
     {
     case pos_Quarterback:   return player2->stats.offense->pass_attempts - player1->stats.offense->pass_attempts;
     case pos_Runningback:   return player2->stats.offense->rush_attempts - player1->stats.offense->rush_attempts;
     case pos_WideReceiver:  return player2->stats.offense->receptions    - player1->stats.offense->receptions;
     case pos_TightEnd:      return player2->stats.offense->receptions    - player1->stats.offense->receptions;
     case pos_DefensiveLine: return player2->stats.defense->sacks         - player1->stats.defense->sacks;
     case pos_Linebacker:    return player2->stats.defense->sacks         - player1->stats.defense->sacks;
     case pos_Cornerback:    return player2->stats.defense->interceptions - player1->stats.defense->interceptions;
     case pos_Safety:        return player2->stats.defense->interceptions - player1->stats.defense->interceptions;
     }

     return 0;
}

static int comparePlayersByPositionAndScore( const void *arg1, const void *arg2 )
{
     const player_s *player1 = ((team_player_s *)arg1)->player;
     const player_s *player2 = ((team_player_s *)arg2)->player;

     if ( player1 == NULL )
     {
          if ( player2 == NULL ) return 0;

          return 1;
     }

     if ( player2 == NULL ) return -1;

     if ( player1->position != player2->position ) return player1->position - player2->position;

     return player2->score - player1->score;
}

static void sortPlayers( team_player_s *players )
{
     int i;

     for ( i = 0; players[i].player != NULL; ++i )
     {
          players[i].player->score = calcPlayerScore( players[i].player );
     }

     if ( print_style == ps_Ratings )
     {
          qsort( players, i, sizeof(team_player_s), comparePlayersByPositionAndScore );
     }
     else if ( print_style == ps_Stats )
     {
          qsort( players, i, sizeof(team_player_s), comparePlayersByPositionAndStats );
     }
}

static void printTeam( const team_s *team )
{
     position_e current_pos = pos_None;

     printf( "%s %s", team->location, team->name );

     if ( print_style == ps_Ratings )
     {
          printf( "  %2d/%2d", team->sim_offense, team->sim_defense );

          printf( "  %s", getDisplayFormation(  team->offensive_formation  ) );
          printf( "  %s", getDisplayPreference( team->offensive_preference ) );

          printf( "\n" );
     }
     else if ( print_style == ps_Stats )
     {
          if ( team->stats != NULL )
          {
               printf( "  %2d - %2d - %2d", team->stats->wins, team->stats->losses, team->stats->ties );
          }

          printf( "\n" );
     }

     if ( team->players == NULL ) return;

     sortPlayers( team->players );

     for ( int i = 0; team->players[i].player != NULL; ++i )
     {
          if ( team->players[i].player->position != current_pos )
          {
               current_pos = team->players[i].player->position;

               printf( "\n" );

               printHeader( team->players[i].player->position );
          }

          printPlayer( team->players[i].player );
     }

     if ( print_style == ps_Stats )
     {
          printf( "\n" );
          printf( "      Returners              KR   YDS   AVG  TD    PR   YDS   AVG  TD\n" );
          //       RB 34 UWKDDIY, mzpezp         0     0  00.0   0     0     0  00.0   0

          for ( int i = 0; team->players[i].player != NULL; ++i )
          {
               player_s *player = team->players[i].player;

               if ( player->stats.returns == NULL ) continue;

               player_returns_stats_s *returns = player->stats.returns;

               if ( returns->kick_returns <= 0  &&  returns->punt_returns <= 0 ) continue;

               char display_name[30 + 1] = { 0 };

               sprintf( display_name, "%s, %s", player->last_name, player->first_name );

               printf( "%-2s %2d %-20s", getDisplayPosition( player->position ), player->number, display_name );

               float yards_per_kr = (returns->kick_returns == 0) ? 0.0 : ((float)returns->kick_return_yards / (float)returns->kick_returns);
               float yards_per_pr = (returns->punt_returns == 0) ? 0.0 : ((float)returns->punt_return_yards / (float)returns->punt_returns);

               printf( "  %3d  %4d", returns->kick_returns, returns->kick_return_yards );
               printf( "  %4.1f", yards_per_kr );
               printf( "  %2d", returns->kick_return_touchdowns );

               printf( "   %3d  %4d", returns->punt_returns, returns->punt_return_yards );
               printf( "  %4.1f", yards_per_pr );
               printf( "  %2d", returns->punt_return_touchdowns );
               printf( "\n" );
          }
     }

     printf( "\n\n" );
}

static void printConference( const conference_s *conference )
{
     printf( "%s\n\n", conference->name );

     if ( conference->teams == NULL ) return;

     for ( int i = 0; conference->teams[i].team != NULL; ++i )
     {
          printTeam( conference->teams[i].team );
     }
}

static void printRosters( const organization_s *organization )
{
     printf( "%s\n\n", organization->name );

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


     if ( argc == 4 )
     {
          rom1_filename = argv[1];
          rom2_filename = argv[2];

          save_state1 = &empty_save_state;
          save_state2 = &empty_save_state;

          switch ( *argv[3] )
          {
          case 'R': print_style = ps_Ratings; break;
          case 'S': print_style = ps_Stats;   break;
          default:  print_style = ps_Ratings; break;
          }
     }
     else if ( argc == 6 )
     {
          rom1_filename = argv[1];
          rom2_filename = argv[3];

          ss1_filename  = argv[2];
          ss2_filename  = argv[4];

          switch ( *argv[5] )
          {
          case 'R': print_style = ps_Ratings; break;
          case 'S': print_style = ps_Stats;   break;
          default:  print_style = ps_Ratings; break;
          }
     }
     else
     {
          printf( "Usage: %s <rom-file1> [save-state1] <rom-file2> [save-state2] <style>\n", argv[0] );
          printf( "       Where <style> in: R    Ratings\n" );
          printf( "                         S    Stats\n"   );

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
