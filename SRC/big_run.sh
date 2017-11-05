#!/bin/tcsh


set PWD = `pwd`
set INFILE = $PWD/INFILE.mother
set BIG_ID = `cat $INFILE|grep TASK_NAME |awk 'NR==1 {print $2}'`
set EQ_LIST = `cat $INFILE |grep EQ_LIST|awk 'NR==1 {print $2}'`
set processes = `cat $INFILE |grep Process_Number|awk 'NR==1 {print $2}'`







set EQ_LIST = $PWD/$EQ_LIST
cp $EQ_LIST $PWD/INPUT_EQ_LIST
set TPWD = `pwd`

set EQ_NUM = `cat $EQ_LIST |wc -l`
set INC = `echo "$EQ_NUM / $processes "|bc -l`
set INC = `printf "%.0f" $INC`
mkdir -p $TPWD/LOG


set PART = 1
while ( $PART <= $processes)
echo "--> Working on $PART / $processes for $BIG_ID"

set BEG = `echo " ($PART - 1)*$INC +1"|bc`
set END = `echo "($PART)*$INC "|bc`
set PART_FILE = $TPWD/LOG/list.EQ.${BIG_ID}.${PART}
cat $EQ_LIST |awk '{ if(NR>='$BEG' && NR<='$END') print $0}' >! $PART_FILE
##cat $PART_FILE
cd $TPWD
set ID = ${BIG_ID}_${PART}
cp $PART_FILE $TPWD/INPUT_EQ_LIST
csh $TPWD/mother.sh $ID 
sleep 3s

rm $TPWD/INPUT_EQ_LIST
@ PART ++
end
