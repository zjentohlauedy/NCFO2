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

static void setQuarterbackSimData( unsigned char *sim_data, const player_s *player )
{
     if ( player->ratings                   == NULL ) return;
     if ( player->extra_ratings.quarterback == NULL ) return;

     player_quarterback_ratings_s *qb = player->extra_ratings.quarterback;

     int rush = (int)round( (double)((player->ratings->max_speed * 3) + getPlayerAcceleration( player )) / 4.0 );
     int pass = (int)round( (double)((qb->pass_control * 2) + qb->pass_speed + qb->pass_accuracy) / 4.0 );

     int pocket = 0;

     if ( rush > pass )
     {
          if ( pass > 0 ) pocket = 2 - (int)round( (double)rush / (double)pass );

          if ( pocket < 0 ) pocket = 0;
     }

     if ( pass >= rush )
     {
          if ( rush > 0 )
          {
               pocket = 1 + (int)round( (double)rush / (double)pass );
          }
          else
          {
               pocket = 3;
          }

          if ( pocket > 3 ) pocket = 3;
     }

     sim_data[0] = (rush<<4) + pass;
     sim_data[1] = pocket;
}

static void setOffenseSimData( unsigned char *sim_data, const player_s *player )
{
     if ( player->ratings               == NULL ) return;
     if ( player->extra_ratings.offense == NULL ) return;

     player_offense_ratings_s *offense = player->extra_ratings.offense;

     int rush   = 0;
     int recv   = 0;
     int yards  = 0;
     int target = 0;

     switch ( player->position )
     {
     case pos_Runningback:
          rush   = (int)round( (double)((player->ratings->max_speed * 2) + player->ratings->hit_power + getPlayerAcceleration( player )) / 4.0 );
          recv   = (int)round( (double)((offense->receiving * 3) + player->ratings->max_speed) / 4.0 );
          yards  = (int)round( (double)player->score / 20.0 );
          target = recv;
          break;

     case pos_WideReceiver:
          rush   = 1;
          recv   = (int)round( (double)((offense->receiving * 3) + player->ratings->max_speed) / 4.0 );
          yards  = (int)round( (double)player->score / 20.0 );
          target = recv;
          break;

     case pos_TightEnd:
          rush   = 1;
          recv   = (int)round( (double)((offense->receiving * 3) + player->ratings->max_speed) / 4.0 );
          yards  = (int)round( (double)player->score / 20.0 );
          target = recv;
          break;
     }

     sim_data[0] = (rush<<4) + recv;
     sim_data[1] = (yards<<4) + target;
}

static void setDefenseSimData( unsigned char *pass_rush, unsigned char *coverage, const player_s *player )
{
     if ( player->ratings               == NULL ) return;
     if ( player->extra_ratings.defense == NULL ) return;

     player_defense_ratings_s *defense = player->extra_ratings.defense;

     switch ( player->position )
     {
     case pos_DefensiveLine:
          *pass_rush = (player->ratings->hit_power * 2) + getPlayerAcceleration( player ) + defense->quickness;
          *coverage  = defense->interceptions;
          break;

     case pos_Linebacker:
          *pass_rush = (player->ratings->hit_power * 2) + getPlayerAcceleration( player ) + defense->quickness;
          *coverage  = player->ratings->max_speed + defense->interceptions;
          break;

     case pos_Cornerback:
          *pass_rush = 1;
          *coverage  = (defense->interceptions * 2) + player->ratings->max_speed + defense->quickness;
          break;

     case pos_Safety:
          *pass_rush = 1;
          *coverage  = (defense->interceptions * 2) + player->ratings->max_speed + defense->quickness;
          break;
     }
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

static void injectData( tsbrom_s *rom, player_s **players )
{
     unsigned char *ratings_ptr = (unsigned char *)rom->team_player_ratings;
     size_t         offset      = 0;
     char           fname[20 + 1] = { 0 };
     char           lname[20 + 1] = { 0 };

     for ( int i = 0; i < 28; ++i )
     {
          struct { int index; int score; } kick_returner = { 0 };
          struct { int index; int score; } punt_returner = { 0 };

          int team_sim_offense = 0;
          int team_sim_defense = 0;

          for ( int j = 0; j < 30; ++j )
          {
               const player_s *player = players[(i * 30) + j];

               // always update the pointer
               int2pointer( 0x86ca + offset, &(rom->player_pointers[i][j]) );

               if ( player == NULL ) continue;

               rom->player_identifiers[offset++] = number2hex( player->number );

               strcpy( fname, player->first_name );
               strcpy( lname, player->last_name  );

               while ( (strlen(fname) + strlen(lname)) > 16 )
               {
                    if ( strlen(fname) > 2 )
                    {
                         // Change fname to be first initial and a "."
                         fname[1] = '.';
                         fname[2] = '\0';
                    }
                    else
                    {
                         lname[16 - strlen(fname)] = '\0';
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
               case 16:
               case 17:
                    team_sim_offense += player->score;
                    break;

               case 18: // dline
               case 19:
               case 20:
               case 21: // lbs
               case 22:
               case 23:
               case 24:
               case 25: // cb
               case 26:
               case 27: // sf
               case 28:
                    team_sim_defense += player->score;
                    break;
               }
          }

          team_sim_offense = (int)round( (double)team_sim_offense / (20.0 * 11.0) );
          team_sim_defense = (int)round( (double)team_sim_defense / (20.0 * 11.0) );

          rom->sim_data[i].team[0] = (team_sim_offense << 4) + team_sim_defense;

          rom->kick_and_punt_returners1[i] = (kick_returner.index << 4) + punt_returner.index;
          rom->kick_and_punt_returners2[i] = rom->kick_and_punt_returners1[i];
     }
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

static void calcPlayerScore( player_s *player, const int season )
{
     if ( player->ratings == NULL ) return;

     applyMaturityAdjustment( player, season );

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

static void organizePlayers( team_player_s *players, const int season )
{
     int members = 0;

     for ( int i = 0; players[i].player != NULL; ++i )
     {
          player_s *player = players[i].player;

          calcPlayerScore( player, season );

          members++;
     }

     qsort( players, members, sizeof(team_player_s), orderByPositionAscendingAndScoreDescending );
}

static boolean_e processPlayers( player_s **rom_players, team_player_s *players, const int team_id, const int season )
{
     if ( players == NULL ) return bl_True;

     organizePlayers( players, season );

     int player_pos_counts[] = { 0, 2, 4, 4, 2, 5, 3, 4, 2, 2, 1, 1 }; // number of players needed by position
     int player_count        =   0;                                    // total number of players used
     int start_idx           =  (team_id - 1) % 24;

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

static boolean_e processTeams( player_s **rom_players, const conference_team_s *teams, const int season )
{
     if ( teams == NULL ) return bl_True;

     for ( int i = 0; teams[i].team != NULL; ++i )
     {
          team_s *team = teams[i].team;

          if ( ! processPlayers( rom_players, team->players, team->team_id, season ) ) return bl_False;
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
     player_s *rom1_players[28 * 30] = { 0 };
     player_s *rom2_players[28 * 30] = { 0 };

     clearErrorMessage();

     initializeRom( rom1 );
     initializeRom( rom2 );

     if ( organization == NULL ) return bl_True;

     if ( organization->conferences == NULL ) return bl_True;

     for ( int i = 0; organization->conferences[i].conference != NULL; ++i )
     {
          conference_s *conference = organization->conferences[i].conference;

          if ( ! processTeams( (i < 4) ? rom1_players : rom2_players, conference->teams, organization->season ) ) return bl_False;
     }

     injectData( rom1, rom1_players );
     injectData( rom2, rom2_players );

     return bl_True;
}
