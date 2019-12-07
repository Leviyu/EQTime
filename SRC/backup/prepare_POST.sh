#!/bin/tcsh
# prepare event list for post -processing

set PWD = `pwd`
set command = $PWD/LOG/.com.post

cat << EOF >! $command
use hongyu_db1;
SELECT EQ_NAME FROM EQ_LIST
WHERE CHECKED_FLAG = 1 &&
POST_DONE IS NULL;
EOF


hongyusql_command $command |awk 'NR>1{print $1}' >! $PWD/EQ_LIST_DIR/POST



