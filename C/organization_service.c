#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "organization.h"


#define TRY( Func ) if ( (rc = (Func)) != SQLITE_OK ) return rc


static int save_players( sqlite3 *db, const team_player_s *team_players )
{
     int rc;

     if ( team_players == NULL ) return SQLITE_OK;

     for ( int i = 0; team_players[i].player != NULL; ++i )
     {
          TRY( save_player( db, team_players[i].player ) );
     }

     return SQLITE_OK;
}

static int save_teams( sqlite3 *db, const conference_team_s *conference_teams )
{
     int rc;

     if ( conference_teams == NULL ) return SQLITE_OK;

     for ( int i = 0; conference_teams[i].team != NULL; ++i )
     {
          TRY( save_team( db, conference_teams[i].team ) );

          TRY( save_players( db, conference_teams[i].team->players ) );
     }

     return SQLITE_OK;
}

static int save_conferences( sqlite3 *db, const organization_conference_s *organization_conferences )
{
     int rc;

     if ( organization_conferences == NULL ) return SQLITE_OK;

     for ( int i = 0; organization_conferences[i].conference != NULL; ++i )
     {
          TRY( save_conference( db, organization_conferences[i].conference ) );

          TRY( save_teams( db, organization_conferences[i].conference->teams ) );
     }

     return SQLITE_OK;
}

int save_organization( sqlite3 *db, const organization_s *organization )
{
     int rc;

     TRY( save_conferences( db, organization->conferences ) );

     return SQLITE_OK;
}


static void get_players( sqlite3 *db, team_player_s *team_players )
{
     if ( team_players == NULL ) return;

     for ( int i = 0; team_players[i].team_id > 0; ++i )
     {
          team_players[i].player = get_player( db, team_players[i].player_id );
     }
}

static void get_teams( sqlite3 *db, conference_team_s *conference_teams, const int season )
{
     if ( conference_teams == NULL ) return;

     for ( int i = 0; conference_teams[i].conference_id > 0; ++i )
     {
          conference_teams[i].team = get_team_for_season( db, conference_teams[i].team_id, season );

          get_players( db, conference_teams[i].team->players );
     }
}

static void get_conferences( sqlite3 *db, organization_conference_s *organization_conferences, const int season )
{
     if ( organization_conferences == NULL ) return;

     for ( int i = 0; organization_conferences[i].organization_id > 0; ++i )
     {
          organization_conferences[i].conference = get_conference( db, organization_conferences[i].conference_id );

          get_teams( db, organization_conferences[i].conference->teams, season );
     }
}

static organization_conference_s *get_organization_conferences( sqlite3 *db, const int organization_id )
{
     static organization_conference_s sentinel = ORGANIZATION_CONFERENCE_SENTINEL;

     data_list_s list = { 0 };

     if ( organization_conferences_t_read_by_organization( db, organization_id, &list ) != SQLITE_OK ) return NULL;

     if ( list.data == NULL ) return NULL;

     if ( add_to_data_list( &list, &sentinel, sizeof(organization_conference_s), 10 ) < 0 )
     {
          free( list.data );

          return NULL;
     }

     return list.data;
}

static organization_s *get_organization_details( sqlite3 *db, const int organization_id )
{
     organization_s *organization = NULL;

     if ( (organization = malloc( sizeof(organization_s) )) == NULL ) return NULL;

     memset( organization, '\0', sizeof(organization_s) );

     organization->organization_id = organization_id;

     if ( organizations_t_read( db, organization ) != SQLITE_OK )
     {
          free( organization );

          return NULL;
     }

     return organization;
}

organization_s *get_organization( sqlite3 *db, const int organization_id )
{
     organization_s *organization = NULL;

     if ( (organization = get_organization_details( db, organization_id )) == NULL ) return NULL;

     organization->conferences = get_organization_conferences( db, organization_id );

     get_conferences( db, organization->conferences, organization->season );

     return organization;
}


void free_organization_conferences( organization_conference_s *organization_conferences )
{
     for ( int i = 0; organization_conferences[i].organization_id > 0; ++i )
     {
          free_conference( organization_conferences[i].conference );
     }

     free( organization_conferences );
}

void free_organization( organization_s *organization )
{
     if ( organization == NULL ) return;

     if ( organization->conferences != NULL ) free_organization_conferences( organization->conferences );

     free( organization );
}
