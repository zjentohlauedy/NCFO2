#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "player.h"


#define TRY( Func ) if ( (rc = (Func)) != SQLITE_OK ) return rc
#define INSERT_IF_UNIQUE( Func ) if ( (rc = (Func)) != SQLITE_OK  &&  rc != SQLITE_CONSTRAINT ) return rc


static int upsert_player_returns_stats( sqlite3 *db, const player_returns_stats_s *player_returns_stats )
{
     int rc;

     if ( (rc = player_returns_stats_t_create( db, player_returns_stats )) == SQLITE_CONSTRAINT )
     {
          return player_returns_stats_t_update( db, player_returns_stats );
     }

     return rc;
}

static int save_player_returns_stats( sqlite3 *db, const player_returns_stats_s *player_returns_stats )
{
     int rc;

     if ( player_returns_stats == NULL ) return SQLITE_OK;

     for ( int i = 0; player_returns_stats[i].player_id >= 0; ++i )
     {
          TRY( upsert_player_returns_stats( db, &player_returns_stats[i] ) );
     }

     return SQLITE_OK;
}

static int upsert_player_kicking_stats( sqlite3 *db, const player_kicking_stats_s *player_kicking_stats )
{
     int rc;

     if ( (rc = player_kicking_stats_t_create( db, player_kicking_stats )) == SQLITE_CONSTRAINT )
     {
          return player_kicking_stats_t_update( db, player_kicking_stats );
     }

     return rc;
}

static int save_player_kicking_stats( sqlite3 *db, const player_kicking_stats_s *player_kicking_stats )
{
     int rc;

     if ( player_kicking_stats == NULL ) return SQLITE_OK;

     for ( int i = 0; player_kicking_stats[i].player_id >= 0; ++i )
     {
          TRY( upsert_player_kicking_stats( db, &player_kicking_stats[i] ) );
     }

     return SQLITE_OK;
}

static int upsert_player_defense_stats( sqlite3 *db, const player_defense_stats_s *player_defense_stats )
{
     int rc;

     if ( (rc = player_defense_stats_t_create( db, player_defense_stats )) == SQLITE_CONSTRAINT )
     {
          return player_defense_stats_t_update( db, player_defense_stats );
     }

     return rc;
}

static int save_player_defense_stats( sqlite3 *db, const player_defense_stats_s *player_defense_stats )
{
     int rc;

     if ( player_defense_stats == NULL ) return SQLITE_OK;

     for ( int i = 0; player_defense_stats[i].player_id >= 0; ++i )
     {
          TRY( upsert_player_defense_stats( db, &player_defense_stats[i] ) );
     }

     return SQLITE_OK;
}

static int upsert_player_offense_stats( sqlite3 *db, const player_offense_stats_s *player_offense_stats )
{
     int rc;

     if ( (rc = player_offense_stats_t_create( db, player_offense_stats )) == SQLITE_CONSTRAINT )
     {
          return player_offense_stats_t_update( db, player_offense_stats );
     }

     return rc;
}

static int save_player_offense_stats( sqlite3 *db, const player_offense_stats_s *player_offense_stats )
{
     int rc;

     if ( player_offense_stats == NULL ) return SQLITE_OK;

     for ( int i = 0; player_offense_stats[i].player_id >= 0; ++i )
     {
          TRY( upsert_player_offense_stats( db, &player_offense_stats[i] ) );
     }

     return SQLITE_OK;
}

static int save_player_accolades( sqlite3 *db, const player_accolade_s *player_accolades )
{
     int rc;

     if ( player_accolades == NULL ) return SQLITE_OK;

     for ( int i = 0; player_accolades[i].player_id >= 0; ++i )
     {
          INSERT_IF_UNIQUE( player_accolades_t_create( db, &player_accolades[i] ) );
     }

     return SQLITE_OK;
}

int save_player( sqlite3 *db, const player_s *player )
{
     int rc;

     TRY( save_player_offense_stats( db, player->stats.offense ) );
     TRY( save_player_defense_stats( db, player->stats.defense ) );
     TRY( save_player_kicking_stats( db, player->stats.kicking ) );
     TRY( save_player_returns_stats( db, player->stats.returns ) );

     return save_player_accolades( db, player->accolades );
}


static player_kicking_ratings_s *get_player_kicking_ratings( sqlite3 *db, const int player_id )
{
     player_kicking_ratings_s *player_kicking_ratings = NULL;

     if ( (player_kicking_ratings = malloc( sizeof(player_kicking_ratings_s) )) == NULL ) return NULL;

     memset( player_kicking_ratings, '\0', sizeof(player_kicking_ratings_s) );

     player_kicking_ratings->player_id = player_id;

     if ( player_kicking_ratings_t_read( db, player_kicking_ratings ) != SQLITE_OK )
     {
          free( player_kicking_ratings );

          return NULL;
     }

     return player_kicking_ratings;
}

static player_defense_ratings_s *get_player_defense_ratings( sqlite3 *db, const int player_id )
{
     player_defense_ratings_s *player_defense_ratings = NULL;

     if ( (player_defense_ratings = malloc( sizeof(player_defense_ratings_s) )) == NULL ) return NULL;

     memset( player_defense_ratings, '\0', sizeof(player_defense_ratings_s) );

     player_defense_ratings->player_id = player_id;

     if ( player_defense_ratings_t_read( db, player_defense_ratings ) != SQLITE_OK )
     {
          free( player_defense_ratings );

          return NULL;
     }

     return player_defense_ratings;
}

static player_offense_ratings_s *get_player_offense_ratings( sqlite3 *db, const int player_id )
{
     player_offense_ratings_s *player_offense_ratings = NULL;

     if ( (player_offense_ratings = malloc( sizeof(player_offense_ratings_s) )) == NULL ) return NULL;

     memset( player_offense_ratings, '\0', sizeof(player_offense_ratings_s) );

     player_offense_ratings->player_id = player_id;

     if ( player_offense_ratings_t_read( db, player_offense_ratings ) != SQLITE_OK )
     {
          free( player_offense_ratings );

          return NULL;
     }

     return player_offense_ratings;
}

static player_quarterback_ratings_s *get_player_quarterback_ratings( sqlite3 *db, const int player_id )
{
     player_quarterback_ratings_s *player_quarterback_ratings = NULL;

     if ( (player_quarterback_ratings = malloc( sizeof(player_quarterback_ratings_s) )) == NULL ) return NULL;

     memset( player_quarterback_ratings, '\0', sizeof(player_quarterback_ratings_s) );

     player_quarterback_ratings->player_id = player_id;

     if ( player_quarterback_ratings_t_read( db, player_quarterback_ratings ) != SQLITE_OK )
     {
          free( player_quarterback_ratings );

          return NULL;
     }

     return player_quarterback_ratings;
}

static player_ratings_s *get_player_ratings( sqlite3 *db, const int player_id )
{
     player_ratings_s *player_ratings = NULL;

     if ( (player_ratings = malloc( sizeof(player_ratings_s) )) == NULL ) return NULL;

     memset( player_ratings, '\0', sizeof(player_ratings_s) );

     player_ratings->player_id = player_id;

     if ( player_ratings_t_read( db, player_ratings ) != SQLITE_OK )
     {
          free( player_ratings );

          return NULL;
     }

     return player_ratings;
}

static player_s *get_player_details( sqlite3 *db, const int player_id )
{
     player_s *player = NULL;

     if ( (player = malloc( sizeof(player_s) )) == NULL ) return NULL;

     memset( player, '\0', sizeof(player_s) );

     player->player_id = player_id;

     if ( players_t_read( db, player ) != SQLITE_OK )
     {
          free( player );

          return NULL;
     }

     return player;
}

player_s *get_player( sqlite3 *db, const int player_id )
{
     player_s *player = NULL;

     if ( (player = get_player_details( db, player_id )) == NULL ) return NULL;

     player->ratings = get_player_ratings( db, player_id );

     switch ( player->position )
     {
     case pos_Quarterback:   player->extra_ratings.quarterback = get_player_quarterback_ratings( db, player_id ); break;

     case pos_Runningback:   player->extra_ratings.offense     = get_player_offense_ratings(     db, player_id ); break;
     case pos_WideReceiver:  player->extra_ratings.offense     = get_player_offense_ratings(     db, player_id ); break;
     case pos_TightEnd:      player->extra_ratings.offense     = get_player_offense_ratings(     db, player_id ); break;

     case pos_DefensiveLine: player->extra_ratings.defense     = get_player_defense_ratings(     db, player_id ); break;
     case pos_Linebacker:    player->extra_ratings.defense     = get_player_defense_ratings(     db, player_id ); break;
     case pos_Cornerback:    player->extra_ratings.defense     = get_player_defense_ratings(     db, player_id ); break;
     case pos_Safety:        player->extra_ratings.defense     = get_player_defense_ratings(     db, player_id ); break;

     case pos_Kicker:        player->extra_ratings.kicking     = get_player_kicking_ratings(     db, player_id ); break;
     case pos_Punter:        player->extra_ratings.kicking     = get_player_kicking_ratings(     db, player_id ); break;
     }

     return player;
}

void free_player( player_s *player )
{
     if ( player == NULL ) return;

     if ( player->ratings != NULL ) free( player->ratings );

     switch ( player->position )
     {
     case pos_Quarterback:   if ( player->extra_ratings.quarterback != NULL ) free( player->extra_ratings.quarterback ); break;

     case pos_Runningback:   if ( player->extra_ratings.offense     != NULL ) free( player->extra_ratings.offense     ); break;
     case pos_WideReceiver:  if ( player->extra_ratings.offense     != NULL ) free( player->extra_ratings.offense     ); break;
     case pos_TightEnd:      if ( player->extra_ratings.offense     != NULL ) free( player->extra_ratings.offense     ); break;

     case pos_DefensiveLine: if ( player->extra_ratings.defense     != NULL ) free( player->extra_ratings.defense     ); break;
     case pos_Linebacker:    if ( player->extra_ratings.defense     != NULL ) free( player->extra_ratings.defense     ); break;
     case pos_Cornerback:    if ( player->extra_ratings.defense     != NULL ) free( player->extra_ratings.defense     ); break;
     case pos_Safety:        if ( player->extra_ratings.defense     != NULL ) free( player->extra_ratings.defense     ); break;

     case pos_Kicker:        if ( player->extra_ratings.kicking     != NULL ) free( player->extra_ratings.kicking     ); break;
     case pos_Punter:        if ( player->extra_ratings.kicking     != NULL ) free( player->extra_ratings.kicking     ); break;
     }

     if ( player->accolades != NULL ) free( player->accolades );

     free( player );
}
