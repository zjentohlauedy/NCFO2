#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "team.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_game_stats_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_game_stats_s *team_game_stats = (team_game_stats_s *)data;

     BIND_INT( statement,  1, team_game_stats->team_id        );
     BIND_INT( statement,  2, team_game_stats->season         );
     BIND_INT( statement,  3, team_game_stats->week           );
     BIND_INT( statement,  4, team_game_stats->game           );
     BIND_INT( statement,  5, team_game_stats->wins           );
     BIND_INT( statement,  6, team_game_stats->losses         );
     BIND_INT( statement,  7, team_game_stats->ties           );
     BIND_INT( statement,  8, team_game_stats->home_wins      );
     BIND_INT( statement,  9, team_game_stats->home_losses    );
     BIND_INT( statement, 10, team_game_stats->home_ties      );
     BIND_INT( statement, 11, team_game_stats->road_wins      );
     BIND_INT( statement, 12, team_game_stats->road_losses    );
     BIND_INT( statement, 13, team_game_stats->road_ties      );
     BIND_INT( statement, 14, team_game_stats->points_scored  );
     BIND_INT( statement, 15, team_game_stats->points_allowed );

     return SQLITE_OK;
}


int team_game_stats_t_create( sqlite3 *db, const team_game_stats_s *team_game_stats )
{
     static char query[]   = "INSERT INTO Team_Game_Stats_T ( Team_Id, Season, Week, Game, Wins, Losses, Ties, Home_Wins, Home_Losses, Home_Ties, Road_Wins, Road_Losses, Road_Ties, Points_Scored, Points_Allowed )"
          /**/                                      "VALUES ( ?,"     "?,"    "?,"  "?,"  "?,"  "?,"    "?,"  "?,"       "?,"         "?,"       "?,"       "?,"         "?,"       "?,"           "?"            ")";

     return execute_update_old( db, query, team_game_stats_t_create_bindings, team_game_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

// By Key:
static int team_game_stats_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_game_stats_s *team_game_stats = (team_game_stats_s *)data;

     BIND_INT( statement,  1, team_game_stats->team_id   );
     BIND_INT( statement,  2, team_game_stats->season    );
     BIND_INT( statement,  3, team_game_stats->week      );
     BIND_INT( statement,  4, team_game_stats->game      );

     return SQLITE_OK;
}

static int team_game_stats_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     team_game_stats_s *team_game_stats = (team_game_stats_s *)data;

     team_game_stats->wins           = sqlite3_column_int( statement,  0 );
     team_game_stats->losses         = sqlite3_column_int( statement,  1 );
     team_game_stats->ties           = sqlite3_column_int( statement,  2 );
     team_game_stats->home_wins      = sqlite3_column_int( statement,  3 );
     team_game_stats->home_losses    = sqlite3_column_int( statement,  4 );
     team_game_stats->home_ties      = sqlite3_column_int( statement,  5 );
     team_game_stats->road_wins      = sqlite3_column_int( statement,  6 );
     team_game_stats->road_losses    = sqlite3_column_int( statement,  7 );
     team_game_stats->road_ties      = sqlite3_column_int( statement,  8 );
     team_game_stats->points_scored  = sqlite3_column_int( statement,  9 );
     team_game_stats->points_allowed = sqlite3_column_int( statement, 10 );

     return SQLITE_OK;
}

int team_game_stats_t_read( sqlite3 *db, team_game_stats_s *team_game_stats )
{
     static char query[] = "SELECT Wins, Losses, Ties, Home_Wins, Home_Losses, Home_Ties, Road_Wins, Road_Losses, Road_Ties, Points_Scored, Points_Allowed "
          /**/               "FROM   Team_Game_Stats_T "
          /**/               "WHERE  Team_Id   = ? "
          /**/               "AND    Season    = ? "
          /**/               "AND    Week      = ? "
          /**/               "AND    Game      = ? ";

     return execute_query( db, query, team_game_stats_t_read_bindings, team_game_stats, team_game_stats_t_read_retrieve, team_game_stats );
}

// By Team:
static int team_game_stats_t_read_by_team_bindings( sqlite3_stmt *statement, const void *data )
{
     const int *team_id = (int *)data;

     return sqlite3_bind_int( statement, 1, *team_id );
}

static int team_game_stats_t_read_by_team_retrieve( sqlite3_stmt *statement, const void *data )
{
     data_list_s *data_list = (data_list_s *)data;

     team_game_stats_s team_game_stats = { 0 };

     team_game_stats.team_id        = sqlite3_column_int( statement,  0 );
     team_game_stats.season         = sqlite3_column_int( statement,  1 );
     team_game_stats.week           = sqlite3_column_int( statement,  2 );
     team_game_stats.game           = sqlite3_column_int( statement,  3 );
     team_game_stats.wins           = sqlite3_column_int( statement,  4 );
     team_game_stats.losses         = sqlite3_column_int( statement,  5 );
     team_game_stats.ties           = sqlite3_column_int( statement,  6 );
     team_game_stats.home_wins      = sqlite3_column_int( statement,  7 );
     team_game_stats.home_losses    = sqlite3_column_int( statement,  8 );
     team_game_stats.home_ties      = sqlite3_column_int( statement,  9 );
     team_game_stats.road_wins      = sqlite3_column_int( statement, 10 );
     team_game_stats.road_losses    = sqlite3_column_int( statement, 11 );
     team_game_stats.road_ties      = sqlite3_column_int( statement, 12 );
     team_game_stats.points_scored  = sqlite3_column_int( statement, 13 );
     team_game_stats.points_allowed = sqlite3_column_int( statement, 14 );

     if ( add_to_data_list( data_list, &team_game_stats, sizeof(team_game_stats_s), 10 ) < 0 ) return SQLITE_ERROR;

     return SQLITE_OK;
}

int team_game_stats_t_read_by_team( sqlite3 *db, const int team_id, data_list_s *team_game_stats )
{
     static char query[] = "SELECT Team_Id, Season, Week, Game, Wins, Losses, Ties, Home_Wins, Home_Losses, Home_Ties, Road_Wins, Road_Losses, Road_Ties, Points_Scored, Points_Allowed FROM Team_Game_Stats_T WHERE Team_Id = ?";

     return execute_query( db, query, team_game_stats_t_read_by_team_bindings, &team_id, team_game_stats_t_read_by_team_retrieve, team_game_stats );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_game_stats_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_game_stats_s *team_game_stats = (team_game_stats_s *)data;

     BIND_INT(  statement,  1, team_game_stats->wins           );
     BIND_INT(  statement,  2, team_game_stats->losses         );
     BIND_INT(  statement,  3, team_game_stats->ties           );
     BIND_INT(  statement,  4, team_game_stats->home_wins      );
     BIND_INT(  statement,  5, team_game_stats->home_losses    );
     BIND_INT(  statement,  6, team_game_stats->home_ties      );
     BIND_INT(  statement,  7, team_game_stats->road_wins      );
     BIND_INT(  statement,  8, team_game_stats->road_losses    );
     BIND_INT(  statement,  9, team_game_stats->road_ties      );
     BIND_INT(  statement, 10, team_game_stats->points_scored  );
     BIND_INT(  statement, 11, team_game_stats->points_allowed );

     BIND_INT(  statement, 12, team_game_stats->team_id   );
     BIND_INT(  statement, 13, team_game_stats->season    );
     BIND_INT(  statement, 14, team_game_stats->week      );
     BIND_INT(  statement, 15, team_game_stats->game      );

     return SQLITE_OK;
}


int team_game_stats_t_update( sqlite3 *db, const team_game_stats_s *team_game_stats )
{
     static char query[]   = "UPDATE Team_Game_Stats_T "
          /**/
          /**/               "SET    Wins           = ?,"
          /**/                      "Losses         = ?,"
          /**/                      "Ties           = ?,"
          /**/                      "Home_Wins      = ?,"
          /**/                      "Home_Losses    = ?,"
          /**/                      "Home_Ties      = ?,"
          /**/                      "Road_Wins      = ?,"
          /**/                      "Road_Losses    = ?,"
          /**/                      "Road_Ties      = ?,"
          /**/                      "Points_Scored  = ?,"
          /**/                      "Points_Allowed = ? "
          /**/
          /**/               "WHERE  Team_Id   = ? "
          /**/               "AND    Season    = ? "
          /**/               "AND    Week      = ? "
          /**/               "AND    Game      = ? ";

     return execute_update_old( db, query, team_game_stats_t_update_bindings, team_game_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int team_game_stats_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const team_game_stats_s *team_game_stats = (team_game_stats_s *)data;

     BIND_INT( statement,  1, team_game_stats->team_id   );
     BIND_INT( statement,  2, team_game_stats->season    );
     BIND_INT( statement,  3, team_game_stats->week      );
     BIND_INT( statement,  4, team_game_stats->game      );

     return SQLITE_OK;
}


int team_game_stats_t_delete( sqlite3 *db, const team_game_stats_s *team_game_stats )
{
     return execute_update_old( db, "DELETE FROM Team_Game_Stats_T WHERE Team_Id = ? AND Season = ? AND Week = ? AND Game = ?", team_game_stats_t_delete_bindings, team_game_stats, NULL, NULL );
}
