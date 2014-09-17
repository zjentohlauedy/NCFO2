#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "file_formats.h"


static char error_message[999 + 1] = { 0 };


static void clearErrorMessage( void )
{
     error_message[0] = '\0';
}

char *getPopulateRomsError( void )
{
     return error_message;
}


static int getPlayerAcceleration( const player_s *player )
{
     if ( player->ratings == NULL ) return 0;

     return (player->ratings->run_speed > player->ratings->rush_power) ? player->ratings->run_speed : player->ratings->rush_power;
}

static int getMaturityAdjustment( const int class, const int maturity )
{
     if      ( class <= 0 ) // freshman
     {
          switch ( maturity )
          {
          case 4:  return 0;
          case 3:  return 1;
          case 2:  return 2;
          case 1:  return 3;
          }
     }
     else if ( class == 1 ) // sophomore
     {
          switch ( maturity )
          {
          case 4:  return 0;
          case 3:  return 1;
          case 2:  return 1;
          case 1:  return 2;
          }
     }
     else if ( class == 2 ) // junior
     {
          switch ( maturity )
          {
          case 4:  return 0;
          case 3:  return 0;
          case 2:  return 1;
          case 1:  return 1;
          }
     }
     else if ( class >= 3 ) // senior
     {
          return 0;
     }

     return 0;
}

static void applyMaturityAdjustment( player_s *player, const int season )
{
     int adjustment = getMaturityAdjustment( season - player->freshman_season, player->maturity );

     if ( player->ratings != NULL )
     {
          if ( (player->ratings->run_speed  -= adjustment) < 0 ) player->ratings->run_speed  = 0;
          if ( (player->ratings->rush_power -= adjustment) < 0 ) player->ratings->rush_power = 0;
          if ( (player->ratings->max_speed  -= adjustment) < 0 ) player->ratings->max_speed  = 0;
          if ( (player->ratings->hit_power  -= adjustment) < 0 ) player->ratings->hit_power  = 0;
     }

     switch ( player->position )
     {
     case pos_Quarterback:
          if ( player->extra_ratings.quarterback == NULL ) break;

          if ( (player->extra_ratings.quarterback->pass_speed       -= adjustment) < 0 ) player->extra_ratings.quarterback->pass_speed       = 0;
          if ( (player->extra_ratings.quarterback->pass_control     -= adjustment) < 0 ) player->extra_ratings.quarterback->pass_control     = 0;
          if ( (player->extra_ratings.quarterback->pass_accuracy    -= adjustment) < 0 ) player->extra_ratings.quarterback->pass_accuracy    = 0;
          if ( (player->extra_ratings.quarterback->avoid_pass_block -= adjustment) < 0 ) player->extra_ratings.quarterback->avoid_pass_block = 0;

          break;
     case pos_Runningback:
     case pos_WideReceiver:
     case pos_TightEnd:
          if ( player->extra_ratings.offense == NULL ) break;

          if ( (player->extra_ratings.offense->ball_control -= adjustment) < 0 ) player->extra_ratings.offense->ball_control = 0;
          if ( (player->extra_ratings.offense->receiving    -= adjustment) < 0 ) player->extra_ratings.offense->receiving    = 0;

          break;
     case pos_DefensiveLine:
     case pos_Linebacker:
     case pos_Cornerback:
     case pos_Safety:
          if ( player->extra_ratings.defense == NULL ) break;

          if ( (player->extra_ratings.defense->interceptions -= adjustment) < 0 ) player->extra_ratings.defense->interceptions = 0;
          if ( (player->extra_ratings.defense->quickness     -= adjustment) < 0 ) player->extra_ratings.defense->quickness     = 0;

          break;
     case pos_Kicker:
     case pos_Punter:
          if ( player->extra_ratings.kicking == NULL ) break;

          if ( (player->extra_ratings.kicking->kicking_ability  -= adjustment) < 0 ) player->extra_ratings.kicking->kicking_ability  = 0;
          if ( (player->extra_ratings.kicking->avoid_kick_block -= adjustment) < 0 ) player->extra_ratings.kicking->avoid_kick_block = 0;

          break;
     }
}

static void calcPlayerScore( player_s *player )
{
     if ( player->ratings == NULL ) return;

     int acceleration = getPlayerAcceleration( player );

     switch ( player->position )
     {
     case pos_Quarterback:
          if ( player->extra_ratings.quarterback == NULL ) break;

          player->score  = player->extra_ratings.quarterback->pass_control     * 8;
          player->score += player->      ratings->            max_speed        * 5;
          player->score += player->extra_ratings.quarterback->pass_speed       * 3;
          player->score += player->      ratings->            hit_power;
          player->score += player->extra_ratings.quarterback->pass_accuracy;
          player->score += player->extra_ratings.quarterback->avoid_pass_block;
          player->score +=                                    acceleration;

          break;
     case pos_Runningback:
          if ( player->extra_ratings.offense == NULL ) break;

          player->score  = player->      ratings->        max_speed    * 8;
          player->score += player->      ratings->        hit_power    * 5;
          player->score += player->extra_ratings.offense->receiving    * 3;
          player->score += player->extra_ratings.offense->ball_control * 2;
          player->score +=                                acceleration * 2;

          break;
     case pos_WideReceiver:
          if ( player->extra_ratings.offense == NULL ) break;

          player->score  = player->extra_ratings.offense->receiving    * 8;
          player->score += player->      ratings->        max_speed    * 6;
          player->score += player->extra_ratings.offense->ball_control * 2;
          player->score += player->      ratings->        hit_power    * 2;
          player->score +=                                acceleration * 2;

          break;
     case pos_TightEnd:
          if ( player->extra_ratings.offense == NULL ) break;

          player->score  = player->      ratings->        hit_power    * 7;
          player->score += player->extra_ratings.offense->receiving    * 7;
          player->score += player->      ratings->        max_speed    * 3;
          player->score += player->extra_ratings.offense->ball_control * 2;
          player->score +=                                acceleration;

          break;
     case pos_OffensiveLine:
          player->score  = player-> ratings-> hit_power    * 13;
          player->score += player-> ratings-> max_speed    *  5;
          player->score +=                    acceleration *  2;

          break;
     case pos_DefensiveLine:
          if ( player->extra_ratings.defense == NULL ) break;

          player->score  = player->      ratings->        hit_power     * 11;
          player->score += player->      ratings->        max_speed     *  5;
          player->score += player->extra_ratings.defense->interceptions;
          player->score += player->extra_ratings.defense->quickness;
          player->score +=                                acceleration  *  2;

          break;
     case pos_Linebacker:
          if ( player->extra_ratings.defense == NULL ) break;

          player->score  = player->      ratings->        hit_power     * 8;
          player->score += player->      ratings->        max_speed     * 5;
          player->score += player->extra_ratings.defense->interceptions * 3;
          player->score += player->extra_ratings.defense->quickness;
          player->score +=                                acceleration  *  2;

          break;
     case pos_Cornerback:
          if ( player->extra_ratings.defense == NULL ) break;

          player->score  = player->      ratings->        max_speed     * 8;
          player->score += player->extra_ratings.defense->interceptions * 5;
          player->score += player->      ratings->        hit_power     * 2;
          player->score += player->extra_ratings.defense->quickness;
          player->score +=                                acceleration  * 3;

          break;
     case pos_Safety:
          if ( player->extra_ratings.defense == NULL ) break;

          player->score  = player->      ratings->        max_speed     * 7;
          player->score += player->extra_ratings.defense->interceptions * 6;
          player->score += player->      ratings->        hit_power     * 3;
          player->score += player->extra_ratings.defense->quickness;
          player->score +=                                acceleration  * 2;

          break;
     case pos_Kicker:
     case pos_Punter:
          if ( player->extra_ratings.kicking == NULL ) break;

          player->score  = player->extra_ratings.kicking->kicking_ability  * 15;
          player->score += player->      ratings->        max_speed        *  3;
          player->score += player->extra_ratings.kicking->avoid_kick_block;
          player->score += player->      ratings->        hit_power;

          break;
     default:
          player->score = 0;
     }
}

static int calcPassingScore( const player_s *player )
{
     if ( player->extra_ratings.quarterback == NULL ) return 0;

     int passing = 0;

     passing += player->extra_ratings.quarterback->pass_control * 5;
     passing += player->extra_ratings.quarterback->pass_speed   * 3;
     passing += player->extra_ratings.quarterback->pass_accuracy;
     passing += player->extra_ratings.quarterback->avoid_pass_block;

     return passing;
}

static int calcRushingScore( const player_s *player )
{
     if ( player->ratings == NULL ) return 0;
     if ( player->position != pos_Quarterback  &&  player->extra_ratings.offense == NULL ) return 0;

     int rushing = 0;

     rushing += player->ratings->max_speed * 4;
     rushing += player->ratings->hit_power * 3;
     rushing += getPlayerAcceleration( player ) * 2;

     if   ( player->position == pos_Quarterback ) rushing += 6;
     else                                         rushing += player->extra_ratings.offense->ball_control;

     return rushing;
}

static int calcReceivingScore( const player_s *player )
{
     if ( player->ratings               == NULL ) return 0;
     if ( player->extra_ratings.offense == NULL ) return 0;

     int receiving = 0;

     receiving += player->extra_ratings.offense->receiving * 5;
     receiving += player->ratings->max_speed * 3;
     receiving += getPlayerAcceleration( player ) * 2;

     return receiving;
}

static int calcPassRushScore( const player_s *player )
{
     if ( player->ratings               == NULL ) return 0;
     if ( player->extra_ratings.defense == NULL ) return 0;

     int passrush = 0;

     passrush += player->ratings->hit_power * 6;
     passrush += player->ratings->max_speed * 2;
     passrush += getPlayerAcceleration( player );
     passrush += player->extra_ratings.defense->quickness;

     return passrush;
}

static int calcCoverageScore( const player_s *player )
{
     if ( player->ratings               == NULL ) return 0;
     if ( player->extra_ratings.defense == NULL ) return 0;

     int coverage = 0;

     coverage += player->extra_ratings.defense->interceptions * 5;
     coverage += player->ratings->max_speed * 3;
     coverage += getPlayerAcceleration( player );
     coverage += player->extra_ratings.defense->quickness;

     return coverage;
}

static void calcPlayerScores( player_s *player )
{
     calcPlayerScore( player );

     switch ( player->position )
     {
     case pos_Quarterback:
          player->passing_score = calcPassingScore( player );
          player->rushing_score = calcRushingScore( player );
          break;

     case pos_Runningback:
     case pos_WideReceiver:
     case pos_TightEnd:
          player->rushing_score   = calcRushingScore(   player );
          player->receiving_score = calcReceivingScore( player );
          break;

     case pos_DefensiveLine:
     case pos_Linebacker:
     case pos_Cornerback:
     case pos_Safety:
          player->passrush_score = calcPassRushScore( player );
          player->coverage_score = calcCoverageScore( player );
          break;
     }
}

static void setQuarterbackSimData( unsigned char *sim_data, const player_s *player )
{
     if ( player->ratings                   == NULL ) return;
     if ( player->extra_ratings.quarterback == NULL ) return;

     int sim_rush   = 0;
     int sim_pass   = 0;
     int sim_pocket = 0;

     if      ( player->passing_score < 50 ) { sim_pass = (rand() % 4) +  4; }
     else if ( player->passing_score < 70 ) { sim_pass = (rand() % 4) +  6; }
     else if ( player->passing_score < 80 ) { sim_pass = (rand() % 4) +  8; }
     else                                   { sim_pass = (rand() % 4) + 10; }

     if      ( player->rushing_score < 50 ) { sim_rush = (rand() % 4) +  2; sim_pocket = 3; }
     else if ( player->rushing_score < 80 ) { sim_rush = (rand() % 4) +  4; sim_pocket = 2; }
     else if ( player->rushing_score < 90 ) { sim_rush = (rand() % 4) +  6; sim_pocket = 1; }
     else                                   { sim_rush = (rand() % 4) +  8; sim_pocket = 0; }

     sim_data[0] = (sim_rush << 4) + sim_pass;
     sim_data[1] = sim_pocket;
}

static void setOffenseSimData( unsigned char *sim_data, const player_s *player )
{
     if ( player->ratings               == NULL ) return;
     if ( player->extra_ratings.offense == NULL ) return;

     int sim_rush   = 0;
     int sim_recv   = 0;
     int sim_yards  = 0;
     int sim_target = 0;

     switch ( player->position )
     {
     case pos_Runningback:
          if      ( player->rushing_score < 50 ) { sim_rush = (rand() % 4) +  4; }
          else if ( player->rushing_score < 80 ) { sim_rush = (rand() % 4) +  6; }
          else if ( player->rushing_score < 90 ) { sim_rush = (rand() % 4) +  8; }
          else                                   { sim_rush = (rand() % 4) + 10; }

          if      ( player->receiving_score <  50 ) { sim_recv = (rand() % 4) + 2; sim_target = 2; }
          else if ( player->receiving_score <  85 ) { sim_recv = (rand() % 4) + 4; sim_target = 4; }
          else if ( player->receiving_score < 100 ) { sim_recv = (rand() % 4) + 6; sim_target = 6; }
          else                                      { sim_recv = (rand() % 4) + 8; sim_target = 8; }

          if      ( player->score < 115 ) { sim_yards = (rand() % 4) +  4; }
          else if ( player->score < 150 ) { sim_yards = (rand() % 4) +  6; }
          else if ( player->score < 170 ) { sim_yards = (rand() % 4) +  8; }
          else                            { sim_yards = (rand() % 4) + 10; }

          break;

     case pos_WideReceiver:
          if      ( player->rushing_score < 50 ) { sim_rush = (rand() % 4) + 2; }
          else if ( player->rushing_score < 80 ) { sim_rush = (rand() % 4) + 4; }
          else if ( player->rushing_score < 90 ) { sim_rush = (rand() % 4) + 6; }
          else                                   { sim_rush = (rand() % 4) + 8; }

          if      ( player->receiving_score <  50 ) { sim_recv = (rand() % 4) +  4; sim_target =  8; }
          else if ( player->receiving_score <  85 ) { sim_recv = (rand() % 4) +  6; sim_target = 10; }
          else if ( player->receiving_score < 100 ) { sim_recv = (rand() % 4) +  8; sim_target = 12; }
          else                                      { sim_recv = (rand() % 4) + 10; sim_target = 14; }

          if      ( player->score < 130 ) { sim_yards = (rand() % 4) +  4; }
          else if ( player->score < 180 ) { sim_yards = (rand() % 4) +  6; }
          else if ( player->score < 190 ) { sim_yards = (rand() % 4) +  8; }
          else                            { sim_yards = (rand() % 4) + 10; }

          break;

     case pos_TightEnd:
          if      ( player->rushing_score < 50 ) { sim_rush = (rand() % 4) + 2; }
          else if ( player->rushing_score < 80 ) { sim_rush = (rand() % 4) + 4; }
          else if ( player->rushing_score < 90 ) { sim_rush = (rand() % 4) + 6; }
          else                                   { sim_rush = (rand() % 4) + 8; }

          if      ( player->receiving_score <  50 ) { sim_recv = (rand() % 4) + 2; sim_target = 1; }
          else if ( player->receiving_score <  85 ) { sim_recv = (rand() % 4) + 4; sim_target = 2; }
          else if ( player->receiving_score < 100 ) { sim_recv = (rand() % 4) + 6; sim_target = 3; }
          else                                      { sim_recv = (rand() % 4) + 8; sim_target = 4; }

          if      ( player->score < 110 ) { sim_yards = (rand() % 4) +  4; }
          else if ( player->score < 150 ) { sim_yards = (rand() % 4) +  6; }
          else if ( player->score < 170 ) { sim_yards = (rand() % 4) +  8; }
          else                            { sim_yards = (rand() % 4) + 10; }

          break;
     }

     sim_data[0] = (sim_rush  << 4) + sim_recv;
     sim_data[1] = (sim_yards << 4) + sim_target;
}

static void setDefenseSimData( unsigned char *passrush_out, unsigned char *coverage_out, const player_s *player )
{
     if ( player->ratings               == NULL ) return;
     if ( player->extra_ratings.defense == NULL ) return;

     unsigned char sim_passrush = 0;
     unsigned char sim_coverage = 0;

     switch ( player->position )
     {
     case pos_DefensiveLine:
          if      ( player->passrush_score < 50 ) { sim_passrush = (rand() % 15) + 15; }
          else if ( player->passrush_score < 70 ) { sim_passrush = (rand() % 15) + 35; }
          else if ( player->passrush_score < 80 ) { sim_passrush = (rand() % 15) + 55; }
          else                                    { sim_passrush = (rand() % 15) + 75; }

          if      ( player->coverage_score < 50 ) { sim_coverage = 1; }
          else if ( player->coverage_score < 80 ) { sim_coverage = 2; }
          else if ( player->coverage_score < 90 ) { sim_coverage = 3; }
          else                                    { sim_coverage = 4; }

          break;

     case pos_Linebacker:
          if      ( player->passrush_score < 50 ) { sim_passrush = (rand() % 10) +  8; }
          else if ( player->passrush_score < 70 ) { sim_passrush = (rand() % 10) + 23; }
          else if ( player->passrush_score < 80 ) { sim_passrush = (rand() % 10) + 38; }
          else                                    { sim_passrush = (rand() % 10) + 53; }

          if      ( player->coverage_score < 50 ) { sim_coverage = (rand() % 8) +  4; }
          else if ( player->coverage_score < 80 ) { sim_coverage = (rand() % 8) + 12; }
          else if ( player->coverage_score < 90 ) { sim_coverage = (rand() % 8) + 20; }
          else                                    { sim_coverage = (rand() % 8) + 28; }

          break;

     case pos_Cornerback:
          if      ( player->passrush_score < 50 ) { sim_passrush = 1; }
          else if ( player->passrush_score < 70 ) { sim_passrush = 2; }
          else if ( player->passrush_score < 80 ) { sim_passrush = 3; }
          else                                    { sim_passrush = 4; }

          if      ( player->coverage_score < 50 ) { sim_coverage = (rand() % 16) +  8; }
          else if ( player->coverage_score < 80 ) { sim_coverage = (rand() % 16) + 32; }
          else if ( player->coverage_score < 90 ) { sim_coverage = (rand() % 16) + 48; }
          else                                    { sim_coverage = (rand() % 16) + 80; }

          break;

     case pos_Safety:
          if      ( player->passrush_score < 50 ) { sim_passrush = 1; }
          else if ( player->passrush_score < 70 ) { sim_passrush = 2; }
          else if ( player->passrush_score < 80 ) { sim_passrush = 3; }
          else                                    { sim_passrush = 4; }

          if      ( player->coverage_score < 50 ) { sim_coverage = (rand() % 16) +  8; }
          else if ( player->coverage_score < 80 ) { sim_coverage = (rand() % 16) + 32; }
          else if ( player->coverage_score < 90 ) { sim_coverage = (rand() % 16) + 48; }
          else                                    { sim_coverage = (rand() % 16) + 80; }

          break;
     }

     *passrush_out = sim_passrush;
     *coverage_out = sim_coverage;
}

static int getTeamSimAdjustment( const player_s *player )
{
     switch ( player->position )
     {
     case pos_Quarterback:
          if      ( player->score < 100 ) return -1;
          else if ( player->score < 135 ) return  0;
          else if ( player->score < 150 ) return  1;
          else                            return  2;
          break;

     case pos_Runningback:
          if      ( player->score < 115 ) return -1;
          else if ( player->score < 150 ) return  0;
          else if ( player->score < 170 ) return  1;
          else                            return  2;
          break;

     case pos_WideReceiver:
          if      ( player->score < 130 ) return -1;
          else if ( player->score < 180 ) return  0;
          else if ( player->score < 190 ) return  1;
          else                            return  2;
          break;

     case pos_TightEnd:
          if      ( player->score < 110 ) return -1;
          else if ( player->score < 150 ) return  0;
          else if ( player->score < 170 ) return  1;
          else                            return  2;
          break;

     case pos_OffensiveLine:
          if      ( player->score < 110 ) return -1;
          else if ( player->score < 175 ) return  0;
          else if ( player->score < 190 ) return  1;
          else                            return  2;
          break;

     case pos_DefensiveLine:
          if      ( player->score < 110 ) return -1;
          else if ( player->score < 165 ) return  0;
          else if ( player->score < 180 ) return  1;
          else                            return  2;
          break;

     case pos_Linebacker:
          if      ( player->score < 110 ) return -1;
          else if ( player->score < 150 ) return  0;
          else if ( player->score < 170 ) return  1;
          else                            return  2;
          break;

     case pos_Cornerback:
          if      ( player->score < 130 ) return -1;
          else if ( player->score < 170 ) return  0;
          else if ( player->score < 185 ) return  1;
          else                            return  2;
          break;

     case pos_Safety:
          if      ( player->score < 130 ) return -1;
          else if ( player->score < 170 ) return  0;
          else if ( player->score < 180 ) return  1;
          else                            return  2;
          break;
     }

     return 0;
}

static void setKickingSimData( unsigned char *sim_data, const player_s *player )
{
     if ( player->ratings               == NULL ) return;
     if ( player->extra_ratings.kicking == NULL ) return;

     player_kicking_ratings_s *kicking = player->extra_ratings.kicking;

     switch ( player->position )
     {
     case pos_Kicker:
          *sim_data |= (kicking->kicking_ability<<4);
          break;

     case pos_Punter:
          *sim_data |= kicking->kicking_ability;
          break;
     }
}

static void setPlayerRatings( tsb_player_ratings_s *player_ratings, const player_s *player )
{
     player_ratings->face[0] = player->face;

     if ( player->ratings == NULL ) return;

     player_ratings->ratings[0] = (player->ratings->rush_power << 4) + player->ratings->run_speed;
     player_ratings->ratings[1] = (player->ratings->max_speed  << 4) + player->ratings->hit_power;
}

static void setQbRatings( tsb_qb_ratings_s *qb_ratings, const player_s *player )
{
     setPlayerRatings( &(qb_ratings->player), player );

     if ( player->extra_ratings.quarterback == NULL ) return;

     qb_ratings->qb_ratings[0] = (player->extra_ratings.quarterback->pass_speed    << 4) + player->extra_ratings.quarterback->pass_control;
     qb_ratings->qb_ratings[1] = (player->extra_ratings.quarterback->pass_accuracy << 4) + player->extra_ratings.quarterback->avoid_pass_block;
}

static void setOffRatings( tsb_off_ratings_s *off_ratings, const player_s *player )
{
     setPlayerRatings( &(off_ratings->player), player );

     if ( player->extra_ratings.offense == NULL ) return;

     off_ratings->off_ratings[0] = (player->extra_ratings.offense->ball_control << 4) + player->extra_ratings.offense->receiving;
}

static void setDefRatings( tsb_def_ratings_s *def_ratings, const player_s *player )
{
     setPlayerRatings( &(def_ratings->player), player );

     if ( player->extra_ratings.defense == NULL ) return;

     def_ratings->def_ratings[0] = (player->extra_ratings.defense->interceptions << 4) + player->extra_ratings.defense->quickness;
}

static void setKickRatings( tsb_kick_ratings_s *kick_ratings, const player_s *player )
{
     setPlayerRatings( &(kick_ratings->player), player );

     if ( player->extra_ratings.kicking == NULL ) return;

     kick_ratings->kick_ratings[0] = (player->extra_ratings.kicking->kicking_ability << 4) + player->extra_ratings.kicking->avoid_kick_block;
}

static void injectData( tsbrom_s *rom, team_s **teams, player_s **players )
{
     unsigned char *ratings_ptr = (unsigned char *)rom->team_player_ratings;
     size_t         offset      = 0;
     char           fname[20 + 1] = { 0 };
     char           lname[20 + 1] = { 0 };

     for ( int i = 0; i < 28; ++i )
     {
          struct { int index; int score; } kick_returner = { 0 };
          struct { int index; int score; } punt_returner = { 0 };

          int team_sim_offense = 8;
          int team_sim_defense = 8;

          if ( teams[i] != NULL  &&  teams[i]->offensive_formation != form_Pro_Set )
          {
               int start_idx = i * 30;

               // Move WR3 to RB2 slot, then shift running backs down: rb2->rb3->rb4->wr3
               player_s *player = players[start_idx + 8];

               players[start_idx + 8] = players[start_idx + 5];
               players[start_idx + 5] = players[start_idx + 4];
               players[start_idx + 4] = players[start_idx + 3];
               players[start_idx + 3] = player;

               if ( teams[i]->offensive_formation == form_Four_Wide )
               {
                    // Switch WR4 and TE1 slots
                    player = players[start_idx + 9];

                    players[start_idx +  9] = players[start_idx + 10];
                    players[start_idx + 10] = player;
               }
          }

          for ( int j = 0; j < 30; ++j )
          {
               const player_s *player = players[(i * 30) + j];

               // always update the pointer
               int2pointer( 0x86ca + offset, &(rom->player_pointers[i][j]) );

               if ( player == NULL ) continue;

               rom->player_identifiers[offset++] = number2hex( player->number );

               strcpy( fname, player->first_name );
               strcpy( lname, player->last_name  );

               while ( (strlen(fname) + strlen(lname)) > 15 )
               {
                    if ( strlen(fname) > 2 )
                    {
                         // Change fname to be first initial and a "."
                         fname[1] = '.';
                         fname[2] = '\0';
                    }
                    else
                    {
                         lname[15 - strlen(fname)] = '\0';
                    }
               }

               memcpy( rom->player_identifiers + offset, lowercase( fname ), strlen(fname) ); offset += strlen(fname);
               memcpy( rom->player_identifiers + offset, uppercase( lname ), strlen(lname) ); offset += strlen(lname);

               switch ( player->position )
               {
               case pos_Quarterback:
               {
                    tsb_qb_ratings_s qb = { 0 };
                    setQbRatings( &qb, player );
                    memcpy( ratings_ptr, &qb, sizeof(tsb_qb_ratings_s) );
                    ratings_ptr += sizeof(tsb_qb_ratings_s);
                    setQuarterbackSimData( rom->sim_data[i].quarterbacks[j], player );
                    break;
               }
               case pos_Runningback:
               case pos_WideReceiver:
                    if ( player->ratings != NULL  &&  player->extra_ratings.offense != NULL )
                    {
                         int kr_score  = (player->ratings->max_speed * 3) + player->extra_ratings.offense->ball_control;
                         int pr_score  = (player->ratings->max_speed * 2) + player->extra_ratings.offense->ball_control;
                         /**/pr_score += getPlayerAcceleration( player );

                         if ( kr_score > kick_returner.score ) { kick_returner.index = j; kick_returner.score = kr_score; }
                         if ( pr_score > punt_returner.score ) { punt_returner.index = j; punt_returner.score = pr_score; }
                    }
                    // Intentional Fallthrough...
               case pos_TightEnd:
               {
                    tsb_off_ratings_s off = { 0 };
                    setOffRatings( &off, player );
                    memcpy( ratings_ptr, &off, sizeof(tsb_off_ratings_s) );
                    ratings_ptr += sizeof(tsb_off_ratings_s);
                    setOffenseSimData( rom->sim_data[i].offense[j - 2], player );
                    break;
               }
               case pos_OffensiveLine:
               {
                    tsb_player_ratings_s p = { 0 };
                    setPlayerRatings( &p, player );
                    memcpy( ratings_ptr, &p, sizeof(tsb_player_ratings_s) );
                    ratings_ptr += sizeof(tsb_player_ratings_s);
                    break;
               }
               case pos_DefensiveLine:
               case pos_Linebacker:
               case pos_Cornerback:
               case pos_Safety:
               {
                    tsb_def_ratings_s def = { 0 };
                    setDefRatings( &def, player );
                    memcpy( ratings_ptr, &def, sizeof(tsb_def_ratings_s) );
                    ratings_ptr += sizeof(tsb_def_ratings_s);
                    setDefenseSimData( &(rom->sim_data[i].defense_pass_rush[j - 17]), &(rom->sim_data[i].defense_coverage[j - 17]), player );
                    break;
               }
               case pos_Kicker:
               case pos_Punter:
               {
                    tsb_kick_ratings_s kick = { 0 };
                    setKickRatings( &kick, player );
                    memcpy( ratings_ptr, &kick, sizeof(tsb_kick_ratings_s) );
                    ratings_ptr += sizeof(tsb_kick_ratings_s);
                    setKickingSimData( rom->sim_data[i].kicking, player );
                    break;
               }
               }

               switch ( j )
               {
               case  0: // qb
               case  2: // rb1
               case  3: // rb2
               case  6: // wr1
               case  7: // wr2
               case 10: // te
               case 12: // oline
               case 13:
               case 14:
               case 15:
               case 16:
                    team_sim_offense += getTeamSimAdjustment( player );
                    break;

               case 17: // dline
               case 18:
               case 19:
               case 20: // lbs
               case 21:
               case 22:
               case 23:
               case 24: // cb
               case 25:
               case 26: // sf
               case 27:
                    team_sim_defense += getTeamSimAdjustment( player );
                    break;
               }
          }

          if ( team_sim_offense <  0 ) team_sim_offense = 0;
          if ( team_sim_defense <  0 ) team_sim_defense = 0;

          if ( team_sim_offense > 15 ) team_sim_offense = 15;
          if ( team_sim_defense > 15 ) team_sim_defense = 15;

          rom->sim_data[i].team[0] = (team_sim_offense << 4) + team_sim_defense;

          rom->kick_and_punt_returners1[i] = (kick_returner.index << 4) + punt_returner.index;
          rom->kick_and_punt_returners2[i] = rom->kick_and_punt_returners1[i];

          if ( teams[i] != NULL )
          {
               rom->formations1[i] = teams[i]->offensive_formation;
               rom->formations2[i] = teams[i]->offensive_formation;

               rom->offensive_preference[i] = teams[i]->offensive_preference;

               if ( teams[i]->offensive_formation == form_Pro_Set )
               {
                    if ( teams[i]->use_feature_back )
                    {
                         rom->default_playbooks[i].rushing[0] = 0x00;
                         rom->default_playbooks[i].rushing[1] = 0x05;

                         rom->default_playbooks[i].passing[0] = 0x60;
                         rom->default_playbooks[i].passing[1] = 0x24;
                    }
                    else
                    {
                         rom->default_playbooks[i].rushing[0] = 0x10;
                         rom->default_playbooks[i].rushing[1] = 0x24;

                         rom->default_playbooks[i].passing[0] = 0x64;
                         rom->default_playbooks[i].passing[1] = 0x66;
                    }
               }
               else if ( teams[i]->offensive_formation == form_Three_Wide )
               {
                    rom->default_playbooks[i].rushing[0] = 0x64;
                    rom->default_playbooks[i].rushing[1] = 0x03;

                    rom->default_playbooks[i].passing[0] = 0x37;
                    rom->default_playbooks[i].passing[1] = 0x40;
               }
               else if ( teams[i]->offensive_formation == form_Four_Wide )
               {
                    rom->default_playbooks[i].rushing[0] = 0x42;
                    rom->default_playbooks[i].rushing[1] = 0x02;

                    rom->default_playbooks[i].passing[0] = 0x13;
                    rom->default_playbooks[i].passing[1] = 0x25;
               }
          }
     }
}

static int orderByPositionAscendingAndScoreDescending( const void *a, const void *b )
{
     if ( a == NULL )
     {
          if ( b == NULL ) return 0;

          return 1;
     }

     if ( b == NULL ) return -1;

     const team_player_s *team_player_a = (team_player_s *)a;
     const team_player_s *team_player_b = (team_player_s *)b;

     const player_s *player_a = team_player_a->player;
     const player_s *player_b = team_player_b->player;

     if ( player_a == NULL )
     {
          if ( player_b == NULL ) return 0;

          return 1;
     }

     if ( player_b == NULL ) return -1;

     if ( player_a->position != player_b->position ) return player_a->position - player_b->position;

     if ( player_a->score != player_b->score ) return player_b->score - player_a->score;

     return 0;
}

static boolean_e selectPlayers( player_s **rom_players, team_player_s *players, const int team_id, const formation_e formation )
{
     if ( players == NULL ) return bl_True;

     int player_pos_counts[] = { 0, 2, 4, 4, 2, 5, 3, 4, 2, 2, 1, 1 }; // number of players needed by position
     int player_count        =   0;                                    // total number of players used
     int start_idx           =  (team_id - 1) % 24;

     if ( formation == form_Four_Wide )
     {
          player_pos_counts[3] = 6;
          player_pos_counts[4] = 0;
     }

     for ( int i = 0; players[i].player != NULL; ++i )
     {
          player_s *player = players[i].player;

          if ( player_pos_counts[player->position] > 0 )
          {
               rom_players[(start_idx * 30) + player_count] = player;

               player_count++;
               player_pos_counts[player->position]--;
          }
     }

     if ( player_count != 30 )
     {
          sprintf( error_message, "%d/30 Players Qualified for Team %d\n", player_count, team_id );

          return bl_False;
     }

     return bl_True;
}

static boolean_e checkForFeatureBack( team_player_s *players, const formation_e formation )
{
     if ( formation == form_Three_Wide  ||  formation == form_Four_Wide ) return bl_True;

     player_s *rb1 = players[2].player;
     player_s *rb2 = players[3].player;

     int rb1_rush_value = (rb1->rushing_score   < 50) ? 0 : (rb1->rushing_score   < 80) ? 1 : (rb1->rushing_score   <  90) ? 2 : 3;
     int rb2_rush_value = (rb2->rushing_score   < 50) ? 0 : (rb2->rushing_score   < 80) ? 1 : (rb2->rushing_score   <  90) ? 2 : 3;

     if ( rb1_rush_value > rb2_rush_value ) return bl_True;

     return bl_False;
}

static preference_e determinePreference( team_player_s *players, const formation_e formation )
{
     int balance;

     player_s *qb1 = players[0].player;
     player_s *rb1 = players[2].player;
     player_s *wr1 = players[6].player;
     player_s *wr2 = players[7].player;

     balance  = (qb1->passing_score   < 50) ?  0 : (qb1->passing_score   < 70) ? 2 : (qb1->passing_score   <  80) ?  3 :  3;
     balance += (qb1->rushing_score   < 50) ?  0 : (qb1->rushing_score   < 80) ? 0 : (qb1->rushing_score   <  90) ? -1 : -1;
     balance += (rb1->rushing_score   < 50) ?  1 : (rb1->rushing_score   < 80) ? 0 : (rb1->rushing_score   <  90) ? -1 : -2;
     balance += (rb1->receiving_score < 50) ? -1 : (rb1->receiving_score < 85) ? 0 : (rb1->receiving_score < 100) ?  1 :  1;
     balance += (wr1->receiving_score < 50) ? -1 : (wr1->receiving_score < 85) ? 0 : (wr1->receiving_score < 100) ?  0 :  1;
     balance += (wr2->receiving_score < 50) ? -1 : (wr2->receiving_score < 85) ? 0 : (wr2->receiving_score < 100) ?  0 :  1;

     if      ( formation == form_Pro_Set )
     {
          player_s *rb2 = players[ 3].player;
          player_s *te1 = players[12].player;

          balance += (rb2->rushing_score   < 50) ?  1 : (rb2->rushing_score   < 80) ? 0 : (rb2->rushing_score   <  90) ? -1 : -2;
          balance += (rb2->receiving_score < 50) ? -1 : (rb2->receiving_score < 85) ? 0 : (rb2->receiving_score < 100) ?  1 :  1;
          balance += (te1->receiving_score < 50) ?  0 : (te1->receiving_score < 85) ? 0 : (te1->receiving_score < 100) ?  0 :  1;
     }
     else if ( formation == form_Three_Wide )
     {
          player_s *wr3 = players[ 8].player;
          player_s *te1 = players[12].player;

          balance += (wr3->receiving_score < 50) ? -1 : (wr3->receiving_score < 85) ? 0 : (wr3->receiving_score < 100) ?  0 :  1;
          balance += (te1->receiving_score < 50) ?  0 : (te1->receiving_score < 85) ? 0 : (te1->receiving_score < 100) ?  0 :  1;
     }
     else if ( formation == form_Four_Wide )
     {
          player_s *wr3 = players[8].player;
          player_s *wr4 = players[9].player;

          balance += (wr3->receiving_score < 50) ? -1 : (wr3->receiving_score < 85) ? 0 : (wr3->receiving_score < 100) ?  0 :  1;
          balance += (wr4->receiving_score < 50) ? -1 : (wr4->receiving_score < 85) ? 0 : (wr4->receiving_score < 100) ?  0 :  1;
     }

     if ( balance < 0 ) balance = 0;
     if ( balance > 3 ) balance = 3;

     switch ( balance )
     {
     case 0: return pref_HeavyRush;
     case 1: return pref_BalancedRush;
     case 2: return pref_BalancedPass;
     case 3: return pref_HeavyPass;
     }

     return pref_BalancedPass;
}

static formation_e determineFormation( team_player_s *players )
{
     formation_e formation = form_Pro_Set;

     player_s *rb2 = players[ 3].player;
     player_s *wr3 = players[ 8].player;
     player_s *wr4 = players[ 9].player;
     player_s *te1 = players[12].player;

     int rb2_rush_value = (rb2->rushing_score   < 50) ? 0 : (rb2->rushing_score   < 80) ? 1 : (rb2->rushing_score   <  90) ? 2 : 3;
     int rb2_recv_value = (rb2->receiving_score < 50) ? 0 : (rb2->receiving_score < 85) ? 1 : (rb2->receiving_score < 100) ? 2 : 3;
     int wr3_value      = (wr3->receiving_score < 50) ? 0 : (wr3->receiving_score < 85) ? 1 : (wr3->receiving_score < 100) ? 2 : 3;

     int rb2_value = (rb2_rush_value < rb2_recv_value) ? rb2_recv_value : rb2_rush_value;

     if ( wr3_value > rb2_value ) formation = form_Three_Wide;

     if ( formation == form_Three_Wide )
     {
          int wr4_value = (wr4->score < 130) ? 0 : (wr4->score < 180) ? 1 : (wr4->score < 190) ? 2 : 3;
          int te1_value = (te1->score < 110) ? 0 : (te1->score < 150) ? 1 : (te1->score < 170) ? 2 : 3;

          if      ( wr4_value > te1_value ) formation = form_Four_Wide;
          else if ( te1_value > wr4_value ) formation = form_Three_Wide;
          else
          {
               wr4_value = (wr4->receiving_score < 50) ? 0 : (wr4->receiving_score < 85) ? 1 : (wr4->receiving_score < 100) ? 2 : 3;
               te1_value = (te1->receiving_score < 50) ? 0 : (te1->receiving_score < 85) ? 1 : (te1->receiving_score < 100) ? 2 : 3;

               if   ( wr4_value > te1_value ) formation = form_Four_Wide;
               else                           formation = form_Three_Wide;
          }
     }

     return formation;
}

static void processPlayers( team_player_s *players, const int season )
{
     if ( players == NULL ) return;

     int members = 0;

     for ( int i = 0; players[i].player != NULL; ++i )
     {
          player_s *player = players[i].player;

          applyMaturityAdjustment( player, season );

          calcPlayerScores( player );

          members++;
     }

     qsort( players, members, sizeof(team_player_s), orderByPositionAscendingAndScoreDescending );
}

static boolean_e processTeams( team_s **rom_teams, player_s **rom_players, const conference_team_s *teams, const int season )
{
     if ( teams == NULL ) return bl_True;

     for ( int i = 0; teams[i].team != NULL; ++i )
     {
          team_s *team = teams[i].team;

          int rom_team_idx = (team->team_id -1) % 24;

          rom_teams[rom_team_idx] = team;

          processPlayers( team->players, season );

          team->offensive_formation  = determineFormation(  team->players                            );
          team->offensive_preference = determinePreference( team->players, team->offensive_formation );
          team->use_feature_back     = checkForFeatureBack( team->players, team->offensive_formation );

          if ( ! selectPlayers( rom_players, team->players, team->team_id, team->offensive_formation ) ) return bl_False;
     }

     return bl_True;
}

void initializeRom( tsbrom_s *rom )
{
     memset( rom->player_identifiers,  0xff, sizeof(rom->player_identifiers)  );
     memset( rom->team_player_ratings, 0xff, sizeof(rom->team_player_ratings) );
     memset( rom->sim_data,            0x00, sizeof(rom->sim_data)            );

     for ( int i = 0; i < 28; ++i )
     {
          for ( int j = 0; j < 30; ++j )
          {
               int2pointer( 0x80ca, &(rom->player_pointers[i][j]) );
          }
     }
}

boolean_e populateRoms( tsbrom_s *rom1, tsbrom_s *rom2, const organization_s *organization )
{
     team_s   *rom1_teams  [28     ] = { 0 };
     team_s   *rom2_teams  [28     ] = { 0 };
     player_s *rom1_players[28 * 30] = { 0 };
     player_s *rom2_players[28 * 30] = { 0 };

     clearErrorMessage();

     initializeRom( rom1 );
     initializeRom( rom2 );

     if ( organization == NULL ) return bl_True;

     if ( organization->conferences == NULL ) return bl_True;

     for ( int i = 0; organization->conferences[i].conference != NULL; ++i )
     {
          conference_s *conference  = organization->conferences[i].conference;

          team_s   **rom_teams   = (i < 4) ? rom1_teams   : rom2_teams;
          player_s **rom_players = (i < 4) ? rom1_players : rom2_players;

          if ( ! processTeams( rom_teams, rom_players, conference->teams, organization->season ) ) return bl_False;
     }

     injectData( rom1, rom1_teams, rom1_players );
     injectData( rom2, rom2_teams, rom2_players );

     return bl_True;
}
