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
set macro = $work_dir/sac.macro
set EQ_SAC_FILE_DIR = `cat ./INFILE |grep EQ_SAC_FILE_DIR|awk '{print $2}'`

# =========================== download eventStation file ======================
set old_event = eventStation.${EQ}
$get_DATA ${EQ}/$old_event $EQ_SAC_FILE_DIR

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
## get incident angle
	set TAUP_DIR = `cat $work_dir/INFILE |grep -w TAUP_DIR |awk 'NR==1 {print $2}'`
	set SHELL_DIR = `cat $work_dir/INFILE |grep -w SHELL_DIR |awk 'NR==1 {print $2}' `
	mkdir -p $TAUP_DIR/$EQ
	set taup_file = $TAUP_DIR/$EQ/taup_time.${EQ}.${STA}
	if( ! -e $taup_file ) then
	csh $SHELL_DIR/c106.from_eventStation_make_big_taup_file.sh  $EQ $STA $work_dir/eventStation.${EQ} $TAUP_DIR/$EQ
	endif

	set incident = `cat $taup_file |grep -w $PHASE |awk 'NR==1 {print $7}'`
#//echo "STA $STA PHASE $PHASE incident $incident"
	if($incident == "") then
	set incident = 0
	endif
#
#set fff_is_multi = `echo $PHASE |grep m `
#if($fff_is_multi == "" ) then
#set phase_travel_time = `cat $taup_file |grep -w $PHASE |awk '$8<180 {print $4}' |awk 'NR==1 {print $1}'`
#else 
#set phase_travel_time = `cat $taup_file |grep -w $PHASE |awk '$8>180&&$8<360 {print $4}'|awk 'NR==1 {print $1}'`
#endif
#if($phase_travel_time == "" )then
#set phase_travel_time = 220
#endif

#set time_cut_min = `echo "$phase_travel_time -210 "|bc -l `
#set time_cut_max = `echo "$phase_travel_time +210 "|bc -l `
#echo "$time_cut_min $time_cut_max cut minmax $STA"


	set sacname1 = ${EQ}.${NET}.${STA}.BHR.sac
	set sacname2 = ${EQ}.${NET}.${STA}.BHZ.sac
	set sacname3 = ${EQ}.${NET}.${STA}.HHR.sac
	set sacname4 = ${EQ}.${NET}.${STA}.HHZ.sac
$get_DATA  ${EQ}/$sacname1 $EQ_SAC_FILE_DIR > & /dev/null 
$get_DATA  ${EQ}/$sacname2  $EQ_SAC_FILE_DIR> & /dev/null
$get_DATA  ${EQ}/$sacname3  $EQ_SAC_FILE_DIR> & /dev/null
$get_DATA  ${EQ}/$sacname4  $EQ_SAC_FILE_DIR> & /dev/null


	if(-e $sacname1 && -e $sacname2 ) then
	set sacR = $sacname1
	set sacZ = $sacname2
	else if (-e $sacname3 && -e $sacname4) then
	set sacR = $sacname3
	set sacZ = $sacname4
	else
	continue
	endif


#filter 
if( $filter_flag == "bp") then
cat> $macro << EOF
r $sacR
rtr
rmean
interpolate delta $DELTA
bp co $bp_min $bp_max n 2 p 2
write over
r $sacZ
rtr 
rmean
interpolate delta $DELTA
bp co $bp_min $bp_max n 2 p 2
write over
q
EOF
sac < $macro > &/dev/null

else if ($filter_flag == "lp") then

cat > $macro << EOF
r $sacR
rtr
rmean
interpolate delta $DELTA
lp co $bp_min  n 2 p 2
write over
r $sacZ
rtr 
rmean
interpolate delta $DELTA
lp co $bp_min  n 2 p 2
write over
q
EOF

sac <$macro >& /dev/null
endif


set R_npts = `saclst npts f $sacR|awk '{print $2}'`
set Z_npts = `saclst npts f $sacZ|awk '{print $2}'`
set comm_npts = `echo $R_npts $Z_npts |awk '{if($1>$2) print $2; else print $1}'`
set npts_diff = `echo "$R_npts - $Z_npts"|bc`


if( $npts_diff > 1000 ) then
echo "R and Z file npts is too far away ERROR STA $STA"
rm $sacR $sacZ
continue
endif

cat >! $macro << EOF
r $sacR $sacZ
interpolate npts $comm_npts
rotate through $incident
w $sacR $sacZ
q
EOF
#//cat $macro
#//sac < $macro 
#sac < $macro > & /dev/null

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


