target extended-remote /dev/ttyBmpGdb
monitor swdp_scan
attach 1
set mem inaccessible-by-default off
