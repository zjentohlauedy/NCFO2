#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "conference.h"


#define TRY( Func ) if ( (rc = (Func)) != SQLITE_OK ) return rc
#define INSERT_IF_UNIQUE( Func ) if ( (rc = (Func)) != SQLITE_OK  &&  rc != SQLITE_CONSTRAINT ) return rc


static conference_team_s *get_conference_teams( sqlite3 *db, const int conference_id )
{
     static conference_team_s sentinel = CONFERENCE_TEAM_SENTINEL;

     data_list_s list = { 0 };

     if ( conference_teams_t_read_by_conference( db, conference_id, &list ) != SQLITE_OK ) return NULL;

     if ( list.data == NULL ) return NULL;

     if ( add_to_data_list( &list, &sentinel, sizeof(conference_team_s), 10 ) < 0 )
     {
          free( list.data );

          return NULL;
     }

     return list.data;
}

static conference_s *get_conference_details( sqlite3 *db, const int conference_id )
{
     conference_s *conference = NULL;

     if ( (conference = malloc( sizeof(conference_s) )) == NULL ) return NULL;

     memset( conference, '\0', sizeof(conference_s) );

     conference->conference_id = conference_id;

     if ( conferences_t_read( db, conference ) != SQLITE_OK )
     {
          free( conference );

          return NULL;
     }

     return conference;
}

conference_s *get_conference( sqlite3 *db, const int conference_id )
{
     conference_s *conference = NULL;

     if ( (conference = get_conference_details( db, conference_id )) == NULL ) return NULL;

     conference->teams = get_conference_teams( db, conference_id );

     return conference;
}


static int upsert_conference_stats( sqlite3 *db, const conference_stats_s *conference_stats )
{
     int rc;

     if ( (rc = conference_stats_t_create( db, conference_stats )) == SQLITE_CONSTRAINT )
     {
          if ( (rc = conference_stats_t_update( db, conference_stats )) != SQLITE_OK ) return rc;
     }

     return rc;
}

static int save_conference_accolades( sqlite3 *db, const conference_accolade_s *conference_accolades )
{
     int rc;

     if ( conference_accolades == NULL ) return SQLITE_OK;

     for ( int i = 0; conference_accolades[i].conference_id >= 0; ++i )
     {
          INSERT_IF_UNIQUE( conference_accolades_t_create( db, &conference_accolades[i] ) );
     }

     return SQLITE_OK;
}

static int save_conference_stats( sqlite3 *db, const conference_stats_s *conference_stats )
{
     int rc;

     if ( conference_stats == NULL ) return SQLITE_OK;

     for ( int i = 0; conference_stats[i].conference_id >= 0; ++i )
     {
          TRY( upsert_conference_stats( db, &conference_stats[i] ) );
     }

     return SQLITE_OK;
}

int save_conference( sqlite3 *db, const conference_s *conference )
{
     int rc;

     if ( (rc = save_conference_stats(     db, conference->stats     )) != SQLITE_OK ) return rc;
     if ( (rc = save_conference_accolades( db, conference->accolades )) != SQLITE_OK ) return rc;

     return SQLITE_OK;
}


void free_conference( conference_s *conference )
{
     if ( conference->teams     != NULL ) free( conference->teams     );
     if ( conference->stats     != NULL ) free( conference->stats     );
     if ( conference->accolades != NULL ) free( conference->accolades );

     free( conference );
}
