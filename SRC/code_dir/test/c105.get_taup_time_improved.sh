#!/bin/tcsh

# ====================================================================
# This is a csh script to return the taup_time for a record
# 
# Input: EQ info
# 		station
# 		phase info
#
# Output: a file store travel time for all phases for current EQ_STA
#
#
# DATE:					Keywords:
#
# Reference:
# ===================================================================


set EQ =	$1
set STA =	$2
set PHASE = $3
set tPHASE = $4

if(`echo $PHASE | grep m ` == "" ) then
set main_PHASE = $PHASE
else
set main_PHASE = `echo $PHASE | sed 's/.$//' `
endif

if(`echo $PHASE | grep m ` == "" ) then
set main_tPHASE = $tPHASE
else
set main_tPHASE = `echo $tPHASE | sed 's/.$//' `
endif

set INFILE = ./INFILE
set TAUP_DIR = `grep TAUP_DIR $INFILE|awk 'NR==1 {print $2}'`
set SHELL_DIR = `grep SHELL_DIR $INFILE|awk 'NR==1 {print $2}'`
set eventStation = ./eventStation.${EQ}

# deal with PPP flag
set PPP_flag = `echo $STA |grep PPP`
if($PPP_flag == "") then
	set a = 1
else
	set STA = `echo $STA |rev |cut -c 5-|rev`
endif



set taup_time_file = $TAUP_DIR/taup_time.${EQ}.${STA}

## if taup_file for current main_PHASE does not exist, create it
if(! -e $taup_time_file ) then
	csh $SHELL_DIR/

	taup_time -mod prem -ph $main_PHASE -h $eq_dep -sta $sta_lat $sta_lon -evt $eq_lat $eq_lon >> $taup_time_file
endif


# Get minor phase taup_time 
# This is done by grabing the first travel time given by taup_time that < 180
# The major phase is done by grabing the first travel time given by taup that is > 180

if($PHASE == $main_PHASE ) then
set result = `cat $taup_time_file | awk 'NR>5 {print $0}' | awk '$8<180 {print $4}' |awk 'NR==1 {print $0}'  `
else 
set result = `cat $taup_time_file | awk 'NR>5 {print $0}' | awk '$8>180 {print $4}' |awk 'NR==1 {print $0}'  `
endif

if($result == "" || $result == " ") then
set result = 0
endif

echo $result
##//echo "main phase is $main_PHASE PHASE is $PHASE"


exit 0
