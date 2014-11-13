#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "conversion.h"
#include "file_formats.h"
#include "save_state.h"


static char *expandName( char *name )
{
     static char  expanded_name[256] = { 0 };
     int   name_len           = strlen(name);

     char *s = expanded_name;

     for ( int i = 0; i < name_len; ++i )
     {
          if   ( isalpha(name[i]) ) sprintf( s, "%c ", name[i] );
          else                      sprintf( s, "%c",  name[i] );

          s += strlen(s);
     }

     return expanded_name;
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

static nst_matchup_s *findMatch( nst_save_state_s *save_state )
{
     if ( save_state->current_week[0] < 17 )
     {
          if ( save_state->game_of_week[0] > 13 ) return NULL;

          return &(save_state->matches[save_state->game_of_week[0]]);
     }

     if ( save_state->game_of_week[0] > 10 ) return NULL;

     return &(save_state->playoffs[save_state->game_of_week[0]].match);
}

static team_s *findTeam( organization_s *organization, int team_idx )
{
     int conference_idx = team_idx / 6;
     int conf_team_idx  = team_idx % 6;

     if ( organization->conferences == NULL ) return NULL;

     if ( conference_idx > 3 ) return NULL;

     return organization->conferences[conference_idx].conference->teams[conf_team_idx].team;
}

static void updateQuarterbackStats( team_s *team, player_s *player, nst_quarterback_game_s *quarterback )
{
     player->stats.offense->pass_attempts   =           quarterback->pass_attempts    [0];
     player->stats.offense->completions     =           quarterback->pass_completions [0];
     player->stats.offense->pass_touchdowns =           quarterback->pass_touchdowns  [0];
     player->stats.offense->interceptions   =           quarterback->interceptions    [0];
     player->stats.offense->pass_yards      = word2int( quarterback->pass_yards           );
     player->stats.offense->rush_attempts   =           quarterback->rush_attempts    [0];
     player->stats.offense->rush_yards      = word2int( quarterback->rush_yards           );
     player->stats.offense->rush_touchdowns =           quarterback->rush_touchdowns  [0];

     team->offense_stats->pass_attempts   += player->stats.offense->pass_attempts;
     team->offense_stats->completions     += player->stats.offense->completions;
     team->offense_stats->pass_touchdowns += player->stats.offense->pass_touchdowns;
     team->offense_stats->interceptions   += player->stats.offense->interceptions;
     team->offense_stats->pass_yards      += player->stats.offense->pass_yards;
     team->offense_stats->rush_attempts   += player->stats.offense->rush_attempts;
     team->offense_stats->rush_yards      += player->stats.offense->rush_yards;
     team->offense_stats->rush_touchdowns += player->stats.offense->rush_touchdowns;
}

static void updateOffenseStats( team_s *team, player_s *player, nst_offense_game_s *offense )
{
     player->stats.offense->receptions           =           offense->receptions           [0];
     player->stats.offense->receiving_yards      = word2int( offense->receiving_yards          );
     player->stats.offense->receiving_touchdowns =           offense->receiving_touchdowns [0];
     player->stats.offense->rush_attempts        =           offense->rush_attempts        [0];
     player->stats.offense->rush_yards           = word2int( offense->rush_yards               );
     player->stats.offense->rush_touchdowns      =           offense->rush_touchdowns      [0];

     player->stats.returns->kick_returns           =           offense->kick_returns  [0];
     player->stats.returns->kick_return_yards      = word2int( offense->kr_yards          );
     player->stats.returns->kick_return_touchdowns =           offense->kr_touchdowns [0];
     player->stats.returns->punt_returns           =           offense->punt_returns  [0];
     player->stats.returns->punt_return_yards      = word2int( offense->pr_yards          );
     player->stats.returns->punt_return_touchdowns =           offense->pr_touchdowns [0];

     team->offense_stats->rush_attempts   += player->stats.offense->rush_attempts;
     team->offense_stats->rush_yards      += player->stats.offense->rush_yards;
     team->offense_stats->rush_touchdowns += player->stats.offense->rush_touchdowns;

     team->kicking_stats->kick_returns           += player->stats.returns->kick_returns;
     team->kicking_stats->kick_return_yards      += player->stats.returns->kick_return_yards;
     team->kicking_stats->kick_return_touchdowns += player->stats.returns->kick_return_touchdowns;
     team->kicking_stats->punt_returns           += player->stats.returns->punt_returns;
     team->kicking_stats->punt_return_yards      += player->stats.returns->punt_return_yards;
     team->kicking_stats->punt_return_touchdowns += player->stats.returns->punt_return_touchdowns;
}

static void ignoreOffensiveLine( void )
{
}

static void updateDefenseStats( team_s *team, player_s *player, nst_defense_game_s *defense )
{
     player->stats.defense->sacks             =           defense->sacks             [0];
     player->stats.defense->interceptions     =           defense->interceptions     [0];
     player->stats.defense->return_yards      = word2int( defense->return_yards          );
     player->stats.defense->return_touchdowns =           defense->return_touchdowns [0];

     team->defense_stats->sacks             += player->stats.defense->sacks;
     team->defense_stats->interceptions     += player->stats.defense->interceptions;
     team->defense_stats->return_yards      += player->stats.defense->return_yards;
     team->defense_stats->return_touchdowns += player->stats.defense->return_touchdowns;
}

static void updateKickerStats( team_s *team, player_s *player, nst_kicker_game_s *kicker )
{
     player->stats.kicking->extra_points_made    = kicker->xp_made     [0];
     player->stats.kicking->extra_point_attempts = kicker->xp_attempts [0];
     player->stats.kicking->field_goals_made     = kicker->fg_made     [0];
     player->stats.kicking->field_goal_attempts  = kicker->fg_attempts [0];

     team->kicking_stats->extra_points_made    += player->stats.kicking->extra_points_made;
     team->kicking_stats->extra_point_attempts += player->stats.kicking->extra_point_attempts;
     team->kicking_stats->field_goals_made     += player->stats.kicking->field_goals_made;
     team->kicking_stats->field_goal_attempts  += player->stats.kicking->field_goal_attempts;
}

static void updatePunterStats( team_s *team, player_s *player, nst_punter_game_s *punter )
{
     player->stats.kicking->punts      =           punter->punts      [0];
     player->stats.kicking->punt_yards = word2int( punter->punt_yards     );

     team->kicking_stats->punts      += player->stats.kicking->punts;
     team->kicking_stats->punt_yards += player->stats.kicking->punt_yards;
}

static void loadGameStats( team_s *road_team, team_s *home_team, nst_save_state_s *save_state, unsigned char *state_file )
{
     int road_score = hex2number( state_file[0x3d1] );
     int home_score = hex2number( state_file[0x3d6] );

     if ( save_state->game_of_week[0] < 17 )
     {
          road_score = hex2number( state_file[0x3d1] );
          home_score = hex2number( state_file[0x3d6] );
     }
     else
     {
          road_score = hex2number( save_state->playoffs[save_state->game_of_week[0]].road_score[0] );
          home_score = hex2number( save_state->playoffs[save_state->game_of_week[0]].home_score[0] );
     }

     for ( int i = 0; i < 30; ++i )
     {
          player_s *player = road_team->players[i].player;

          if      ( i <  2 ) updateQuarterbackStats( road_team, player, &(save_state->road_team[0].quarterback[i     ]) );
          else if ( i < 12 ) updateOffenseStats(     road_team, player, &(save_state->road_team[0].offense    [i -  2]) );
          else if ( i < 17 ) ignoreOffensiveLine();
          else if ( i < 28 ) updateDefenseStats(     road_team, player, &(save_state->road_team[0].defense    [i - 17]) );
          else if ( i < 29 ) updateKickerStats(      road_team, player, &(save_state->road_team[0].kicker     [     0]) );
          else               updatePunterStats(      road_team, player, &(save_state->road_team[0].punter     [     0]) );
     }

     for ( int i = 0; i < 30; ++i )
     {
          player_s *player = home_team->players[i].player;

          if      ( i <  2 ) updateQuarterbackStats( home_team, player, &(save_state->home_team[0].quarterback[i     ]) );
          else if ( i < 12 ) updateOffenseStats(     home_team, player, &(save_state->home_team[0].offense    [i -  2]) );
          else if ( i < 17 ) ignoreOffensiveLine();
          else if ( i < 28 ) updateDefenseStats(     home_team, player, &(save_state->home_team[0].defense    [i - 17]) );
          else if ( i < 29 ) updateKickerStats(      home_team, player, &(save_state->home_team[0].kicker     [     0]) );
          else               updatePunterStats(      home_team, player, &(save_state->home_team[0].punter     [     0]) );
     }

     road_team->stats->points_scored = road_score;
     home_team->stats->points_scored = home_score;
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

static void printBoxscore( team_s *road_team, team_s *home_team, nst_save_state_s *save_state )
{
     printf( "{\"road_team\":" );

     printTeam( road_team );

     printf( ",\"home_team\":" );

     printTeam( home_team );

     printf( "}" );
}

int main( const int argc, const char const *argv[] )
{
     static nst_save_state_s  empty_save_state = { 0 };
     const  char             *rom_filename     = NULL;
     const  char             *ss_filename      = NULL;

     tsbrom_s         *rom            = NULL;
     nst_save_state_s *save_state     = NULL;
     nst_matchup_s    *match          = NULL;
     unsigned char    *state_file     = NULL;
     organization_s   *organization   = NULL;
     team_s           *road_team      = NULL;
     team_s           *home_team      = NULL;
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

     if ( (organization = convertOrganization( rom, &empty_save_state, rom, &empty_save_state, 0, bg_None )) == NULL )
     {
          printf( "Error converting rom and save state data: %s\n", getConvertOrganizationError() );

          free( rom        );
          free( state_file );

          return EXIT_FAILURE;
     }

     if ( (match = findMatch( save_state )) == NULL )
     {
          printf( "Unable to retrieve match\n" );

          free( rom        );
          free( state_file );

          free_organization( organization );

          return EXIT_FAILURE;
     }

     if ( (road_team = findTeam( organization, match->road[0] )) == NULL )
     {
          printf( "Road team <%d> not found in rom\n", match->road[0] );

          free( rom        );
          free( state_file );

          free_organization( organization );

          return EXIT_FAILURE;
     }

     if ( (home_team = findTeam( organization, match->home[0] )) == NULL )
     {
          printf( "Home team <%d> not found in rom\n", match->home[0] );

          free( rom        );
          free( state_file );

          free_organization( organization );

          return EXIT_FAILURE;
     }

     loadGameStats( road_team, home_team, save_state, state_file );

     printBoxscore( road_team, home_team, save_state );

     free( rom        );
     free( state_file );

     free_organization( organization );

     return EXIT_SUCCESS;
}
