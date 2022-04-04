# home-server

## temp_check.cron
Designed to be run with cron, this basic script checks server temperature and sends a Pushbullet warning if it is too high

## trash.cron
A simple bash script designed for cron to take a designated list of pre-defined trash folders, take a log of files that have been recently added and remove them after a given amount of days.

## plexUpdate.sh
Checks installed version of plexmediaserver on a debian bare-metal install and updates if a newer version is available (must be ran as root). Also keeps a version of the previous installer in a given directory in case a rollback is required. Designed for the beta release (plex pass required), X-Plex-Token must be modified in the script (see https://support.plex.tv/articles/204059436-finding-an-authentication-token-x-plex-token/)

## kill_default_quality.sh
This script is designed for Tautulli to give a warning to users that appear to have not changed their default quality settings in Plex. It will run the JBOP kill_stream.py script (https://github.com/blacktwin/JBOPS/tree/master/killstream) for non-whitelisted users, once per day per device.
