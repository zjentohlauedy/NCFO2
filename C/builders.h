#ifndef __INC_BUILDERS_H__
#define __INC_BUILDERS_H__

#include "organization.h"
#include "conference.h"

organization_s *buildOrganization(                                   const int organization_id );
void            buildIntoOrganization( organization_s *organization, const int organization_id );

conference_s *buildConference(                               const int conference_id );
void          buildIntoConference( conference_s *conference, const int conference_id );

conference_team_s *buildConferenceTeam(                                    const int conference_id, const int team_id );
void               buildIntoConferenceTeam( conference_team_s *conference, const int conference_id, const int team_id );

#endif
