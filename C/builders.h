#ifndef __INC_BUILDERS_H__
#define __INC_BUILDERS_H__

#include "organization.h"
#include "conference.h"
#include "team.h"
#include "player.h"

organization_s *buildOrganization(                                   const int organization_id );
void            buildIntoOrganization( organization_s *organization, const int organization_id );

conference_s *buildConference(                               const int conference_id );
void          buildIntoConference( conference_s *conference, const int conference_id );

conference_team_s *buildConferenceTeam(                                    const int conference_id, const int team_id );
void               buildIntoConferenceTeam( conference_team_s *conference, const int conference_id, const int team_id );

conference_stats_s *buildConferenceStats(                                    const int conference_id, const int season, const bowl_game_e bowl_game );
void               buildIntoConferenceStats( conference_stats_s *conference, const int conference_id, const int season, const bowl_game_e bowl_game );

conference_accolade_s *buildConferenceAccolade(                                        const int conference_id, const int season, const conference_accolade_e accolade );
void                   buildIntoConferenceAccolade( conference_accolade_s *conference, const int conference_id, const int season, const conference_accolade_e accolade );

team_s *buildTeam(                   const int team_id );
void    buildIntoTeam( team_s *team, const int team_id );

team_player_s *buildTeamPlayer(                          const int team_id, const int season, const int player_id );
void           buildIntoTeamPlayer( team_player_s *team, const int team_id, const int season, const int player_id );

team_stats_s *buildTeamStats(                         const int team_id, const int season, const bowl_game_e bowl_game );
void          buildIntoTeamStats( team_stats_s *team, const int team_id, const int season, const bowl_game_e bowl_game );

team_offense_stats_s *buildTeamOffenseStats(                                 const int team_id, const int season, const bowl_game_e bowl_game );
void                  buildIntoTeamOffenseStats( team_offense_stats_s *team, const int team_id, const int season, const bowl_game_e bowl_game );

team_defense_stats_s *buildTeamDefenseStats(                                 const int team_id, const int season, const bowl_game_e bowl_game );
void                  buildIntoTeamDefenseStats( team_defense_stats_s *team, const int team_id, const int season, const bowl_game_e bowl_game );

team_kicking_stats_s *buildTeamKickingStats(                                 const int team_id, const int season, const bowl_game_e bowl_game );
void                  buildIntoTeamKickingStats( team_kicking_stats_s *team, const int team_id, const int season, const bowl_game_e bowl_game );

team_accolade_s *buildTeamAccolade(                            const int team_id, const int season, const team_accolade_e accolade );
void             buildIntoTeamAccolade( team_accolade_s *team, const int team_id, const int season, const team_accolade_e accolade );

player_s *buildPlayer(                       const int player_id );
void      buildIntoPlayer( player_s *player, const int player_id );

player_ratings_s *buildPlayerRatings(                                       const int player_id );
void              buildIntoPlayerRatings( player_ratings_s *player_ratings, const int player_id );

player_quarterback_ratings_s *buildPlayerQuarterbackRatings(                                                               const int player_id );
void                          buildIntoPlayerQuarterbackRatings( player_quarterback_ratings_s *player_quarterback_ratings, const int player_id );

player_offense_ratings_s *buildPlayerOffenseRatings(                                                       const int player_id );
void                      buildIntoPlayerOffenseRatings( player_offense_ratings_s *player_offense_ratings, const int player_id );

player_defense_ratings_s *buildPlayerDefenseRatings(                                                       const int player_id );
void                      buildIntoPlayerDefenseRatings( player_defense_ratings_s *player_defense_ratings, const int player_id );

player_kicking_ratings_s *buildPlayerKickingRatings(                                                       const int player_id );
void                      buildIntoPlayerKickingRatings( player_kicking_ratings_s *player_kicking_ratings, const int player_id );

player_offense_stats_s *buildPlayerOffenseStats(                                     const int player_id, const int season, const bowl_game_e bowl_game );
void                    buildIntoPlayerOffenseStats( player_offense_stats_s *player, const int player_id, const int season, const bowl_game_e bowl_game );

player_defense_stats_s *buildPlayerDefenseStats(                                     const int player_id, const int season, const bowl_game_e bowl_game );
void                    buildIntoPlayerDefenseStats( player_defense_stats_s *player, const int player_id, const int season, const bowl_game_e bowl_game );

player_kicking_stats_s *buildPlayerKickingStats(                                     const int player_id, const int season, const bowl_game_e bowl_game );
void                    buildIntoPlayerKickingStats( player_kicking_stats_s *player, const int player_id, const int season, const bowl_game_e bowl_game );

#endif
