#ifndef _INC_FILE_FORMATS_H_
#define _INC_FILE_FORMATS_H_

#include "bool.h"
#include "organization.h"

typedef enum
{
     ht_Two_Stripe   = 0x40,
     ht_One_Stripe   = 0x41,
     ht_Ram_Horn     = 0x42,
     ht_Tiger_Stripe = 0x43

} tsb_helmet_type_e;

typedef struct
{
     unsigned char identifier [ 4 ];
     unsigned char prg_banks  [ 1 ];
     unsigned char chr_banks  [ 1 ];
     unsigned char flags      [ 1 ];
     unsigned char filler     [ 9 ];

} nes_header_s;

typedef struct
{
     unsigned char value [ 2 ];

} nes_pointer_s;

typedef struct
{
     unsigned char number     [  1 ];
     unsigned char first_name [ 20 ];
     unsigned char last_name  [ 20 ];

} tsb_player_s;

typedef struct
{
     unsigned char ratings [ 2 ]; // [RP, RS], [MS, HP]
     unsigned char face    [ 1 ];

} tsb_player_ratings_s;

typedef struct
{
     tsb_player_ratings_s player;
     unsigned char        qb_ratings [ 2 ]; // [PS, PC ], [PA, APB]

} tsb_qb_ratings_s;

typedef struct
{
     tsb_player_ratings_s player;
     unsigned char        off_ratings [ 1 ]; // [BC, REC]

} tsb_off_ratings_s;

typedef struct
{
     tsb_player_ratings_s player;
     unsigned char        def_ratings [ 1 ]; // [PI, QU ]

} tsb_def_ratings_s;

typedef struct
{
     tsb_player_ratings_s player;
     unsigned char        kick_ratings [ 1 ]; // [KA, AKB]

} tsb_kick_ratings_s;

typedef struct
{
     tsb_qb_ratings_s     quarterback [  2 ];
     tsb_off_ratings_s    offense     [ 10 ];
     tsb_player_ratings_s linesmen    [  5 ];
     tsb_def_ratings_s    defense     [ 11 ];
     tsb_kick_ratings_s   kickers     [  2 ];

} tsb_ratings_team_s;

typedef struct
{
     unsigned char rushing [ 2 ];
     unsigned char passing [ 2 ];

} tsb_playbook_s;

typedef struct
{
     unsigned char defensive_speed [ 1 ];
     unsigned char offensive_speed [ 1 ];
     unsigned char interception    [ 1 ];
     unsigned char pass_control    [ 1 ];
     unsigned char reception       [ 1 ];

} tsb_com_juice_s;

typedef struct
{
     unsigned char road [ 1 ];
     unsigned char home [ 1 ];

} tsb_schedule_game_s;

typedef struct
{
     nes_pointer_s       week_pointers  [   17 ];
     unsigned char       games_per_week [   10 ];
     unsigned char       unused_weeks   [    7 ];
     unsigned char       separator      [    1 ];
     tsb_schedule_game_s games          [   10 ][ 12 ];
     unsigned char       filler         [ 5445 ];

} tsb_schedule_s;

typedef union
{
     unsigned char  bytes[ 5737 ];
     tsb_schedule_s ncfo;

} tsb_schedule_u;

typedef struct
{
     unsigned char quarterbacks      [ 2][2]; // [Rush, Pass], [0,   Pckt]
     unsigned char offense           [10][2]; // [Rush, Recv], [Yds, Targ]
     unsigned char defense_pass_rush [11];    // One byte for each player 0-255
     unsigned char defense_coverage  [11];    // One byte for each player 0-255
     unsigned char kicking           [ 1];    // [K-KA, P-KA]
     unsigned char team              [ 1];    // [Off,  Def ]

} tsb_sim_data_s;

typedef struct
{
     unsigned char selections [ 8]; // Based on what play call the defense makes, e.g. ^A, <B, >A, etc.

} tsb_action_s;

typedef struct
{
     unsigned char name      [15];
     unsigned char formation [ 1];
     tsb_action_s  actions;

} tsb_play_s;

typedef struct
{
     unsigned char bytes [42];

} tsb_playbook_gfx_s;

typedef struct
{                // helmet type:    Two_Stripe           One_Stripe             Ram_Horn               Tiger_Stripe
     unsigned char helmet   [3]; // inner/helmet/outer   hilite/helmet/stripe   spiral/helmet/hilite   hilite/helmet/stripes
     unsigned char facemask [2]; // edge/center
     unsigned char logo     [3];

} helmet_colors_s;

typedef struct
{
     unsigned char home      [3]; // helmet/dark skin color/jersey
     unsigned char road      [3]; // helmet/dark skin color/jersey
     unsigned char use_flags [4]; // bitwise by opponent: 0 - use home, 1 - use road; last 2 bits always 0

} tsb_uniforms_s;

typedef struct
{
     unsigned char home      [2]; // helmet/jersey
     unsigned char road      [2]; // helmet/jersey
     unsigned char use_flags [4]; // bitwise by opponent: 0 - use home, 1 - use road; last 2 bits always 0

} tsb_uniforms2_s;

typedef struct
{
     unsigned char jersey_dark_shadow [1];
     unsigned char jersey_main_color  [1];
     unsigned char jersey_shadow      [1];

     unsigned char helmet_highlight   [1];
     unsigned char helmet_main_color  [1];

} tsb_uniforms3_s;

typedef struct
{
     unsigned char helmet_pants_color [1];

     unsigned char jersey_main_color1 [1]; // should be the same as color 2
     unsigned char jersey_highlight   [1];
     unsigned char jersey_main_color2 [1]; // should be the same as color 1

} tsb_uniforms4_s;

typedef struct
{
     unsigned char top_left_tile     [1];
     unsigned char top_right_tile    [1];
     unsigned char bottom_left_tile  [1];
     unsigned char bottom_right_tile [1];
     unsigned char palette           [1];

} mini_helmet_s;

typedef struct
{
     nes_header_s       header;                                         // 0x00000 - 0x0000f
     nes_pointer_s      team_pointers               [    28 ];          // 0x00010 - 0x00047
     nes_pointer_s      player_pointers             [    28 ][ 30 ];    // 0x00048 - 0x006d7
     nes_pointer_s      end_player_pointers         [     1 ];          // 0x006d8 - 0x006d9
     unsigned char      player_identifiers          [ 10550 ];          // 0x006da - 0x0300f
     tsb_ratings_team_s team_player_ratings         [    28 ];          // 0x03010 - 0x03cdb
     unsigned char      filler1                     [   820 ];          // 0x03cdc - 0x0400f
     unsigned char      play_formations_positioning [  1024 ];          // 0x04010 - 0x0440f
     nes_pointer_s      offensive_play_pointers     [  1280 ];          // 0x04410 - 0x04e0f
     unsigned char      unknown01                   [   512 ];          // 0x04e10 - 0x0500f
     unsigned char      unknown02                   [  4096 ];          // 0x05010 - 0x0600f
     unsigned char      unknown03                   [  8192 ];          // 0x06010 - 0x0800f
     unsigned char      player_offensive_commands   [  8192 ];          // 0x08010 - 0x0a00f
     unsigned char      player_defensive_commands   [  8192 ];          // 0x0a010 - 0x0c00f
     unsigned char      unknown04                   [  8192 ];          // 0x0c010 - 0x0e00f  - seems to be intro/credits info/graphics?
     unsigned char      unknown05a                  [  4864 ];          // 0x0e010 - 0x1000f  - seems to be text/gphx for stats/shedule/game intro
     unsigned char      team_ids                    [    28 ];          // 0x0f310 - 0x0f32b
     nes_pointer_s      player_ids                  [    28 ][ 30 ];    // 0x0f32c - 0x0f9bb
     unsigned char      unknown05b                  [  1620 ];          // 0x0f9bc - 0x1000f  - seems to be text/gphx for stats/shedule/game intro
     unsigned char      unknown06                   [  8192 ];          // 0x10010 - 0x1200f
     unsigned char      unknown07                   [  8192 ];          // 0x12010 - 0x1400f
     unsigned char      unknown08                   [  4096 ];          // 0x14010 - 0x1500f
     unsigned char      unknown09                   [  4096 ];          // 0x15010 - 0x1600f
     unsigned char      unknown10                   [  4096 ];          // 0x16010 - 0x1700f
     unsigned char      unknown11a                  [  4435 ];          // 0x17010 - 0x18162
     tsb_sim_data_s     sim_data                    [    28 ];          // 0x18163 - 0x186a2
     unsigned char      unknown11b                  [  6509 ];          // 0x186a3 - 0x1a00f
     unsigned char      unknown12                   [  1536 ];          // 0x1a010 - 0x1a60f
     unsigned char      unknown13                   [  1536 ];          // 0x1a610 - 0x1ac0f
     unsigned char      unknown14                   [  1024 ];          // 0x1ac10 - 0x1b00f
     unsigned char      unknown15                   [  4096 ];          // 0x1b010 - 0x1c00f
     unsigned char      unknown16                   [  4864 ];          // 0x1c010 - 0x1d30f
     tsb_playbook_s     default_playbooks           [    28 ];          // 0x1d310 - 0x1d37f
     unsigned char      pro_bowl_playbooks          [    16 ];          // 0x1d380 - 0x1d38f
     unsigned char      filler2                     [   128 ];          // 0x1d390 - 0x1d40f
     tsb_play_s         offensive_plays             [    64 ];          // 0x1d410 - 0x1da0f
     unsigned char      filler3                     [   512 ];          // 0x1da10 - 0x1dc0f
     tsb_action_s       defensive_reactions         [    64 ];          // 0x1dc10 - 0x1de0f
     unsigned char      filler4                     [   256 ];          // 0x1de10 - 0x1df0f
     tsb_com_juice_s    computer_juice              [    17 ];          // 0x1df10 - 0x1df64
     unsigned char      filler5                     [   171 ];          // 0x1df65 - 0x1e00f
     unsigned char      unknown17                   [  7168 ];          // 0x1e010 - 0x1fc0f
     nes_pointer_s      team_abbr_pointers          [    28 ];          // 0x1fc10 - 0x1fc47
     nes_pointer_s      conference_abbr_pointers    [     4 ];          // 0x1fc48 - 0x1fc49
     nes_pointer_s      team_loc_pointers           [    28 ];          // 0x1fc50 - 0x1fc87
     nes_pointer_s      conference_loc_pointers     [     4 ];          // 0x1fc88 - 0x1fc8f
     nes_pointer_s      team_name_pointers          [    28 ];          // 0x1fc90 - 0x1fcc7
     nes_pointer_s      conference_name_pointers    [     4 ];          // 0x1fcc8 - 0x1fccf
     nes_pointer_s      down_name_pointers          [     4 ];          // 0x1fcd0 - 0x1fcd7
     nes_pointer_s      unknown_pointers1           [     4 ];          // 0x1fcd8 - 0x1fcdf
     nes_pointer_s      misc_name_pointers          [    16 ];          // 0x1fce0 - 0x1fcff
     unsigned char      team_conference_names       [   784 ];          // 0x1fd00 - 0x2000f
     unsigned char      unknown18a                  [  8144 ];          // 0x20010 - 0x21fdf
     unsigned char      formations1                 [    28 ];          // 0x21fe0 - 0x21ffb
     unsigned char      unknown18b                  [  6615 ];          // 0x21ffc - 0x239d2
     unsigned char      kick_and_punt_returners1    [    28 ];          // 0x239d3 - 0x239ee - 1 byte per team by nibble: [KR, PR]
     unsigned char      unknown18c                  [   471 ];          // 0x239ef - 0x23d78
     mini_helmet_s      mini_helmets                [    28 ];          // 0x23bc6 - 0x23c51
     unsigned char      mini_helmet_logo_offsets    [    15 ];          // 0x23c52 - 0x23c60
     unsigned char      mini_helmet_logo_designs    [   136 ];          // 0x23c61 - 0x23ce8
     unsigned char      unknown18d                  [   144 ];          // 0x23ce9 - 0x23d78
     helmet_colors_s    helmet_colors               [    28 ];          // 0x23d79 - 0x23e58
     unsigned char      helmet_types                [    28 ];          // 0x23e59 - 0x23e74
     unsigned char      unknown18e                  [   411 ];          // 0x23e75 - 0x2400f
     unsigned char      unknown19                   [  8192 ];          // 0x24010 - 0x2600f
     unsigned char      unknown20                   [  2048 ];          // 0x26010 - 0x2680f
     unsigned char      unknown21a                  [  3318 ];          // 0x26810 - 0x27525
     unsigned char      target_rusher               [    32 ];          // 0x27506 - 0x27525
     unsigned char      offensive_preference        [    28 ];          // 0x27526 - 0x27541
     unsigned char      filler6                     [     4 ];          // 0x27542 - 0x27545
     tsb_playbook_gfx_s playbook_gfx                [    64 ];          // 0x27546 - 0x27fc5
     unsigned char      filler7                     [    74 ];          // 0x27fc6 - 0x2800f
     unsigned char      unknown22                   [ 16384 ];          // 0x28010 - 0x2c00f
     unsigned char      unknown23a                  [   724 ];          // 0x2c010 - 0x2c2e3
     tsb_uniforms_s     field_uniforms              [    28 ];          // 0x2c2e4 - 0x2c3fb
     unsigned char      unknown23b                  [  3092 ];          // 0x2c3fc - 0x2d00f
     unsigned char      unknown24                   [  4096 ];          // 0x2d010 - 0x2e00f
     unsigned char      unknown25                   [  8192 ];          // 0x2e010 - 0x3000f
     unsigned char      unknown26a                  [  4400 ];          // 0x30010 - 0x3113f
     unsigned char      team_background_colors      [    28 ];          // 0x31140 - 0x3115b
     unsigned char      unknown26b                  [  3364 ];          // 0x3115c - 0x31e7f
     unsigned char      formations2                 [    28 ];          // 0x31e80 - 0x31e9b
     unsigned char      unknown26c                  [   372 ];          // 0x31e9c - 0x3200f
     unsigned char      unknown27a                  [  2243 ];          // 0x32010 - 0x328d2
     unsigned char      kick_and_punt_returners2    [    28 ];          // 0x328d3 - 0x328ee - 1 byte per team by nibble: [KR, PR]
     unsigned char      unknown27b                  [   184 ];          // 0x328ef - 0x329a6
     tsb_schedule_u     schedule                    [     1 ];          // 0x329a7 - 0x3400f
     unsigned char      unknown28a                  [   712 ];          // 0x34010 - 0x342d7
     tsb_uniforms2_s    cutscene_uniforms           [    28 ];          // 0x342d8 - 0x343b7 - in-game action cutscenes
     unsigned char      unknown28b                  [    48 ];          // 0x343b8 - 0x343e7
     tsb_uniforms3_s    cutscene_uniforms2          [    28 ];          // 0x343e8 - 0x34473 - division champions cutscene
     unsigned char      unknown28c                  [    32 ];          // 0x34474 - 0x34493
     tsb_uniforms4_s    cutscene_uniforms3          [    28 ];          // 0x34494 - 0x34503 - conference champions & overall champions cutscene
     unsigned char      unknown28d                  [  5900 ];          // 0x34504 - 0x35c0f
     unsigned char      unknown29                   [  1024 ];          // 0x35c10 - 0x3600f
     unsigned char      unknown30                   [  8192 ];          // 0x36010 - 0x3800f
     unsigned char      unknown31                   [ 16384 ];          // 0x38010 - 0x3c00f
     unsigned char      unknown32                   [ 16368 ];          // 0x3c010 - 0x3ffff
     unsigned char      unknown33                   [ 12304 ];          // 0x40000 - 0x4300f
     unsigned char      unknown34                   [   768 ];          // 0x43010 - 0x4330f
     unsigned char      unknown35                   [  3328 ];          // 0x43310 - 0x4400f
     unsigned char      unknown36                   [ 24576 ];          // 0x44010 - 0x4a00f
     unsigned char      unknown37                   [  8192 ];          // 0x4a010 - 0x4c00f
     unsigned char      unknown38                   [ 16384 ];          // 0x4c010 - 0x5000f
     unsigned char      unknown39                   [ 23552 ];          // 0x50010 - 0x55c0f
     unsigned char      unknown40                   [ 25600 ];          // 0x55c10 - 0x5c00f
     unsigned char      unknown41                   [ 16384 ];          // 0x5c010 - 0x6000f

} tsbrom_s;


char *getFileUtilsError(    void );
char *getPopulateRomsError( void );

tsbrom_s  *readTsbRom(  const char *filename                              );
boolean_e  writeTsbRom( const char *filename, const tsbrom_s *tsbrom_file );

boolean_e populateRoms( tsbrom_s *rom1, tsbrom_s *rom2, const organization_s *organization );

char *lowercase(     const char *s );
char *uppercase(     const char *s );
char *normalizeCase( const char *s );

int  word2int(                  const unsigned char *word );
void int2word( const int value,       unsigned char *word );

int  pointer2int(                  const nes_pointer_s *ptr );
void int2pointer( const int value,       nes_pointer_s *ptr );

int number2hex( const int number );
int hex2number( const int hex    );

#endif
