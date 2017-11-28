#!/usr/bin/env bash
#
mapfile -t AMERICAN_PLAYERS < <(grep -E '^(QB|RB|WR|TE|OL|DL|LB|CB|S |K |P ) ' season_digest.txt | head -30 | cut -c 1-28)
mapfile -t NATIONAL_PLAYERS < <(grep -E '^(QB|RB|WR|TE|OL|DL|LB|CB|S |K |P ) ' season_digest.txt | tail -30 | cut -c 1-28)
mapfile ALL_PLAYERS < <(../Ruby/print_rosters.rb . R | grep -E '^(QB|RB|WR|TE|OL|DL|LB|CB|S |K |P ) ')

IFS=""
FIRST_PLAYER=1
for player in "${AMERICAN_PLAYERS[@]}"
do
    PLAYER_ID=$(echo "${ALL_PLAYERS[@]}" | grep "$player" | awk '{print $NF}')

    if [[ $FIRST_PLAYER -eq 1 ]]
    then
        printf "%s" $PLAYER_ID
        FIRST_PLAYER=0
    else
        printf ",%s" $PLAYER_ID
    fi
done

printf "\n"

FIRST_PLAYER=1
for player in "${NATIONAL_PLAYERS[@]}"
do
    PLAYER_ID=$(echo "${ALL_PLAYERS[@]}" | grep "$player" | awk '{print $NF}')

    if [[ $FIRST_PLAYER -eq 1 ]]
    then
        printf "%s" $PLAYER_ID
        FIRST_PLAYER=0
    else
        printf ",%s" $PLAYER_ID
    fi
done

printf "\n"
