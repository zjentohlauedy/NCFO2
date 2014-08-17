#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "conversion.h"

static char error_message[ 999 + 1 ] = { 0 };

char *getConvertOrganizationError( void )
{
     return error_message;
}


static int getLastNameOffset( const unsigned char *name, const int length )
{
     for ( int i = 0; i < length; ++i )
     {
          if ( isupper( name[i] ) ) return i;
     }

     return -1;
}

static player_ratings_s *convertPlayerRatings( const tsb_player_ratings_s *tsb_ratings )
{
     player_ratings_s *ratings = NULL;

     if ( (ratings = malloc( sizeof(player_ratings_s) )) == NULL )
     {
          sprintf( error_message, "Cannot allocate memory for player ratings" );

          return bl_False;
     }

     memset( ratings, '\0', sizeof(player_ratings_s) );

     ratings->run_speed  = (tsb_ratings->ratings[0] &  0x0f);
     ratings->rush_power = (tsb_ratings->ratings[0] >> 4   );
     ratings->max_speed  = (tsb_ratings->ratings[1] >> 4   );
     ratings->hit_power  = (tsb_ratings->ratings[1] &  0x0f);

     return ratings;
}

static boolean_e convertQuarterback(
     const tsb_qb_ratings_s  *tsb_ratings,
     const unsigned char     *sim_data,
     const nst_quarterback_s *stats,
     const int                season,
     const bowl_game_e        bowl,
     /**/  player_s          *player )
{
     player_quarterback_ratings_s *qb_ratings = NULL;
     player_offense_stats_s       *off_stats  = NULL;

     if ( (qb_ratings = malloc( sizeof(player_quarterback_ratings_s) )) == NULL )
     {
          sprintf( error_message, "Cannot allocate memory for player quarterback ratings" );

          return bl_False;
     }

     memset( qb_ratings, '\0', sizeof(player_quarterback_ratings_s) );

     if ( (off_stats = malloc( sizeof(player_offense_stats_s) )) == NULL )
     {
          sprintf( error_message, "Cannot allocate memory for player offense stats" );

          free( qb_ratings );

          return bl_False;
     }

     memset( off_stats, '\0', sizeof(player_offense_stats_s) );

     qb_ratings->pass_speed       = (tsb_ratings->qb_ratings[0] >> 4   );
     qb_ratings->pass_control     = (tsb_ratings->qb_ratings[0] &  0x0f);
     qb_ratings->pass_accuracy    = (tsb_ratings->qb_ratings[1] >> 4   );
     qb_ratings->avoid_pass_block = (tsb_ratings->qb_ratings[1] &  0x0f);

     qb_ratings->sim_rush         = (sim_data[0] >> 4   );
     qb_ratings->sim_pass         = (sim_data[0] &  0x0f);
     qb_ratings->sim_pocket       = (sim_data[1] &  0x0f);

     off_stats->player_id       = player->player_id;
     off_stats->season          = season;
     off_stats->bowl_game       = bowl;
     off_stats->pass_attempts   = ((stats->pass_attempts_modifier    [0] &  0x03) << 8) + stats->pass_attempts    [0];
     off_stats->completions     = ((stats->pass_completions_modifier [0] &  0x03) << 8) + stats->pass_completions [0];
     off_stats->interceptions   =  (stats->pass_completions_modifier [0] >> 2   );
     off_stats->pass_yards      = ((stats->yards_modifiers           [0] &  0xf8) << 5) + stats->pass_yards       [0];
     off_stats->pass_touchdowns =  (stats->pass_attempts_modifier    [0] >> 2   );
     off_stats->rush_attempts   =   stats->rush_attempts             [0];
     off_stats->rush_yards      = ((stats->yards_modifiers           [0] &  0x07) << 8) + stats->rush_yards       [0];
     off_stats->rush_touchdowns =  (stats->rush_touchdowns           [0] >> 2   );

     if ( (player->ratings = convertPlayerRatings( &(tsb_ratings->player) )) == NULL )
     {
          free( qb_ratings );
          free( off_stats  );

          return bl_False;
     }

     player->position                  = pos_Quarterback;
     player->extra_ratings.quarterback = qb_ratings;
     player->stats.offense             = off_stats;

     return bl_True;
}

static player_offense_stats_s *convertOffenseStats( const nst_offense_s *stats, const int season, const bowl_game_e bowl, const int player_id )
{
     player_offense_stats_s *off_stats = NULL;

     if ( (off_stats = malloc( sizeof(player_offense_stats_s) )) == NULL )
     {
          sprintf( error_message, "Cannot allocate memory for player offense stats" );

          return NULL;
     }

     memset( off_stats, '\0', sizeof(player_offense_stats_s) );

     off_stats->player_id            = player_id;
     off_stats->season               = season;
     off_stats->bowl_game            = bowl;
     off_stats->rush_attempts        =   stats->rush_attempts       [0];
     off_stats->rush_yards           = ((stats->rush_yards_modifier2[0] &  0x03) << 10) + ((stats->rush_yards_modifier1[0] & 0x03) << 8) + stats->rush_yards     [0];
     off_stats->rush_touchdowns      =  (stats->rush_yards_modifier2[0] >> 2   );
     off_stats->receptions           =   stats->receptions          [0];
     off_stats->receiving_yards      = ((stats->rec_yards_modifier  [0] &  0x0f) <<  8)                                                  + stats->receiving_yards[0];
     off_stats->receiving_touchdowns =  (stats->pr_yards_modifier   [0] >> 2   );

     return off_stats;
}

static player_returns_stats_s *convertReturnsStats( const nst_offense_s *stats, const int season, const bowl_game_e bowl, const int player_id  )
{
     player_returns_stats_s *ret_stats = NULL;

     if ( (ret_stats = malloc( sizeof(player_returns_stats_s) )) == NULL )
     {
          sprintf( error_message, "Cannot allocate memory for player returns stats" );

          return NULL;
     }

     memset( ret_stats, '\0', sizeof(player_returns_stats_s) );

     ret_stats->player_id              = player_id;
     ret_stats->season                 = season;
     ret_stats->bowl_game              = bowl;
     ret_stats->kick_returns           =  (stats->kick_returns         [0] >> 1);
     ret_stats->kick_return_yards      = ((stats->kr_yards_modifier    [0] &  0x07) << 8) + stats->kr_yards[0];
     ret_stats->kick_return_touchdowns =  (stats->kr_yards_modifier    [0] >> 3);
     ret_stats->punt_returns           =  (stats->rush_yards_modifier1 [0] >> 2);
     ret_stats->punt_return_yards      = ((stats->pr_yards_modifier    [0] &  0x03) << 8) + stats->pr_yards[0];
     ret_stats->punt_return_touchdowns =  (stats->rec_yards_modifier   [0] >> 4);

     return ret_stats;
}

static player_offense_ratings_s *convertOffenseRatings( const tsb_off_ratings_s *tsb_ratings, const unsigned char *sim_data )
{
     player_offense_ratings_s *off_ratings = NULL;

     if ( (off_ratings = malloc( sizeof(player_offense_ratings_s) )) == NULL )
     {
          sprintf( error_message, "Cannot allocate memory for player offense ratings" );

          return NULL;
     }

     memset( off_ratings, '\0', sizeof(player_offense_ratings_s) );

     off_ratings->ball_control = (tsb_ratings->off_ratings[0] >> 4   );
     off_ratings->receiving    = (tsb_ratings->off_ratings[0] &  0x0f);

     off_ratings->sim_rush     = (sim_data[0] >> 4   );
     off_ratings->sim_catch    = (sim_data[0] &  0x0f);
     off_ratings->sim_yards    = (sim_data[1] >> 4   );
     off_ratings->sim_target   = (sim_data[1] &  0x0f);

     return off_ratings;
}

static boolean_e convertRunningback(
     const tsb_off_ratings_s *tsb_ratings,
     const unsigned char     *sim_data,
     const nst_offense_s     *stats,
     const int                season,
     const bowl_game_e        bowl,
     /**/  player_s          *player )
{
     player->position              = pos_Runningback;

     if ( (player->ratings = convertPlayerRatings( &(tsb_ratings->player) )) == NULL )
     {
          return bl_False;
     }

     if ( (player->extra_ratings.offense = convertOffenseRatings( tsb_ratings, sim_data )) == NULL )
     {
          free( player->ratings );

          return bl_False;
     }

     if ( (player->stats.offense = convertOffenseStats( stats, season, bowl, player->player_id )) == NULL )
     {
          free( player->ratings );
          free( player->extra_ratings.offense );

          return bl_False;
     }

     if ( (player->stats.returns = convertReturnsStats( stats, season, bowl, player->player_id )) == NULL )
     {
          free( player->ratings );
          free( player->extra_ratings.offense );
          free( player->stats.offense         );

          return bl_False;
     }

     return bl_True;
}

static boolean_e convertWideReceiver(
     const tsb_off_ratings_s *tsb_ratings,
     const unsigned char     *sim_data,
     const nst_offense_s     *stats,
     const int                season,
     const bowl_game_e        bowl,
     /**/  player_s          *player )
{
     player->position = pos_WideReceiver;

     if ( (player->ratings = convertPlayerRatings( &(tsb_ratings->player) )) == NULL )
     {
          return bl_False;
     }

     if ( (player->extra_ratings.offense = convertOffenseRatings( tsb_ratings, sim_data )) == NULL )
     {
          free( player->ratings );

          return bl_False;
     }

     if ( (player->stats.offense = convertOffenseStats( stats, season, bowl, player->player_id )) == NULL )
     {
          free( player->ratings );
          free( player->extra_ratings.offense );

          return bl_False;
     }

     if ( (player->stats.returns = convertReturnsStats( stats, season, bowl, player->player_id )) == NULL )
     {
          free( player->ratings );
          free( player->extra_ratings.offense );
          free( player->stats.offense         );

          return bl_False;
     }

     return bl_True;
}

static boolean_e convertTightEnd(
     const tsb_off_ratings_s *tsb_ratings,
     const unsigned char     *sim_data,
     const nst_offense_s     *stats,
     const int                season,
     const bowl_game_e        bowl,
     /**/  player_s          *player )
{
     player->position = pos_TightEnd;

     if ( (player->ratings = convertPlayerRatings( &(tsb_ratings->player) )) == NULL )
     {
          return bl_False;
     }

     if ( (player->extra_ratings.offense = convertOffenseRatings( tsb_ratings, sim_data )) == NULL )
     {
          free( player->ratings );

          return bl_False;
     }

     if ( (player->stats.offense = convertOffenseStats( stats, season, bowl, player->player_id )) == NULL )
     {
          free( player->ratings );
          free( player->extra_ratings.offense );

          return bl_False;
     }

     if ( (player->stats.returns = convertReturnsStats( stats, season, bowl, player->player_id )) == NULL )
     {
          free( player->ratings );
          free( player->extra_ratings.offense );
          free( player->stats.offense         );

          return bl_False;
     }

     return bl_True;
}

static boolean_e convertOffensiveLine( const tsb_player_ratings_s *tsb_ratings, player_s *player )
{
     player->position = pos_OffensiveLine;

     if ( (player->ratings = convertPlayerRatings( tsb_ratings )) == NULL )
     {
          return bl_False;
     }

     return bl_True;
}

static player_defense_stats_s *convertDefenseStats( const nst_defense_s *stats, const int season, const bowl_game_e bowl, const int player_id )
{
     player_defense_stats_s *def_stats = NULL;

     if ( (def_stats = malloc( sizeof(player_defense_stats_s) )) == NULL )
     {
          sprintf( error_message, "Cannot allocate memory for player defense stats" );

          return NULL;
     }

     memset( def_stats, '\0', sizeof(player_defense_stats_s) );

     def_stats->player_id         = player_id;
     def_stats->season            = season;
     def_stats->bowl_game         = bowl;
     def_stats->sacks             = (stats->sacks[0] >> 1);
     def_stats->interceptions     = ((stats->sacks[0] & 0x01) << 4) + (stats->interceptions[0] >> 4);
     def_stats->return_yards      = ((stats->interceptions[0] & 0x01) << 8) + stats->ir_yards[0];
     def_stats->return_touchdowns = ((stats->interceptions[0] & 0x0e) >> 1);

     return def_stats;
}

static player_defense_ratings_s *convertDefenseRatings( const tsb_def_ratings_s *tsb_ratings, const tsb_sim_data_s *sim_data, const int idx )
{
     player_defense_ratings_s *def_ratings = NULL;

     if ( (def_ratings = malloc( sizeof(player_defense_ratings_s) )) == NULL )
     {
          sprintf( error_message, "Cannot allocate memory for player defense ratings" );

          return NULL;
     }

     memset( def_ratings, '\0', sizeof(player_defense_ratings_s) );

     def_ratings->interceptions = (tsb_ratings->def_ratings[0] >> 4   );
     def_ratings->quickness     = (tsb_ratings->def_ratings[0] &  0x0f);

     def_ratings->sim_pass_rush = sim_data->defense_pass_rush[idx];
     def_ratings->sim_coverage  = sim_data->defense_coverage [idx];

     return def_ratings;
}

static boolean_e convertDefensiveLine(
     const tsb_def_ratings_s *tsb_ratings,
     const tsb_sim_data_s    *sim_data,
     const int                idx,
     const nst_defense_s     *stats,
     const int                season,
     const bowl_game_e        bowl,
     /**/  player_s          *player )
{
     player->position = pos_DefensiveLine;

     if ( (player->ratings = convertPlayerRatings( &(tsb_ratings->player) )) == NULL )
     {
          return bl_False;
     }

     if ( (player->extra_ratings.defense = convertDefenseRatings( tsb_ratings, sim_data, idx )) == NULL )
     {
          free( player->ratings );

          return bl_False;
     }

     if ( (player->stats.defense = convertDefenseStats( stats, season, bowl, player->player_id )) == NULL )
     {
          free( player->ratings );
          free( player->extra_ratings.defense );

          return bl_False;
     }

     return bl_True;
}

static boolean_e convertLinebacker(
     const tsb_def_ratings_s *tsb_ratings,
     const tsb_sim_data_s    *sim_data,
     const int                idx,
     const nst_defense_s     *stats,
     const int                season,
     const bowl_game_e        bowl,
     /**/  player_s          *player )
{
     player->position = pos_Linebacker;

     if ( (player->ratings = convertPlayerRatings( &(tsb_ratings->player) )) == NULL )
     {
          return bl_False;
     }

     if ( (player->extra_ratings.defense = convertDefenseRatings( tsb_ratings, sim_data, idx )) == NULL )
     {
          free( player->ratings );

          return bl_False;
     }

     if ( (player->stats.defense = convertDefenseStats( stats, season, bowl, player->player_id )) == NULL )
     {
          free( player->ratings );
          free( player->extra_ratings.defense );

          return bl_False;
     }

     return bl_True;
}

static boolean_e convertCornerback(
     const tsb_def_ratings_s *tsb_ratings,
     const tsb_sim_data_s    *sim_data,
     const int                idx,
     const nst_defense_s     *stats,
     const int                season,
     const bowl_game_e        bowl,
     /**/  player_s          *player )
{
     player->position = pos_Cornerback;

     if ( (player->ratings = convertPlayerRatings( &(tsb_ratings->player) )) == NULL )
     {
          return bl_False;
     }

     if ( (player->extra_ratings.defense = convertDefenseRatings( tsb_ratings, sim_data, idx )) == NULL )
     {
          free( player->ratings );

          return bl_False;
     }

     if ( (player->stats.defense = convertDefenseStats( stats, season, bowl, player->player_id )) == NULL )
     {
          free( player->ratings );
          free( player->extra_ratings.defense );

          return bl_False;
     }

     return bl_True;
}

static boolean_e convertSafety(
     const tsb_def_ratings_s *tsb_ratings,
     const tsb_sim_data_s    *sim_data,
     const int                idx,
     const nst_defense_s     *stats,
     const int                season,
     const bowl_game_e        bowl,
     /**/  player_s          *player )
{
     player->position = pos_Safety;

     if ( (player->ratings = convertPlayerRatings( &(tsb_ratings->player) )) == NULL )
     {
          return bl_False;
     }

     if ( (player->extra_ratings.defense = convertDefenseRatings( tsb_ratings, sim_data, idx )) == NULL )
     {
          free( player->ratings );

          return bl_False;
     }

     if ( (player->stats.defense = convertDefenseStats( stats, season, bowl, player->player_id )) == NULL )
     {
          free( player->ratings );
          free( player->extra_ratings.defense );

          return bl_False;
     }

     return bl_True;
}

static player_kicking_ratings_s *convertKickingRatings( const tsb_kick_ratings_s *tsb_ratings )
{
     player_kicking_ratings_s *kick_ratings = NULL;

     if ( (kick_ratings = malloc( sizeof(player_kicking_ratings_s) )) == NULL )
     {
          sprintf( error_message, "Cannot allocate memory for player kicking ratings" );

          return NULL;
     }

     memset( kick_ratings, '\0', sizeof(player_kicking_ratings_s) );

     kick_ratings->kicking_ability  = (tsb_ratings->kick_ratings[0] >> 4   );
     kick_ratings->avoid_kick_block = (tsb_ratings->kick_ratings[0] &  0x0f);

     return kick_ratings;
}

static boolean_e convertKicker(
     const tsb_kick_ratings_s *tsb_ratings,
     const tsb_sim_data_s     *sim_data,
     const nst_kicker_s       *stats,
     const int                 season,
     const bowl_game_e         bowl,
     /**/  player_s           *player )
{
     player_kicking_stats_s *kick_stats = NULL;

     player->position = pos_Kicker;

     if ( (player->ratings = convertPlayerRatings( &(tsb_ratings->player) )) == NULL )
     {
          return bl_False;
     }

     if ( (player->extra_ratings.kicking = convertKickingRatings( tsb_ratings )) == NULL )
     {
          free( player->ratings );

          return bl_False;
     }

     if ( (kick_stats = malloc( sizeof(player_kicking_stats_s) )) == NULL )
     {
          sprintf( error_message, "Cannot allocate memory for player kicking stats" );

          free( player->ratings );
          free( player->extra_ratings.kicking );

          return bl_False;
     }

     memset( kick_stats, '\0', sizeof(player_kicking_stats_s) );

     kick_stats->player_id            = player->player_id;
     kick_stats->season               = season;
     kick_stats->bowl_game            = bowl;
     kick_stats->extra_point_attempts = stats->xp_attempts [0];
     kick_stats->extra_points_made    = stats->xp_made     [0];
     kick_stats->field_goal_attempts  = stats->fg_attempts [0];
     kick_stats->field_goals_made     = stats->fg_made     [0];

     player->stats.kicking = kick_stats;

     player->extra_ratings.kicking->sim_kicking = (sim_data->kicking[0] >> 4);

     return bl_True;
}

static boolean_e convertPunter(
     const tsb_kick_ratings_s *tsb_ratings,
     const tsb_sim_data_s     *sim_data,
     const nst_punter_s       *stats,
     const int                 season,
     const bowl_game_e         bowl,
     /**/  player_s           *player )
{
     player_kicking_stats_s *kick_stats = NULL;

     player->position = pos_Punter;

     if ( (player->ratings = convertPlayerRatings( &(tsb_ratings->player) )) == NULL )
     {
          return bl_False;
     }

     if ( (player->extra_ratings.kicking = convertKickingRatings( tsb_ratings )) == NULL )
     {
          free( player->ratings );

          return bl_False;
     }

     if ( (kick_stats = malloc( sizeof(player_kicking_stats_s) )) == NULL )
     {
          sprintf( error_message, "Cannot allocate memory for player kicking stats" );

          free( player->ratings );
          free( player->extra_ratings.kicking );

          return bl_False;
     }

     memset( kick_stats, '\0', sizeof(player_kicking_stats_s) );

     kick_stats->player_id            = player->player_id;
     kick_stats->season               = season;
     kick_stats->bowl_game            = bowl;
     kick_stats->punts                = stats->punts[0];
     kick_stats->punt_yards           = ((stats->punt_yards_modifier[0] & 0x0f) << 8) + stats->punt_yards[0];

     player->stats.kicking = kick_stats;

     player->extra_ratings.kicking->sim_kicking = (sim_data->kicking[0] & 0x0f);

     return bl_True;
}

static team_player_s *convertPlayers(
     const tsbrom_s         *rom,
     const nst_save_state_s *save_state,
     const int               season,
     const bowl_game_e       bowl,
     const int               team_idx )
{
     team_player_s  team_player_sentinel = TEAM_PLAYER_SENTINEL;
     team_player_s *players              = NULL;

     if ( (players = malloc( sizeof(team_player_s) * 31 )) == NULL )
     {
          sprintf( error_message, "Cannot allocate memory for team players" );

          return NULL;
     }

     memset( players, '\0', sizeof(team_player_s) * 31 );

     for ( int i = 0; i < 30; ++i )
     {
          int player_idx = (team_idx * 30) + i;

          players[i].team_id   = team_idx + 1;
          players[i].player_id = player_idx + 1;

          if ( (players[i].player = malloc( sizeof(player_s) )) == NULL )
          {
               sprintf( error_message, "Cannot allocate memory for player %d", player_idx + 1 );

               free( players );

               return NULL;
          }

          memset( players[i].player, '\0', sizeof(player_s) );

          players[i].player->player_id = players[i].player_id;

          int rom_team_idx = team_idx % 24;

          int start_offset = pointer2int( &(rom->player_pointers[rom_team_idx][i    ]) ) - 0x86ca;
          int end_offset   = pointer2int( &(rom->player_pointers[rom_team_idx][i + 1]) ) - 0x86ca;

          players[i].player->number = hex2number( rom->player_identifiers[start_offset] );

          start_offset++;

          int lname_offset = getLastNameOffset( rom->player_identifiers + start_offset, end_offset - start_offset ) + start_offset;

          memcpy( players[i].player->first_name, rom->player_identifiers + start_offset, lname_offset - start_offset );
          memcpy( players[i].player->last_name,  rom->player_identifiers + lname_offset, end_offset   - lname_offset );

          strcpy( players[i].player->first_name, normalizeCase( players[i].player->first_name ) );
          strcpy( players[i].player->last_name,  normalizeCase( players[i].player->last_name  ) );

          const tsb_ratings_team_s *team_player_ratings = &(rom->team_player_ratings[rom_team_idx]);
          const tsb_sim_data_s     *sim_data            = &(rom->sim_data[rom_team_idx]);
          const nst_players_s      *player_stats        = NULL;
          const injury_bits_s      *injuries            = NULL;
          const condition_bits_s   *conditions          = NULL;

          if ( rom_team_idx < 9 )
          {
               player_stats =   save_state->stats1[rom_team_idx    ].player_stats;
               injuries     = &(save_state->stats1[rom_team_idx    ].injuries);
               conditions   = &(save_state->stats1[rom_team_idx    ].conditions);
          }
          else
          {
               player_stats =   save_state->stats2[rom_team_idx - 9].player_stats;
               injuries     = &(save_state->stats1[rom_team_idx - 9].injuries);
               conditions   = &(save_state->stats1[rom_team_idx - 9].conditions);
          }

          switch ( i )
          {
          case  0: players[i].player->condition = conditions->qb1; players[i].player->injured = (injuries->qb1 != 0); break;
          case  1: players[i].player->condition = conditions->qb2; players[i].player->injured = (injuries->qb2 != 0); break;
          case  2: players[i].player->condition = conditions->rb1; players[i].player->injured = (injuries->rb1 != 0); break;
          case  3: players[i].player->condition = conditions->rb2; players[i].player->injured = (injuries->rb2 != 0); break;
          case  4: players[i].player->condition = conditions->rb3; players[i].player->injured = (injuries->rb3 != 0); break;
          case  5: players[i].player->condition = conditions->rb4; players[i].player->injured = (injuries->rb4 != 0); break;
          case  6: players[i].player->condition = conditions->wr1; players[i].player->injured = (injuries->wr1 != 0); break;
          case  7: players[i].player->condition = conditions->wr2; players[i].player->injured = (injuries->wr2 != 0); break;
          case  8: players[i].player->condition = conditions->wr3; players[i].player->injured = (injuries->wr3 != 0); break;
          case  9: players[i].player->condition = conditions->wr4; players[i].player->injured = (injuries->wr4 != 0); break;
          case 10: players[i].player->condition = conditions->te1; players[i].player->injured = (injuries->te1 != 0); break;
          case 11: players[i].player->condition = conditions->te2; players[i].player->injured = (injuries->te2 != 0); break;
          case 12: players[i].player->condition = conditions->ol1;                                                    break;
          case 13: players[i].player->condition = conditions->ol2;                                                    break;
          case 14: players[i].player->condition = conditions->ol3;                                                    break;
          case 15: players[i].player->condition = conditions->ol4;                                                    break;
          case 16: players[i].player->condition = conditions->ol5;                                                    break;
          case 17: players[i].player->condition = conditions->dl1;                                                    break;
          case 18: players[i].player->condition = conditions->dl2;                                                    break;
          case 19: players[i].player->condition = conditions->dl3;                                                    break;
          case 20: players[i].player->condition = conditions->lb1;                                                    break;
          case 21: players[i].player->condition = conditions->lb2;                                                    break;
          case 22: players[i].player->condition = conditions->lb3;                                                    break;
          case 23: players[i].player->condition = conditions->lb4;                                                    break;
          case 24: players[i].player->condition = conditions->cb1;                                                    break;
          case 25: players[i].player->condition = conditions->cb2;                                                    break;
          case 26: players[i].player->condition = conditions->s1;                                                     break;
          case 27: players[i].player->condition = conditions->s2;                                                     break;
          case 28: players[i].player->condition = conditions->k;                                                      break;
          case 29: players[i].player->condition = conditions->p;                                                      break;
          }

          switch ( i )
          {
          case  0:
          case  1:
               if ( ! convertQuarterback( &(team_player_ratings->quarterback[i]),
                                          sim_data->quarterbacks[i],
                                          &(player_stats->quarterback[i]),
                                          season, bowl,
                                          players[i].player ) )
               {
                    // cleanup
                    return NULL;
               }

               break;

          case  2:
          case  3:
          case  4:
          case  5:
               if ( ! convertRunningback( &(team_player_ratings->offense[i - 2]),
                                          sim_data->offense[i - 2],
                                          &(player_stats->offense[i - 2]),
                                          season, bowl,
                                          players[i].player ) )
               {
                    // cleanup
                    return NULL;
               }

               break;

          case  6:
          case  7:
          case  8:
          case  9:
               if ( ! convertWideReceiver( &(team_player_ratings->offense[i - 2]),
                                           sim_data->offense[i - 2],
                                           &(player_stats->offense[i - 2]),
                                           season, bowl,
                                           players[i].player ) )
               {
                    // cleanup
                    return NULL;
               }

               break;

          case 10:
          case 11:
               if ( ! convertTightEnd( &(team_player_ratings->offense[i - 2]),
                                       sim_data->offense[i - 2],
                                       &(player_stats->offense[i - 2]),
                                       season, bowl,
                                       players[i].player ) )
               {
                    // cleanup
                    return NULL;
               }

               break;

          case 12:
          case 13:
          case 14:
          case 15:
          case 16:
               if ( ! convertOffensiveLine( &(team_player_ratings->linesmen[i - 12]), players[i].player ) )
               {
                    // cleanup
                    return NULL;
               }

               break;

          case 17:
          case 18:
          case 19:
               if ( ! convertDefensiveLine( &(team_player_ratings->defense[i - 17]),
                                            sim_data, i - 17,
                                            &(player_stats->defense[i - 17]),
                                            season, bowl,
                                            players[i].player ) )
               {
                    // cleanup
                    return NULL;
               }

               break;

          case 20:
          case 21:
          case 22:
          case 23:
               if ( ! convertLinebacker( &(team_player_ratings->defense[i - 17]),
                                         sim_data, i - 17,
                                         &(player_stats->defense[i - 17]),
                                         season, bowl,
                                         players[i].player ) )
               {
                    // cleanup
                    return NULL;
               }

               break;

          case 24:
          case 25:
               if ( ! convertCornerback( &(team_player_ratings->defense[i - 17]),
                                         sim_data, i - 17,
                                         &(player_stats->defense[i - 17]),
                                         season, bowl,
                                         players[i].player ) )
               {
                    // cleanup
                    return NULL;
               }

               break;

          case 26:
          case 27:
               if ( ! convertSafety( &(team_player_ratings->defense[i - 17]),
                                     sim_data, i - 17,
                                     &(player_stats->defense[i - 17]),
                                     season, bowl,
                                     players[i].player ) )
               {
                    // cleanup
                    return NULL;
               }

               break;

          case 28:
               if ( ! convertKicker( &(team_player_ratings->kickers[0]),
                                     sim_data,
                                     player_stats->kicker,
                                     season, bowl,
                                     players[i].player ) )
               {
                    // cleanup
                    return NULL;
               }

               break;

          case 29:
               if ( ! convertPunter( &(team_player_ratings->kickers[1]),
                                     sim_data,
                                     player_stats->punter,
                                     season, bowl,
                                     players[i].player ) )
               {
                    // cleanup
                    return NULL;
               }

               break;
          }
     }

     players[30] = team_player_sentinel;

     return players;
}

static team_stats_s *convertTeamStats(
     const nst_teams_s *tsb_stats,
     const int          season,
     const bowl_game_e  bowl,
     const int          team_id )
{
     team_stats_s *stats = NULL;

     if ( (stats = malloc( sizeof(team_stats_s) )) == NULL )
     {
          sprintf( error_message, "Cannot allocate memory for team stats" );

          return NULL;
     }

     memset( stats, '\0', sizeof(team_stats_s) );

     stats->team_id        =           team_id;
     stats->season         =           season;
     stats->bowl_game      =           bowl;
     stats->wins           =           tsb_stats->wins[0];
     stats->losses         =           tsb_stats->losses[0];
     stats->ties           =           tsb_stats->ties[0];
     stats->points_scored  = word2int( tsb_stats->points_scored  );
     stats->points_allowed = word2int( tsb_stats->points_allowed );

     return stats;
}

static team_offense_stats_s *convertTeamOffenseStats(
     const team_player_s *players,
     const int            season,
     const bowl_game_e    bowl,
     const int            team_id )
{
     team_offense_stats_s *offense = NULL;

     if ( (offense = malloc( sizeof(team_offense_stats_s) )) == NULL )
     {
          sprintf( error_message, "Cannot allocate memory for team offense stats" );

          return NULL;
     }

     memset( offense, '\0', sizeof(team_offense_stats_s) );

     offense->team_id   = team_id;
     offense->season    = season;
     offense->bowl_game = bowl;

     for ( int i = 0; players[i].player != NULL; ++i )
     {
          const player_s *player = players[i].player;

          if ( player->stats.offense == NULL ) continue;

          offense->pass_attempts   += player->stats.offense->pass_attempts;
          offense->completions     += player->stats.offense->completions;
          offense->interceptions   += player->stats.offense->interceptions;
          offense->pass_yards      += player->stats.offense->pass_yards;
          offense->pass_touchdowns += player->stats.offense->pass_touchdowns;
          offense->rush_attempts   += player->stats.offense->rush_attempts;
          offense->rush_yards      += player->stats.offense->rush_yards;
          offense->rush_touchdowns += player->stats.offense->rush_touchdowns;
     }

     return offense;
}

static team_defense_stats_s *convertTeamDefenseStats(
     const team_player_s *players,
     const int            season,
     const bowl_game_e    bowl,
     const int            team_id )
{
     team_defense_stats_s *defense = NULL;

     if ( (defense = malloc( sizeof(team_defense_stats_s) )) == NULL )
     {
          sprintf( error_message, "Cannot allocate memory for team defense stats" );

          return NULL;
     }

     memset( defense, '\0', sizeof(team_defense_stats_s) );

     defense->team_id   = team_id;
     defense->season    = season;
     defense->bowl_game = bowl;

     for ( int i = 0; players[i].player != NULL; ++i )
     {
          const player_s *player = players[i].player;

          if ( player->stats.defense == NULL ) continue;

          defense->sacks             += player->stats.defense->sacks;
          defense->interceptions     += player->stats.defense->interceptions;
          defense->return_yards      += player->stats.defense->return_yards;
          defense->return_touchdowns += player->stats.defense->return_touchdowns;
     }

     return defense;
}

static team_kicking_stats_s *convertTeamKickingStats(
     const team_player_s *players,
     const int            season,
     const bowl_game_e    bowl,
     const int            team_id )
{
     team_kicking_stats_s *kicking = NULL;

     if ( (kicking = malloc( sizeof(team_kicking_stats_s) )) == NULL )
     {
          sprintf( error_message, "Cannot allocate memory for team kicking stats" );

          return NULL;
     }

     memset( kicking, '\0', sizeof(team_kicking_stats_s) );

     kicking->team_id   = team_id;
     kicking->season    = season;
     kicking->bowl_game = bowl;

     for ( int i = 0; players[i].player != NULL; ++i )
     {
          const player_s *player = players[i].player;

          if ( player->stats.kicking != NULL )
          {
               kicking->extra_point_attempts   += player->stats.kicking->extra_point_attempts;
               kicking->extra_points_made      += player->stats.kicking->extra_points_made;
               kicking->field_goal_attempts    += player->stats.kicking->field_goal_attempts;
               kicking->field_goals_made       += player->stats.kicking->field_goals_made;
               kicking->punts                  += player->stats.kicking->punts;
               kicking->punt_yards             += player->stats.kicking->punt_yards;
          }

          if ( player->stats.returns != NULL )
          {
               kicking->kick_returns           += player->stats.returns->kick_returns;
               kicking->kick_return_yards      += player->stats.returns->kick_return_yards;
               kicking->kick_return_touchdowns += player->stats.returns->kick_return_touchdowns;
               kicking->punt_returns           += player->stats.returns->punt_returns;
               kicking->punt_return_yards      += player->stats.returns->punt_return_yards;
               kicking->punt_return_touchdowns += player->stats.returns->punt_return_touchdowns;
          }
     }

     return kicking;
}

static void fixTeamLocation( char *location )
{
     if ( strcmp( location, "N. Carolina" ) == 0 ) sprintf( location, "North Carolina" );
     if ( strcmp( location, "S. Carolina" ) == 0 ) sprintf( location, "South Carolina" );
}

static void fixTeamName( char *name )
{
     if ( strcmp( name, "B. Bears"   ) == 0 ) sprintf( name, "Black Bears"     );
     if ( strcmp( name, "S. Knights" ) == 0 ) sprintf( name, "Scarlet Knights" );
     if ( strcmp( name, "Mntneers"   ) == 0 ) sprintf( name, "Mountaineers"    );
     if ( strcmp( name, "C. Tide"    ) == 0 ) sprintf( name, "Crimson Tide"    );
     if ( strcmp( name, "F. Illini"  ) == 0 ) sprintf( name, "Fighting Illini" );
     if ( strcmp( name, "N. Lions"   ) == 0 ) sprintf( name, "Nittany Lions"   );
     if ( strcmp( name, "G. Bears"   ) == 0 ) sprintf( name, "Golden Bears"    );
     if ( strcmp( name, "C. Huskers" ) == 0 ) sprintf( name, "Corn Huskers"    );
     if ( strcmp( name, "G. Gophers" ) == 0 ) sprintf( name, "Golden Gophers"  );
     if ( strcmp( name, "F. Sioux"   ) == 0 ) sprintf( name, "Fighting Sioux"  );
     if ( strcmp( name, "R. Cajuns"  ) == 0 ) sprintf( name, "Ragin Cajuns"    );
}

static conference_team_s *convertTeams(
     const tsbrom_s         *rom,
     const nst_save_state_s *save_state,
     const int               season,
     const bowl_game_e       bowl,
     const int               conference_idx )
{
     conference_team_s  conference_team_sentinel = CONFERENCE_TEAM_SENTINEL;
     conference_team_s *teams                    = NULL;

     if ( (teams = malloc( sizeof(conference_team_s) * 7 )) == NULL )
     {
          sprintf( error_message, "Cannot allocate memory for conference teams" );

          return NULL;
     }

     memset( teams, '\0', sizeof(conference_team_s) * 7 );

     for ( int i = 0; i < 6; ++i )
     {
          int team_idx = (conference_idx * 6) + i;

          teams[i].conference_id = conference_idx + 1;
          teams[i].team_id       = team_idx + 1;

          if ( (teams[i].team = malloc( sizeof(team_s) )) == NULL )
          {
               sprintf( error_message, "Cannot allocate memory for team %d", team_idx + 1 );

               free( teams );

               return NULL;
          }

          memset( teams[i].team, '\0', sizeof(team_s) );

          teams[i].team->team_id = teams[i].team_id;

          int rom_team_idx = ((conference_idx % 4) * 6) + i;

          int abbr_offset =  pointer2int( &(rom->team_abbr_pointers[rom_team_idx    ]) ) - 0xbcf0;
          int abbr_length = (pointer2int( &(rom->team_abbr_pointers[rom_team_idx + 1]) ) - 0xbcf0) - abbr_offset;
          int locn_offset =  pointer2int( &(rom->team_loc_pointers[ rom_team_idx    ]) ) - 0xbcf0;
          int locn_length = (pointer2int( &(rom->team_loc_pointers[ rom_team_idx + 1]) ) - 0xbcf0) - locn_offset;
          int name_offset =  pointer2int( &(rom->team_name_pointers[rom_team_idx    ]) ) - 0xbcf0;
          int name_length = (pointer2int( &(rom->team_name_pointers[rom_team_idx + 1]) ) - 0xbcf0) - name_offset;

          sprintf( teams[i].team->name,         "%.*s", name_length, rom->team_conference_names + name_offset );
          sprintf( teams[i].team->location,     "%.*s", locn_length, rom->team_conference_names + locn_offset );
          sprintf( teams[i].team->abbreviation, "%.*s", abbr_length, rom->team_conference_names + abbr_offset );

          strcpy( teams[i].team->name,     normalizeCase( teams[i].team->name     ) );
          strcpy( teams[i].team->location, normalizeCase( teams[i].team->location ) );

          fixTeamName(     teams[i].team->name     );
          fixTeamLocation( teams[i].team->location );

          teams[i].team->sim_offense = (rom->sim_data[rom_team_idx].team[0] >> 4   );
          teams[i].team->sim_defense = (rom->sim_data[rom_team_idx].team[0] &  0x0f);

          const nst_teams_s *team_stats = NULL;

          if ( rom_team_idx < 9 ) team_stats = save_state->stats1[rom_team_idx    ].team_stats;
          else                    team_stats = save_state->stats2[rom_team_idx - 9].team_stats;

          teams[i].team->stats         = convertTeamStats( team_stats, season, bowl, teams[i].team_id );
          teams[i].team->players       = convertPlayers( rom, save_state, season, bowl, team_idx );
          teams[i].team->offense_stats = convertTeamOffenseStats( teams[i].team->players, season, bowl, teams[i].team_id );
          teams[i].team->defense_stats = convertTeamDefenseStats( teams[i].team->players, season, bowl, teams[i].team_id );
          teams[i].team->kicking_stats = convertTeamKickingStats( teams[i].team->players, season, bowl, teams[i].team_id );
     }

     teams[6] = conference_team_sentinel;

     return teams;
}

static organization_conference_s *assembleConferences( void )
{
     organization_conference_s  organization_conference_sentinel = ORGANIZATION_CONFERENCE_SENTINEL;
     organization_conference_s *conferences                      = NULL;

     if ( (conferences = malloc( sizeof(organization_conference_s) * 9 )) == NULL )
     {
          sprintf( error_message, "Cannot allocate memory for organization conferences" );

          return NULL;
     }

     memset( conferences, '\0', sizeof(organization_conference_s) * 9 );

     for ( int i = 0; i < 8; ++i )
     {
          conferences[i].organization_id = 1;
          conferences[i].conference_id   = i + 1;

          if ( (conferences[i].conference = malloc( sizeof(conference_s) )) == NULL )
          {
               sprintf( error_message, "Cannot allocate memory for conference %d", i + 1 );

               free( conferences );

               return NULL;
          }

          memset( conferences[i].conference, '\0', sizeof(conference_s) );

          conferences[i].conference->conference_id = i + 1;
     }

     sprintf( conferences[0].conference->name, "New England" );
     sprintf( conferences[1].conference->name, "Atlantic"    );
     sprintf( conferences[2].conference->name, "Southeast"   );
     sprintf( conferences[3].conference->name, "Great Lake"  );
     sprintf( conferences[4].conference->name, "Southwest"   );
     sprintf( conferences[5].conference->name, "Northwest"   );
     sprintf( conferences[6].conference->name, "Midwest"     );
     sprintf( conferences[7].conference->name, "South"       );

     conferences[8] = organization_conference_sentinel;

     return conferences;
}

organization_s *convertOrganization(
     const tsbrom_s         *rom1,
     const nst_save_state_s *save_state1,
     const tsbrom_s         *rom2,
     const nst_save_state_s *save_state2,
     const int               season,
     const bowl_game_e       bowl )
{
     organization_s *organization = NULL;

     if ( rom1        == NULL ||
          rom2        == NULL ||
          save_state1 == NULL ||
          save_state1 == NULL    )
     {
          sprintf( error_message, "Must supply two TSB Roms and two Save States" );

          return NULL;
     }

     if ( (organization = malloc( sizeof(organization_s) )) == NULL )
     {
          sprintf( error_message, "Cannot allocate memory for organization" );

          return NULL;
     }

     memset( organization, '\0', sizeof(organization_s) );

     /**/     organization->organization_id  =  1;
     sprintf( organization->name,               "National College Football Organization" );
     sprintf( organization->abbreviation,       "NCFO"                                   );
     /**/     organization->season           =  1;

     organization->conferences = assembleConferences();

     for ( int i = 0; i < 4; ++i )
     {
          organization->conferences[i].conference->teams = convertTeams( rom1, save_state1, season, bowl, i );
     }

     for ( int i = 4; i < 8; ++i )
     {
          organization->conferences[i].conference->teams = convertTeams( rom2, save_state2, season, bowl, i );
     }

     return organization;
}
