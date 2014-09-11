#ifndef __INC_PLAYER_H__
#define __INC_PLAYER_H__

#include <sqlite3.h>
#include "bool.h"
#include "data_list.h"
#include "bowls.h"

#define PLAYER_ACCOLADE_SENTINEL      { -1, -1, pacc_None }
#define PLAYER_OFFENSE_STATS_SENTINEL { -1, -1, bg_None, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }
#define PLAYER_DEFENSE_STATS_SENTINEL { -1, -1, bg_None, -1, -1, -1, -1 }
#define PLAYER_KICKING_STATS_SENTINEL { -1, -1, bg_None, -1, -1, -1, -1, -1, -1 }
#define PLAYER_RETURNS_STATS_SENTINEL { -1, -1, bg_None, -1, -1, -1, -1, -1, -1 }

typedef enum
{
     pacc_None                                     =  0,
     pacc_Passing_Champion                         =  1,
     pacc_Atlantic_Passing_Champion                =  2,
     pacc_Great_Lake_Passing_Champion              =  3,
     pacc_Midwest_Passing_Champion                 =  4,
     pacc_New_England_Passing_Champion             =  5,
     pacc_Northwest_Passing_Champion               =  6,
     pacc_South_Passing_Champion                   =  7,
     pacc_Southeast_Passing_Champion               =  8,
     pacc_Southwest_Passing_Champion               =  9,
     pacc_Rushing_Champion                         = 10,
     pacc_Atlantic_Rushing_Champion                = 11,
     pacc_Great_Lake_Rushing_Champion              = 12,
     pacc_Midwest_Rushing_Champion                 = 13,
     pacc_New_England_Rushing_Champion             = 14,
     pacc_Northwest_Rushing_Champion               = 15,
     pacc_South_Rushing_Champion                   = 16,
     pacc_Southeast_Rushing_Champion               = 17,
     pacc_Southwest_Rushing_Champion               = 18,
     pacc_Receiving_Champion                       = 19,
     pacc_Atlantic_Receiving_Champion              = 20,
     pacc_Great_Lake_Receiving_Champion            = 21,
     pacc_Midwest_Receiving_Champion               = 22,
     pacc_New_England_Receiving_Champion           = 23,
     pacc_Northwest_Receiving_Champion             = 24,
     pacc_South_Receiving_Champion                 = 25,
     pacc_Southeast_Receiving_Champion             = 26,
     pacc_Southwest_Receiving_Champion             = 27,
     pacc_Offensive_Player_of_the_Year             = 28,
     pacc_Atlantic_Offensive_Player_of_the_Year    = 29,
     pacc_Great_Lake_Offensive_Player_of_the_Year  = 30,
     pacc_Midwest_Offensive_Player_of_the_Year     = 31,
     pacc_New_England_Offensive_Player_of_the_Year = 32,
     pacc_Northwest_Offensive_Player_of_the_Year   = 33,
     pacc_South_Offensive_Player_of_the_Year       = 34,
     pacc_Southeast_Offensive_Player_of_the_Year   = 35,
     pacc_Southwest_Offensive_Player_of_the_Year   = 36,
     pacc_Defensive_Player_of_the_Year             = 37,
     pacc_Atlantic_Defensive_Player_of_the_Year    = 38,
     pacc_Great_Lake_Defensive_Player_of_the_Year  = 39,
     pacc_Midwest_Defensive_Player_of_the_Year     = 40,
     pacc_New_England_Defensive_Player_of_the_Year = 41,
     pacc_Northwest_Defensive_Player_of_the_Year   = 42,
     pacc_South_Defensive_Player_of_the_Year       = 43,
     pacc_Southeast_Defensive_Player_of_the_Year   = 44,
     pacc_Southwest_Defensive_Player_of_the_Year   = 45,
     pacc_Best_Quarterback_Rating                  = 46,
     pacc_Best_Completion_Percent                  = 47,
     pacc_Most_Yards_Passing                       = 48,
     pacc_Most_Passing_Touchdowns                  = 49,
     pacc_Most_Yards_Rushing                       = 50,
     pacc_Most_Rushing_Touchdowns                  = 51,
     pacc_Most_Receptions                          = 52,
     pacc_Most_Yards_Receiving                     = 53,
     pacc_Most_Receiving_Touchdowns                = 54,
     pacc_Most_All_Purpose_Yards                   = 55,
     pacc_Most_All_Purpose_Touchdowns              = 56,
     pacc_Most_Sacks                               = 57,
     pacc_Most_Interceptions                       = 58,
     pacc_Most_Return_Yards                        = 59,
     pacc_Most_Return_Touchdowns                   = 60,
     pacc_Best_Field_Goal_Percentage               = 61,
     pacc_Most_Points_Kicked                       = 62,
     pacc_Highest_Yards_Per_Punt                   = 63,
     pacc_All_American                             = 64

} player_accolade_e;

typedef enum
{
     cnd_Bad       = 0,
     cnd_Average   = 1,
     cnd_Good      = 2,
     cnd_Excellent = 3

} condition_e;

typedef struct
{
     int                player_id;
     int                season;
     player_accolade_e  accolade;

} player_accolade_s;

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
     int          kick_returns;
     int          kick_return_yards;
     int          kick_return_touchdowns;
     int          punt_returns;
     int          punt_return_yards;
     int          punt_return_touchdowns;

} player_returns_stats_s;

typedef struct
{
     int          player_id;
     int          season;
     bowl_game_e  bowl_game;
     int          extra_point_attempts;
     int          extra_points_made;
     int          field_goal_attempts;
     int          field_goals_made;
     int          punts;
     int          punt_yards;

} player_kicking_stats_s;

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
     // Simulation data:
     int         sim_kicking;

} player_kicking_ratings_s;

typedef struct
{
     int         player_id;
     int         interceptions;
     int         quickness;
     // Simulation data:
     int         sim_pass_rush;
     int         sim_coverage;

} player_defense_ratings_s;

typedef struct
{
     int         player_id;
     int         ball_control;
     int         receiving;
     // Simulation data:
     int         sim_rush;
     int         sim_catch;
     int         sim_yards;
     int         sim_target;

} player_offense_ratings_s;

typedef struct
{
     int         player_id;
     int         pass_speed;
     int         pass_control;
     int         pass_accuracy;
     int         avoid_pass_block;
     // Simulation data:
     int         sim_rush;
     int         sim_pass;
     int         sim_pocket;

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
     player_offense_stats_s *offense;
     player_defense_stats_s *defense;
     player_kicking_stats_s *kicking;
     player_returns_stats_s *returns;

} player_stats_s;

typedef union
{
     player_quarterback_ratings_s *quarterback;
     player_offense_ratings_s     *offense;
     player_defense_ratings_s     *defense;
     player_kicking_ratings_s     *kicking;

} extra_player_ratings_u;

typedef struct
{
     int                     player_id;
     char                    first_name      [ 20 + 1 ];
     char                    last_name       [ 20 + 1 ];
     int                     face;
     position_e              position;
     int                     number;
     int                     freshman_season;
     int                     maturity;
     player_ratings_s       *ratings;
     extra_player_ratings_u  extra_ratings;
     player_accolade_s      *accolades;
     player_stats_s          stats;
     // internal use:
     int                     score;
     int                     passing_score;
     int                     rushing_score;
     int                     receiving_score;
     int                     passrush_score;
     int                     coverage_score;
     boolean_e               injured;
     condition_e             condition;

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

int player_kicking_stats_t_create(         sqlite3 *db,                      const player_kicking_stats_s *player_kicking_stats );
int player_kicking_stats_t_read(           sqlite3 *db,                            player_kicking_stats_s *player_kicking_stats );
int player_kicking_stats_t_read_by_player( sqlite3 *db, const int player_id,       data_list_s            *player_kicking_stats );
int player_kicking_stats_t_update(         sqlite3 *db,                      const player_kicking_stats_s *player_kicking_stats );
int player_kicking_stats_t_delete(         sqlite3 *db,                      const player_kicking_stats_s *player_kicking_stats );

int player_returns_stats_t_create(         sqlite3 *db,                      const player_returns_stats_s *player_returns_stats );
int player_returns_stats_t_read(           sqlite3 *db,                            player_returns_stats_s *player_returns_stats );
int player_returns_stats_t_read_by_player( sqlite3 *db, const int player_id,       data_list_s            *player_returns_stats );
int player_returns_stats_t_update(         sqlite3 *db,                      const player_returns_stats_s *player_returns_stats );
int player_returns_stats_t_delete(         sqlite3 *db,                      const player_returns_stats_s *player_returns_stats );

int player_accolades_t_create(         sqlite3 *db,                      const player_accolade_s *player_accolade  );
int player_accolades_t_read_by_player( sqlite3 *db, const int player_id,       data_list_s       *player_accolades );
int player_accolades_t_delete(         sqlite3 *db,                      const player_accolade_s *player_accolade  );

player_s *get_player(    sqlite3 *db, const int       player_id );
int       save_player(   sqlite3 *db, const player_s *player    );
void      free_player(                      player_s *player    );

#endif
