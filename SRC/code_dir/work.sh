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

	foreach PHASE ( `echo $EQ_PHASE_LIST `)
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
