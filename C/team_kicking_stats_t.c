#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "team.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_kicking_stats_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_kicking_stats_s *team_kicking_stats = (team_kicking_stats_s *)data;

     BIND_INT( statement,  1, team_kicking_stats->team_id                );
     BIND_INT( statement,  2, team_kicking_stats->season                 );
     BIND_INT( statement,  3, team_kicking_stats->bowl_game              );
     BIND_INT( statement,  4, team_kicking_stats->extra_point_attempts   );
     BIND_INT( statement,  5, team_kicking_stats->extra_points_made      );
     BIND_INT( statement,  6, team_kicking_stats->field_goal_attempts    );
     BIND_INT( statement,  7, team_kicking_stats->field_goals_made       );
     BIND_INT( statement,  8, team_kicking_stats->punts                  );
     BIND_INT( statement,  9, team_kicking_stats->punt_yards             );
     BIND_INT( statement, 10, team_kicking_stats->kick_returns           );
     BIND_INT( statement, 11, team_kicking_stats->kick_return_yards      );
     BIND_INT( statement, 12, team_kicking_stats->kick_return_touchdowns );
     BIND_INT( statement, 13, team_kicking_stats->punt_returns           );
     BIND_INT( statement, 14, team_kicking_stats->punt_return_yards      );
     BIND_INT( statement, 15, team_kicking_stats->punt_return_touchdowns );

     return SQLITE_OK;
}


int team_kicking_stats_t_create( sqlite3 *db, const team_kicking_stats_s *team_kicking_stats )
{
     static char query[]   = "INSERT INTO Team_Kicking_Stats_T ( Team_Id, Season, Bowl_Game, Extra_Point_Attempts, Extra_Points_Made, Field_Goal_Attempts, Field_Goals_Made, Punts, Punt_Yards, Kick_Returns, Kick_Return_Yards, Kick_Return_Touchdowns, Punt_Returns, Punt_Return_Yards, Punt_Return_Touchdowns )"
          /**/                                         "VALUES ( ?,"     "?,"    "?,"       "?,"                  "?,"               "?,"                 "?,"              "?,"   "?,"        "?,"          "?,"               "?,"                    "?,"          "?,"               "?"                    ")";

     return execute_update_old( db, query, team_kicking_stats_t_create_bindings, team_kicking_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

// By Key:
static int team_kicking_stats_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_kicking_stats_s *team_kicking_stats = (team_kicking_stats_s *)data;

     BIND_INT( statement, 1, team_kicking_stats->team_id   );
     BIND_INT( statement, 2, team_kicking_stats->season    );
     BIND_INT( statement, 3, team_kicking_stats->bowl_game );

     return SQLITE_OK;
}

static int team_kicking_stats_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     team_kicking_stats_s *team_kicking_stats = (team_kicking_stats_s *)data;

     team_kicking_stats->extra_point_attempts   = sqlite3_column_int( statement,  0 );
     team_kicking_stats->extra_points_made      = sqlite3_column_int( statement,  1 );
     team_kicking_stats->field_goal_attempts    = sqlite3_column_int( statement,  2 );
     team_kicking_stats->field_goals_made       = sqlite3_column_int( statement,  3 );
     team_kicking_stats->punts                  = sqlite3_column_int( statement,  4 );
     team_kicking_stats->punt_yards             = sqlite3_column_int( statement,  5 );
     team_kicking_stats->kick_returns           = sqlite3_column_int( statement,  6 );
     team_kicking_stats->kick_return_yards      = sqlite3_column_int( statement,  7 );
     team_kicking_stats->kick_return_touchdowns = sqlite3_column_int( statement,  8 );
     team_kicking_stats->punt_returns           = sqlite3_column_int( statement,  9 );
     team_kicking_stats->punt_return_yards      = sqlite3_column_int( statement, 10 );
     team_kicking_stats->punt_return_touchdowns = sqlite3_column_int( statement, 11 );

     return SQLITE_OK;
}

int team_kicking_stats_t_read( sqlite3 *db, team_kicking_stats_s *team_kicking_stats )
{
     static char query[]   = "SELECT Extra_Point_Attempts, Extra_Points_Made, Field_Goal_Attempts, Field_Goals_Made, Punts, Punt_Yards, Kick_Returns, Kick_Return_Yards, Kick_Return_Touchdowns, Punt_Returns, Punt_Return_Yards, Punt_Return_Touchdowns "
          /**/               "FROM   Team_Kicking_Stats_T "
          /**/               "WHERE  Team_Id      = ? "
          /**/               "AND    Season       = ? "
          /**/               "AND    Bowl_Game = ? ";

     return execute_query( db, query, team_kicking_stats_t_read_bindings, team_kicking_stats, team_kicking_stats_t_read_retrieve, team_kicking_stats );
}

// By Team:
static int team_kicking_stats_t_read_bindings_by_team( sqlite3_stmt *statement, const void *data )
{
     const int *team_id = (int *)data;

     return sqlite3_bind_int( statement, 1, *team_id );
}

static int team_kicking_stats_t_read_retrieve_by_team( sqlite3_stmt *statement, const void *data )
{
     data_list_s *data_list = (data_list_s *)data;

     team_kicking_stats_s team_kicking_stats = { 0 };

     team_kicking_stats.team_id                = sqlite3_column_int( statement,  0 );
     team_kicking_stats.season                 = sqlite3_column_int( statement,  1 );
     team_kicking_stats.bowl_game              = sqlite3_column_int( statement,  2 );
     team_kicking_stats.extra_point_attempts   = sqlite3_column_int( statement,  3 );
     team_kicking_stats.extra_points_made      = sqlite3_column_int( statement,  4 );
     team_kicking_stats.field_goal_attempts    = sqlite3_column_int( statement,  5 );
     team_kicking_stats.field_goals_made       = sqlite3_column_int( statement,  6 );
     team_kicking_stats.punts                  = sqlite3_column_int( statement,  7 );
     team_kicking_stats.punt_yards             = sqlite3_column_int( statement,  8 );
     team_kicking_stats.kick_returns           = sqlite3_column_int( statement,  9 );
     team_kicking_stats.kick_return_yards      = sqlite3_column_int( statement, 10 );
     team_kicking_stats.kick_return_touchdowns = sqlite3_column_int( statement, 11 );
     team_kicking_stats.punt_returns           = sqlite3_column_int( statement, 12 );
     team_kicking_stats.punt_return_yards      = sqlite3_column_int( statement, 13 );
     team_kicking_stats.punt_return_touchdowns = sqlite3_column_int( statement, 14 );

     if ( add_to_data_list( data_list, &team_kicking_stats, sizeof(team_kicking_stats_s), 10 ) < 0 ) return SQLITE_ERROR;

     return SQLITE_OK;
}

int team_kicking_stats_t_read_by_team( sqlite3 *db, const int team_id, data_list_s *team_kicking_stats )
{
     static char query[] = "SELECT Team_Id, Season, Bowl_Game, Extra_Point_Attempts, Extra_Points_Made, Field_Goal_Attempts, Field_Goals_Made, Punts, Punt_Yards, Kick_Returns, Kick_Return_Yards, Kick_Return_Touchdowns, Punt_Returns, Punt_Return_Yards, Punt_Return_Touchdowns FROM Team_Kicking_Stats_T WHERE Team_Id = ?";

     return execute_query( db, query, team_kicking_stats_t_read_bindings_by_team, &team_id, team_kicking_stats_t_read_retrieve_by_team, team_kicking_stats );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_kicking_stats_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_kicking_stats_s *team_kicking_stats = (team_kicking_stats_s *)data;

     BIND_INT( statement,  1, team_kicking_stats->extra_point_attempts   );
     BIND_INT( statement,  2, team_kicking_stats->extra_points_made      );
     BIND_INT( statement,  3, team_kicking_stats->field_goal_attempts    );
     BIND_INT( statement,  4, team_kicking_stats->field_goals_made       );
     BIND_INT( statement,  5, team_kicking_stats->punts                  );
     BIND_INT( statement,  6, team_kicking_stats->punt_yards             );
     BIND_INT( statement,  7, team_kicking_stats->kick_returns           );
     BIND_INT( statement,  8, team_kicking_stats->kick_return_yards      );
     BIND_INT( statement,  9, team_kicking_stats->kick_return_touchdowns );
     BIND_INT( statement, 10, team_kicking_stats->punt_returns           );
     BIND_INT( statement, 11, team_kicking_stats->punt_return_yards      );
     BIND_INT( statement, 12, team_kicking_stats->punt_return_touchdowns );

     BIND_INT( statement, 13, team_kicking_stats->team_id   );
     BIND_INT( statement, 14, team_kicking_stats->season    );
     BIND_INT( statement, 15, team_kicking_stats->bowl_game );

     return SQLITE_OK;
}


int team_kicking_stats_t_update( sqlite3 *db, const team_kicking_stats_s *team_kicking_stats )
{
     static char query[]   = "UPDATE Team_Kicking_Stats_T "
          /**/
          /**/               "SET    Extra_Point_Attempts   = ?,"
          /**/                      "Extra_Points_Made      = ?,"
          /**/                      "Field_Goal_Attempts    = ?,"
          /**/                      "Field_Goals_Made       = ?,"
          /**/                      "Punts                  = ?,"
          /**/                      "Punt_Yards             = ?,"
          /**/                      "Kick_Returns           = ?,"
          /**/                      "Kick_Return_Yards      = ?,"
          /**/                      "Kick_Return_Touchdowns = ?,"
          /**/                      "Punt_Returns           = ?,"
          /**/                      "Punt_Return_Yards      = ?,"
          /**/                      "Punt_Return_Touchdowns = ? "
          /**/
          /**/               "WHERE  Team_Id   = ? "
          /**/               "AND    Season    = ? "
          /**/               "AND    Bowl_Game = ? ";

     return execute_update_old( db, query, team_kicking_stats_t_update_bindings, team_kicking_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_kicking_stats_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_kicking_stats_s *team_kicking_stats = (team_kicking_stats_s *)data;

     BIND_INT( statement, 1, team_kicking_stats->team_id   );
     BIND_INT( statement, 2, team_kicking_stats->season    );
     BIND_INT( statement, 3, team_kicking_stats->bowl_game );

     return SQLITE_OK;
}


int team_kicking_stats_t_delete( sqlite3 *db, const team_kicking_stats_s *team_kicking_stats )
{
     return execute_update_old( db, "DELETE FROM Team_Kicking_Stats_T WHERE Team_Id = ? AND Season = ? AND Bowl_Game = ?", team_kicking_stats_t_delete_bindings, team_kicking_stats, NULL, NULL );
}
