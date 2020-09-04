#!/bin/bash
# checks installed version of plexmediaserver on a debian bare-metal install and updates if a newer version is available
# also keeps a version of the previous installer in given directory in case a rollback is required
# designed for the beta version (plex pass required), enter plex token below (see https://support.plex.tv/articles/204059436-finding-an-authentication-token-x-plex-token/)
PLEX_TOKEN=a1b2c3d4e5a1b2c3d4e5
UPDATE_DIR=/home/user/plexUpdate

[ ! -d "$UPDATE_DIR" ] && mkdir -p "$UPDATE_DIR"
cd "$UPDATE_DIR"

InstalledFile=$(dpkg --list | grep plexmediaserver | grep -oP '(?<= )\d\S*(?= )')
echo "Installed version is plexmediaserver_$InstalledFile"

# get new filename from headers (-L for redirect, -I for headers, be wary of hidden characters (check with cat -v))
NewFile=$(curl -sIL 'https://plex.tv/downloads/latest/5?channel=8&build=linux-x86_64&distro=debian&X-Plex-Token='"${PLEX_TOKEN}" | grep -o 'plexmediaserver.*\.deb')
echo "Latest version is $NewFile"

if [[ ! -z $(echo "$NewFile" | grep "$InstalledFile") ]]
then
        echo "Latest Version Is Already Installed, Exiting"
        cd -
        exit
fi

echo

OldFile=$(ls | grep ^LASTplexmediaserver)
if [[ ! -z "$OldFile" ]]
then
        echo "Deleting old version $(echo OldFile | grep plexmediaserver.*)"
        rm $OldFile
fi

NewOldFile=$(ls | grep ^plexmediaserver)
if [[ ! -z "$NewOldFile" ]]
then
        echo "Keeping a copy of current version $NewOldFile"
        mv "$NewOldFile" "LAST$NewOldFile"
fi

echo "Downloading latest version $NewFile"
echo
wget --content-disposition 'https://plex.tv/downloads/latest/5?channel=8&build=linux-x86_64&distro=debian&X-Plex-Token='"${PLEX_TOKEN}"
echo
echo "Installing latest version $NewFile"
apt -y install ./"$NewFile"
echo "Plex Update Installed"

cd -
