AR = ar
ARFLAGS = -rc
CC = cc
CFLAGS = -std=c99 -g -Werror-implicit-function-declaration
LDFLAGS = -L. -lm -lsqlite3

progs = \

tests = \
   test_organizations_t \
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
   test_team_accolades_t \
   test_players_t \
   test_player_ratings_t \
   test_player_quarterback_ratings_t \
   test_player_offense_ratings_t \
   test_player_defense_ratings_t \
   test_player_kicking_ratings_t \
   test_player_offense_stats_t

ALL: PROGS TESTS

PROGS: $(progs) TAGS

TESTS: $(tests) ncfo_test.db TAGS

test_organizations_t: test_organizations_t.o organizations_t.o sql_query.o libncfobuilders.a libncfocompares.a
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


NCFOBUILDERS_OBJECTS = \
 build_organization.o \
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
 build_team_accolade.o \
 build_player.o \
 build_player_ratings.o \
 build_player_quarterback_ratings.o \
 build_player_offense_ratings.o \
 build_player_defense_ratings.o \
 build_player_kicking_ratings.o \
 build_player_offense_stats.o

libncfobuilders.a: $(NCFOBUILDERS_OBJECTS)
	$(AR) $(ARFLAGS) $@ $^


NCFOCOMPARES_OBJECTS = \
 compare_organizations.o \
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
 compare_team_accolades.o \
 compare_players.o \
 compare_player_ratings.o \
 compare_player_quarterback_ratings.o \
 compare_player_offense_ratings.o \
 compare_player_defense_ratings.o \
 compare_player_kicking_ratings.o \
 compare_player_offense_stats.o

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
