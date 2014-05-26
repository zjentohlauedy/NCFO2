#ifndef __INC_CONFERENCE_H__
#define __INC_CONFERENCE_H__

#include <sqlite3.h>
#include "data_list.h"

typedef enum
{
     bg_None             = 0,
     bg_CottonBowl       = 1,
     bg_OrangeBowl       = 2,
     bg_RoseBowl         = 3,
     bg_SugarBowl        = 4,
     bg_FiestaBowl       = 5,
     bg_LibertyBowl      = 6,
     bg_NCFOChampionship = 7

} bowl_game_e;

typedef enum
{
     cacc_None                 = 0,
     cacc_CottonBowlChampions  = 1,
     cacc_OrangeBowlChampions  = 2,
     cacc_RoseBowlChampions    = 3,
     cacc_SugarBowlChampions   = 4,
     cacc_FiestaBowlChampions  = 5,
     cacc_LibertyBowlChampions = 6,
     cacc_NCFOChampions        = 7

} conference_accolade_e;

typedef struct
{
     int                    conference_id;
     int                    season;
     conference_accolade_e  accolade;

} conference_accolade_s;

typedef struct
{
     int            conference_id;
     int            season;
     bowl_game_e    bowl_game;
     int            wins;
     int            losses;
     int            home_wins;
     int            home_losses;
     int            road_wins;
     int            road_losses;
     int            points_scored;
     int            points_allowed;

} conference_stats_s;

typedef struct
{
     int     conference_id;
     int     team_id;

} conference_team_s;

typedef struct
{
     int                  conference_id;
     char                 name          [20 + 1];

} conference_s;


int conferences_t_create( sqlite3 *db, const conference_s *conference );
int conferences_t_read(   sqlite3 *db,       conference_s *conference );
int conferences_t_update( sqlite3 *db, const conference_s *conference );
int conferences_t_delete( sqlite3 *db, const conference_s *conference );

int conference_teams_t_create(             sqlite3 *db,                          const conference_team_s *conference_team  );
int conference_teams_t_read_by_conference( sqlite3 *db, const int conference_id,         data_list_s     *conference_teams );
int conference_teams_t_delete(             sqlite3 *db,                          const conference_team_s *conference_team  );

int conference_stats_t_create(             sqlite3 *db,                          const conference_stats_s *conference_stats );
int conference_stats_t_read(               sqlite3 *db,                                conference_stats_s *conference_stats );
int conference_stats_t_read_by_conference( sqlite3 *db, const int conference_id,         data_list_s      *conference_stats );
int conference_stats_t_update(             sqlite3 *db,                          const conference_stats_s *conference_stats );
int conference_stats_t_delete(             sqlite3 *db,                          const conference_stats_s *conference_stats );

int conference_accolades_t_create(             sqlite3 *db,                          const conference_accolade_s *conference_accolade  );
int conference_accolades_t_read_by_conference( sqlite3 *db, const int conference_id,         data_list_s         *conference_accolades );
int conference_accolades_t_delete(             sqlite3 *db,                          const conference_accolade_s *conference_accolade  );

#endif
