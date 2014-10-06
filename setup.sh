#!/usr/bin/env bash
#

if [[ .ncfo1.nst.bak -nt ncfo1.nst ||
      .ncfo2.nst.bak -nt ncfo2.nst ]];
then
    echo "WARNING: backup files are newer than save files!";
    exit;
fi

echo "Backing up save state files..."

cp ncfo1.nst .ncfo1.nst.bak
cp ncfo2.nst .ncfo2.nst.bak

