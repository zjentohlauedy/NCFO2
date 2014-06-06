#ifndef __INC_PLAYER_H__
#define __INC_PLAYER_H__

#include <sqlite3.h>
#include "data_list.h"
#include "bowls.h"

typedef enum
{
     pos_None          =  0,
     pos_Quarterback   =  1,
     pos_Runningback   =  2,
     pos_WideReceiver  =  3,
     pos_TightEnd      =  4,
     pos_OffensiveLine =  5,
     pos_DefensiveLine =  6,
     pos_Linebacker    =  7,
     pos_Cornerback    =  8,
     pos_Safety        =  9,
     pos_Kicker        = 10,
     pos_Punter        = 11

} position_e;

typedef struct
{
     int          player_id;
     int          season;
     bowl_game_e  bowl_game;
     int          sacks;
     int          interceptions;
     int          return_yards;
     int          return_touchdowns;

} player_defense_stats_s;

typedef struct
{
     int          player_id;
     int          season;
     bowl_game_e  bowl_game;
     int          pass_attempts;
     int          completions;
     int          interceptions;
     int          pass_yards;
     int          pass_touchdowns;
     int          rush_attempts;
     int          rush_yards;
     int          rush_touchdowns;
     int          receptions;
     int          receiving_yards;
     int          receiving_touchdowns;

} player_offense_stats_s;

typedef struct
{
     int         player_id;
     int         kicking_ability;
     int         avoid_kick_block;

} player_kicking_ratings_s;

typedef struct
{
     int         player_id;
     int         interceptions;
     int         quickness;

} player_defense_ratings_s;

typedef struct
{
     int         player_id;
     int         ball_control;
     int         receiving;

} player_offense_ratings_s;

typedef struct
{
     int         player_id;
     int         pass_speed;
     int         pass_control;
     int         pass_accuracy;
     int         avoid_pass_block;

} player_quarterback_ratings_s;

typedef struct
{
     int         player_id;
     int         run_speed;
     int         rush_power;
     int         max_speed;
     int         hit_power;

} player_ratings_s;

typedef struct
{
     int         player_id;
     char        first_name      [ 20 + 1 ];
     char        last_name       [ 20 + 1 ];
     int         face;
     position_e  position;
     int         number;
     int         freshman_season;
     int         maturity;

} player_s;


int players_t_create( sqlite3 *db, const player_s *player );
int players_t_read(   sqlite3 *db,       player_s *player );
int players_t_update( sqlite3 *db, const player_s *player );
int players_t_delete( sqlite3 *db, const player_s *player );

int player_ratings_t_create( sqlite3 *db, const player_ratings_s *player_ratings );
int player_ratings_t_read(   sqlite3 *db,       player_ratings_s *player_ratings );
int player_ratings_t_update( sqlite3 *db, const player_ratings_s *player_ratings );
int player_ratings_t_delete( sqlite3 *db, const player_ratings_s *player_ratings );

int player_quarterback_ratings_t_create( sqlite3 *db, const player_quarterback_ratings_s *player_quarterback_ratings );
int player_quarterback_ratings_t_read(   sqlite3 *db,       player_quarterback_ratings_s *player_quarterback_ratings );
int player_quarterback_ratings_t_update( sqlite3 *db, const player_quarterback_ratings_s *player_quarterback_ratings );
int player_quarterback_ratings_t_delete( sqlite3 *db, const player_quarterback_ratings_s *player_quarterback_ratings );

int player_offense_ratings_t_create( sqlite3 *db, const player_offense_ratings_s *player_offense_ratings );
int player_offense_ratings_t_read(   sqlite3 *db,       player_offense_ratings_s *player_offense_ratings );
int player_offense_ratings_t_update( sqlite3 *db, const player_offense_ratings_s *player_offense_ratings );
int player_offense_ratings_t_delete( sqlite3 *db, const player_offense_ratings_s *player_offense_ratings );

int player_defense_ratings_t_create( sqlite3 *db, const player_defense_ratings_s *player_defense_ratings );
int player_defense_ratings_t_read(   sqlite3 *db,       player_defense_ratings_s *player_defense_ratings );
int player_defense_ratings_t_update( sqlite3 *db, const player_defense_ratings_s *player_defense_ratings );
int player_defense_ratings_t_delete( sqlite3 *db, const player_defense_ratings_s *player_defense_ratings );

int player_kicking_ratings_t_create( sqlite3 *db, const player_kicking_ratings_s *player_kicking_ratings );
int player_kicking_ratings_t_read(   sqlite3 *db,       player_kicking_ratings_s *player_kicking_ratings );
int player_kicking_ratings_t_update( sqlite3 *db, const player_kicking_ratings_s *player_kicking_ratings );
int player_kicking_ratings_t_delete( sqlite3 *db, const player_kicking_ratings_s *player_kicking_ratings );

int player_offense_stats_t_create(         sqlite3 *db,                      const player_offense_stats_s *player_offense_stats );
int player_offense_stats_t_read(           sqlite3 *db,                            player_offense_stats_s *player_offense_stats );
int player_offense_stats_t_read_by_player( sqlite3 *db, const int player_id,       data_list_s            *player_offense_stats );
int player_offense_stats_t_update(         sqlite3 *db,                      const player_offense_stats_s *player_offense_stats );
int player_offense_stats_t_delete(         sqlite3 *db,                      const player_offense_stats_s *player_offense_stats );

int player_defense_stats_t_create(         sqlite3 *db,                      const player_defense_stats_s *player_defense_stats );
int player_defense_stats_t_read(           sqlite3 *db,                            player_defense_stats_s *player_defense_stats );
int player_defense_stats_t_read_by_player( sqlite3 *db, const int player_id,       data_list_s            *player_defense_stats );
int player_defense_stats_t_update(         sqlite3 *db,                      const player_defense_stats_s *player_defense_stats );
int player_defense_stats_t_delete(         sqlite3 *db,                      const player_defense_stats_s *player_defense_stats );

#endif
