#!/bin/sh
#
# on the Poray X5 and on the Toplink AP01 boards the reset switch enters a 20ms undetermined state
# which causes a reset switch event in the preinit failsafe script which in turn triggers a
# boot into failsafe mode. So the systems never fully boot.  A short delay is required for the 
# reset switch to settle before the failsafe script is exectluted.  This script must be exectuted
# before the failsafe script to work propertly.  
#
# 2 seconds is more than enough time since the transient is on 20ms in duration.

. /lib/ramips.sh

case $(ramips_board_name) in
   x5 |\
   top-ap01)
	sleep 2
	;;
esac
