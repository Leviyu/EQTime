#!/bin/tcsh

set PWD = $1
set ID = $2


set WORKDIR = `cat $PWD/INFILE |grep DATADIR |awk 'NR==1 {print $2}'`
mkdir -p $WORKDIR
mkdir -p $WORKDIR/$ID



set old_file = $PWD/INFILE
set new_file = $WORKDIR/${ID}/INFILE
cp $old_file $new_file

set bigdir = `cat $old_file|grep BIGDIR|awk '{print $2}'`

foreach NAME ( CURRENT_DIR  PLOTDIR C_DIR SHELL_DIR TAUP_DIR ED_CHECK_FILE T_star_lib )
	set full_name = "<${NAME}>"
	set name_content = `cat $old_file |grep $NAME |awk 'NR==1 {print $2}'`
	set new_name = "<${NAME}>\t\t\t\t\t\t$bigdir/$name_content"
	sed -i "/${full_name}/c ${new_name}" $new_file
end


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

set INFILE = ${WORKDIR}/$EQ/INFILE
set DIR =						`grep -w CURRENT_DIR $INFILE | awk '{print $2}'`
set PLOTDIR =					`grep -w PLOTDIR $INFILE | awk 'NR==1 {print $2}'`
set C_DIR =						`grep -w "<C_DIR>" $INFILE | awk '{print $2}'`
set TAUP_DIR =					`grep -w TAUP_DIR $INFILE | awk '{print $2}'`
set SHELL_DIR =					`grep -w SHELL_DIR $INFILE | awk '{print $2}'`

set DATADIR = $WORKDIR/$ID
set big_id = `echo $ID |awk -F"." '{print $1}'`

set PLOTDIR = $PLOTDIR/$big_id
mkdir -p $DATADIR 
mkdir -p $PLOTDIR
cp $SHELL_DIR/CMT.data $DATADIR/
cp $PWD/code_dir/INPUT_EQ_LIST $DATADIR/
cp $PWD/code_dir/GOOD_MULTI_EVENT $DATADIR/
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
	#/bin/rm -rf $DATADIR/$EQ

end #EQ

