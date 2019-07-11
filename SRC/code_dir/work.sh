#!/bin/csh
# ====================================================================
# This is a csh script for ESF 
# 
# Input: INFIL ---- all the parameter that is needed
#
# DATE: Sep 12 2014			Keywords: ESF
# ===================================================================


## ======= Read in parameter ===========
set PWD = 		$1
set DATADIR = 	$2
set PLOTDIR = 	$3
set EQ =		$4
set C_DIR = 	$5
set SHELL_DIR = $6
set ID =		$7
set DIR = 		$8


set INFILE = $DATADIR/INFILE
set EQ_PHASE_LIST = `grep -w PHASE_LIST $INFILE | awk 'NR==1 {print $0}' |cut -f2-`
#echo "phase list is $EQ_PHASE_LIST"
set ED_CHECK_FILE = `grep -w ED_CHECK_FILE $INFILE | awk 'NR==1 {print $2}' `
/bin/rm -r $PLOTDIR/$EQ >& /dev/null
/bin/rm -r $DATADIR/$EQ >& /dev/null
mkdir -p $PLOTDIR/$EQ
mkdir -p $DATADIR/$EQ
cp $DATADIR/CMT.data $DATADIR/$EQ/
cp $DATADIR/INFILE* $DATADIR/$EQ/
cp $DATADIR/GOOD_MULTI_EVENT $DATADIR/$EQ/

ls $DATADIR/GOOD_MULTI_EVENT
ls $DATADIR/$EQ/GOOD_MULTI_EVENT

set IS_MULTI_PHASE = `cat $INFILE |grep IS_MULTI_PHASE|awk '{print $2}'`
echo ">>> IS_MULTI_PHASE is $IS_MULTI_PHASE"
set GOOD_MULTI_EVENT = $DATADIR/$EQ/GOOD_MULTI_EVENT

	foreach PHASE ( `echo $EQ_PHASE_LIST `)
		# here we check if current phase is multi phase and if it is within our list
		if( $IS_MULTI_PHASE == "YES" ) then
			set simple_phase = `python $SHELL_DIR/NAMING_CONVENTION.py $PHASE`
			set IS_PHASE_EXIST = `cat $GOOD_MULTI_EVENT |grep $EQ |grep -w $simple_phase |awk '{print $1}'`
			echo "---> orig phase is $PHASE simple phase is $simple_phase IS_PHASE_EXIST $IS_PHASE_EXIST"
			if( $IS_PHASE_EXIST == "" || $IS_PHASE_EXIST == " " ) then
				echo "--------->"
				echo "---------> Current Phase is skipped cause of user choice in GOOD_MULTI_EVENT"
				continue
			endif
		endif



		echo "============================================================================="
		echo "****Empirical Wavelet Algorithm Running for $EQ   PHASE: $PHASE **********`date`"
		set INPUT = ($EQ $DATADIR  $PLOTDIR $C_DIR $SHELL_DIR   $DIR $PHASE)
		set father =					$SHELL_DIR/c00.go_for_ESF.sh
		csh $father $INPUT
		echo "***Empirical Wavelet Algorithm Ending for $EQ   PHASE: $PHASE **********`date`"
		echo "============================================================================="

	end #  PHASE
csh $PWD/code_dir/plot.sh $ID $EQ $DATADIR




set ed_check_file = $ED_CHECK_FILE
set fff = `cat $ed_check_file |grep $EQ |awk 'NR==1 {print $1}' `
if($fff != "" ) then
mv $PLOTDIR/$EQ $PLOTDIR/${EQ}_update
endif
