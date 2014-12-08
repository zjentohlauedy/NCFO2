#ifndef _INC_CONVERSION_H_
#define _INC_CONVERSION_H_

#include "organization.h"
#include "file_formats.h"
#include "save_state.h"


#define ROAD_TEAM_RAM_OFFSET   0x6c
#define HOME_TEAM_RAM_OFFSET   0x6d

#define ROAD_TEAM_SCORE_OFFSET 0x399
#define HOME_TEAM_SCORE_OFFSET 0x39e


typedef struct
{
     team_s *road_team;
     team_s *home_team;

     organization_s *_organization;

} bowl_game_s;


char *getConvertOrganizationError( void );
organization_s *convertOrganization(
     const tsbrom_s         *rom1,
     const nst_save_state_s *save_state1,
     const tsbrom_s         *rom2,
     const nst_save_state_s *save_state2,
     const int               season,
     const bowl_game_e       bowl
     );

char        *getConvertBowlGameError( void );
bowl_game_s *convertBowlGame( const tsbrom_s *rom, const nst_save_state_s *save_state, const int season, const bowl_game_e bowl );
void         copyScores( nst_save_state_s *save_state, const unsigned char *state_file );
void         freeBowlGame( bowl_game_s *bowl_game );

char        *getConvertAAGameError( void );
bowl_game_s *convertAAGame( const tsbrom_s *rom, const unsigned char *ram, const nst_save_state_s *save_state, const int season, const bowl_game_e bowl );

#endif
