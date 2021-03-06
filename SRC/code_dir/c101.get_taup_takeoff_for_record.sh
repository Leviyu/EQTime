#!/bin/tcsh

# ====================================================================
# This is a csh script to return the taup_time for a record
# ===================================================================

set EQ =	$1
set STA =	$2
set PHASE = $3

if(`echo $STA |grep PPP ` != "" ) then
set STA = `echo $STA |rev|cut -c 5-|rev`
endif



if(`echo $PHASE | grep m ` == "" ) then
set main_PHASE = $PHASE
else
set main_PHASE = `echo $PHASE | sed 's/.$//' `
endif

if(`echo $PHASE |grep S` != "") then
set COMP = T
else
set COMP = Z
endif



set INFILE = ./INFILE
set TAUP_DIR = `grep TAUP_DIR $INFILE|awk 'NR==1 {print $2}'`
set SHELL_DIR = `grep SHELL_DIR $INFILE|awk 'NR==1 {print $2}'`
set eventStation = ./eventStation.${EQ}.${PHASE}.${COMP}

set TMP = `grep -w $STA $eventStation|awk 'NR==1 {print $0}'`
set eq_lat = $TMP[11]
set eq_lon = $TMP[12]
set eq_dep = $TMP[13]
set sta_lat = $TMP[9]
set sta_lon = $TMP[10]

mkdir -p $TAUP_DIR
mkdir -p $TAUP_DIR/$EQ
set taup_time_file = $TAUP_DIR/$EQ/taup_time.${EQ}.${STA}

if(! -e $taup_time_file ) then
	csh $SHELL_DIR/c106.from_eventStation_make_big_taup_file.sh $EQ $STA $eventStation $TAUP_DIR/$EQ
endif

# Get minor phase taup_time 
# This is done by grabing the first travel time given by taup_time that < 180
# The major phase is done by grabing the first travel time given by taup that is > 180
if($PHASE == $main_PHASE ) then
set result = `cat $taup_time_file | grep -w $main_PHASE | awk '$8<180 {print $6}' |awk 'NR==1 {print $0}'  `
else 
set result = `cat $taup_time_file | grep -w $main_PHASE  | awk '$8>180 {print $6}' |awk 'NR==1 {print $0}'  `
endif

if($result == "" || $result == " ") then
set result = 0
endif

echo $result


exit 0
