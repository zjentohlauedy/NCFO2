#!/usr/bin/env bash
#

if [[ schedule.csv -nt schedule.ods ]];
then
    echo "WARNING: csv file is newer than ods file!";
    exit;
fi

echo "Converting schedule.ods to csv..."
soffice -env:UserInstallation=file://$HOME/.libreoffice-alt --headless --convert-to csv --outdir . ./schedule.ods

~/NES/NCFO/Ruby/boxscores.rb
