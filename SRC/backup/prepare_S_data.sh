#!/bin/tcsh



set YEAR = 2012
set event_file = ./EQ_LIST_DIR/LIST.S.${YEAR}
set command = ./.command


cat << EOF >! $command
use hongyu_db1;
SELECT EQ_NAME FROM EQ_LIST 
WHERE ON_EQ1 = 1
AND STRIKE IS NOT NULL
AND EQ_YEAR = $YEAR
AND EW_DATASET is NULL
AND STA_NUM > 1000;
EOF

hongyusql_command $command |awk 'NR>1 {print $0}' >! $event_file


