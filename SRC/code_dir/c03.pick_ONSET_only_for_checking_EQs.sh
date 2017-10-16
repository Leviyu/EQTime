#!/bin/csh



# ====================================================================
# This is a csh script 
# 1. pick the ONSET of ES only for all checking EQs
#	
#	when picking the ONSET in sac
#	I  only need to pick the ONSET of S and SS
#	ScS  SSS ONSET can all be hardwired to ONSET of S
#	ScSScS are hardwired to SS
#
# DATE:	Nov 26 2014				Keywords:
# ===================================================================

set EQ = $1
set PHASE = $2
set filter = $3
set COMP = $4
set DIR = $5
set DATADIR = $6
set PWD = `pwd`
set DELTA = `grep DELTA $DIR/INFILE |awk '{print $2}'`

	set WORKDIR = $DATADIR/${EQ}
	set macro = $WORKDIR/pick.macro

	cd $WORKDIR
	# read in sac file and pick
	if( $PHASE == "S" ) then
		set sacfile = $WORKDIR/${PHASE}_ES.sac
# construct macro file 
cat <<EOF> $macro
r $sacfile
p1
ppk savelocs on
w $sacfile
quit
EOF
sac < $macro > & /dev/null

		# get pick npts
		set npts = `saclst A f $sacfile|awk '{print $2}'`
		set ONSET = `printf "%.0f" $npts`
	endif

		set tmp_out = ${PHASE}_ONSET.tmp.out
		echo ONSET $ONSET >$tmp_out 
cd $PWD
exit 0
