#ifndef __INC_ORGANIZATION_H__
#define __INC_ORGANIZATION_H__

#include <sqlite3.h>
#include "data_list.h"
#include "conference.h"

#define ORGANIZATION_CONFERENCE_SENTINEL  { -1, -1 }

typedef struct
{
     int           organization_id;
     int           conference_id;
     conference_s *conference;

} organization_conference_s;

typedef struct
{
     int                        organization_id;
     char                       name            [40 + 1];
     char                       abbreviation    [ 4 + 1];
     int                        season;
     organization_conference_s *conferences;

} organization_s;

typedef struct
{
     int season;
     int week;
     int game;
     int road_team_id;
     int home_team_id;
} match_s;

int organizations_t_create(   sqlite3 *db, const organization_s    *organization  );
int organizations_t_read(     sqlite3 *db,       organization_s    *organization  );
int organizations_t_update(   sqlite3 *db, const organization_s    *organization  );
int organizations_t_delete(   sqlite3 *db, const organization_s    *organization  );

int organization_conferences_t_create(               sqlite3 *db,                            const organization_conference_s *organization_conference  );
int organization_conferences_t_read_by_organization( sqlite3 *db, const int organization_id,       data_list_s               *organization_conferences );
int organization_conferences_t_delete(               sqlite3 *db,                            const organization_conference_s *organization_conference  );

int matches_t_create( sqlite3 *db, const match_s *match );
int matches_t_read(   sqlite3 *db,       match_s *match );
int matches_t_update( sqlite3 *db, const match_s *match );
int matches_t_delete( sqlite3 *db, const match_s *match );

organization_s *get_organization(  sqlite3 *db, const int             organization_id );
int             save_organization( sqlite3 *db, const organization_s *organization    );
void            free_organization(                    organization_s *organization    );

#endif
