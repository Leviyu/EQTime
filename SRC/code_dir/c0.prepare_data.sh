#!/bin/csh

# ==================================================
#	This Script prepare the data for ESF
#	
#	Hongyu DATE: 
#	Key words: 
# ==================================================


set EQ = $1
set DATADIR = $2
set work_dir = $3
set PLOTDIR = $4
set SRCDIR = $5
set DISTMIN = $6
set DISTMAX = $7
set PHASE = $8
set COMP = $9
set filter_flag = $10
set get_DATA = get_EQ_sac

echo "---> Prepare Data for $EQ PHASE $PHASE"
cd $work_dir

# =========================== download eventStation file ======================
set old_event = eventStation.${EQ}
$get_DATA ${EQ}/$old_event 

set event = $work_dir/eventStation.${EQ}.${PHASE}.${COMP}
# ========================== find the right distance range ===================
awk '{if (NR>1 && $3>='$DISTMIN' && $3 <= '$DISTMAX' ) print $0}' $old_event |sort -n -k 3 > $event
set total_num = `cat  $event |wc -l`
echo "Total records: $total_num"

set INFILE = $work_dir/INFILE_${PHASE}
set bp_min = `grep -w Frequency_range $INFILE | awk '{print $2}'`
set bp_max = `grep -w Frequency_range $INFILE | awk '{print $3}'`
set LONG_BEG = `grep -w LONG_BEG $INFILE |awk '{print $2}'`
set LONG_LEN = `grep -w LONG_LEN $INFILE |awk '{print $2}'`

set DELTA = `cat $work_dir/INFILE |grep DELTA |awk '{print $2}'`


# ========================= download sac file =================
foreach STA (`cat $event |awk '{print $1}'`)
	set tmp1 = `awk -v sta=$STA '$1==sta {print $0}' $event`
	set EQ = $tmp1[19]
	set NET = $tmp1[2]
	set DIST = $tmp1[3]
	set EQ_DEP = $tmp1[13]

	set sacname1 = ${EQ}.${NET}.${STA}.BH${COMP}.sac
	set sacname2 = ${EQ}.${NET}.${STA}.HH${COMP}.sac
	##if(-e $sacname1 || -e $sacname2 ) then
	##set sac_file_already_exist = 0
	##else
$get_DATA  ${EQ}/$sacname1  > & /dev/null 
##$get_DATA  ${EQ}/$sacname1   &
$get_DATA  ${EQ}/$sacname2  > & /dev/null
##$get_DATA  ${EQ}/$sacname2    &
	##endif


	if(-e $sacname1 ) then
	set sacname = $sacname1
	else if (-e $sacname2 ) then
	set sacname = $sacname2
	else
	continue
	endif

set macro = $work_dir/sac.macro
cat /dev/null >! $macro

# ================================
# Deal with SS Huawei
if( $PHASE == "SS" ) then
cat<< EOF >> $macro
r $sacname
hilbert
write over
EOF
#//sac < $macro > & /dev/null
endif


# ================================
# Deal with SSS Huawei
if( $PHASE == "SSS" || $PHASE == "SSSm" ) then
cat<< EOF >> $macro
r $sacname
mul -1
write over
EOF
#sac < $macro > & /dev/null
endif

# ================================
# Deal with SSSS Huawei
if( $PHASE == "SSSS" || $PHASE == "SSSSm" ) then
cat<< EOF >> $macro
r $sacname
hilbert
write over
EOF
#sac < $macro > & /dev/null
endif

# ================================
# Deal with SSSSS Huawei
if( $PHASE == "SSSSS" || $PHASE == "SSSSSm" ) then
set huawei == "haha"
endif

# ================================
# Deal with S6 Huawei
if( $PHASE == "SSSSSS" || $PHASE == "SSSSSSm"  ) then
cat<< EOF >> $macro
r $sacname
hilbert
mul -1
write over
EOF
#sac < $macro > & /dev/null
endif




#filter 
if( $filter_flag == "bp") then
cat<< EOF >> $macro
r $sacname
rtr
rmean
interpolate delta ${DELTA}
bp co $bp_min $bp_max n 2 p 2
write over
EOF
#sac < $macro > &/dev/null

else if ($filter_flag == "lp") then

cat<< EOF >> $macro
r $sacname
rtr
rmean
interpolate delta ${DELTA}
lp co $bp_min n 2 p 2
write over
EOF

#sac <$macro >& /dev/null

endif


sac <$macro >& /dev/null


	end #foreach

# =============================================================
# throw away stations that reported to different network and just keep one
# =============================================================
/bin/rm $work_dir/event_uniq >& /dev/null
awk '{print $1}' $event |sort -u > $work_dir/sta.list
foreach sta (`cat $work_dir/sta.list`)
	grep $sta $event |awk 'NR==1 {print $0}' >> $work_dir/event_uniq
	end


# sort according to distance	
sort -n -k 3 event_uniq |uniq > $event

#clean up 
/bin/rm $work_dir/sta.list $work_dir/event_uniq


exit 0


