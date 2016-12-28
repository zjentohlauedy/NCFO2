AR = ar
ARFLAGS = -rc
CC = cc
CFLAGS = -std=c99 -g -Werror-implicit-function-declaration -D_GNU_SOURCE
LDFLAGS = -L. -lm -lsqlite3 -lz

progs = \
   export_season \
   decompress_save_state \
   print_rosters \
   standings \
   extract_data \
   extract_bowl_game_data \
   extract_aa_game_data \
   injuries \
   reorg \
   set_uniforms \
   set_playoff_uniforms \
   copy_playoff_teams \
   copy_all_americans \
   boxscore \
   import_season \
   import_bowl_game \
   import_aa_game

tests = \
   test_file_utils \
   test_populate_roms \
   test_generate_schedule \
   test_organizations_t \
   test_organization_conferences_t \
   test_conferences_t \
   test_conference_teams_t \
   test_conference_stats_t \
   test_conference_accolades_t \
   test_teams_t \
   test_team_players_t \
   test_team_stats_t \
   test_team_offense_stats_t \
   test_team_defense_stats_t \
   test_team_kicking_stats_t \
   test_team_game_stats_t \
   test_team_game_offense_stats_t \
   test_team_game_defense_stats_t \
   test_team_accolades_t \
   test_players_t \
   test_player_ratings_t \
   test_player_quarterback_ratings_t \
   test_player_offense_ratings_t \
   test_player_defense_ratings_t \
   test_player_kicking_ratings_t \
   test_player_offense_stats_t \
   test_player_defense_stats_t \
   test_player_kicking_stats_t \
   test_player_returns_stats_t \
   test_player_game_offense_stats_t \
   test_player_game_defense_stats_t \
   test_player_game_returns_stats_t \
   test_player_game_kicking_stats_t \
   test_player_accolades_t\
   test_player_service \
   test_team_service \
   test_conference_service \
   test_organization_service \
   test_convert_organization \
   test_convert_bowl_game \
   test_convert_aa_game

ALL: PROGS TESTS

PROGS: $(progs) TAGS

export_season: export_season.o file_utils.o populate_roms.o generate_schedule.o libncfodata.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfodata

decompress_save_state: decompress_save_state.o file_utils.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

print_rosters: print_rosters.o file_utils.o convert_organization.o libncfodata.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfodata

standings: standings.o file_utils.o convert_organization.o libncfodata.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfodata

extract_data: extract_data.o file_utils.o convert_organization.o libncfodata.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfodata

extract_bowl_game_data: extract_bowl_game_data.o file_utils.o convert_bowl_game.o convert_organization.o libncfodata.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfodata

extract_aa_game_data: extract_aa_game_data.o file_utils.o convert_aa_game.o convert_bowl_game.o convert_organization.o libncfodata.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfodata

injuries: injuries.o file_utils.o convert_organization.o libncfodata.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfodata

reorg: reorg.o file_utils.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

set_uniforms: set_uniforms.o file_utils.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

set_playoff_uniforms: set_playoff_uniforms.o file_utils.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

copy_playoff_teams: copy_playoff_teams.o file_utils.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

copy_all_americans: copy_all_americans.o file_utils.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

boxscore: boxscore.o file_utils.o convert_organization.o libncfodata.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfodata

import_season: import_season.o file_utils.o convert_organization.o libncfodata.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfodata

import_bowl_game: import_bowl_game.o file_utils.o convert_bowl_game.o convert_organization.o libncfodata.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfodata

import_aa_game: import_aa_game.o file_utils.o convert_aa_game.o convert_bowl_game.o convert_organization.o libncfodata.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfodata


TESTS: $(tests) ncfo_test.db TAGS

test_file_utils: test_file_utils.o file_utils.o generate_schedule.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_populate_roms: test_populate_roms.o populate_roms.o file_utils.o libncfodata.a libncfobuilders.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfodata -lncfobuilders

test_generate_schedule: test_generate_schedule.o generate_schedule.o file_utils.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

test_organizations_t: test_organizations_t.o organizations_t.o sql_query.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_organization_conferences_t: test_organization_conferences_t.o organization_conferences_t.o sql_query.o data_list.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_conferences_t: test_conferences_t.o conferences_t.o sql_query.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_conference_teams_t: test_conference_teams_t.o conference_teams_t.o sql_query.o data_list.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_conference_stats_t: test_conference_stats_t.o conference_stats_t.o sql_query.o data_list.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_conference_accolades_t: test_conference_accolades_t.o conference_accolades_t.o sql_query.o data_list.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_teams_t: test_teams_t.o teams_t.o sql_query.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_team_players_t: test_team_players_t.o team_players_t.o sql_query.o data_list.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_team_stats_t: test_team_stats_t.o team_stats_t.o sql_query.o data_list.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_team_offense_stats_t: test_team_offense_stats_t.o team_offense_stats_t.o sql_query.o data_list.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_team_defense_stats_t: test_team_defense_stats_t.o team_defense_stats_t.o sql_query.o data_list.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_team_kicking_stats_t: test_team_kicking_stats_t.o team_kicking_stats_t.o sql_query.o data_list.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_team_game_stats_t: test_team_game_stats_t.o team_game_stats_t.o sql_query.o data_list.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_team_game_offense_stats_t: test_team_game_offense_stats_t.o team_game_offense_stats_t.o sql_query.o data_list.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_team_game_defense_stats_t: test_team_game_defense_stats_t.o team_game_defense_stats_t.o sql_query.o data_list.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_team_accolades_t: test_team_accolades_t.o team_accolades_t.o sql_query.o data_list.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_players_t: test_players_t.o players_t.o sql_query.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_player_ratings_t: test_player_ratings_t.o player_ratings_t.o sql_query.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_player_quarterback_ratings_t: test_player_quarterback_ratings_t.o player_quarterback_ratings_t.o sql_query.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_player_offense_ratings_t: test_player_offense_ratings_t.o player_offense_ratings_t.o sql_query.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_player_defense_ratings_t: test_player_defense_ratings_t.o player_defense_ratings_t.o sql_query.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_player_kicking_ratings_t: test_player_kicking_ratings_t.o player_kicking_ratings_t.o sql_query.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_player_offense_stats_t: test_player_offense_stats_t.o player_offense_stats_t.o sql_query.o data_list.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_player_defense_stats_t: test_player_defense_stats_t.o player_defense_stats_t.o sql_query.o data_list.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_player_kicking_stats_t: test_player_kicking_stats_t.o player_kicking_stats_t.o sql_query.o data_list.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_player_returns_stats_t: test_player_returns_stats_t.o player_returns_stats_t.o sql_query.o data_list.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_player_game_offense_stats_t: test_player_game_offense_stats_t.o player_game_offense_stats_t.o sql_query.o data_list.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_player_game_defense_stats_t: test_player_game_defense_stats_t.o player_game_defense_stats_t.o sql_query.o data_list.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_player_game_returns_stats_t: test_player_game_returns_stats_t.o player_game_returns_stats_t.o sql_query.o data_list.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_player_game_kicking_stats_t: test_player_game_kicking_stats_t.o player_game_kicking_stats_t.o sql_query.o data_list.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_player_accolades_t: test_player_accolades_t.o player_accolades_t.o sql_query.o data_list.o libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfobuilders -lncfocompares

test_player_service: test_player_service.o libncfodata.a libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfodata -lncfobuilders -lncfocompares

test_team_service: test_team_service.o libncfodata.a libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfodata -lncfobuilders -lncfocompares

test_conference_service: test_conference_service.o libncfodata.a libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfodata -lncfobuilders -lncfocompares

test_organization_service: test_organization_service.o libncfodata.a libncfobuilders.a libncfocompares.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfodata -lncfobuilders -lncfocompares

test_convert_organization: test_convert_organization.o convert_organization.o file_utils.o libncfodata.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfodata

test_convert_bowl_game: test_convert_bowl_game.o convert_bowl_game.o convert_organization.o file_utils.o libncfodata.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfodata

test_convert_aa_game: test_convert_aa_game.o convert_aa_game.o convert_bowl_game.o convert_organization.o file_utils.o libncfodata.a
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lncfodata

NCFODATA_OBJECTS = \
 sql_query.o \
 data_list.o \
 organizations_t.o \
 organization_conferences_t.o \
 conference_accolades_t.o \
 conference_stats_t.o \
 conferences_t.o \
 conference_teams_t.o \
 player_accolades_t.o \
 player_defense_ratings_t.o \
 player_defense_stats_t.o \
 player_game_defense_stats_t.o \
 player_game_kicking_stats_t.o \
 player_game_offense_stats_t.o \
 player_game_returns_stats_t.o \
 player_kicking_ratings_t.o \
 player_kicking_stats_t.o \
 player_offense_ratings_t.o \
 player_offense_stats_t.o \
 player_quarterback_ratings_t.o \
 player_ratings_t.o \
 player_returns_stats_t.o \
 players_t.o \
 team_accolades_t.o \
 team_defense_stats_t.o \
 team_game_defense_stats_t.o \
 team_game_offense_stats_t.o \
 team_game_stats_t.o \
 team_kicking_stats_t.o \
 team_offense_stats_t.o \
 team_players_t.o \
 team_stats_t.o \
 teams_t.o \
 player_service.o \
 team_service.o \
 conference_service.o \
 organization_service.o

libncfodata.a: $(NCFODATA_OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

NCFOBUILDERS_OBJECTS = \
 build_organization.o \
 build_organization_conference.o \
 build_conference.o \
 build_conference_team.o \
 build_conference_stats.o \
 build_conference_accolade.o \
 build_team.o \
 build_team_player.o \
 build_team_stats.o \
 build_team_offense_stats.o \
 build_team_defense_stats.o \
 build_team_kicking_stats.o \
 build_team_game_stats.o \
 build_team_game_offense_stats.o \
 build_team_game_defense_stats.o \
 build_team_accolade.o \
 build_player.o \
 build_player_ratings.o \
 build_player_quarterback_ratings.o \
 build_player_offense_ratings.o \
 build_player_defense_ratings.o \
 build_player_kicking_ratings.o \
 build_player_offense_stats.o \
 build_player_defense_stats.o \
 build_player_kicking_stats.o \
 build_player_returns_stats.o \
 build_player_game_defense_stats.o \
 build_player_game_offense_stats.o \
 build_player_game_returns_stats.o \
 build_player_game_kicking_stats.o \
 build_player_accolade.o

libncfobuilders.a: $(NCFOBUILDERS_OBJECTS)
	$(AR) $(ARFLAGS) $@ $^


NCFOCOMPARES_OBJECTS = \
 compare_organizations.o \
 compare_organization_conferences.o \
 compare_conferences.o \
 compare_conference_teams.o \
 compare_conference_stats.o \
 compare_conference_accolades.o \
 compare_teams.o \
 compare_team_players.o \
 compare_team_stats.o \
 compare_team_offense_stats.o \
 compare_team_defense_stats.o \
 compare_team_kicking_stats.o \
 compare_team_game_stats.o \
 compare_team_game_offense_stats.o \
 compare_team_game_defense_stats.o \
 compare_team_accolades.o \
 compare_players.o \
 compare_player_ratings.o \
 compare_player_quarterback_ratings.o \
 compare_player_offense_ratings.o \
 compare_player_defense_ratings.o \
 compare_player_kicking_ratings.o \
 compare_player_offense_stats.o \
 compare_player_defense_stats.o \
 compare_player_kicking_stats.o \
 compare_player_returns_stats.o \
 compare_player_game_defense_stats.o \
 compare_player_game_offense_stats.o \
 compare_player_game_returns_stats.o \
 compare_player_game_kicking_stats.o \
 compare_player_accolades.o

libncfocompares.a: $(NCFOCOMPARES_OBJECTS)
	$(AR) $(ARFLAGS) $@ $^


%.o: %.c *.h
	$(CC) $(CFLAGS) -c $<

ncfo_test.db: schema.sql
	rm $@
	sqlite3 $@ < $<

TAGS: *.h *.c
	etags *.h *.c

.PHONY: clean
clean:
	rm $(tests) $(progs) *.o *.a
