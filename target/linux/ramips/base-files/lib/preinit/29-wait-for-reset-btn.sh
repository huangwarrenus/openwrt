#!/bin/sh
#
# on the Poray and Toplink AP01 boards the reset switch enters a 20ms undetermined state
# which causes a reset switch event in the preinit failsafe script which in turn triggers a
# boot into failsafe mode. So the systems never fully boot.  A short delay is required for the 
# reset switch to settle before the failsafe script is exectluted.  This script must be exectuted
# before the failsafe script to work propertly.  
#
# 2 seconds is more than enough time since the transient is on 20ms in duration.

. /lib/functions.sh
. /lib/ramips.sh

fs_wait_for_poray_reset() {
    case $(ramips_board_name) in
       m3 |\
       m4 |\
       x5 |\
       x8 |\
       top-ap01)
    	    echo "Wait for reset button on $(ramips_board_name) to settle"
	    sleep 2
	    ;;
    esac
}

boot_hook_add preinit_main fs_wait_for_poray_reset
