#!/bin/tcsh



set event = ./eventStation.201506231218.SS.T

gcc -o coucou *.c -lm


foreach STA (`cat $event|awk '{print $1}'`)

	set TMP = `cat $event |grep -w $STA `
	set eq_dep = $TMP[13]
	set DIST = $TMP[3]
	set PHASE = SS

	set take_off = `taup_time -mod prem -ph $PHASE -deg $DIST -h $eq_dep |awk 'NR==6 {print $6}'`

	set AZ = $TMP[5]

coucou $take_off $AZ $STA





end 
