#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "conversion.h"
#include "organization.h"

static char error_message[ 999 + 1 ] = { 0 };

char *getConvertBowlGameError( void )
{
     return error_message;
}


static const nst_matchup_s *findMatch( const nst_save_state_s *save_state )
{
     if ( save_state->current_week[0] < 17 )
     {
          return NULL;
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

static void updateQuarterbackStats( team_s *team, player_s *player, const nst_quarterback_game_s *quarterback )
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

static void updateOffenseStats( team_s *team, player_s *player, const nst_offense_game_s *offense )
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

static void updateDefenseStats( team_s *team, player_s *player, const nst_defense_game_s *defense )
{
     player->stats.defense->sacks             =           defense->sacks             [0];
     player->stats.defense->interceptions     =           defense->interceptions     [0];
     player->stats.defense->return_yards      = word2int( defense->return_yards          );
     player->stats.defense->return_touchdowns =           defense->return_touchdowns [0];

     if ( player->stats.defense->return_yards > 65500 ) player->stats.defense->return_yards = 0;

     team->defense_stats->sacks             += player->stats.defense->sacks;
     team->defense_stats->interceptions     += player->stats.defense->interceptions;
     team->defense_stats->return_yards      += player->stats.defense->return_yards;
     team->defense_stats->return_touchdowns += player->stats.defense->return_touchdowns;
}

static void updateKickerStats( team_s *team, player_s *player, const nst_kicker_game_s *kicker )
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

static void updatePunterStats( team_s *team, player_s *player, const nst_punter_game_s *punter )
{
     player->stats.kicking->punts      =           punter->punts      [0];
     player->stats.kicking->punt_yards = word2int( punter->punt_yards     );

     team->kicking_stats->punts      += player->stats.kicking->punts;
     team->kicking_stats->punt_yards += player->stats.kicking->punt_yards;
}

static void loadGameStats( team_s *road_team, team_s *home_team, const nst_save_state_s *save_state )
{
     int road_score = hex2number( save_state->playoffs[save_state->game_of_week[0]].road_score[0] );
     int home_score = hex2number( save_state->playoffs[save_state->game_of_week[0]].home_score[0] );

     road_team->stats->wins           = (road_score  > home_score) ? 1 : 0;
     road_team->stats->losses         = (home_score  > road_score) ? 1 : 0;
     road_team->stats->ties           = (road_score == home_score) ? 1 : 0;
     road_team->stats->points_scored  = road_score;
     road_team->stats->points_allowed = home_score;

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

     home_team->stats->wins           = (home_score  > road_score) ? 1 : 0;
     home_team->stats->losses         = (road_score  > home_score) ? 1 : 0;
     home_team->stats->ties           = (road_score == home_score) ? 1 : 0;
     home_team->stats->points_scored  = home_score;
     home_team->stats->points_allowed = road_score;

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
}

bowl_game_s *convertBowlGame( const tsbrom_s *rom, const nst_save_state_s *save_state, const int season, const bowl_game_e bowl )
{
     const nst_matchup_s    *match            = NULL;
     /**/  nst_save_state_s  empty_save_state = { 0 };
     /**/  bowl_game_s      *bowl_game        = NULL;

     if ( (bowl_game = malloc( sizeof(bowl_game_s) )) == NULL )
     {
          sprintf( error_message, "Unable to allocate memory for bowl game: %s", strerror(errno) );

          return NULL;
     }

     if ( (bowl_game->_organization = convertOrganization( rom, &empty_save_state, rom, &empty_save_state, season, bowl )) == NULL )
     {
          sprintf( error_message, "Error converting rom data: %s", getConvertOrganizationError() );

          freeBowlGame( bowl_game );

          return NULL;
     }

     if ( (match = findMatch( save_state )) == NULL )
     {
          sprintf( error_message, "Unable to retrieve match from save state" );

          freeBowlGame( bowl_game );

          return NULL;
     }

     if ( (bowl_game->road_team = findTeam( bowl_game->_organization, match->road[0] )) == NULL )
     {
          sprintf( error_message, "Road team <%d> not found in rom", match->road[0] );

          freeBowlGame( bowl_game );

          return NULL;
     }

     if ( (bowl_game->home_team = findTeam( bowl_game->_organization, match->home[0] )) == NULL )
     {
          sprintf( error_message, "Home team <%d> not found in rom", match->home[0] );

          freeBowlGame( bowl_game );

          return NULL;
     }

     loadGameStats( bowl_game->road_team, bowl_game->home_team, save_state );

     return bowl_game;
}

void copyScores( nst_save_state_s *save_state, const unsigned char *ram )
{
     save_state->playoffs[save_state->game_of_week[0]].road_score[0] = ram[ROAD_TEAM_SCORE_OFFSET];
     save_state->playoffs[save_state->game_of_week[0]].home_score[0] = ram[HOME_TEAM_SCORE_OFFSET];
}

void freeBowlGame( bowl_game_s *bowl_game )
{
     if ( bowl_game == NULL ) return;

     if ( bowl_game->_organization != NULL ) free_organization( bowl_game->_organization );

     free( bowl_game );
}
