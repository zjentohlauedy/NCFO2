#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
     printf( "{\"first_name\":\"%s\"",                     player->first_name );
     printf( ",\"last_name\":\"%s\"",                      player->last_name  );
     printf( ",\"position\":\"%s\"",   getDisplayPosition( player->position   ) );
     printf( ",\"number\":%d",                             player->number     );

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
     printf( "{\"name\":\"%s\"",             team->name                 );
     printf( ",\"location\":\"%s\"",         team->location             );
     printf( ",\"abbreviation\":\"%s\"",     team->abbreviation         );

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

static void printGameData( bowl_game_s *bowl_game )
{
     printf( "{\"road_team\":" );

     printTeam( bowl_game->road_team );

     printf( ",\"home_team\":" );

     printTeam( bowl_game->home_team );

     printf( "}" );
}

int main( const int argc, const char const *argv[] )
{
     static nst_save_state_s  empty_save_state = { 0 };
     const  char             *rom_filename     = NULL;
     const  char             *ss_filename      = NULL;

     tsbrom_s         *rom            = NULL;
     nst_save_state_s *save_state     = NULL;
     unsigned char    *ram            = NULL;
     unsigned char    *state_file     = NULL;
     bowl_game_s      *bowl_game      = NULL;
     int               save_state_len = 0;


     if ( argc != 3 )
     {
          printf( "Usage: %s <rom_file> <save_state_file>\n", argv[0] );

          return EXIT_SUCCESS;
     }

     rom_filename = argv[1];
     ss_filename  = argv[2];

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

     if ( (bowl_game = convertAAGame( rom, ram, save_state, 0, bg_None )) == NULL )
     {
          printf( "Error converting bowl game: %s\n", getConvertBowlGameError() );

          free( rom        );
          free( state_file );

          return EXIT_FAILURE;
     }

     printGameData( bowl_game );

     free( rom        );
     free( state_file );

     freeBowlGame( bowl_game );

     return EXIT_SUCCESS;
}
