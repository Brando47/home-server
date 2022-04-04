#!/bin/bash
#This script will run the JBOP kill_stream script for non-whitelisted users, once per day per device. It is designed to be triggered by Tautulli.

#in tautulli notification agent:
#	set conditions:
#		Quality Profile contains '720p'
#		Video Resolution does not contain '720'
#		WAN Bandwidth is less than '10000' (optional - adjust to server capability)

#under Script Arguments:
#{datestamp} {user} {player} {session_id}

DAY=$1
USER=$2
PLAYER=$3
SESSION_ID=$4

SCRIPT_LOCATION='/scripts'
LOG_FILE=$SCRIPT_LOCATION'/kill_default_quality.log'
MESSAGE="ATTENTION ${USER^^}. It appears as though you have not changed your default quality settings for Plex on this device. Please increase your quality settings to avoid transcoding where possible. This warning will repeat daily unless you request with the server admin to be whitelisted."
USER_WHITELIST='example_user1,example_user2'

#function to log the infraction and kill the stream
kill_stream () {
	echo "$USER;$PLAYER" >> $LOG_FILE
	$SCRIPT_LOCATION/kill_stream.py --jbop stream --username "$USER" --sessionId "$SESSION_ID" --killMessage "$MESSAGE"
	exit
}

#exit script if user is on the whitelist
if [[ $(echo "$USER_WHITELIST" | grep "$USER") ]]
then
	echo "$USER;$PLAYER;:::WHITELISTED" >> $LOG_FILE
	exit
fi

#clean log file each day
if [ "$(head -n 1 $LOG_FILE)" != "$DAY" ]
then
	echo "$DAY" > $LOG_FILE
	kill_stream
fi

#test the log file and allow (exit script) if a user has already been warned that day for that device
[[ $(grep -x "$USER;$PLAYER" "$LOG_FILE") ]] && exit

#update log file and kick user
kill_stream
