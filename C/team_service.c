#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "team.h"


#define TRY( Func ) if ( (rc = (Func)) != SQLITE_OK ) return rc
#define INSERT_IF_UNIQUE( Func ) if ( (rc = (Func)) != SQLITE_OK  &&  rc != SQLITE_CONSTRAINT ) return rc


static int upsert_team_kicking_stats( sqlite3 *db, const team_kicking_stats_s *team_kicking_stats )
{
     int rc;

     if ( (rc = team_kicking_stats_t_create( db, team_kicking_stats )) == SQLITE_CONSTRAINT )
     {
          return team_kicking_stats_t_update( db, team_kicking_stats );
     }

     return rc;
}

static int save_team_kicking_stats( sqlite3 *db, const team_kicking_stats_s *team_kicking_stats )
{
     int rc;

     if ( team_kicking_stats == NULL ) return SQLITE_OK;

     for ( int i = 0; team_kicking_stats[i].team_id >= 0; ++i )
     {
          TRY( upsert_team_kicking_stats( db, &team_kicking_stats[i] ) );
     }

     return SQLITE_OK;
}

static int upsert_team_defense_stats( sqlite3 *db, const team_defense_stats_s *team_defense_stats )
{
     int rc;

     if ( (rc = team_defense_stats_t_create( db, team_defense_stats )) == SQLITE_CONSTRAINT )
     {
          return team_defense_stats_t_update( db, team_defense_stats );
     }

     return rc;
}

static int save_team_defense_stats( sqlite3 *db, const team_defense_stats_s *team_defense_stats )
{
     int rc;

     if ( team_defense_stats == NULL ) return SQLITE_OK;

     for ( int i = 0; team_defense_stats[i].team_id >= 0; ++i )
     {
          TRY( upsert_team_defense_stats( db, &team_defense_stats[i] ) );
     }

     return SQLITE_OK;
}

static int upsert_team_offense_stats( sqlite3 *db, const team_offense_stats_s *team_offense_stats )
{
     int rc;

     if ( (rc = team_offense_stats_t_create( db, team_offense_stats )) == SQLITE_CONSTRAINT )
     {
          return team_offense_stats_t_update( db, team_offense_stats );
     }

     return rc;
}

static int save_team_offense_stats( sqlite3 *db, const team_offense_stats_s *team_offense_stats )
{
     int rc;

     if ( team_offense_stats == NULL ) return SQLITE_OK;

     for ( int i = 0; team_offense_stats[i].team_id >= 0; ++i )
     {
          TRY( upsert_team_offense_stats( db, &team_offense_stats[i] ) );
     }

     return SQLITE_OK;
}

static int upsert_team_stats( sqlite3 *db, const team_stats_s *team_stats )
{
     int rc;

     if ( (rc = team_stats_t_create( db, team_stats )) == SQLITE_CONSTRAINT )
     {
          return team_stats_t_update( db, team_stats );
     }

     return rc;
}

static int save_team_stats( sqlite3 *db, const team_stats_s *team_stats )
{
     int rc;

     if ( team_stats == NULL ) return SQLITE_OK;

     for ( int i = 0; team_stats[i].team_id >= 0; ++i )
     {
          TRY( upsert_team_stats( db, &team_stats[i] ) );
     }

     return SQLITE_OK;
}

static int save_team_accolades( sqlite3 *db, const team_accolade_s *team_accolades )
{
     int rc;

     if ( team_accolades == NULL ) return SQLITE_OK;

     for ( int i = 0; team_accolades[i].team_id >= 0; ++i )
     {
          INSERT_IF_UNIQUE( team_accolades_t_create( db, &team_accolades[i] ) );
     }

     return SQLITE_OK;
}

int save_team( sqlite3 *db, const team_s *team )
{
     int rc;

     TRY( save_team_accolades(     db, team->accolades     ) );
     TRY( save_team_stats(         db, team->stats         ) );
     TRY( save_team_offense_stats( db, team->offense_stats ) );
     TRY( save_team_defense_stats( db, team->defense_stats ) );
     TRY( save_team_kicking_stats( db, team->kicking_stats ) );

     return SQLITE_OK;
}


static team_player_s *get_team_players_for_season( sqlite3 *db, const int team_id, const int season )
{
     static team_player_s sentinel = TEAM_PLAYER_SENTINEL;

     data_list_s list = { 0 };

     team_player_s team_player = { 0 };

     team_player.team_id = team_id;
     team_player.season  = season;

     if ( team_players_t_read_by_team_and_season( db, &team_player, &list ) != SQLITE_OK ) return NULL;

     if ( add_to_data_list( &list, &sentinel, sizeof(team_player_s), 10 ) < 0 )
     {
          free( list.data );

          return NULL;
     }

     return list.data;
}

static team_player_s *get_team_players( sqlite3 *db, const int team_id )
{
     static team_player_s sentinel = TEAM_PLAYER_SENTINEL;

     data_list_s list = { 0 };

     if ( team_players_t_read_by_team( db, team_id, &list ) != SQLITE_OK ) return NULL;

     if ( add_to_data_list( &list, &sentinel, sizeof(team_player_s), 10 ) < 0 )
     {
          free( list.data );

          return NULL;
     }

     return list.data;
}

static team_s *get_team_details( sqlite3 *db, const int team_id )
{
     team_s *team = NULL;

     if ( (team = malloc( sizeof(team_s) )) == NULL ) return NULL;

     memset( team, '\0', sizeof(team_s) );

     team->team_id = team_id;

     if ( teams_t_read( db, team ) != SQLITE_OK )
     {
          free( team );

          return NULL;
     }

     return team;
}

team_s *get_team_for_season( sqlite3 *db, const int team_id, const int season )
{
     team_s *team = NULL;

     if ( (team = get_team_details( db, team_id )) == NULL ) return NULL;

     team->players = get_team_players_for_season( db, team_id, season );

     return team;
}

team_s *get_team( sqlite3 *db, const int team_id )
{
     team_s *team = NULL;

     if ( (team = get_team_details( db, team_id )) == NULL ) return NULL;

     team->players = get_team_players( db, team_id );

     return team;
}


void free_team_players( team_player_s *team_players )
{
     for ( int i = 0; team_players[i].team_id > 0; ++i )
     {
          free_player( team_players[i].player );
     }

     free( team_players );
}

void free_team( team_s *team )
{
     if ( team == NULL ) return;

     if ( team->players ) free_team_players( team->players );

     free( team );
}
