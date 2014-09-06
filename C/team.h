#ifndef __INC_TEAM_H__
#define __INC_TEAM_H__

#include <sqlite3.h>
#include "data_list.h"
#include "bowls.h"
#include "player.h"

#define TEAM_PLAYER_SENTINEL         { -1, -1, -1 }
#define TEAM_ACCOLADE_SENTINEL       { -1, -1, tacc_None }
#define TEAM_STATS_SENTINEL          { -1, -1, bg_None, -1, -1, -1, -1, -1, -1, -1, -1 }
#define TEAM_OFFENSE_STATS_SENTINEL  { -1, -1, bg_None, -1, -1, -1, -1, -1, -1, -1, -1 }
#define TEAM_DEFENSE_STATS_SENTINEL  { -1, -1, bg_None, -1, -1, -1, -1 }
#define TEAM_KICKING_STATS_SENTINEL  { -1, -1, bg_None, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }

typedef enum
{
     form_Pro_Set    = 0,
     form_Three_Wide = 1,
     form_Four_Wide  = 2

} formation_e;

typedef enum
{
     pref_BalancedRush = 0,
     pref_HeavyRush    = 1,
     pref_BalancedPass = 2,
     pref_HeavyPass    = 3

} preference_e;

typedef enum
{
     tacc_None                 = 0,
     tacc_ConferenceChampions  = 1,
     tacc_CottonBowlChampions  = 2,
     tacc_OrangeBowlChampions  = 3,
     tacc_RoseBowlChampions    = 4,
     tacc_SugarBowlChampions   = 5,
     tacc_FiestaBowlChampions  = 6,
     tacc_LibertyBowlChampions = 7,
     tacc_NCFOChampions        = 8

} team_accolade_e;

typedef struct
{
     int              team_id;
     int              season;
     team_accolade_e  accolade;

} team_accolade_s;

typedef struct
{
     int          team_id;
     int          season;
     bowl_game_e  bowl_game;
     int          extra_point_attempts;
     int          extra_points_made;
     int          field_goal_attempts;
     int          field_goals_made;
     int          punts;
     int          punt_yards;
     int          kick_returns;
     int          kick_return_yards;
     int          kick_return_touchdowns;
     int          punt_returns;
     int          punt_return_yards;
     int          punt_return_touchdowns;

} team_kicking_stats_s;

typedef struct
{
     int          team_id;
     int          season;
     bowl_game_e  bowl_game;
     int          sacks;
     int          interceptions;
     int          return_yards;
     int          return_touchdowns;

} team_defense_stats_s;

typedef struct
{
     int          team_id;
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

} team_offense_stats_s;

typedef struct
{
     int          team_id;
     int          season;
     bowl_game_e  bowl_game;
     int          wins;
     int          losses;
     int          ties;
     int          home_wins;
     int          home_losses;
     int          home_ties;
     int          road_wins;
     int          road_losses;
     int          road_ties;
     int          points_scored;
     int          points_allowed;

} team_stats_s;

typedef struct
{
     int       team_id;
     int       season;
     int       player_id;
     player_s *player;

} team_player_s;

typedef struct
{
     int                   team_id;
     char                  name         [20 + 1];
     char                  location     [20 + 1];
     char                  abbreviation [ 4 + 1];
     team_player_s        *players;
     team_accolade_s      *accolades;
     team_stats_s         *stats;
     team_offense_stats_s *offense_stats;
     team_defense_stats_s *defense_stats;
     team_kicking_stats_s *kicking_stats;
     // Simulation data:
     int                   sim_offense;
     int                   sim_defense;
     // Offense:
     formation_e           offensive_formation;
     preference_e          offensive_preference;

} team_s;


int teams_t_create( sqlite3 *db, const team_s *team );
int teams_t_read(   sqlite3 *db,       team_s *team );
int teams_t_update( sqlite3 *db, const team_s *team );
int teams_t_delete( sqlite3 *db, const team_s *team );

int team_players_t_create(                  sqlite3 *db,                                   const team_player_s *team_player  );
int team_players_t_read_by_team(            sqlite3 *db, const int            team_id,           data_list_s   *team_players );
int team_players_t_read_by_team_and_season( sqlite3 *db, const team_player_s *team_player,       data_list_s   *team_players );
int team_players_t_delete(                  sqlite3 *db,                                   const team_player_s *team_player  );

int team_stats_t_create(       sqlite3 *db,                    const team_stats_s *team_stats );
int team_stats_t_read(         sqlite3 *db,                          team_stats_s *team_stats );
int team_stats_t_read_by_team( sqlite3 *db, const int team_id,       data_list_s  *team_stats );
int team_stats_t_update(       sqlite3 *db,                    const team_stats_s *team_stats );
int team_stats_t_delete(       sqlite3 *db,                    const team_stats_s *team_stats );

int team_offense_stats_t_create(       sqlite3 *db,                    const team_offense_stats_s *team_offense_stats );
int team_offense_stats_t_read(         sqlite3 *db,                          team_offense_stats_s *team_offense_stats );
int team_offense_stats_t_read_by_team( sqlite3 *db, const int team_id,       data_list_s          *team_offense_stats );
int team_offense_stats_t_update(       sqlite3 *db,                    const team_offense_stats_s *team_offense_stats );
int team_offense_stats_t_delete(       sqlite3 *db,                    const team_offense_stats_s *team_offense_stats );

int team_defense_stats_t_create(       sqlite3 *db,                    const team_defense_stats_s *team_defense_stats );
int team_defense_stats_t_read(         sqlite3 *db,                          team_defense_stats_s *team_defense_stats );
int team_defense_stats_t_read_by_team( sqlite3 *db, const int team_id,       data_list_s          *team_defense_stats );
int team_defense_stats_t_update(       sqlite3 *db,                    const team_defense_stats_s *team_defense_stats );
int team_defense_stats_t_delete(       sqlite3 *db,                    const team_defense_stats_s *team_defense_stats );

int team_kicking_stats_t_create(       sqlite3 *db,                    const team_kicking_stats_s *team_kicking_stats );
int team_kicking_stats_t_read(         sqlite3 *db,                          team_kicking_stats_s *team_kicking_stats );
int team_kicking_stats_t_read_by_team( sqlite3 *db, const int team_id,       data_list_s          *team_kicking_stats );
int team_kicking_stats_t_update(       sqlite3 *db,                    const team_kicking_stats_s *team_kicking_stats );
int team_kicking_stats_t_delete(       sqlite3 *db,                    const team_kicking_stats_s *team_kicking_stats );

int team_accolades_t_create(       sqlite3 *db,                    const team_accolade_s *team_accolade  );
int team_accolades_t_read_by_team( sqlite3 *db, const int team_id,       data_list_s     *team_accolades );
int team_accolades_t_delete(       sqlite3 *db,                    const team_accolade_s *team_accolade  );

team_s *get_team(            sqlite3 *db, const int     team_id                   );
team_s *get_team_for_season( sqlite3 *db, const int     team_id, const int season );
int     save_team(           sqlite3 *db, const team_s *team                      );
void    free_team(                              team_s *team                      );

#endif
