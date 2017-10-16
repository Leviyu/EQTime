#!/bin/csh

# ==================================================
#	This Script preprocess the data for esf
#	
#	Hongyu DATE: 
#	Key words: 
# ==================================================


set EQ = $1
set DATADIR = $2
set PLOTDIR = $3
set C_DIR = $4
set SHELL_DIR = $5
set PHASE = $6

set work_dir = $DATADIR/${EQ}
set SRCDIR = $SHELL_DIR

set script_name = c01.preprocess_of_${PHASE}_for_$EQ
echo "--->  preprocess for EWF for $EQ PHASE:$PHASE "

set INFILE = $work_dir/INFILE_${PHASE}
set event_file = eventStation.$EQ
set PHASE = `grep -w PHASE $INFILE |awk '{print $2}'`
set COMP = `grep -w COMP $INFILE |awk '{print $2}'`
set polarity_file = eventinfo.polarity.${PHASE}.${COMP}
set LONG_BEG = `grep -w LONG_BEG $INFILE |awk '{print $2}'`
set LONG_LEN = `grep -w LONG_LEN $INFILE |awk '{print $2}'`
set PHASE_BEG = `grep -w PHASE_BEG $INFILE |awk '{print $2}'`
set PHASE_LEN = `grep -w PHASE_LEN $INFILE |awk '{print $2}'`
set NOISE_BEG = `grep -w NOISE_BEG $INFILE |awk '{print $2}'`
set NOISE_LEN = `grep -w PHASE_LEN $INFILE |awk '{print $2}'`
set DISTMIN = `grep -w DIST_MIN_MAX $INFILE |awk '{print $2}'`
# for Sdiff get min dist
if($PHASE == "Sdiff" && $DISTMIN == "non") then
set DISTMIN = `csh $SHELL_DIR/c06.get_S_Sdiff_boundary_distance.sh $EQ $work_dir $PHASE|awk '{print $2+1}'`
echo "---> Sdiff min distance is $DISTMIN"
endif
if($PHASE == "Pdiff" && $DISTMIN == "non") then
set DISTMIN = `csh $SHELL_DIR/c06.get_S_Sdiff_boundary_distance.sh $EQ $work_dir $PHASE|awk '{print $2+1}'`
echo "---> Pdiff min distance is $DISTMIN"
endif

set DISTMAX = `grep -w DIST_MIN_MAX $INFILE |awk '{print $3}'`
# for S get max dist
if($PHASE == "S" && $DISTMAX == "non") then
set DISTMAX = `csh $SHELL_DIR/c06.get_S_Sdiff_boundary_distance.sh $EQ $work_dir $PHASE |awk '{print $2}'`
echo "---> S max distance is $DISTMAX"
endif
if($PHASE == "P" && $DISTMAX == "non") then
set DISTMAX = `csh $SHELL_DIR/c06.get_S_Sdiff_boundary_distance.sh $EQ $work_dir $PHASE |awk '{print $2}'`
echo "---> P max distance is $DISTMAX"
endif
set velocity_or_displacement = `grep -w velocity_or_displacement $INFILE |awk 'NR==1 {print $2}'`
set SNR_CUT = `grep -w SNR_CUT $INFILE | awk 'NR==1 {print $2}'`
set CCC_CUT = `grep -w CCC_CUT $INFILE | awk 'NR==1 {print $2}'`
set filter_flag =  `grep filter_flag $INFILE | awk '{print $2}'`
set DELTA = `grep -w  DELTA $work_dir/INFILE | awk 'NR==1 {print $2}'`



# ===================================================
# 			copy data from eq2 to local server
# ===================================================
set INPUT = ( $EQ $DATADIR $work_dir $PLOTDIR  $SRCDIR)
set c0_INPUT = ( $INPUT $DISTMIN $DISTMAX $PHASE $COMP $filter_flag  )
date
if( `echo $PHASE |grep P` == "") then
csh $SRCDIR/c0.prepare_data.sh							$c0_INPUT
else
csh $SRCDIR/c0.prepare_data_for_P.sh					$c0_INPUT
endif
date

# ===================================================
# 			work on polarity info
# ===================================================
set INPUT = ( $EQ $DATADIR $PLOTDIR $C_DIR $SHELL_DIR )
set c15_INPUT = ($INPUT $DISTMIN $DISTMAX $PHASE $COMP $filter_flag  )
csh $SRCDIR/c15.get_polarity.sh							$c15_INPUT
date


# ===================================================
# 			work on c code and go for it
# ===================================================
if(! -e $work_dir/EWM ) then
cd $C_DIR/lib
echo "we are current making source"
make > & /dev/null
cp EWM $work_dir/
endif

cd $work_dir
set c_input = $work_dir/input.c
echo $EQ $PHASE $DELTA  > ! $c_input

##./EWM.out << EOF  >> & $c_logfile
./EWM $c_input

date
exit 0
