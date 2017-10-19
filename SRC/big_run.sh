#!/bin/tcsh

set TPWD = `pwd`

#sleep 5h
# Ed_checked_2004_not_processed
# this is the 45 EQ that is the left over of 
# part1 and part2 for 2004 onward patch
set BIG_ID = Patch11
set EQ_LIST = $TPWD/EQ_LIST.PATCH11

set processes = 4

set EQ_NUM = `cat $EQ_LIST |wc -l`
set INC = `echo "$EQ_NUM / $processes "|bc -l`
set INC = `printf "%.0f" $INC`


set PART = 1
while ( $PART <= $processes)
echo "--> Working on $PART / $processes for $BIG_ID"

set BEG = `echo " ($PART - 1)*$INC +1"|bc`
set END = `echo "($PART)*$INC "|bc`
set PART_FILE = $TPWD/list.EQ.${BIG_ID}.${PART}
cat $EQ_LIST |awk '{ if(NR>='$BEG' && NR<='$END') print $0}' >! $PART_FILE
##cat $PART_FILE
cd $TPWD
set ID = ${BIG_ID}_ID_${PART}
cp $PART_FILE $TPWD/INPUT_EQ_LIST
csh $TPWD/mother.sh $ID
sleep 5s

@ PART ++
end
