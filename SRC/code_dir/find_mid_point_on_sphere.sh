#!/bin/tcsh


set SHELL_DIR = $1
set lat1 = $2
set lon1 = $3
set lat2 = $4
set lon2 = $5
set program = $SHELL_DIR/find_mid_point_on_sphere

##//echo $SHELL_DIR

set tmp_file = ./mid.tmp
cat /dev/null > $tmp_file
##echo $lat1 $lon1 $lat2 $lon2
$program << EOF >> $tmp_file
$lat1 $lon1 $lat2 $lon2
EOF

cat $tmp_file 
