#!/bin/csh



# ====================================================================
# This is a csh script 
# 1. pick the ONSET of ES and ploted the picked sac file into plots
# 2. calculate the dt_obs_prem for each record
#	
#	when picking the ONSET in sac
#	I  only need to pick the ONSET of S and SS
#	ScS ScSScS   SSS ONSET can all be hardwired to ONSET of S
#	ScSScS are hardwired to SS
#
#
# DATE:		Apr 2015	Keywords:
# ===================================================================
set EQ = $1
set PHASE = $2
set filter = $3
set COMP = $4
set DIR = $5
set DATADIR = $6
set DELTA = $7
set PWD = `pwd`

	set WORKDIR = $DATADIR/${EQ}
	set macro = $WORKDIR/pick.macro

	cd $WORKDIR
	set ONSET_file = S_ONSET.tmp.out
## this is in seconds
	set ONSET = `awk '{print $2}' $ONSET_file`


	#calculate dt for every record
	set eventpick = $WORKDIR/eventpick.${EQ}.${PHASE}.${COMP}
	set eventinfo = $WORKDIR/eventinfo.${EQ}.${PHASE}.${COMP}

	set OUT = $WORKDIR/${PHASE}.eventinfo.out.test
	/bin/rm $OUT > & /dev/null
	foreach STA (`cat $eventpick |awk '{print $1}'`)
		set phase_peak_time = `grep -w $STA $eventpick | awk '{print $2}'`
		set phase_peak_npts = `grep -w $STA $eventpick | awk '{print $3}'`
		set stretch_factor = `grep  -w $STA $eventpick | awk '{print $4}'`
##echo "$PHASE $STA $phase_peak_time $phase_peak_npts $stretch_factor "
			if($phase_peak_npts == 0) then
			set dt_obs_prem = 1000
			grep -w  $STA $eventinfo |awk -v dt=$dt_obs_prem  'NR==1 {$14=dt; print $0}' >> $OUT
			else
			set dt_obs_prem = `echo "$phase_peak_time - ($phase_peak_npts*$DELTA - $ONSET ) * $stretch_factor"|bc -l`
			set short_dt = `printf "%.3f" $dt_obs_prem`
			grep  -w $STA $eventinfo |awk -v dt=$short_dt  '{$19=dt; print $0 }' >> $OUT
			endif
		end
cp $OUT $eventinfo

cd $PWD
exit 0
