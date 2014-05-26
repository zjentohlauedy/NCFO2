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
   test_conference_stats_t

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


NCFOBUILDERS_OBJECTS = \
 build_organization.o \
 build_conference.o \
 build_conference_team.o \
 build_conference_stats.o

libncfobuilders.a: $(NCFOBUILDERS_OBJECTS)
	$(AR) $(ARFLAGS) $@ $^


NCFOCOMPARES_OBJECTS = \
 compare_organizations.o \
 compare_conferences.o \
 compare_conference_teams.o \
 compare_conference_stats.o

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
