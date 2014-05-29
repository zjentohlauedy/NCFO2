#ifndef __INC_COMPARES_H__
#define __INC_COMPARES_H__

#include "organization.h"
#include "conference.h"
#include "team.h"
#include "player.h"

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

#define compareTeams( A, B ) do { char *ret; if ( (ret = _compareTeams( (A), (B) )) != NULL ) return ret; } while ( 0 )
char  *_compareTeams( team_s *expected, team_s *actual );

#define compareTeamPlayers( A, B ) do { char *ret; if ( (ret = _compareTeamPlayers( (A), (B) )) != NULL ) return ret; } while ( 0 )
char  *_compareTeamPlayers( team_player_s *expected, team_player_s *actual );

#define compareTeamStats( A, B ) do { char *ret; if ( (ret = _compareTeamStats( (A), (B) )) != NULL ) return ret; } while ( 0 )
char  *_compareTeamStats( team_stats_s *expected, team_stats_s *actual );

#define compareTeamOffenseStats( A, B ) do { char *ret; if ( (ret = _compareTeamOffenseStats( (A), (B) )) != NULL ) return ret; } while ( 0 )
char  *_compareTeamOffenseStats( team_offense_stats_s *expected, team_offense_stats_s *actual );

#define compareTeamDefenseStats( A, B ) do { char *ret; if ( (ret = _compareTeamDefenseStats( (A), (B) )) != NULL ) return ret; } while ( 0 )
char  *_compareTeamDefenseStats( team_defense_stats_s *expected, team_defense_stats_s *actual );

#define compareTeamKickingStats( A, B ) do { char *ret; if ( (ret = _compareTeamKickingStats( (A), (B) )) != NULL ) return ret; } while ( 0 )
char  *_compareTeamKickingStats( team_kicking_stats_s *expected, team_kicking_stats_s *actual );

#define compareTeamAccolades( A, B ) do { char *ret; if ( (ret = _compareTeamAccolades( (A), (B) )) != NULL ) return ret; } while ( 0 )
char  *_compareTeamAccolades( team_accolade_s *expected, team_accolade_s *actual );

#define comparePlayers( A, B ) do { char *ret; if ( (ret = _comparePlayers( (A), (B) )) != NULL ) return ret; } while ( 0 )
char  *_comparePlayers( player_s *expected, player_s *actual );

#define comparePlayerRatings( A, B ) do { char *ret; if ( (ret = _comparePlayerRatings( (A), (B) )) != NULL ) return ret; } while ( 0 )
char  *_comparePlayerRatings( player_ratings_s *expected, player_ratings_s *actual );

#endif
