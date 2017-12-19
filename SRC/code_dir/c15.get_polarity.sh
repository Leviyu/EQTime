#!/bin/csh
# ====================================================================
# This is a csh script to get the polarity for record
# 
# 	We dont use polar info for ScS and Sdiff
#
# Input:	CMT 
#			PHASE
#			COMP
# Output:
#
#
# DATE:					Keywords:
#
# Reference:
# ===================================================================


set EQ = $1
set DATADIR = $2
set PLOTDIR = $3
set C_DIR = $4
set SHELL_DIR = $5
set DISTMIN = $6
set DISTMAX = $7
set PHASE = $8
set COMP = $9
set filter_flag = $10

set work_dir = $DATADIR/$EQ
cd $DATADIR/$EQ
set polarity_info = $DATADIR/$EQ/eventinfo.polarity.${PHASE}.${COMP}
set polar_PPP = $DATADIR/$EQ/PPP.polar
cat /dev/null >! $polar_PPP
set eventStation = $DATADIR/$EQ/eventStation.${EQ}.${PHASE}.${COMP}
set CMT_file = $SHELL_DIR/CMT.data
set CMT = `grep -w $EQ $CMT_file`
echo "----> Looking for CMT Info "

set EQ_name_found = `echo $CMT |awk '{print $1}'`

# if we dont find CMT info, we dont use CMT
if( $EQ_name_found  == "" ) then
echo "----> CMT for $EQ is not found"
awk '{print $1,0}' $eventStation > $polarity_info
exit 0
else 
echo "----> CMT for $EQ is $CMT"
endif


set strike =  $CMT[2]
set dip = $CMT[3]
set rake = $CMT[4]

set CMT2RAD = $SHELL_DIR/radiation.f90
f95 $CMT2RAD -o CMT2RAD
./CMT2RAD << EOF > & /dev/null
$strike $dip $rake
EOF

# find the component P SV SH for current phase
if(`echo $PHASE |grep P` != "") then
	set the_comp = P
endif

if(`echo $PHASE |grep S` != "") then
	if( $COMP == T ) then
	set the_comp = SH
	else if($COMP == R ) then
	set the_comp = SV
	endif
endif


set RAD_code = $SHELL_DIR/radiation_th,az.f
f95 $RAD_code -o RADTHAZ
/bin/rm -r $polarity_info > & /dev/null
foreach STA (`awk '{print $1}' $eventStation`)
	set TMP = `awk -v sta=$STA '$1==sta {print $0}' $eventStation`
	set NET = $TMP[2]
	set DIST = $TMP[3]
	set AZ = $TMP[5]
	set EQ_DEP = $TMP[13]



	set take_off = `csh $SHELL_DIR/c101.get_taup_takeoff_for_record.sh $EQ $STA $PHASE|awk 'NR==1 {print $1}'`

##if(`echo $PHASE | grep m ` != "" ) then
##set PHASE = `echo $PHASE | sed 's/.$//' `
##endif
##set take_off = `taup_time -mod prem -ph $PHASE -deg $DIST -h $EQ_DEP | awk 'NR==6 {print $6}'`

##echo $STA $strike $dip $rake $the_comp $take_off $AZ	
./RADTHAZ <<EOF >$work_dir/polarity.tmp
$strike $dip $rake
$the_comp
$take_off $AZ
EOF

	set polar_pre = `awk -v sta=$STA 'NR==1 {print $1}' $work_dir/polarity.tmp`
	##set polar_pre = 0.1
# =================================================
## for records that have polarity prediction < -0.15 or > 0.15, then we produce a new station called
# ${STA}_PPP
	set new_sta = ${STA}_PPP
	set small_flag = `echo $polar_pre |awk '{if ($1 > -0.15 && $1 < 0.15) print "Y"}'`
	if($small_flag == "Y") then
##echo $new_sta
		## add new station info into eventinfo
		cat $eventStation |awk -v dd=${STA}  ' {if ( $1 == dd ) print $0}'|awk -v kk=$new_sta '{$1=kk; print $0}' >> $eventStation
		##cat $eventStation |awk -v dd=${STA}  ' {if ( $1 == dd ) print $0}'|awk -v kk=$new_sta '{$1=kk; print $0}' 

		## produce new sac file
#set sac_file = `ls ${EQ}.${NET}.${STA}.*sac`
#set new_sac = ${EQ}.${NET}.${new_sta}.BHT.sac
		set sac_file = ${EQ}.${NET}.${STA}.BH${COMP}.sac
		set sac_file2 = ${EQ}.${NET}.${STA}.HH${COMP}.sac
		set new_sac = ${EQ}.${NET}.${new_sta}.BH${COMP}.sac

##sac << EOF >> /dev/null
##r $sac_file 
##mul -1 
##w $new_sac
##EOF
		if(-e $sac_file ) then
			cp $sac_file $new_sac
		else
			cp $sac_file2 $new_sac
		endif
		echo "$new_sta $polar_pre" >> $polar_PPP
		
	endif


# =================================================

	
	echo "$STA $polar_pre" >> $polarity_info

	end	#STA

cat $polar_PPP >> $polarity_info


	echo "--> Looking for CMT and polar info DONE"

exit 0


