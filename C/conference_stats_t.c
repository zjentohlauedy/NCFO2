#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "conference.h"
#include "sql_query.h"


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ CREATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int conference_stats_t_create_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const conference_stats_s *conference_stats = (conference_stats_s *)data;

     BIND_INT( statement,  1, conference_stats->conference_id   );
     BIND_INT( statement,  2, conference_stats->season          );
     BIND_INT( statement,  3, conference_stats->bowl_game       );
     BIND_INT( statement,  4, conference_stats->wins            );
     BIND_INT( statement,  5, conference_stats->losses          );
     BIND_INT( statement,  6, conference_stats->home_wins       );
     BIND_INT( statement,  7, conference_stats->home_losses     );
     BIND_INT( statement,  8, conference_stats->road_wins       );
     BIND_INT( statement,  9, conference_stats->road_losses     );
     BIND_INT( statement, 10, conference_stats->points_scored   );
     BIND_INT( statement, 11, conference_stats->points_allowed  );

     return SQLITE_OK;
}


int conference_stats_t_create( sqlite3 *db, const conference_stats_s *conference_stats )
{
     static char query[]   = "INSERT INTO Conference_Stats_T ( Conference_Id, Season, Bowl_Game, Wins, Losses, Home_Wins, Home_Losses, Road_Wins, Road_Losses, Points_Scored, Points_Allowed )"
          /**/                                       "VALUES ( ?,"           "?,"    "?,"       "?,"  "?,"    "?,"       "?,"         "?,"       "?,"         "?,"           "?"            ")";

     return execute_update_old( db, query, conference_stats_t_create_bindings, conference_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ READ ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

// By Key:
static int conference_stats_t_read_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const conference_stats_s *conference_stats = (conference_stats_s *)data;

     BIND_INT( statement,  1, conference_stats->conference_id );
     BIND_INT( statement,  2, conference_stats->season        );
     BIND_INT( statement,  3, conference_stats->bowl_game     );

     return SQLITE_OK;
}

static int conference_stats_t_read_retrieve( sqlite3_stmt *statement, const void *data )
{
     conference_stats_s *conference_stats = (conference_stats_s *)data;

     conference_stats->wins           = sqlite3_column_int( statement, 0 );
     conference_stats->losses         = sqlite3_column_int( statement, 1 );
     conference_stats->home_wins      = sqlite3_column_int( statement, 2 );
     conference_stats->home_losses    = sqlite3_column_int( statement, 3 );
     conference_stats->road_wins      = sqlite3_column_int( statement, 4 );
     conference_stats->road_losses    = sqlite3_column_int( statement, 5 );
     conference_stats->points_scored  = sqlite3_column_int( statement, 6 );
     conference_stats->points_allowed = sqlite3_column_int( statement, 7 );

     return SQLITE_OK;
}

int conference_stats_t_read( sqlite3 *db, conference_stats_s *conference_stats )
{
     static char query[]   = "SELECT Wins, Losses, Home_Wins, Home_Losses, Road_Wins, Road_Losses, Points_Scored, Points_Allowed "
          /**/               "FROM   Conference_Stats_T "
          /**/               "WHERE  Conference_Id  = ? "
          /**/               "AND    Season         = ? "
          /**/               "AND    Bowl_Game      = ? ";

     return execute_query( db, query, conference_stats_t_read_bindings, conference_stats, conference_stats_t_read_retrieve, conference_stats );
}

// By Conference:
static int conference_stats_t_read_by_conference_bindings( sqlite3_stmt *statement, const void *data )
{
     const int *conference_id = (int *)data;

     return sqlite3_bind_int( statement, 1, *conference_id );
}

static int conference_stats_t_read_by_conference_retrieve( sqlite3_stmt *statement, const void *data )
{
     data_list_s *data_list = (data_list_s *)data;

     conference_stats_s conference_stats = { 0 };

     conference_stats.conference_id  = sqlite3_column_int( statement,  0 );
     conference_stats.season         = sqlite3_column_int( statement,  1 );
     conference_stats.bowl_game      = sqlite3_column_int( statement,  2 );
     conference_stats.wins           = sqlite3_column_int( statement,  3 );
     conference_stats.losses         = sqlite3_column_int( statement,  4 );
     conference_stats.home_wins      = sqlite3_column_int( statement,  5 );
     conference_stats.home_losses    = sqlite3_column_int( statement,  6 );
     conference_stats.road_wins      = sqlite3_column_int( statement,  7 );
     conference_stats.road_losses    = sqlite3_column_int( statement,  8 );
     conference_stats.points_scored  = sqlite3_column_int( statement,  9 );
     conference_stats.points_allowed = sqlite3_column_int( statement, 10 );

     if ( add_to_data_list( data_list, &conference_stats, sizeof(conference_stats_s), 10 ) < 0 ) return SQLITE_ERROR;

     return SQLITE_OK;
}

int conference_stats_t_read_by_conference( sqlite3 *db, const int conference_id, data_list_s *conference_stats )
{
     static char query[]   = "SELECT Conference_Id, Season, Bowl_Game, Wins, Losses, Home_Wins, Home_Losses, Road_Wins, Road_Losses, Points_Scored, Points_Allowed FROM   Conference_Stats_T WHERE  Conference_Id  = ? ";

     return execute_query( db, query, conference_stats_t_read_by_conference_bindings, &conference_id, conference_stats_t_read_by_conference_retrieve, conference_stats );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ UPDATE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int conference_stats_t_update_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const conference_stats_s *conference_stats = (conference_stats_s *)data;

     BIND_INT(  statement,  1, conference_stats->wins           );
     BIND_INT(  statement,  2, conference_stats->losses         );
     BIND_INT(  statement,  3, conference_stats->home_wins      );
     BIND_INT(  statement,  4, conference_stats->home_losses    );
     BIND_INT(  statement,  5, conference_stats->road_wins      );
     BIND_INT(  statement,  6, conference_stats->road_losses    );
     BIND_INT(  statement,  7, conference_stats->points_scored  );
     BIND_INT(  statement,  8, conference_stats->points_allowed );

     BIND_INT(  statement,  9, conference_stats->conference_id );
     BIND_INT(  statement, 10, conference_stats->season        );
     BIND_INT(  statement, 11, conference_stats->bowl_game     );

     return SQLITE_OK;
}


int conference_stats_t_update( sqlite3 *db, const conference_stats_s *conference_stats )
{
     static char query[]   = "UPDATE Conference_Stats_T "
          /**/
          /**/               "SET    Wins            = ?,"
          /**/                      "Losses          = ?,"
          /**/                      "Home_Wins       = ?,"
          /**/                      "Home_Losses     = ?,"
          /**/                      "Road_Wins       = ?,"
          /**/                      "Road_Losses     = ?,"
          /**/                      "Points_Scored   = ?,"
          /**/                      "Points_Allowed  = ? "
          /**/
          /**/               "WHERE  Conference_Id = ? "
          /**/               "AND    Season        = ? "
          /**/               "AND    Bowl_Game     = ? ";

     return execute_update_old( db, query, conference_stats_t_update_bindings, conference_stats, NULL, NULL );
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ DELETE ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static int conference_stats_t_delete_bindings( sqlite3_stmt *statement, const void *data )
{
     int rc;

     const conference_stats_s *conference_stats = (conference_stats_s *)data;

     BIND_INT( statement,  1, conference_stats->conference_id );
     BIND_INT( statement,  2, conference_stats->season        );
     BIND_INT( statement,  3, conference_stats->bowl_game     );

     return SQLITE_OK;
}


int conference_stats_t_delete( sqlite3 *db, const conference_stats_s *conference_stats )
{
     return execute_update_old( db, "DELETE FROM Conference_Stats_T WHERE Conference_Id = ? AND Season = ? AND Bowl_Game = ?", conference_stats_t_delete_bindings, conference_stats, NULL, NULL );
}
