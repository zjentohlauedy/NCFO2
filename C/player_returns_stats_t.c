#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_returns_stats_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_returns_stats_s *player_returns_stats = (player_returns_stats_s *)data;

     BIND_INT( statement,  1, player_returns_stats->player_id              );
     BIND_INT( statement,  2, player_returns_stats->season                 );
     BIND_INT( statement,  3, player_returns_stats->bowl_game              );
     BIND_INT( statement,  4, player_returns_stats->kick_returns           );
     BIND_INT( statement,  5, player_returns_stats->kick_return_yards      );
     BIND_INT( statement,  6, player_returns_stats->kick_return_touchdowns );
     BIND_INT( statement,  7, player_returns_stats->punt_returns           );
     BIND_INT( statement,  8, player_returns_stats->punt_return_yards      );
     BIND_INT( statement,  9, player_returns_stats->punt_return_touchdowns );

     return SQLITE_OK;
}


int player_returns_stats_t_create( sqlite3 *db, const player_returns_stats_s *player_returns_stats )
{
     static char query[]   = "INSERT INTO Player_Returns_Stats_T ( Player_Id, Season, Bowl_Game, Kick_Returns, Kick_Return_Yards, Kick_Return_Touchdowns, Punt_Returns, Punt_Return_Yards, Punt_Return_Touchdowns )"
          /**/                                           "VALUES ( ?,"       "?,"    "?,"       "?,"          "?,"               "?,"                    "?,"          "?,"               "?"                    ")";

     return execute_update_old( db, query, player_returns_stats_t_create_bindings, player_returns_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

// By Key:
static int player_returns_stats_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_returns_stats_s *player_returns_stats = (player_returns_stats_s *)data;

     BIND_INT( statement, 1, player_returns_stats->player_id );
     BIND_INT( statement, 2, player_returns_stats->season    );
     BIND_INT( statement, 3, player_returns_stats->bowl_game );

     return SQLITE_OK;
}

static int player_returns_stats_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     player_returns_stats_s *player_returns_stats = (player_returns_stats_s *)data;

     player_returns_stats->kick_returns           = sqlite3_column_int( statement,  0 );
     player_returns_stats->kick_return_yards      = sqlite3_column_int( statement,  1 );
     player_returns_stats->kick_return_touchdowns = sqlite3_column_int( statement,  2 );
     player_returns_stats->punt_returns           = sqlite3_column_int( statement,  3 );
     player_returns_stats->punt_return_yards      = sqlite3_column_int( statement,  4 );
     player_returns_stats->punt_return_touchdowns = sqlite3_column_int( statement,  5 );

     return SQLITE_OK;
}

int player_returns_stats_t_read( sqlite3 *db, player_returns_stats_s *player_returns_stats )
{
     static char query[]   = "SELECT Kick_Returns, Kick_Return_Yards, Kick_Return_Touchdowns, Punt_Returns, Punt_Return_Yards, Punt_Return_Touchdowns "
          /**/               "FROM   Player_Returns_Stats_T "
          /**/               "WHERE  Player_Id      = ? "
          /**/               "AND    Season       = ? "
          /**/               "AND    Bowl_Game = ? ";

     return execute_query( db, query, player_returns_stats_t_read_bindings, player_returns_stats, player_returns_stats_t_read_retrieve, player_returns_stats );
}

// By Player:
static int player_returns_stats_t_read_bindings_by_player( sqlite3_stmt *statement, const void *data )
{
     const int *player_id = (int *)data;

     return sqlite3_bind_int( statement, 1, *player_id );
}

static int player_returns_stats_t_read_retrieve_by_player( sqlite3_stmt *statement, const void *data )
{
     data_list_s *data_list = (data_list_s *)data;

     player_returns_stats_s player_returns_stats = { 0 };

     player_returns_stats.player_id              = sqlite3_column_int( statement,  0 );
     player_returns_stats.season                 = sqlite3_column_int( statement,  1 );
     player_returns_stats.bowl_game              = sqlite3_column_int( statement,  2 );
     player_returns_stats.kick_returns           = sqlite3_column_int( statement,  3 );
     player_returns_stats.kick_return_yards      = sqlite3_column_int( statement,  4 );
     player_returns_stats.kick_return_touchdowns = sqlite3_column_int( statement,  5 );
     player_returns_stats.punt_returns           = sqlite3_column_int( statement,  6 );
     player_returns_stats.punt_return_yards      = sqlite3_column_int( statement,  7 );
     player_returns_stats.punt_return_touchdowns = sqlite3_column_int( statement,  8 );

     if ( add_to_data_list( data_list, &player_returns_stats, sizeof(player_returns_stats_s), 10 ) < 0 ) return SQLITE_ERROR;

     return SQLITE_OK;
}

int player_returns_stats_t_read_by_player( sqlite3 *db, const int player_id, data_list_s *player_returns_stats )
{
     static char query[] = "SELECT Player_Id, Season, Bowl_Game, Kick_Returns, Kick_Return_Yards, Kick_Return_Touchdowns, Punt_Returns, Punt_Return_Yards, Punt_Return_Touchdowns FROM Player_Returns_Stats_T WHERE Player_Id = ?";

     return execute_query( db, query, player_returns_stats_t_read_bindings_by_player, &player_id, player_returns_stats_t_read_retrieve_by_player, player_returns_stats );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_returns_stats_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_returns_stats_s *player_returns_stats = (player_returns_stats_s *)data;

     BIND_INT( statement, 1, player_returns_stats->kick_returns           );
     BIND_INT( statement, 2, player_returns_stats->kick_return_yards      );
     BIND_INT( statement, 3, player_returns_stats->kick_return_touchdowns );
     BIND_INT( statement, 4, player_returns_stats->punt_returns           );
     BIND_INT( statement, 5, player_returns_stats->punt_return_yards      );
     BIND_INT( statement, 6, player_returns_stats->punt_return_touchdowns );

     BIND_INT( statement, 7, player_returns_stats->player_id );
     BIND_INT( statement, 8, player_returns_stats->season    );
     BIND_INT( statement, 9, player_returns_stats->bowl_game );

     return SQLITE_OK;
}


int player_returns_stats_t_update( sqlite3 *db, const player_returns_stats_s *player_returns_stats )
{
     static char query[]   = "UPDATE Player_Returns_Stats_T "
          /**/
          /**/               "SET    Kick_Returns           = ?,"
          /**/                      "Kick_Return_Yards      = ?,"
          /**/                      "Kick_Return_Touchdowns = ?,"
          /**/                      "Punt_Returns           = ?,"
          /**/                      "Punt_Return_Yards      = ?,"
          /**/                      "Punt_Return_Touchdowns = ? "
          /**/
          /**/               "WHERE  Player_Id = ? "
          /**/               "AND    Season    = ? "
          /**/               "AND    Bowl_Game = ? ";

     return execute_update_old( db, query, player_returns_stats_t_update_bindings, player_returns_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_returns_stats_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_returns_stats_s *player_returns_stats = (player_returns_stats_s *)data;

     BIND_INT( statement, 1, player_returns_stats->player_id );
     BIND_INT( statement, 2, player_returns_stats->season    );
     BIND_INT( statement, 3, player_returns_stats->bowl_game );

     return SQLITE_OK;
}


int player_returns_stats_t_delete( sqlite3 *db, const player_returns_stats_s *player_returns_stats )
{
     return execute_update_old( db, "DELETE FROM Player_Returns_Stats_T WHERE Player_Id = ? AND Season = ? AND Bowl_Game = ?", player_returns_stats_t_delete_bindings, player_returns_stats, NULL, NULL );
}
