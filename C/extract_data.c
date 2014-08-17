#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "conversion.h"
#include "file_formats.h"
#include "save_state.h"

typedef enum
{
     ps_JSON,

     ps_Stats,
     ps_Ratings

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
     printf( "{\"first_name\":\"%s\"",                     player->first_name );
     printf( ",\"last_name\":\"%s\"",                      player->last_name  );
     printf( ",\"position\":\"%s\"",   getDisplayPosition( player->position   ) );
     printf( ",\"number\":%d",                             player->number     );
     printf( ",\"score\":%d",          calcPlayerScore(    player             ) );

     if ( player->ratings != NULL )
     {
          printf( ",\"ratings\":" );

          printf( "{\"run_speed\":%d",  player->ratings->run_speed  );
          printf( ",\"rush_power\":%d", player->ratings->rush_power );
          printf( ",\"max_speed\":%d",  player->ratings->max_speed  );
          printf( ",\"hit_power\":%d",  player->ratings->hit_power  );

          switch ( player->position )
          {
          case pos_Quarterback:
               if ( player->extra_ratings.quarterback != NULL )
               {
                    player_quarterback_ratings_s *quarterback = player->extra_ratings.quarterback;

                    printf( ",\"pass_speed\":%d",       quarterback->pass_speed       );
                    printf( ",\"pass_control\":%d",     quarterback->pass_control     );
                    printf( ",\"pass_accuracy\":%d",    quarterback->pass_accuracy    );
                    printf( ",\"avoid_pass_block\":%d", quarterback->avoid_pass_block );
                    printf( ",\"sim_rush\":%d",         quarterback->sim_rush         );
                    printf( ",\"sim_pass\":%d",         quarterback->sim_pass         );
                    printf( ",\"sim_pocket\":%d",       quarterback->sim_pocket       );
               }

               break;

          case pos_Runningback:
          case pos_WideReceiver:
          case pos_TightEnd:
               if ( player->extra_ratings.offense != NULL )
               {
                    player_offense_ratings_s *offense = player->extra_ratings.offense;

                    printf( ",\"ball_control\":%d", offense->ball_control );
                    printf( ",\"receiving\":%d",    offense->receiving    );
                    printf( ",\"sim_rush\":%d",     offense->sim_rush     );
                    printf( ",\"sim_catch\":%d",    offense->sim_catch    );
                    printf( ",\"sim_yards\":%d",    offense->sim_yards    );
                    printf( ",\"sim_target\":%d",   offense->sim_target   );
               }

               break;

          case pos_DefensiveLine:
          case pos_Linebacker:
          case pos_Cornerback:
          case pos_Safety:
               if ( player->extra_ratings.defense != NULL )
               {
                    player_defense_ratings_s *defense = player->extra_ratings.defense;

                    printf( ",\"interceptions\":%d", defense->interceptions );
                    printf( ",\"quickness\":%d",     defense->quickness     );
                    printf( ",\"sim_pass_rush\":%d", defense->sim_pass_rush );
                    printf( ",\"sim_coverage\":%d",  defense->sim_coverage  );
               }

               break;

          case pos_Kicker:
          case pos_Punter:
               if ( player->extra_ratings.kicking != NULL )
               {
                    player_kicking_ratings_s *kicking = player->extra_ratings.kicking;

                    printf( ",\"kicking_ability\":%d",  kicking->kicking_ability  );
                    printf( ",\"avoid_kick_block\":%d", kicking->avoid_kick_block );
                    printf( ",\"sim_kicking\":%d",      kicking->sim_kicking      );
               }

               break;
          }

          printf( "}" );
     }

     printf( ",\"stats\":{" );

     if ( player->stats.offense != NULL )
     {
          player_offense_stats_s *offense = player->stats.offense;

          printf( "\"offense\":" );

          printf( "{\"pass_attempts\":%d",        offense->pass_attempts        );
          printf( ",\"completions\":%d",          offense->completions          );
          printf( ",\"interceptions\":%d",        offense->interceptions        );
          printf( ",\"pass_yards\":%d",           offense->pass_yards           );
          printf( ",\"pass_touchdowns\":%d",      offense->pass_touchdowns      );
          printf( ",\"rush_attempts\":%d",        offense->rush_attempts        );
          printf( ",\"rush_yards\":%d",           offense->rush_yards           );
          printf( ",\"rush_touchdowns\":%d",      offense->rush_touchdowns      );
          printf( ",\"receptions\":%d",           offense->receptions           );
          printf( ",\"receiving_yards\":%d",      offense->receiving_yards      );
          printf( ",\"receiving_touchdowns\":%d", offense->receiving_touchdowns );

          printf( "}" );
     }

     if ( player->stats.defense != NULL )
     {
          player_defense_stats_s *defense = player->stats.defense;

          printf( "\"defense\":" );

          printf( "{\"sacks\":%d",             defense->sacks             );
          printf( ",\"interceptions\":%d",     defense->interceptions     );
          printf( ",\"return_yards\":%d",      defense->return_yards      );
          printf( ",\"return_touchdowns\":%d", defense->return_touchdowns );

          printf( "}" );
     }

     if ( player->stats.kicking != NULL )
     {
          player_kicking_stats_s *kicking = player->stats.kicking;

          printf( "\"kicking\":" );

          printf( "{\"extra_point_attempts\":%d", kicking->extra_point_attempts );
          printf( ",\"extra_points_made\":%d",    kicking->extra_points_made    );
          printf( ",\"field_goal_attempts\":%d",  kicking->field_goal_attempts  );
          printf( ",\"field_goals_made\":%d",     kicking->field_goals_made     );
          printf( ",\"punts\":%d",                kicking->punts                );
          printf( ",\"punt_yards\":%d",           kicking->punt_yards           );

          printf( "}" );
     }

     if ( player->stats.returns != NULL )
     {
          player_returns_stats_s *returns = player->stats.returns;

          printf( ",\"returns\":" );

          printf( "{\"kick_returns\":%d",           returns->kick_returns           );
          printf( ",\"kick_return_yards\":%d",      returns->kick_return_yards      );
          printf( ",\"kick_return_touchdowns\":%d", returns->kick_return_touchdowns );
          printf( ",\"punt_returns\":%d",           returns->punt_returns           );
          printf( ",\"punt_return_yards\":%d",      returns->punt_return_yards      );
          printf( ",\"punt_return_touchdowns\":%d", returns->punt_return_touchdowns );

          printf( "}" );
     }

     printf( "}" );
     printf( "}" );
}

static void printTeam( const team_s *team )
{
     printf( "{\"name\":\"%s\"",         team->name         );
     printf( ",\"location\":\"%s\"",     team->location     );
     printf( ",\"abbreviation\":\"%s\"", team->abbreviation );
     printf( ",\"sim_offense\":%d",      team->sim_offense  );
     printf( ",\"sim_defense\":%d",      team->sim_defense  );


     if ( team->stats != NULL )
     {
          printf( ",\"stats\":" );

          printf( "{\"wins\":%d",           team->stats->wins           );
          printf( ",\"losses\":%d",         team->stats->losses         );
          printf( ",\"ties\":%d",           team->stats->ties           );
          printf( ",\"points_scored\":%d",  team->stats->points_scored  );
          printf( ",\"points_allowed\":%d", team->stats->points_allowed );

          printf( "}" );
     }

     if ( team->offense_stats != NULL )
     {
          printf( ",\"offense_stats\":" );

          printf( "{\"pass_attempts\":%d",   team->offense_stats->pass_attempts   );
          printf( ",\"completions\":%d",     team->offense_stats->completions     );
          printf( ",\"interceptions\":%d",   team->offense_stats->interceptions   );
          printf( ",\"pass_yards\":%d",      team->offense_stats->pass_yards      );
          printf( ",\"pass_touchdowns\":%d", team->offense_stats->pass_touchdowns );
          printf( ",\"rush_attempts\":%d",   team->offense_stats->rush_attempts   );
          printf( ",\"rush_yards\":%d",      team->offense_stats->rush_yards      );
          printf( ",\"rush_touchdowns\":%d", team->offense_stats->rush_touchdowns );

          printf( "}" );
     }

     if ( team->defense_stats != NULL )
     {
          printf( ",\"defense_stats\":" );

          printf( "{\"sacks\":%d",             team->defense_stats->sacks             );
          printf( ",\"interceptions\":%d",     team->defense_stats->interceptions     );
          printf( ",\"return_yards\":%d",      team->defense_stats->return_yards      );
          printf( ",\"return_touchdowns\":%d", team->defense_stats->return_touchdowns );

          printf( "}" );
     }

     if ( team->kicking_stats != NULL )
     {
          printf( ",\"kicking_stats\":" );

          printf( "{\"extra_point_attempts\":%d",   team->kicking_stats->extra_point_attempts   );
          printf( ",\"extra_points_made\":%d",      team->kicking_stats->extra_points_made      );
          printf( ",\"field_goal_attempts\":%d",    team->kicking_stats->field_goal_attempts    );
          printf( ",\"field_goals_made\":%d",       team->kicking_stats->field_goals_made       );
          printf( ",\"punts\":%d",                  team->kicking_stats->punts                  );
          printf( ",\"punt_yards\":%d",             team->kicking_stats->punt_yards             );
          printf( ",\"kick_returns\":%d",           team->kicking_stats->kick_returns           );
          printf( ",\"kick_return_yards\":%d",      team->kicking_stats->kick_return_yards      );
          printf( ",\"kick_return_touchdowns\":%d", team->kicking_stats->kick_return_touchdowns );
          printf( ",\"punt_returns\":%d",           team->kicking_stats->punt_returns           );
          printf( ",\"punt_return_yards\":%d",      team->kicking_stats->punt_return_yards      );
          printf( ",\"punt_return_touchdowns\":%d", team->kicking_stats->punt_return_touchdowns );

          printf( "}" );
     }

     if ( team->players != NULL )
     {
          printf( ",\"players\":[" );

          for ( int i = 0; team->players[i].player != NULL; ++i )
          {
               if ( i > 0 ) printf( "," );

               printPlayer( team->players[i].player );
          }

          printf( "]" );
     }

     printf( "}" );
}

static void printConference( const conference_s *conference )
{
     printf( "{\"name\":\"%s\"", conference->name );


     if ( conference->teams != NULL )
     {
          printf( ",\"teams\":[" );

          for ( int i = 0; conference->teams[i].team != NULL; ++i )
          {
               if ( i > 0 ) printf( "," );

               printTeam( conference->teams[i].team );
          }

          printf( "]" );
     }

     printf( "}" );
}

static void printData( const organization_s *organization )
{
     printf( "{" );

     printf( "\"name\":\"%s\"",          organization->name         );
     printf( ",\"abbreviation\":\"%s\"", organization->abbreviation );
     printf( ",\"season\":null",         organization->name         );

     if ( organization->conferences != NULL )
     {
          printf( ",\"conferences\":[" );

          for ( int i = 0; organization->conferences[i].conference != NULL; ++i )
          {
               if ( i > 0 ) printf( "," );

               printConference( organization->conferences[i].conference );
          }

          printf( "]" );
     }

     printf( "}\n" );
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

     printData( organization );

     free( rom1 );
     free( rom2 );
     free( state_file1 );
     free( state_file2 );

     free_organization( organization );

     return EXIT_SUCCESS;
}
