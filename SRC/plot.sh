#!/bin/csh
# ====================================================================
# This is a csh script for ESF 
# 
# Input: INFIL ---- all the parameter that is needed
#
# DATE: Sep 12 2014			Keywords: ESF
# ===================================================================


## ======= Read in parameter ===========
set PWD = `pwd`
set INFILE = ${PWD}/INFILE
set DIR =						`grep -w CURRENT_DIR $INFILE | awk '{print $2}'`
set DATADIR =					`grep -w DATADIR $INFILE | awk '{print $2}'`
set PLOTDIR =					`grep -w PLOTDIR $INFILE | awk 'NR==1 {print $2}'`
set C_DIR =						`grep -w C_DIR $INFILE | awk '{print $2}'`
set TAUP_DIR =					`grep -w TAUP_DIR $INFILE | awk '{print $2}'`
set SHELL_DIR =					`grep -w SHELL_DIR $INFILE | awk '{print $2}'`


set ID = $1
set EQ = $2
if($ID == "" || $ID == " ") then
echo "======================================================"
echo "=============Be careful My friend, ID does not exist!========================="
echo "======================================================"
exit 0
endif


echo "======================================================"
echo "=============Working on ID $ID in Progress========================="
echo "======================================================"


set DATADIR = $DATADIR/$ID
set PLOTDIR = $PLOTDIR/$ID
mkdir -p $DATADIR 
mkdir -p $PLOTDIR
set EQ_LIST = $DATADIR/INPUT_EQ_LIST


set INFILE = $DATADIR/INFILE
set EQ_PHASE_LIST = `grep -w PHASE_LIST $INFILE | awk 'NR==1 {print $0}' |cut -d ' ' -f2-`
mkdir -p $PLOTDIR/$EQ
mkdir -p $DATADIR/$EQ
cp $DATADIR/CMT.data $DATADIR/$EQ/
cp $DATADIR/INFILE* $DATADIR/$EQ/

# plot STD with color
set csh_file = $SHELL_DIR/c25.plot_EW_with_STD_in_color.sh 
csh $csh_file $EQ $DIR $SHELL_DIR $PLOTDIR $DATADIR $C_DIR $DIR
##set plot_ES = $SHELL_DIR/c02.plot_phase_EW_STD.sh
##csh $plot_ES ${EQ} $DIR $SHELL_DIR $PLOTDIR $DATADIR $C_DIR $DIR


##set plot_ES = $SHELL_DIR/c07.plot_phase_statistics.sh	
##csh $plot_ES ${EQ}  $DIR $SHELL_DIR $PLOTDIR $DATADIR


##set csh_file = $SHELL_DIR/c20.plot_record_section_for_one_EQ_one_PHASE.sh
##csh $csh_file $EQ $DATADIR $PLOTDIR $C_DIR $SHELL_DIR $DIR

set csh_file = $SHELL_DIR/c22.plot_record_section_distsum.sh
csh $csh_file $EQ $DATADIR $PLOTDIR $C_DIR $SHELL_DIR 

##echo "---> Plotting record profile for $EQ $PHASE"
##set csh_file = $SHELL_DIR/c20.plot_record_section_for_one_EQ_one_PHASE.sh
##csh $csh_file $EQ $DATADIR $PLOTDIR $C_DIR $SHELL_DIR 

#set csh_file = $SHELL_DIR/c30.algorithm_process_flow_plot.sh
#csh $csh_file $EQ $DATADIR $PLOTDIR $C_DIR $SHELL_DIR 

##set csh_file = $SHELL_DIR/c31_algorithm_process_flow_new.sh
##csh $csh_file $EQ $DATADIR $PLOTDIR $C_DIR $SHELL_DIR 

##echo "---> Plotting record profile distsum for $EQ $PHASE"
##set csh_file = $SHELL_DIR/c22.plot_record_section_distsum.sh
##csh $csh_file $EQ $DATADIR $PLOTDIR $C_DIR $SHELL_DIR $DIR
#/bin/rm $DATADIR/$EQ/20*sac > & /dev/null &
#/bin/rm $DATADIR/$EQ/19*sac > & /dev/null &
# delete all taup_time file
#/bin/rm $TAUP_DIR/taup_time.${EQ}.* 

