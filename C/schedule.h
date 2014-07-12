#ifndef _INC_SCHEDULE_H_
#define _INC_SCHEDULE_H_

#include "file_formats.h"

typedef struct
{
     int road;
     int home;

} game_s;

typedef struct
{
     game_s *games;

} week_s;

typedef struct
{
     week_s *weeks;

} schedule_s;

char *getGenerateScheduleError( void );

schedule_s *generateSchedule( tsbrom_s *rom1, tsbrom_s *rom2 );
void        freeSchedule( schedule_s *schedule );

boolean_e  writeSchedule( const char *filename, const schedule_s *schedule );

#endif
