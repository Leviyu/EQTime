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
set get_DATA = $SRCDIR/get_EQ_sac

echo "---> Prepare Data for $EQ PHASE $PHASE"
cd $work_dir
set EQ_SAC_FILE_DIR = `cat $work_dir/INFILE |grep EQ_SAC_FILE_DIR |awk '{print $2}'`
set Fix_missing_sta_flag = `cat $work_dir/INFILE |grep Fix_missing_sta_flag |awk '{print $2}'`

# =========================== download eventStation file ======================
set old_event = eventStation.${EQ}
set old_event2 = eventStation.dump.${EQ}
$get_DATA ${EQ}/$old_event $EQ_SAC_FILE_DIR
$get_DATA ${EQ}/$old_event2 $EQ_SAC_FILE_DIR

set tmp_event = eventStation.tmp2
cat $old_event $old_event2 |sort|uniq >! $tmp_event


set event = $work_dir/eventStation.${EQ}.${PHASE}.${COMP}
# ========================== find the right distance range ===================
awk '{if ( $3>='$DISTMIN' && $3 <= '$DISTMAX' ) print $0}' $tmp_event |sort -n -k 3 > $event


# ======
# we check to make sure when Reprocessing_Flag is on, we only selected those
# that is picked
set Reprocessing_Flag = `cat $work_dir/INFILE |grep Reprocessing_Flag |awk '{print $2}'`
if($Reprocessing_Flag == 1) then
echo "-> Reprocessing_Flag"
#set picked_eventinfo = ~/Catalog_Plots/mother_dir/C01_make_eventinfo/TravelTime_all.Dec1_2017/travel_time.ascii
set Reprocessing_event = $work_dir/event.Reprocessing_Flag
cat /dev/null >! $Reprocessing_event
set current_eq_phase_list = $work_dir/list.Reprocessing_Flag.${EQ}.${PHASE}
set command = $work_dir/.command
cat << EOF >! $command
use hongyu_db1;
SELECT STA,DT2,POLAR_FLAG,POLARITY FROM EQTIME 
WHERE EQ_NAME = ${EQ} && PHASE = "${PHASE}";
EOF
#SELECT STA,DT+MANUAL_SHIFT,POLAR_FLAG,POLARITY FROM EQTIME 
hongyusql_command $command |awk 'NR > 1 {print $0}' > ! $current_eq_phase_list

#//cat $picked_eventinfo|grep -w $EQ |grep -w $PHASE |awk '{print $1,$19+$46,$13,$43}' >! $current_eq_phase_list
foreach STA (`cat $current_eq_phase_list|awk '{print $1}'`)
	cat $event |grep -w $STA >> $Reprocessing_event
end #STA
mv $Reprocessing_event $event

endif # Reprocessing_Flag
# ======






# =============================================================
# if Fix_missing_sta_flag == 1
# we use previous S_ES as empirical wavelet and only process for those 
# that has not been procesed
# =============================================================
if($Fix_missing_sta_flag == 1) then
echo "Fix_missing_sta_flag processing begin"
set new_event_list = $work_dir/old.event.list
cat $event |awk '{print $1}'|sort|uniq >! $new_event_list

set old_event_file = /mnt/soddisk/soduser/Hongyu_DIR/eventStation_orig/bk_nov_16/eventStation.${EQ}
set old_event_list = $work_dir/new.event.list
cat $old_event_file |awk '{print $1}'|sort|uniq >! $old_event_list

set diff_list = $work_dir/diff.event.list
comm $old_event_list $new_event_list -13 >! $diff_list
set missing_sta_event = $work_dir/missing.sta.list
cat /dev/null >! $missing_sta_event
foreach sta (`cat $diff_list`)
	grep -w $sta $event |awk 'NR==1 {print $0}' >> $missing_sta_event
	end
mv $missing_sta_event $event
set S_ES_DIR = /mnt/data2/hongyu/Catalog_Plots/mother_dir/C01_make_eventinfo/eventinfo_dir/S_ES_DIR_all
cp $S_ES_DIR/${EQ}.S_ES $work_dir/main_ES.out
endif



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
$get_DATA  ${EQ}/$sacname1  $EQ_SAC_FILE_DIR> & /dev/null 
##$get_DATA  ${EQ}/$sacname1   &
$get_DATA  ${EQ}/$sacname2  $EQ_SAC_FILE_DIR> & /dev/null
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
bp co $bp_min $bp_max n 2 p 1
write over
EOF
#sac < $macro > &/dev/null

else if ($filter_flag == "lp") then

cat<< EOF >> $macro
r $sacname
rtr
rmean
interpolate delta ${DELTA}
lp co $bp_min n 2 p 1
write over
EOF


else
cat<< EOF >> $macro
r $sacname
rtr
rmean
interpolate delta ${DELTA}
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


