#ifndef __INC_COMPARES_H__
#define __INC_COMPARES_H__

#include "organization.h"
#include "conference.h"

#define compareOrganizations( A, B ) do { char *ret; if ( (ret = _compareOrganizations( (A), (B) )) != NULL ) return ret; } while ( 0 )
char  *_compareOrganizations( organization_s *expected, organization_s *actual );

#define compareConferences( A, B ) do { char *ret; if ( (ret = _compareConferences( (A), (B) )) != NULL ) return ret; } while ( 0 )
char  *_compareConferences( conference_s *expected, conference_s *actual );

#define compareConferenceTeams( A, B ) do { char *ret; if ( (ret = _compareConferenceTeams( (A), (B) )) != NULL ) return ret; } while ( 0 )
char  *_compareConferenceTeams( conference_team_s *expected, conference_team_s *actual );

#define compareConferenceStats( A, B ) do { char *ret; if ( (ret = _compareConferenceStats( (A), (B) )) != NULL ) return ret; } while ( 0 )
char  *_compareConferenceStats( conference_stats_s *expected, conference_stats_s *actual );

#define compareConferenceAccolades( A, B ) do { char *ret; if ( (ret = _compareConferenceAccolades( (A), (B) )) != NULL ) return ret; } while ( 0 )
char  *_compareConferenceAccolades( conference_accolade_s *expected, conference_accolade_s *actual );

#endif
