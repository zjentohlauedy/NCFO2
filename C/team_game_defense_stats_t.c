#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "team.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_game_defense_stats_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_game_defense_stats_s *team_game_defense_stats = (team_game_defense_stats_s *)data;

     BIND_INT( statement,  1, team_game_defense_stats->team_id           );
     BIND_INT( statement,  2, team_game_defense_stats->season            );
     BIND_INT( statement,  3, team_game_defense_stats->week              );
     BIND_INT( statement,  4, team_game_defense_stats->game              );
     BIND_INT( statement,  5, team_game_defense_stats->sacks             );
     BIND_INT( statement,  6, team_game_defense_stats->interceptions     );
     BIND_INT( statement,  7, team_game_defense_stats->return_yards      );
     BIND_INT( statement,  8, team_game_defense_stats->return_touchdowns );

     return SQLITE_OK;
}


int team_game_defense_stats_t_create( sqlite3 *db, const team_game_defense_stats_s *team_game_defense_stats )
{
     static char query[]   = "INSERT INTO Team_Game_Defense_Stats_T ( Team_Id, Season, Week, Game, Sacks, Interceptions, Return_Yards, Return_Touchdowns )"
          /**/                                              "VALUES ( ?,"     "?,"    "?,"  "?,"  "?,"   "?,"           "?,"          "?"               ")";

     return execute_update_old( db, query, team_game_defense_stats_t_create_bindings, team_game_defense_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

// By Key:
static int team_game_defense_stats_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_game_defense_stats_s *team_game_defense_stats = (team_game_defense_stats_s *)data;

     BIND_INT( statement, 1, team_game_defense_stats->team_id   );
     BIND_INT( statement, 2, team_game_defense_stats->season    );
     BIND_INT( statement, 3, team_game_defense_stats->week      );
     BIND_INT( statement, 4, team_game_defense_stats->game      );

     return SQLITE_OK;
}

static int team_game_defense_stats_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     team_game_defense_stats_s *team_game_defense_stats = (team_game_defense_stats_s *)data;

     team_game_defense_stats->sacks             = sqlite3_column_int( statement,  0 );
     team_game_defense_stats->interceptions     = sqlite3_column_int( statement,  1 );
     team_game_defense_stats->return_yards      = sqlite3_column_int( statement,  2 );
     team_game_defense_stats->return_touchdowns = sqlite3_column_int( statement,  3 );

     return SQLITE_OK;
}

int team_game_defense_stats_t_read( sqlite3 *db, team_game_defense_stats_s *team_game_defense_stats )
{
     static char query[]   = "SELECT Sacks, Interceptions, Return_Yards, Return_Touchdowns "
          /**/               "FROM   Team_Game_Defense_Stats_T "
          /**/               "WHERE  Team_Id   = ? "
          /**/               "AND    Season    = ? "
          /**/               "AND    Week      = ? "
          /**/               "AND    Game      = ? ";

     return execute_query( db, query, team_game_defense_stats_t_read_bindings, team_game_defense_stats, team_game_defense_stats_t_read_retrieve, team_game_defense_stats );
}

// By Team:
static int team_game_defense_stats_t_read_bindings_by_team( sqlite3_stmt *statement, const void *data )
{
     const int *team_id = (int *)data;

     return sqlite3_bind_int( statement, 1, *team_id );
}

static int team_game_defense_stats_t_read_retrieve_by_team( sqlite3_stmt *statement, const void *data )
{
     data_list_s *data_list = (data_list_s *)data;

     team_game_defense_stats_s team_game_defense_stats = { 0 };

     team_game_defense_stats.team_id           = sqlite3_column_int( statement, 0 );
     team_game_defense_stats.season            = sqlite3_column_int( statement, 1 );
     team_game_defense_stats.week              = sqlite3_column_int( statement, 2 );
     team_game_defense_stats.game              = sqlite3_column_int( statement, 3 );
     team_game_defense_stats.sacks             = sqlite3_column_int( statement, 4 );
     team_game_defense_stats.interceptions     = sqlite3_column_int( statement, 5 );
     team_game_defense_stats.return_yards      = sqlite3_column_int( statement, 6 );
     team_game_defense_stats.return_touchdowns = sqlite3_column_int( statement, 7 );

     if ( add_to_data_list( data_list, &team_game_defense_stats, sizeof(team_game_defense_stats_s), 10 ) < 0 ) return SQLITE_ERROR;

     return SQLITE_OK;
}

int team_game_defense_stats_t_read_by_team( sqlite3 *db, const int team_id, data_list_s *team_game_defense_stats )
{
     static char query[] = "SELECT Team_Id, Season, Week, Game, Sacks, Interceptions, Return_Yards, Return_Touchdowns FROM Team_Game_Defense_Stats_T WHERE Team_Id = ?";

     return execute_query( db, query, team_game_defense_stats_t_read_bindings_by_team, &team_id, team_game_defense_stats_t_read_retrieve_by_team, team_game_defense_stats );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_game_defense_stats_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_game_defense_stats_s *team_game_defense_stats = (team_game_defense_stats_s *)data;

     BIND_INT( statement, 1, team_game_defense_stats->sacks             );
     BIND_INT( statement, 2, team_game_defense_stats->interceptions     );
     BIND_INT( statement, 3, team_game_defense_stats->return_yards      );
     BIND_INT( statement, 4, team_game_defense_stats->return_touchdowns );

     BIND_INT( statement, 5, team_game_defense_stats->team_id   );
     BIND_INT( statement, 6, team_game_defense_stats->season    );
     BIND_INT( statement, 7, team_game_defense_stats->week      );
     BIND_INT( statement, 8, team_game_defense_stats->game      );

     return SQLITE_OK;
}


int team_game_defense_stats_t_update( sqlite3 *db, const team_game_defense_stats_s *team_game_defense_stats )
{
     static char query[]   = "UPDATE Team_Game_Defense_Stats_T "
          /**/
          /**/               "SET    Sacks             = ?,"
          /**/                      "Interceptions     = ?,"
          /**/                      "Return_Yards      = ?,"
          /**/                      "Return_Touchdowns = ? "
          /**/
          /**/               "WHERE  Team_Id   = ? "
          /**/               "AND    Season    = ? "
          /**/               "AND    Week      = ? "
          /**/               "AND    Game      = ? ";

     return execute_update_old( db, query, team_game_defense_stats_t_update_bindings, team_game_defense_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_game_defense_stats_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_game_defense_stats_s *team_game_defense_stats = (team_game_defense_stats_s *)data;

     BIND_INT( statement, 1, team_game_defense_stats->team_id   );
     BIND_INT( statement, 2, team_game_defense_stats->season    );
     BIND_INT( statement, 3, team_game_defense_stats->week      );
     BIND_INT( statement, 4, team_game_defense_stats->game      );

     return SQLITE_OK;
}


int team_game_defense_stats_t_delete( sqlite3 *db, const team_game_defense_stats_s *team_game_defense_stats )
{
     return execute_update_old( db, "DELETE FROM Team_Game_Defense_Stats_T WHERE Team_Id = ? AND Season = ? AND Week = ? AND Game = ?", team_game_defense_stats_t_delete_bindings, team_game_defense_stats, NULL, NULL );
}
