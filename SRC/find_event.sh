#!/bin/tcsh

set PWD = `pwd`
set command = $PWD/com


cat <<EOF >! $command
use hongyu_db1;
SELECT DISTINCT EQ_NAME FROM EQTIME;
EOF


hongyusql_command $command|grep 1994
