#!/usr/bin/env bash
#

if [[ -f ncfo1.nst || -f ncfo2.nst ]];
then
    if [[ .ncfo1.nst.bak -nt ncfo1.nst ||
          .ncfo2.nst.bak -nt ncfo2.nst ]];
    then
        echo "WARNING: backup files are newer than save files!";
        exit;
    fi

    echo "Backing up save state files..."

    cp ncfo1.nst .ncfo1.nst.bak
    cp ncfo2.nst .ncfo2.nst.bak
fi

if [[ .ncfo1.nes.bak -nt ncfo1.nes ||
      .ncfo2.nes.bak -nt ncfo2.nes ]];
then
    echo "WARNING: backup files are newer than rom files!";
    exit;
fi

echo "Backing up rom files..."

cp ncfo1.nes .ncfo1.nes.bak
cp ncfo2.nes .ncfo2.nes.bak

echo "Parsing schedule..."

if [[ .setgames -nt schedule.csv ]]
then
    echo "WARNING: schedule.csv hasn't been updated!";
    exit;
fi

~/NES/NCFO/Ruby/next_game_day.rb schedule.csv > .setgames

source .setgames

echo "Setting uniforms..."
~/NES/NCFO/C/set_uniforms .ncfo1.nes.bak .ncfo2.nes.bak ncfo1.nes ncfo2.nes
