#!/bin/csh


# ====================================================================
# This is a csh script for getting the boundary distance between S and Sdiff
# 	using taup_time
# 
# Input: 
#		EQ_name
#
#
# Output:
#		boundary distance
#
# DATE:	Oct 10 2014				Keywords: 
# ===================================================================

set EQ = $1
set DATADIR = $2
set PHASE = $3
set work_dir = $DATADIR/get_S_Sdiff_boundary_work_dir
mkdir -p $work_dir
cd $work_dir
# 1. first get the eventStation of EQ
set event = eventStation.${EQ}
get_EQ_sac ${EQ}/$event

# 2. get the boundary distance
set TMP = `awk 'NR==5 {print $0}' $event`
set EQ_dep = $TMP[13]
set EQ_lat = $TMP[11]
set EQ_lon = $TMP[12]

set DIST = 95
while( $DIST  < 105)
	set timeinfo = `taup_time -mod prem -ph $PHASE -h $EQ_dep -deg  $DIST |awk 'NR==6 {print $4}'`
##echo $DIST $timeinfo
	if( $timeinfo == "" ) then
	break
	endif

@ DIST ++
	end
set DIST = `echo "$DIST - 1"|bc -l`

echo "S_Sidff_boundary_distance_is $DIST"

cd ..
/bin/rm -r $work_dir

