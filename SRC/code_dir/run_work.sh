#!/bin/csh
# ====================================================================
# This is a csh script for ESF 
# 
# Input: INFIL ---- all the parameter that is needed
#
# DATE: Sep 12 2014			Keywords: ESF
# ===================================================================


## ======= Read in parameter ===========


set ID = $1
set WORKDIR = $2
set PWD = $3
if($ID == "" || $ID == " ") then
echo "======================================================"
echo "=============Be careful My friend, ID does not exist!========================="
echo "======================================================"
exit 0
endif

set EQ = $ID

echo "======================================================"
echo "=============Working on ID $ID in Progress========================="
echo "======================================================"

mkdir -p $WORKDIR/$EQ
cp $PWD/INFILE $WORKDIR/$EQ/


set INFILE = ${WORKDIR}/$EQ/INFILE
set DIR =						`grep -w CURRENT_DIR $INFILE | awk 'NR==1 {print $2}'`
set PLOTDIR =					`grep -w PLOTDIR $INFILE | awk 'NR==1 {print $2}'`
set C_DIR =						`grep -w "<C_DIR>" $INFILE | awk 'NR==1 {print $2}'`
set TAUP_DIR =					`grep -w TAUP_DIR $INFILE | awk 'NR==1 {print $2}'`
set SHELL_DIR =					`grep -w SHELL_DIR $INFILE | awk 'NR==1 {print $2}'`


#set DATADIR = $DATADIR/$ID
set DATADIR = $WORKDIR/$ID

set big_id = `echo $ID |awk -F"_" '{print $1}'`

set PLOTDIR = $PLOTDIR/$big_id
mkdir -p $DATADIR 
mkdir -p $PLOTDIR
cp $SHELL_DIR/CMT.data $DATADIR/
cp $PWD/code_dir/INPUT_EQ_LIST $DATADIR/
echo "------------------ ahahah"
cp $PWD/EQ_LIST_DIR/GOOD_MULTI_EVENT $DATADIR/
cp $PWD/INFILE_DIR/INFILE* $DATADIR/
cp $SHELL_DIR/c40.generate_master_pdf.sh $PLOTDIR
set EQ_LIST = $DATADIR/INPUT_EQ_LIST

echo "c dir is $C_DIR "
echo "copy CDIR to WORKDIR"
echo "cp $C_DIR $WORKDIR/$ID/ -r "
cp $C_DIR $WORKDIR/$ID/ -r 
set C_DIR = $WORKDIR/$ID/c_lib
mkdir -p $PWD/LOG

foreach EQ (`cat $EQ_LIST`)
	set INP = ( $PWD $DATADIR $PLOTDIR $EQ $C_DIR $SHELL_DIR $ID $DIR)
	echo "---> Working on $ID $EQ "
	csh $PWD/code_dir/work.sh $INP > & $PWD/LOG/logfile.${ID}.${EQ}
end #EQ

