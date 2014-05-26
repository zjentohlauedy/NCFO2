#!/usr/bin/env bash
#

TESTARGS=""

DBFILE="ncfo_test.db"

FAIL=0
TESTS=0
PASSED=0
FAILED=0

function runTestProg {

    re="^.*Tests Run: ([0-9]+), Passed: ([0-9]+), Failed: ([0-9]+).*$"

    echo
    printf "%s:\n" $1
    RESULTS=$( $1 $TESTARGS )

    if [ $? -ne 0 ]; then
        FAIL=1
    fi

    printf "$RESULTS"

    if [[ "$RESULTS" =~ $re ]]
    then
        TESTS=$[  TESTS  + BASH_REMATCH[1] ]
        PASSED=$[ PASSED + BASH_REMATCH[2] ]
        FAILED=$[ FAILED + BASH_REMATCH[3] ]
    fi
}

make TESTS

if [ $? -ne 0 ]; then exit; fi

echo
echo

TESTARGS=$DBFILE

runTestProg test_organizations_t
runTestProg test_conferences_t
runTestProg test_conference_teams_t
runTestProg test_conference_stats_t

if [ $FAIL -ne 0 ]; then

    echo
    printf "\033[1;31mThere were $FAILED test failures in $TESTS tests...\033[0m\n"
    echo

    exit
fi

echo
printf "\033[1;32m$TESTS TESTS COMPLETED SUCCESSFULLY\033[0m\n"
echo

exit
