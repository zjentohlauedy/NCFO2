#ifndef _INC_SAVE_STATE_H_
#define _INC_SAVE_STATE_H_


typedef enum
{
     ni_Injured = 0,
     ni_Healthy = 3

} nst_injury_e;

typedef enum
{
     nc_Bad       = 0,
     nc_Average   = 1,
     nc_Good      = 2,
     nc_Excellent = 3

} nst_condition_e;


typedef struct
{
     unsigned char road [ 1 ];
     unsigned char home [ 1 ];

} nst_matchup_s;

/*
** pass attempts - 10 bits
** completions - 10 bits
** pass td - 6 bits
** pass int - 6 bits
** pass yards - 13 bits
** rush attempts - 8 bits
** rush yards - 11 bits
** rush td - 6 bits
*/

typedef struct
{
     unsigned char pass_attempts             [ 1 ]; // lowest 8 bits of pass att highest 2 in modifier
     unsigned char pass_completions          [ 1 ]; // lowest 8 bits of comp highest 2 in modifier
     unsigned char pass_attempts_modifier    [ 1 ]; // bits 1 & 2 = highest 2 bits of pass att; bits 3-8 = pass td
     unsigned char pass_completions_modifier [ 1 ]; // bits 1 & 2 = highest 2 bits of comp; bits 3-8 = pass int
     unsigned char pass_yards                [ 1 ]; // lowest 8 bits of pass yards
     unsigned char rush_attempts             [ 1 ];
     unsigned char rush_yards                [ 1 ]; // lowest 8 bits of rush yards
     unsigned char yards_modifiers           [ 1 ]; // bits 1-3 = highest 3 bits of rush yards; bits 4-8 = highest 5 bits of pass yards
     unsigned char rush_touchdowns           [ 1 ]; // bits 1 & 2 - ignored; bits 3-8 = rush td

} nst_quarterback_s;

/*
** rush attempts - 8 bits
** rush yards - 12 bits
** rush td - 6 bits
** receptions - 8 bits
** rec yards - 12 bits
** rec td - 6 bits
** kick returns - 7 bits
** kr yards - 11 bits
** kr td - 5 bits
** punt returns - 6 bits
** pr yards - 10 bits
** pr td - 4 bits
*/

typedef struct
{
     unsigned char receptions           [ 1 ];
     unsigned char receiving_yards      [ 1 ]; // lowest 8 bits of rec yards
     unsigned char pr_yards_modifier    [ 1 ]; // bits 1 & 2 = highest 2 bits of pr yards; bits 3-8 = rec td
     unsigned char kick_returns         [ 1 ]; // bit 1 ignored; bits 2-8 = kick returns
     unsigned char kr_yards             [ 1 ]; // lowest 8 bits of kr yards
     unsigned char kr_yards_modifier    [ 1 ]; // bits 1-3 = highest 3 bits of kr yards; bits 4-8 = kr td
     unsigned char rush_yards_modifier1 [ 1 ]; // bits 1 & 2 = highest 2 bits of rush yards; bits 3-8 = punt returns
     unsigned char pr_yards             [ 1 ]; // lowest 8 bits of pr yards
     unsigned char rec_yards_modifier   [ 1 ]; // bits 1-4 = highest 4 bits of rec yards; bits 5-8 = pr td
     unsigned char rush_attempts        [ 1 ];
     unsigned char rush_yards           [ 1 ]; // lowest 8 bits of rush yards
     unsigned char rush_yards_modifier2 [ 1 ]; // bits 1 & 2 = highest 2 bits of rush yards; bits 3-8 = rush td

} nst_offense_s;

/*
** sacks - 7 bits
** int - 5 bits
** ir yards - 9 bits
** ir td - 3 bits
*/

typedef struct
{
     unsigned char sacks         [ 1 ]; // bit 1 = highest bit of interceptions; bits 2-8 = sacks
     unsigned char interceptions [ 1 ]; // bit 1 = highest bit of ir yards; bits 2-4 = ir td; bits 5-8 = lowest 4 bits of ints
     unsigned char ir_yards      [ 1 ]; // lowest 8 bits of ir yards

} nst_defense_s;

typedef struct
{
     unsigned char xp_attempts [ 1 ];
     unsigned char xp_made     [ 1 ];
     unsigned char fg_attempts [ 1 ];
     unsigned char fg_made     [ 1 ];

} nst_kicker_s;

typedef struct
{
     unsigned char punts               [ 1 ];
     unsigned char punt_yards          [ 1 ]; // lowest 8 bits of punt yards
     unsigned char punt_yards_modifier [ 1 ]; // bits 1-4 = highest 4 bits of punt yards; bits 5-8 unused

} nst_punter_s;

typedef struct
{
     nst_quarterback_s quarterback [  2 ];
     nst_offense_s     offense     [ 10 ];
     nst_defense_s     defense     [ 11 ];
     nst_kicker_s      kicker      [  1 ];
     nst_punter_s      punter      [  1 ];

} nst_players_s;

typedef struct
{
     unsigned char wins               [ 1 ];
     unsigned char losses             [ 1 ];
     unsigned char ties               [ 1 ];
     unsigned char points_scored      [ 2 ];
     unsigned char points_allowed     [ 2 ];
     unsigned char pass_yards_allowed [ 2 ];
     unsigned char rush_yards_allowed [ 2 ];

} nst_teams_s;

typedef struct
{
     unsigned char rb2 : 2;
     unsigned char rb1 : 2;
     unsigned char qb2 : 2;
     unsigned char qb1 : 2;

     unsigned char wr2 : 2;
     unsigned char wr1 : 2;
     unsigned char rb4 : 2;
     unsigned char rb3 : 2;

     unsigned char te2 : 2;
     unsigned char te1 : 2;
     unsigned char wr4 : 2;
     unsigned char wr3 : 2;

} injury_bits_s;

typedef struct
{
     unsigned char rb2 : 2;
     unsigned char rb1 : 2;
     unsigned char qb2 : 2;
     unsigned char qb1 : 2;

     unsigned char wr2 : 2;
     unsigned char wr1 : 2;
     unsigned char rb4 : 2;
     unsigned char rb3 : 2;

     unsigned char te2 : 2;
     unsigned char te1 : 2;
     unsigned char wr4 : 2;
     unsigned char wr3 : 2;

     unsigned char ol4 : 2;
     unsigned char ol3 : 2;
     unsigned char ol2 : 2;
     unsigned char ol1 : 2;

     unsigned char dl3 : 2;
     unsigned char dl2 : 2;
     unsigned char dl1 : 2;
     unsigned char ol5 : 2;

     unsigned char lb4 : 2;
     unsigned char lb3 : 2;
     unsigned char lb2 : 2;
     unsigned char lb1 : 2;

     unsigned char s2  : 2;
     unsigned char s1  : 2;
     unsigned char cb2 : 2;
     unsigned char cb1 : 2;

     unsigned char k   : 2;
     unsigned char p   : 2;
     unsigned char end : 4;

} condition_bits_s;

typedef struct
{
     nst_players_s    player_stats [ 1 ];
     nst_teams_s      team_stats   [ 1 ];
     unsigned char    playbook     [ 4 ]; // Each hex nibble is a play in each play slot. The order is Run1, Run2 // Run3, Run4 // Pass1, Pass2 // Pass3, Pass4.
     unsigned char    starters     [ 4 ]; // Each hex nibble is a player where 0 = QB1, 1 = QB2, 2 = RB1, 3 = RB2, 4 = RB3, 5 = RB4, 6 = WR1, 7 = WR2, 8 = WR3, 9 = WR4, A = TE1, and B = TE2. The order goes QB, RB1 // RB2, WR1 // WR2, TE // KR, PR.
     injury_bits_s    injuries;           // Two bits per position (00 = healthy, 11 = injured) The order is QB1, QB2, RB1, RB2 // RB3, RB4, WR1, WR2 // WR3, WR4, TE1, TE2.
     condition_bits_s conditions;         // Two bits per position (00 = Bad, 01 = Average, 10 = Good, and 11 = Excellent) Order goes QB1, QB2, RB1, RB2 // RB3, RB4, WR1, WR2 // WR3, WR4, TE1, TE2 // C, LG, RG, LT // RT, RE, NT, LE // ROLB, RILB, LILB, LOLB // RCB, LCB, FS, SS // K, P. The final nibble always appears to be 5.

} nst_stats_s;

typedef struct
{
     unsigned char pass_attempts    [ 1 ];
     unsigned char pass_completions [ 1 ];
     unsigned char pass_touchdowns  [ 1 ];
     unsigned char interceptions    [ 1 ];
     unsigned char pass_yards       [ 2 ];
     unsigned char rush_attempts    [ 1 ];
     unsigned char rush_yards       [ 2 ];
     unsigned char rush_touchdowns  [ 1 ];

} nst_quarterback_game_s;

typedef struct
{
     unsigned char receptions           [ 1 ];
     unsigned char receiving_yards      [ 2 ];
     unsigned char receiving_touchdowns [ 1 ];
     unsigned char kick_returns         [ 1 ];
     unsigned char kr_yards             [ 2 ];
     unsigned char kr_touchdowns        [ 1 ];
     unsigned char punt_returns         [ 1 ];
     unsigned char pr_yards             [ 2 ];
     unsigned char pr_touchdowns        [ 1 ];
     unsigned char rush_attempts        [ 1 ];
     unsigned char rush_yards           [ 2 ];
     unsigned char rush_touchdowns      [ 1 ];

} nst_offense_game_s;

typedef struct
{
     unsigned char sacks             [ 1 ];
     unsigned char interceptions     [ 1 ];
     unsigned char return_yards      [ 2 ];
     unsigned char return_touchdowns [ 1 ];

} nst_defense_game_s;

typedef struct
{
     unsigned char xp_attempts [ 1 ];
     unsigned char xp_made     [ 1 ];
     unsigned char fg_attempts [ 1 ];
     unsigned char fg_made     [ 1 ];

} nst_kicker_game_s;

typedef struct
{
     unsigned char punts      [ 1 ];
     unsigned char punt_yards [ 2 ];

} nst_punter_game_s;

typedef struct
{
     nst_quarterback_game_s quarterback [  2 ];
     nst_offense_game_s     offense     [ 10 ];
     nst_defense_game_s     defense     [ 11 ];
     nst_kicker_game_s      kicker      [  1 ];
     nst_punter_game_s      punter      [  1 ];

} nst_game_stats_s;

typedef struct
{
     nst_matchup_s match;
     unsigned char road_score [ 1 ];
     unsigned char home_score [ 1 ];

} nst_playoff_s;

typedef struct
{
     unsigned char    unknown1      [ 1030 ]; // 0x0000 - 0x06bd
     nst_game_stats_s road_team     [    1 ]; // 0x0406 - 0x04f7
     unsigned char    unknown2      [   19 ]; // 0x04f8 - 0x050a
     nst_game_stats_s home_team     [    1 ]; // 0x050b - 0x05fc
     unsigned char    unknown3      [  193 ]; // 0x05fd - 0x06bd
     unsigned char    checksum      [    2 ]; // 0x06be - 0x06bf
     unsigned char    unknown4      [  152 ]; // 0x06c0 - 0x0757
     unsigned char    current_week  [    1 ]; // 0x0758 - 0x0758
     unsigned char    game_of_week  [    1 ]; // 0x0759 - 0x0759 - also doubles as the postseason game counter - week never changes
     nst_matchup_s    matches       [   14 ]; // 0x075a - 0x0775
     nst_playoff_s    playoffs      [   11 ]; // 0x0776 - 0x07a1
     unsigned char    playoff_teams [   12 ]; // 0x07a2 - 0x07ad
     nst_stats_s      stats1        [    9 ]; // 0x07ae - 0x0efd
     unsigned char    unknown6      [  260 ]; // 0x0efe - 0x1001 - For some reason the teams are broken up by this empty space
     nst_stats_s      stats2        [   19 ]; // 0x1002 - 0x1f71
     unsigned char    unknown7      [  142 ]; // 0x1f72 - 0x1fff

} nst_save_state_s;


unsigned char    *readNstSaveState(  const char *filename, int *save_state_size );
boolean_e         writeNstSaveState( const char *filename, const unsigned char *save_state, const size_t size );
nst_save_state_s *getSaveStateStats(                       const unsigned char *save_state, const size_t size );

#endif
