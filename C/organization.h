#ifndef __INC_ORGANIZATION_H__
#define __INC_ORGANIZATION_H__

#include <sqlite3.h>
#include "data_list.h"

typedef struct
{
     int                organization_id;
     char               name            [40 + 1];
     char               abbreviation    [ 4 + 1];
     int                season;

} organization_s;

int organizations_t_create(   sqlite3 *db, const organization_s    *organization  );
int organizations_t_read(     sqlite3 *db,       organization_s    *organization  );
int organizations_t_update(   sqlite3 *db, const organization_s    *organization  );
int organizations_t_delete(   sqlite3 *db, const organization_s    *organization  );

#endif
