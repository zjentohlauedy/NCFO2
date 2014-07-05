#include <stdio.h>
#include <string.h>
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

     for ( int i = 0; i < 28; ++i )
     {
          for ( int j = 0; j < 30; ++j )
          {
               const player_s *player = players[(i * 30) + j];

               // always update the pointer
               int2pointer( 0x80ca + offset, &(rom->player_pointers[i][j]) );

               if ( player == NULL ) continue;

               rom->player_identifiers[offset++] = player->number;

               memcpy( rom->player_identifiers + offset, lowercase( player->first_name ), strlen(player->first_name) ); offset += strlen(player->first_name);
               memcpy( rom->player_identifiers + offset, uppercase( player->last_name  ), strlen(player->last_name)  ); offset += strlen(player->last_name);

               switch ( player->position )
               {
               case pos_Quarterback:
               {
                    tsb_qb_ratings_s qb = { 0 };
                    setQbRatings( &qb, player );
                    memcpy( ratings_ptr, &qb, sizeof(tsb_qb_ratings_s) );
                    ratings_ptr += sizeof(tsb_qb_ratings_s);
                    break;
               }
               case pos_Runningback:
               case pos_WideReceiver:
               case pos_TightEnd:
               {
                    tsb_off_ratings_s off = { 0 };
                    setOffRatings( &off, player );
                    memcpy( ratings_ptr, &off, sizeof(tsb_off_ratings_s) );
                    ratings_ptr += sizeof(tsb_off_ratings_s);
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
                    break;
               }
               case pos_Kicker:
               case pos_Punter:
               {
                    tsb_kick_ratings_s kick = { 0 };
                    setKickRatings( &kick, player );
                    memcpy( ratings_ptr, &kick, sizeof(tsb_kick_ratings_s) );
                    ratings_ptr += sizeof(tsb_kick_ratings_s);
                    break;
               }
               }
          }
     }
}

static boolean_e processPlayers( player_s **rom_players, const team_player_s *players, const int team_idx )
{
     if ( players == NULL ) return bl_True;

     int player_pos_counts[] = { 0, 2, 4, 4, 2, 5, 3, 4, 2, 2, 1, 1 }; // number of players needed by position
     int player_count        =   0;                                    // total number of players used

     for ( int i = 0; players[i].player != NULL; ++i )
     {
          player_s *player = players[i].player;

          if ( player_pos_counts[player->position] > 0 )
          {
               rom_players[(team_idx * 30) + player_count] = player;

               player_count++;
               player_pos_counts[player->position]--;
          }
     }

     // if player count != 30 it's an error
     if ( player_count != 30 )
     {
          sprintf( error_message, "%d/30 Players Qualified for Team %d\n", player_count, team_idx );

          return bl_False;
     }

     return bl_True;
}

static boolean_e processTeams( player_s **rom_players, const conference_team_s *teams )
{
     if ( teams == NULL ) return bl_True;

     for ( int i = 0; teams[i].team != NULL; ++i )
     {
          team_s *team = teams[i].team;

          if ( ! processPlayers( rom_players, team->players, i) ) return bl_False;
     }

     return bl_True;
}

void initializeRom( tsbrom_s *rom )
{
     memset( rom->player_identifiers,  0xff, sizeof(rom->player_identifiers)  );
     memset( rom->team_player_ratings, 0xff, sizeof(rom->team_player_ratings) );

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

          if ( ! processTeams( (i < 4) ? rom1_players : rom2_players, conference->teams ) ) return bl_False;
     }

     injectData( rom1, rom1_players );
     injectData( rom2, rom2_players );

     return bl_True;
}
