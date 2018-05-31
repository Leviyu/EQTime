#!/bin/tcsh

set PWD = `pwd`

set event = $PWD/eventinfo.M7
set new_event = $PWD/.new

cat $event|awk '{print $12,$1,$20,$19}' >! $new_event


set com = $PWD/.com

cat << EOF >! $com
use hongyu_db1;
create table tmp50 (
EQ_NAME varchar(15) NULL,
STA varchar(15) NULL,
PHASE varchar(15) NULL,
DT float NULL
);
LOAD DATA LOCAL INFILE '$new_event' INTO TABLE tmp50
FIELDS TERMINATED BY ' '
LINES TERMINATED BY '\n';
EOF

#//hongyusql_command $com

cat << EOF >! $com
use hongyu_db1;
update EQTIME INNER JOIN tmp50
on 
tmp50.EQ_NAME = EQTIME.EQ_NAME AND
tmp50.STA = EQTIME.STA AND
tmp50.PHASE = EQTIME.PHASE 
set EQTIME.DT2 = tmp50.DT
WHERE
tmp50.EQ_NAME = EQTIME.EQ_NAME AND
tmp50.STA = EQTIME.STA AND
tmp50.PHASE = EQTIME.PHASE;
EOF
hongyusql_command $com

