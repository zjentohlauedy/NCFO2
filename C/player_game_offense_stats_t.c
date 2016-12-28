#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_game_offense_stats_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_game_offense_stats_s *player_game_offense_stats = (player_game_offense_stats_s *)data;

     BIND_INT( statement,  1, player_game_offense_stats->player_id            );
     BIND_INT( statement,  2, player_game_offense_stats->season               );
     BIND_INT( statement,  3, player_game_offense_stats->week                 );
     BIND_INT( statement,  4, player_game_offense_stats->game                 );
     BIND_INT( statement,  5, player_game_offense_stats->pass_attempts        );
     BIND_INT( statement,  6, player_game_offense_stats->completions          );
     BIND_INT( statement,  7, player_game_offense_stats->interceptions        );
     BIND_INT( statement,  8, player_game_offense_stats->pass_yards           );
     BIND_INT( statement,  9, player_game_offense_stats->pass_touchdowns      );
     BIND_INT( statement, 10, player_game_offense_stats->rush_attempts        );
     BIND_INT( statement, 11, player_game_offense_stats->rush_yards           );
     BIND_INT( statement, 12, player_game_offense_stats->rush_touchdowns      );
     BIND_INT( statement, 13, player_game_offense_stats->receptions           );
     BIND_INT( statement, 14, player_game_offense_stats->receiving_yards      );
     BIND_INT( statement, 15, player_game_offense_stats->receiving_touchdowns );

     return SQLITE_OK;
}


int player_game_offense_stats_t_create( sqlite3 *db, const player_game_offense_stats_s *player_game_offense_stats )
{
     static char query[]   = "INSERT INTO Player_Game_Offense_Stats_T ( Player_Id, Season, Week, Game, Pass_Attempts, Completions, Interceptions, Pass_Yards, Pass_Touchdowns, Rush_Attempts, Rush_Yards, Rush_Touchdowns, Receptions, Receiving_Yards, Receiving_Touchdowns )"
          /**/                                                "VALUES ( ?,"       "?,"    "?,"  "?,"  "?,"           "?,"         "?,"           "?,"        "?,"             "?,"           "?,"        "?,"             "?,"        "?,"             "?"                  ")";

     return execute_update_old( db, query, player_game_offense_stats_t_create_bindings, player_game_offense_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

// By Key:
static int player_game_offense_stats_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_game_offense_stats_s *player_game_offense_stats = (player_game_offense_stats_s *)data;

     BIND_INT( statement, 1, player_game_offense_stats->player_id );
     BIND_INT( statement, 2, player_game_offense_stats->season    );
     BIND_INT( statement, 3, player_game_offense_stats->week      );
     BIND_INT( statement, 4, player_game_offense_stats->game      );

     return SQLITE_OK;
}

static int player_game_offense_stats_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     player_game_offense_stats_s *player_game_offense_stats = (player_game_offense_stats_s *)data;

     player_game_offense_stats->pass_attempts        = sqlite3_column_int( statement,  0 );
     player_game_offense_stats->completions          = sqlite3_column_int( statement,  1 );
     player_game_offense_stats->interceptions        = sqlite3_column_int( statement,  2 );
     player_game_offense_stats->pass_yards           = sqlite3_column_int( statement,  3 );
     player_game_offense_stats->pass_touchdowns      = sqlite3_column_int( statement,  4 );
     player_game_offense_stats->rush_attempts        = sqlite3_column_int( statement,  5 );
     player_game_offense_stats->rush_yards           = sqlite3_column_int( statement,  6 );
     player_game_offense_stats->rush_touchdowns      = sqlite3_column_int( statement,  7 );
     player_game_offense_stats->receptions           = sqlite3_column_int( statement,  8 );
     player_game_offense_stats->receiving_yards      = sqlite3_column_int( statement,  9 );
     player_game_offense_stats->receiving_touchdowns = sqlite3_column_int( statement, 10 );

     return SQLITE_OK;
}

int player_game_offense_stats_t_read( sqlite3 *db, player_game_offense_stats_s *player_game_offense_stats )
{
     static char query[]   = "SELECT Pass_Attempts, Completions, Interceptions, Pass_Yards, Pass_Touchdowns, Rush_Attempts, Rush_Yards, Rush_Touchdowns, Receptions, Receiving_Yards, Receiving_Touchdowns "
          /**/               "FROM   Player_Game_Offense_Stats_T "
          /**/               "WHERE  Player_Id      = ? "
          /**/               "AND    Season         = ? "
          /**/               "AND    Week           = ? "
          /**/               "AND    Game           = ? ";

     return execute_query( db, query, player_game_offense_stats_t_read_bindings, player_game_offense_stats, player_game_offense_stats_t_read_retrieve, player_game_offense_stats );
}

// By Player:
static int player_game_offense_stats_t_read_bindings_by_player( sqlite3_stmt *statement, const void *data )
{
     const int *player_id = (int *)data;

     return sqlite3_bind_int( statement, 1, *player_id );
}

static int player_game_offense_stats_t_read_retrieve_by_player( sqlite3_stmt *statement, const void *data )
{
     data_list_s *data_list = (data_list_s *)data;

     player_game_offense_stats_s player_game_offense_stats = { 0 };

     player_game_offense_stats.player_id            = sqlite3_column_int( statement,  0 );
     player_game_offense_stats.season               = sqlite3_column_int( statement,  1 );
     player_game_offense_stats.week                 = sqlite3_column_int( statement,  2 );
     player_game_offense_stats.game                 = sqlite3_column_int( statement,  3 );
     player_game_offense_stats.pass_attempts        = sqlite3_column_int( statement,  4 );
     player_game_offense_stats.completions          = sqlite3_column_int( statement,  5 );
     player_game_offense_stats.interceptions        = sqlite3_column_int( statement,  6 );
     player_game_offense_stats.pass_yards           = sqlite3_column_int( statement,  7 );
     player_game_offense_stats.pass_touchdowns      = sqlite3_column_int( statement,  8 );
     player_game_offense_stats.rush_attempts        = sqlite3_column_int( statement,  9 );
     player_game_offense_stats.rush_yards           = sqlite3_column_int( statement, 10 );
     player_game_offense_stats.rush_touchdowns      = sqlite3_column_int( statement, 11 );
     player_game_offense_stats.receptions           = sqlite3_column_int( statement, 12 );
     player_game_offense_stats.receiving_yards      = sqlite3_column_int( statement, 13 );
     player_game_offense_stats.receiving_touchdowns = sqlite3_column_int( statement, 14 );

     if ( add_to_data_list( data_list, &player_game_offense_stats, sizeof(player_game_offense_stats_s), 10 ) < 0 ) return SQLITE_ERROR;

     return SQLITE_OK;
}

int player_game_offense_stats_t_read_by_player( sqlite3 *db, const int player_id, data_list_s *player_game_offense_stats )
{
     static char query[] = "SELECT Player_Id, Season, Week, Game, Pass_Attempts, Completions, Interceptions, Pass_Yards, Pass_Touchdowns, Rush_Attempts, Rush_Yards, Rush_Touchdowns, Receptions, Receiving_Yards, Receiving_Touchdowns FROM Player_Game_Offense_Stats_T WHERE Player_Id = ?";

     return execute_query( db, query, player_game_offense_stats_t_read_bindings_by_player, &player_id, player_game_offense_stats_t_read_retrieve_by_player, player_game_offense_stats );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_game_offense_stats_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_game_offense_stats_s *player_game_offense_stats = (player_game_offense_stats_s *)data;

     BIND_INT( statement,  1, player_game_offense_stats->pass_attempts        );
     BIND_INT( statement,  2, player_game_offense_stats->completions          );
     BIND_INT( statement,  3, player_game_offense_stats->interceptions        );
     BIND_INT( statement,  4, player_game_offense_stats->pass_yards           );
     BIND_INT( statement,  5, player_game_offense_stats->pass_touchdowns      );
     BIND_INT( statement,  6, player_game_offense_stats->rush_attempts        );
     BIND_INT( statement,  7, player_game_offense_stats->rush_yards           );
     BIND_INT( statement,  8, player_game_offense_stats->rush_touchdowns      );
     BIND_INT( statement,  9, player_game_offense_stats->receptions           );
     BIND_INT( statement, 10, player_game_offense_stats->receiving_yards      );
     BIND_INT( statement, 11, player_game_offense_stats->receiving_touchdowns );

     BIND_INT( statement, 12, player_game_offense_stats->player_id );
     BIND_INT( statement, 13, player_game_offense_stats->season    );
     BIND_INT( statement, 14, player_game_offense_stats->week      );
     BIND_INT( statement, 15, player_game_offense_stats->game      );

     return SQLITE_OK;
}


int player_game_offense_stats_t_update( sqlite3 *db, const player_game_offense_stats_s *player_game_offense_stats )
{
     static char query[]   = "UPDATE Player_Game_Offense_Stats_T "
          /**/
          /**/               "SET    Pass_Attempts        = ?,"
          /**/                      "Completions          = ?,"
          /**/                      "Interceptions        = ?,"
          /**/                      "Pass_Yards           = ?,"
          /**/                      "Pass_Touchdowns      = ?,"
          /**/                      "Rush_Attempts        = ?,"
          /**/                      "Rush_Yards           = ?,"
          /**/                      "Rush_Touchdowns      = ?,"
          /**/                      "Receptions           = ?,"
          /**/                      "Receiving_Yards      = ?,"
          /**/                      "Receiving_Touchdowns = ? "
          /**/
          /**/               "WHERE  Player_Id = ? "
          /**/               "AND    Season    = ? "
          /**/               "AND    Week      = ? "
          /**/               "AND    Game      = ? ";

     return execute_update_old( db, query, player_game_offense_stats_t_update_bindings, player_game_offense_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int player_game_offense_stats_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const player_game_offense_stats_s *player_game_offense_stats = (player_game_offense_stats_s *)data;

     BIND_INT( statement, 1, player_game_offense_stats->player_id );
     BIND_INT( statement, 2, player_game_offense_stats->season    );
     BIND_INT( statement, 3, player_game_offense_stats->week      );
     BIND_INT( statement, 4, player_game_offense_stats->game      );

     return SQLITE_OK;
}


int player_game_offense_stats_t_delete( sqlite3 *db, const player_game_offense_stats_s *player_game_offense_stats )
{
     return execute_update_old( db, "DELETE FROM Player_Game_Offense_Stats_T WHERE Player_Id = ? AND Season = ? AND Week = ? AND Game = ?", player_game_offense_stats_t_delete_bindings, player_game_offense_stats, NULL, NULL );
}
