#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_defense_stats_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_defense_stats_s *player_defense_stats = (player_defense_stats_s *)data;

     BIND_INT( statement,  1, player_defense_stats->player_id         );
     BIND_INT( statement,  2, player_defense_stats->season            );
     BIND_INT( statement,  3, player_defense_stats->bowl_game         );
     BIND_INT( statement,  4, player_defense_stats->sacks             );
     BIND_INT( statement,  5, player_defense_stats->interceptions     );
     BIND_INT( statement,  6, player_defense_stats->return_yards      );
     BIND_INT( statement,  7, player_defense_stats->return_touchdowns );

     return SQLITE_OK;
}


int player_defense_stats_t_create( sqlite3 *db, const player_defense_stats_s *player_defense_stats )
{
     static char query[]   = "INSERT INTO Player_Defense_Stats_T ( Player_Id, Season, Bowl_Game, Sacks, Interceptions, Return_Yards, Return_Touchdowns )"
          /**/                                           "VALUES ( ?,"       "?,"    "?,"       "?,"   "?,"           "?,"          "?"               ")";

     return execute_update_old( db, query, player_defense_stats_t_create_bindings, player_defense_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

// By Key:
static int player_defense_stats_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_defense_stats_s *player_defense_stats = (player_defense_stats_s *)data;

     BIND_INT( statement, 1, player_defense_stats->player_id );
     BIND_INT( statement, 2, player_defense_stats->season    );
     BIND_INT( statement, 3, player_defense_stats->bowl_game );

     return SQLITE_OK;
}

static int player_defense_stats_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     player_defense_stats_s *player_defense_stats = (player_defense_stats_s *)data;

     player_defense_stats->sacks             = sqlite3_column_int( statement,  0 );
     player_defense_stats->interceptions     = sqlite3_column_int( statement,  1 );
     player_defense_stats->return_yards      = sqlite3_column_int( statement,  2 );
     player_defense_stats->return_touchdowns = sqlite3_column_int( statement,  3 );

     return SQLITE_OK;
}

int player_defense_stats_t_read( sqlite3 *db, player_defense_stats_s *player_defense_stats )
{
     static char query[]   = "SELECT Sacks, Interceptions, Return_Yards, Return_Touchdowns "
          /**/               "FROM   Player_Defense_Stats_T "
          /**/               "WHERE  Player_Id      = ? "
          /**/               "AND    Season       = ? "
          /**/               "AND    Bowl_Game = ? ";

     return execute_query( db, query, player_defense_stats_t_read_bindings, player_defense_stats, player_defense_stats_t_read_retrieve, player_defense_stats );
}

// By Player:
static int player_defense_stats_t_read_bindings_by_player( sqlite3_stmt *statement, const void *data )
{
     const int *player_id = (int *)data;

     return sqlite3_bind_int( statement, 1, *player_id );
}

static int player_defense_stats_t_read_retrieve_by_player( sqlite3_stmt *statement, const void *data )
{
     data_list_s *data_list = (data_list_s *)data;

     player_defense_stats_s player_defense_stats = { 0 };

     player_defense_stats.player_id         = sqlite3_column_int( statement,  0 );
     player_defense_stats.season            = sqlite3_column_int( statement,  1 );
     player_defense_stats.bowl_game         = sqlite3_column_int( statement,  2 );
     player_defense_stats.sacks             = sqlite3_column_int( statement,  3 );
     player_defense_stats.interceptions     = sqlite3_column_int( statement,  4 );
     player_defense_stats.return_yards      = sqlite3_column_int( statement,  5 );
     player_defense_stats.return_touchdowns = sqlite3_column_int( statement,  6 );

     if ( add_to_data_list( data_list, &player_defense_stats, sizeof(player_defense_stats_s), 10 ) < 0 ) return SQLITE_ERROR;

     return SQLITE_OK;
}

int player_defense_stats_t_read_by_player( sqlite3 *db, const int player_id, data_list_s *player_defense_stats )
{
     static char query[] = "SELECT Player_Id, Season, Bowl_Game, Sacks, Interceptions, Return_Yards, Return_Touchdowns FROM Player_Defense_Stats_T WHERE Player_Id = ?";

     return execute_query( db, query, player_defense_stats_t_read_bindings_by_player, &player_id, player_defense_stats_t_read_retrieve_by_player, player_defense_stats );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_defense_stats_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_defense_stats_s *player_defense_stats = (player_defense_stats_s *)data;

     BIND_INT( statement, 1, player_defense_stats->sacks             );
     BIND_INT( statement, 2, player_defense_stats->interceptions     );
     BIND_INT( statement, 3, player_defense_stats->return_yards      );
     BIND_INT( statement, 4, player_defense_stats->return_touchdowns );

     BIND_INT( statement, 5, player_defense_stats->player_id );
     BIND_INT( statement, 6, player_defense_stats->season    );
     BIND_INT( statement, 7, player_defense_stats->bowl_game );

     return SQLITE_OK;
}


int player_defense_stats_t_update( sqlite3 *db, const player_defense_stats_s *player_defense_stats )
{
     static char query[]   = "UPDATE Player_Defense_Stats_T "
          /**/
          /**/               "SET    Sacks             = ?,"
          /**/                      "Interceptions     = ?,"
          /**/                      "Return_Yards      = ?,"
          /**/                      "Return_Touchdowns = ? "
          /**/
          /**/               "WHERE  Player_Id = ? "
          /**/               "AND    Season    = ? "
          /**/               "AND    Bowl_Game = ? ";

     return execute_update_old( db, query, player_defense_stats_t_update_bindings, player_defense_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_defense_stats_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_defense_stats_s *player_defense_stats = (player_defense_stats_s *)data;

     BIND_INT( statement, 1, player_defense_stats->player_id );
     BIND_INT( statement, 2, player_defense_stats->season    );
     BIND_INT( statement, 3, player_defense_stats->bowl_game );

     return SQLITE_OK;
}


int player_defense_stats_t_delete( sqlite3 *db, const player_defense_stats_s *player_defense_stats )
{
     return execute_update_old( db, "DELETE FROM Player_Defense_Stats_T WHERE Player_Id = ? AND Season = ? AND Bowl_Game = ?", player_defense_stats_t_delete_bindings, player_defense_stats, NULL, NULL );
}
