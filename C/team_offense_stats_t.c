#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "team.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_offense_stats_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_offense_stats_s *team_offense_stats = (team_offense_stats_s *)data;

     BIND_INT( statement,  1, team_offense_stats->team_id         );
     BIND_INT( statement,  2, team_offense_stats->season          );
     BIND_INT( statement,  3, team_offense_stats->bowl_game       );
     BIND_INT( statement,  4, team_offense_stats->pass_attempts   );
     BIND_INT( statement,  5, team_offense_stats->completions     );
     BIND_INT( statement,  6, team_offense_stats->pass_yards      );
     BIND_INT( statement,  7, team_offense_stats->pass_touchdowns );
     BIND_INT( statement,  8, team_offense_stats->rush_attempts   );
     BIND_INT( statement,  9, team_offense_stats->rush_yards      );
     BIND_INT( statement, 10, team_offense_stats->rush_touchdowns );

     return SQLITE_OK;
}


int team_offense_stats_t_create( sqlite3 *db, const team_offense_stats_s *team_offense_stats )
{
     static char query[]   = "INSERT INTO Team_Offense_Stats_T ( Team_Id, Season, Bowl_Game, Pass_Attempts, Completions, Pass_Yards, Pass_Touchdowns, Rush_Attempts, Rush_Yards, Rush_Touchdowns )"
          /**/                                         "VALUES ( ?,"     "?,"    "?,"       "?,"           "?,"         "?,"        "?,"             "?,"           "?,"        "?"             ")";

     return execute_update_old( db, query, team_offense_stats_t_create_bindings, team_offense_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

// By Key:
static int team_offense_stats_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_offense_stats_s *team_offense_stats = (team_offense_stats_s *)data;

     BIND_INT( statement, 1, team_offense_stats->team_id   );
     BIND_INT( statement, 2, team_offense_stats->season    );
     BIND_INT( statement, 3, team_offense_stats->bowl_game );

     return SQLITE_OK;
}

static int team_offense_stats_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     team_offense_stats_s *team_offense_stats = (team_offense_stats_s *)data;

     team_offense_stats->pass_attempts   = sqlite3_column_int( statement,  0 );
     team_offense_stats->completions     = sqlite3_column_int( statement,  1 );
     team_offense_stats->pass_yards      = sqlite3_column_int( statement,  2 );
     team_offense_stats->pass_touchdowns = sqlite3_column_int( statement,  3 );
     team_offense_stats->rush_attempts   = sqlite3_column_int( statement,  4 );
     team_offense_stats->rush_yards      = sqlite3_column_int( statement,  5 );
     team_offense_stats->rush_touchdowns = sqlite3_column_int( statement,  6 );

     return SQLITE_OK;
}

int team_offense_stats_t_read( sqlite3 *db, team_offense_stats_s *team_offense_stats )
{
     static char query[]   = "SELECT Pass_Attempts, Completions, Pass_Yards, Pass_Touchdowns, Rush_Attempts, Rush_Yards, Rush_Touchdowns "
          /**/               "FROM   Team_Offense_Stats_T "
          /**/               "WHERE  Team_Id      = ? "
          /**/               "AND    Season       = ? "
          /**/               "AND    Bowl_Game = ? ";

     return execute_query( db, query, team_offense_stats_t_read_bindings, team_offense_stats, team_offense_stats_t_read_retrieve, team_offense_stats );
}

// By Team:
static int team_offense_stats_t_read_bindings_by_team( sqlite3_stmt *statement, const void *data )
{
     const int *team_id = (int *)data;

     return sqlite3_bind_int( statement, 1, *team_id );
}

static int team_offense_stats_t_read_retrieve_by_team( sqlite3_stmt *statement, const void *data )
{
     data_list_s *data_list = (data_list_s *)data;

     team_offense_stats_s team_offense_stats = { 0 };

     team_offense_stats.team_id         = sqlite3_column_int( statement, 0 );
     team_offense_stats.season          = sqlite3_column_int( statement, 1 );
     team_offense_stats.bowl_game       = sqlite3_column_int( statement, 2 );
     team_offense_stats.pass_attempts   = sqlite3_column_int( statement, 3 );
     team_offense_stats.completions     = sqlite3_column_int( statement, 4 );
     team_offense_stats.pass_yards      = sqlite3_column_int( statement, 5 );
     team_offense_stats.pass_touchdowns = sqlite3_column_int( statement, 6 );
     team_offense_stats.rush_attempts   = sqlite3_column_int( statement, 7 );
     team_offense_stats.rush_yards      = sqlite3_column_int( statement, 8 );
     team_offense_stats.rush_touchdowns = sqlite3_column_int( statement, 9 );

     if ( add_to_data_list( data_list, &team_offense_stats, sizeof(team_offense_stats_s), 10 ) < 0 ) return SQLITE_ERROR;

     return SQLITE_OK;
}

int team_offense_stats_t_read_by_team( sqlite3 *db, const int team_id, data_list_s *team_offense_stats )
{
     static char query[] = "SELECT Team_Id, Season, Bowl_Game, Pass_Attempts, Completions, Pass_Yards, Pass_Touchdowns, Rush_Attempts, Rush_Yards, Rush_Touchdowns FROM Team_Offense_Stats_T WHERE Team_Id = ?";

     return execute_query( db, query, team_offense_stats_t_read_bindings_by_team, &team_id, team_offense_stats_t_read_retrieve_by_team, team_offense_stats );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_offense_stats_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_offense_stats_s *team_offense_stats = (team_offense_stats_s *)data;

     BIND_INT( statement,  1, team_offense_stats->pass_attempts   );
     BIND_INT( statement,  2, team_offense_stats->completions     );
     BIND_INT( statement,  3, team_offense_stats->pass_yards      );
     BIND_INT( statement,  4, team_offense_stats->pass_touchdowns );
     BIND_INT( statement,  5, team_offense_stats->rush_attempts   );
     BIND_INT( statement,  6, team_offense_stats->rush_yards      );
     BIND_INT( statement,  7, team_offense_stats->rush_touchdowns );

     BIND_INT( statement,  8, team_offense_stats->team_id   );
     BIND_INT( statement,  9, team_offense_stats->season    );
     BIND_INT( statement, 10, team_offense_stats->bowl_game );

     return SQLITE_OK;
}


int team_offense_stats_t_update( sqlite3 *db, const team_offense_stats_s *team_offense_stats )
{
     static char query[]   = "UPDATE Team_Offense_Stats_T "
          /**/
          /**/               "SET    Pass_Attempts   = ?,"
          /**/                      "Completions     = ?,"
          /**/                      "Pass_Yards      = ?,"
          /**/                      "Pass_Touchdowns = ?,"
          /**/                      "Rush_Attempts   = ?,"
          /**/                      "Rush_Yards      = ?,"
          /**/                      "Rush_Touchdowns = ? "
          /**/
          /**/               "WHERE  Team_Id   = ? "
          /**/               "AND    Season    = ? "
          /**/               "AND    Bowl_Game = ? ";

     return execute_update_old( db, query, team_offense_stats_t_update_bindings, team_offense_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_offense_stats_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_offense_stats_s *team_offense_stats = (team_offense_stats_s *)data;

     BIND_INT( statement, 1, team_offense_stats->team_id   );
     BIND_INT( statement, 2, team_offense_stats->season    );
     BIND_INT( statement, 3, team_offense_stats->bowl_game );

     return SQLITE_OK;
}


int team_offense_stats_t_delete( sqlite3 *db, const team_offense_stats_s *team_offense_stats )
{
     return execute_update_old( db, "DELETE FROM Team_Offense_Stats_T WHERE Team_Id = ? AND Season = ? AND Bowl_Game = ?", team_offense_stats_t_delete_bindings, team_offense_stats, NULL, NULL );
}
