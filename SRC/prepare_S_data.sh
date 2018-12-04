#!/bin/tcsh

set event_file = ./EQ_LIST_DIR/SdataPatch1
set command = ./.command


cat << EOF >! $command
use hongyu_db1;
SELECT EQ_NAME FROM EQ_LIST 
WHERE ON_EQ1 = 1
AND STRIKE IS NOT NULL
AND EQ_YEAR = 2016
AND EW_DATASET is NULL
AND STA_NUM > 1300;
EOF

hongyusql_command $command |awk 'NR>1 {print $0}' >! $event_file


