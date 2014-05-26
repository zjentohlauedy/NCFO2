#ifndef __INC_CONFERENCE_H__
#define __INC_CONFERENCE_H__

#include <sqlite3.h>
#include "data_list.h"

typedef struct
{
     int     conference_id;
     int     team_id;

} conference_team_s;

typedef struct
{
     int                  conference_id;
     char                 name          [20 + 1];

} conference_s;


int conferences_t_create( sqlite3 *db, const conference_s *conference );
int conferences_t_read(   sqlite3 *db,       conference_s *conference );
int conferences_t_update( sqlite3 *db, const conference_s *conference );
int conferences_t_delete( sqlite3 *db, const conference_s *conference );

int conference_teams_t_create(             sqlite3 *db,                          const conference_team_s *conference_team  );
int conference_teams_t_read_by_conference( sqlite3 *db, const int conference_id,         data_list_s     *conference_teams );
int conference_teams_t_delete(             sqlite3 *db,                          const conference_team_s *conference_team  );

#endif
