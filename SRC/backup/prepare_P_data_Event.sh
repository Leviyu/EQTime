#!/bin/tcsh

set event_file = ./EQ_LIST_DIR/PdataPatch3
set command = ./.command


cat << EOF >! $command
use hongyu_db1;
SELECT EQ_NAME FROM EQ_LIST 
WHERE ON_EQ1 = 1
AND STRIKE IS NOT NULL
AND STA_NUM > 1300
;
EOF

hongyusql_command $command |awk 'NR>1 {print $0}' >! $event_file


set checked_file = $PWD/EQ_LIST_DIR/.checked
set selected_file = $PWD/EQ_LIST_DIR/.selected 
cat $PWD/EQ_LIST_DIR/CHECKED_P_WAVE |sort|uniq >! $checked_file
cat $event_file >! $selected_file

comm -13 $checked_file $selected_file  >! $event_file







