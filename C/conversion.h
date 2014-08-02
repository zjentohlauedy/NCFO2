#ifndef _INC_CONVERSION_H_
#define _INC_CONVERSION_H_

#include "organization.h"
#include "file_formats.h"
#include "save_state.h"

char *getConvertOrganizationError( void );
organization_s *convertOrganization(
     const tsbrom_s         *rom1,
     const nst_save_state_s *save_state1,
     const tsbrom_s         *rom2,
     const nst_save_state_s *save_state2,
     const int               season,
     const bowl_game_e       bowl
     );

#endif
