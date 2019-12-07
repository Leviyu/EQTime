#!/bin/csh

# ==================================================
#	This Script plot the catalog plot of ESF
#	
#	Hongyu DATE: 
#	Key words: 
# ==================================================


# 1. get all the zero weight eventinfo
set PWD = `pwd`
cd $PWD/DATADIR
set eventinfo = $PWD/DATADIR/eventinfo.zeroweight




#set EQ = 201704050609
#set PHASE = S
set EQ = $1
set PHASE = $2

set D1 = `ls /DATA1/EQTime/DATADIR/POST2*/$EQ -d` 
#echo $D1
set D2 = `echo $D1 |rev |awk -F"/" '{print $2"/"$3"/"$4"/"$5"/"}'|rev`
#echo $D2


set DATADIR = $D2
set PLOTDIR_main = $PWD/PLOTDIR
set PLOTDIR = $PLOTDIR_main/$EQ
mkdir -p $PLOTDIR
set SHELL_DIR = ~/EQTime/SRC/code_dir


set INFILE = $DATADIR/$EQ/INFILE_${PHASE}
set COMP = `grep -w COMP $INFILE |awk 'NR==1 {print $2}'`
set filter_flag = `grep -w filter_flag $INFILE |awk 'NR==1 {print $2}'`
set velocity_or_displacement = `grep -w velocity_or_displacement $INFILE |awk 'NR==1 {print $2}'`
set PHASE_BEG = `cat $INFILE |grep -w PHASE_BEG |awk '{print $2}'`
set PHASE_LEN = `cat $INFILE |grep -w PHASE_LEN |awk '{print $2}'`
set ED_CHECK_FILE1 = `cat $DATADIR/INFILE |grep -w ED_CHECK_FILE |awk '{print $2}'`
set PHASE_END = `echo "$PHASE_BEG + $PHASE_LEN"|bc -l `
set work_dir = $DATADIR/$EQ
set input = $work_dir/input
##set check_info = $DIR/storage_dir/${EQ}_${PHASE}_STA_LIST

set ESinfo = $work_dir/eventinfo.${EQ}.${PHASE}.${COMP}.tmp2
cat $eventinfo |grep -w $EQ |grep -w $PHASE >! $ESinfo


set total_number = `cat $ESinfo |wc -l`
echo "---> total num for $EQ $PHASE is $total_number "

set ESinfo_tmp = $work_dir/eventinfo.${EQ}.${PHASE}.${COMP}.tmp.tmp2



set BENCHMARK_FLAG = `cat $DATADIR/$EQ/INFILE |grep -w BENCHMARK_FLAG|awk '{print $2}'`
set reprocessing_flag = `cat $DATADIR/$EQ/INFILE |grep -w Reprocessing_Flag|awk '{print $2}'`

set ed_check_file = $ED_CHECK_FILE1

set new_patch_SS = /mnt/data2/hongyu/new_git/EWM/new_patch_SS.list
set exist_in_new_patch = `cat $new_patch_SS |grep $EQ |awk 'NR==1 {print $1}'`
if( $PHASE == "SS" && $exist_in_new_patch != "" ) then
## for SS , here is the rule
# if is exist in new_patch_SS.list, which is SS from patch1-4
# then we check them as good and plot them at the bottom.
# else we use ed_checked
	set ed_check_file = $new_patch_SS
endif

if( ! -e $ed_check_file ) then
	set fff = ""
	else
	set fff = `cat $ed_check_file |grep -w $EQ|grep -w $PHASE |awk 'NR==1 {print $1}'`
endif

set total_record_number = `cat $ESinfo |wc -l`
## we seperate the PPP sta into an extra file
#set PPP_sta_event = $work_dir/event.PPP
#cat $ESinfo |grep PPP >! $PPP_sta_event
#cat $ESinfo |grep -v PPP >! $ESinfo_tmp
#if($fff != "") then
#cat $ESinfo >! $ESinfo_tmp
#endif
#cp $ESinfo_tmp $ESinfo


echo "=============================================="
echo "---> Begin Catalog Plot for $EQ PHASE: $PHASE "

set tmp_ps = $PLOTDIR/tmp.ps
/bin/rm $tmp_ps 2>&/dev/null
set PWD = `pwd`









# ====================================================
# 			set output pdf file and plot parameter
# ====================================================
# sort according to $14 quality flag of recrods
#set ESinfo_tmp = $work_dir/eventinfo.${EQ}.${PHASE}.${COMP}.${filter_flag}_tmp.tmp2
#mv $ESinfo $ESinfo_tmp
## quality = 2 or quality = 3 record goes into info first


# copy eventinfo to PLOTDIR
cp $ESinfo $PLOTDIR/




set OUTFILE_pdf = $PLOTDIR/${EQ}_${PHASE}_${COMP}_${filter_flag}_catalog_all_record.pdf
if($fff != "") then
set OUTFILE_pdf = $PLOTDIR/u${EQ}_${PHASE}_${COMP}_${filter_flag}_catalog_all_record.pdf
endif





# =============================
# gmt setting and Page Setting 
cd $work_dir
gmtset TICK_PEN = 0.2p
gmtset ANNOT_FONT_SIZE_PRIMARY = 5p
gmtset FRAME_PEN = 0.4p
gmtset ANNOT_OFFSET_PRIMARY = 0.1c
gmtset PAPER_MEDIA = A4

set PLOT_PER_PAGE = 16
set PAGE = 1
set current_record = 1
set ploted_record = 1
set arrow_parameter = "-Svh0.005i/0.08i/0.01i"


set record_with_low_amp_list = $PLOTDIR/list.station.zero_amplitude.${EQ}.${PHASE}.txt
cat /dev/null >! $record_with_low_amp_list



# =============================

# establish a file containing the records that should be thrown away because of being too close to other phases only for good records


# ====================================================
# 	plot record one by one
# ====================================================

echo " -->figure out how many trace is actually being plotte"


set record_num = `cat $ESinfo |wc -l`

set num_record_getting_skipped = 0
set new_ESinfo = $work_dir/new.esinfo
cat /dev/null >! $new_ESinfo

foreach sta(`cat $ESinfo |awk '{print $1}'`)
set long = $work_dir/${EQ}.${sta}.${PHASE}.${COMP}.long
set emp = $work_dir/${EQ}.${sta}.${PHASE}.${COMP}.emp
set gau = $work_dir/${EQ}.${sta}.${PHASE}.${COMP}.gau
set orig_emp = $work_dir/${EQ}.${sta}.${PHASE}.${COMP}.orig.emp
set STA = $sta


if($fff != "") then
		set TMP = `cat $ESinfo|awk -v dd=$sta '$1==dd {print $0}'`
		set sta_SNR = $TMP[18]
		set ed_check_flag = `cat $ed_check_file |grep -w $EQ |grep -w $PHASE |grep -w $sta |awk 'NR==1 {print $1}'`
		set too_small = `echo $sta_SNR |awk '{if($1 < 1.5 ) print "small"}'`
		if($PHASE == "S" && $too_small == "small" && $ed_check_flag == "" ) then
@ num_record_getting_skipped ++
echo "$PHASE $sta SNR_less_1point6" >> $record_with_low_amp_list
continue
		endif
endif
##echo "---> sta $sta cmt $CMT_polar_prediction"

## make sure that long is not horizontal line, meaning the amplitude max is greater then 0.5
set long_amp = `cat $long |minmax -C |awk '{print $4}'`
set long_amp_flag = `echo $long_amp |awk '{ if($1 > 0.1 ) print "Y"; else print "N" }'`
if($long_amp_flag == "Y") then
set a = 1
else 
@ num_record_getting_skipped ++
echo "$PHASE $sta long_amp_too_small" >> $record_with_low_amp_list
continue
endif

if( ! -e $long || ! -e $emp ) then
@ num_record_getting_skipped ++
echo "$PHASE $sta long_or_emp_not_exist ">> $record_with_low_amp_list
continue
endif

## for records have non/inf value, we skip them
set fff_long_inf1 = `cat $long |grep inf |awk 'NR==1 {print $1}'`
set fff_long_inf2 = `cat $long |grep nan |awk 'NR==1 {print $1}'`
if($fff_long_inf1 != "" || $fff_long_inf2 != "" ) then
@ num_record_getting_skipped ++
echo "$PHASE $sta nan/inf_in_long_file ">> $record_with_low_amp_list
continue
endif


cat $ESinfo |grep -w $sta >> $new_ESinfo

end # sta
mv $new_ESinfo $ESinfo

set record_num = `cat $ESinfo |wc -l`
echo "--->> $num_record_getting_skipped / $total_record_number records is getting skipped "


set num_record_plotted = `echo "$total_record_number - $num_record_getting_skipped"|bc `






foreach sta (`cat $ESinfo |awk '{print $1}'`)

## if sta contains PPP continue
##set PPP_sta = `echo $sta |grep PPP `
##if( $PPP_sta == "" || $PPP_sta == " " ) then
	##set a = 1
##else
##continue
##endif



##echo "---> Working on plotting $sta PHASE is $PHASE"
set long = $work_dir/${EQ}.${sta}.${PHASE}.${COMP}.long
set emp = $work_dir/${EQ}.${sta}.${PHASE}.${COMP}.emp
set phase = $work_dir/${EQ}.${sta}.${PHASE}.${COMP}.phase
set gau = $work_dir/${EQ}.${sta}.${PHASE}.${COMP}.gau
set orig_emp = $work_dir/${EQ}.${sta}.${PHASE}.${COMP}.orig.emp
#set CMT_polar_prediction_file = $work_dir/eventinfo.polarity.${PHASE}.${COMP}
#set CMT_polar_tmp  = `grep -w $sta $CMT_polar_prediction_file |awk 'NR==1 {print $2}'` 
#set CMT_polar_prediction = `printf "%.2f" $CMT_polar_tmp`
##echo "---> sta $sta cmt $CMT_polar_prediction"


## make sure that long is not horizontal line, meaning the amplitude max is greater then 0.5
set long_amp = `cat $long |minmax -C |awk '{print $4}'`
set long_amp_flag = `echo $long_amp |awk '{ if($1 > 0.1 ) print "Y"; else print "N" }'`
if($long_amp_flag == "Y") then
set a = 1
else 
echo $sta >> $record_with_low_amp_list
##@ current_record ++
@ ploted_record ++
continue
endif





if( ! -e $long ) then
##echo "$long does not exist! ========"
##@ current_record ++
@ ploted_record ++
continue
endif
if(! -e $emp ) then
##echo "$emp does not exist! ========="
##@ current_record ++
@ ploted_record ++
continue
endif

## for records have non/inf value, we skip them
set fff_long_inf1 = `cat $long |grep inf |awk 'NR==1 {print $1}'`
set fff_long_inf2 = `cat $long |grep nan |awk 'NR==1 {print $1}'`
if($fff_long_inf1 != "" || $fff_long_inf2 != "" ) then
@ ploted_record ++
continue
endif

if($fff != "") then
		set TMP = `cat $ESinfo|awk -v dd=$sta '$1==dd {print $0}'`
		set sta_SNR = $TMP[18]
		set ed_check_flag = `cat $ed_check_file |grep -w $EQ |grep -w $PHASE |grep -w $sta |awk 'NR==1 {print $1}'`
		set too_small = `echo $sta_SNR |awk '{if($1 < 1.5 ) print "small"}'`
		if($PHASE == "S" && $too_small == "small" && $ed_check_flag == "" ) then
		##if($PHASE != "S" && $sta_SNR < 1.6  ) then
@ ploted_record ++
continue
		endif
endif

# =============== get info prepared section ============
set infoES  = `awk -v STA=$sta 'STA==$1 {print $0}' $ESinfo`
set STA = $infoES[1]
set NET = $infoES[2]
set DIST = `printf "%.1f" $infoES[3]`
set AZ = `printf "%.1f" $infoES[4]`
set BAZ = `printf "%.1f" $infoES[5]`
set STA_lat = `printf "%.1f" $infoES[6]`
set STA_lon = `printf "%.1f" $infoES[7]`
set EQ_lat = `printf "%.1f" $infoES[8]`
set EQ_lon = `printf "%.1f" $infoES[9]`
set EQ_dep = `printf "%.1f" $infoES[10]`
set EQ_mag = `printf "%.1f" $infoES[11]`
set EQ_name = $infoES[12]
set polar_flag = $infoES[13]
set quality_flag = $infoES[14]
set CMT_polar_prediction = $infoES[43]
## here we hardwire the picking info with our big eventinfo.final
##set final_event = $DIR/eventinfo.final
##ls $final_event
##set quality_flag = `cat $final_event |grep -w $EQ |grep -w $sta |grep -w $PHASE |awk 'NR==1 {print $14}'`
if($quality_flag == "") then
set quality_flag = -1
endif




##if( -e $check_info ) then
##set check_flag = `grep -w $STA $check_info |awk 'NR==1 {print $1}'`
##if($check_flag == $STA ) then
##set quality_flag = 1
##else 
##set quality_flag = -1
##endif
##endif



set prem = $infoES[15]
set phase_amplitude = $infoES[16]
set ccc = $infoES[17]
set SNR = $infoES[18]
set SNR2 = $infoES[31]
set dt_obs_prem = $infoES[19]
##echo "STA $STA dt_obs_prem is $dt_obs_prem"
set best_stretch_ccc = $infoES[21]
set best_stretch_coeff = $infoES[22]
set tstar_ccc = $infoES[36]
set tstar = $infoES[35]

set CCC3 = $infoES[37]

##set tstar = $best_stretch_coeff
set misfit = $infoES[23]
set misfit2 = $infoES[32]
set misfit_pre = $infoES[38]
set misfit_bak = $infoES[39]
set record_gaussian_factor = $infoES[40]

set NOISE_BEG = $infoES[27]
set NOISE_LEN = $infoES[28]
set NOISE_END = `echo "$NOISE_BEG + $NOISE_LEN"|bc -l`
set PHASE_WINDOW_BEG = $infoES[29]
set PHASE_WINDOW_END = `echo "$PHASE_WINDOW_BEG + $PHASE_LEN"|bc`

set record_weight = $infoES[30]

set misfit2T_pre = $infoES[46]
set misfit2T_bak = $infoES[48]
set misfit3T_pre = $infoES[47]
set misfit3T_bak = $infoES[49]
set SNR_peak_trough = $infoES[50]
set SNR_peak_ratio = $infoES[51]


set ONSET = $infoES[33]
set ENDSET = $infoES[34]
set one_period = $infoES[54]


##for ScS cut from -80 to 80 sec
##if($PHASE == "ScS") then
##awk '{if($1>-70 && $1 < 70) print $0}' $long > $work_dir/long_tmp
##mv $work_dir/long_tmp $long
##endif

set xy_tmp = `minmax -C $long`
set xmin = $xy_tmp[1]
set xmax = $xy_tmp[2]
set ymin = $xy_tmp[3]
set ymax = $xy_tmp[4]
# we compute surface wave time window and ignore everything within the window


## Now lets deal with the traffic phase part

##echo "current record is $current_record"

	if( $current_record == 1 ) then
		set OUTFILE = $PLOTDIR/catalog_all_eventstation_page_${PAGE}.ps
cat << EOF > ${OUTFILE}
[ /_objdef {ZaDb} /type /dict /OBJ pdfmark
[ {ZaDb} <<
    /Type /Font
    /Subtype /Type1
    /Name /ZaDb
    /BaseFont /ZapfDingbats
>> /PUT pdfmark
[ /_objdef {Helv} /type /dict /OBJ pdfmark
[ {Helv} <<
    /Type /Font
    /Subtype /Type1
    /Name /Helv
    /BaseFont /Helvetica
>> /PUT pdfmark
[ /_objdef {aform} /type /dict /OBJ pdfmark
[ /_objdef {afields} /type /array /OBJ pdfmark
[ {aform} <<
    /Fields {afields}
    /DR << /Font << /ZaDb {ZaDb} /Helv {Helv} >> >>
    /DA (/Helv 0 Tf 0 g)
    /NeedAppearances true
>> /PUT pdfmark
[ {Catalog} << /AcroForm {aform} >> /PUT pdfmark
EOF
		pstext -JX6i/1i -R-1/1/-1/1 -K -Y10.5i -N -P<<END>> $OUTFILE
		0 0.5 11 0 0 CB Catalog Plot for $EQ PHASE: $PHASE COMP:$COMP PAGE:$PAGE in $velocity_or_displacement filter: $filter_flag NUM: $num_record_plotted
		0 0.1 10 0 0 CB EQ_lat: $EQ_lat EQ_lon: $EQ_lon EQ_depth: $EQ_dep Mw: $EQ_mag 
END
		pstext -JX -R -O -K -N -P -X-0.3i <<END>>$OUTFILE
END
		# add flipping flag instruction
		psxy -JX -R0/7/-1/1 -Sc0.1 -G255/0/0 -Y-0.3i -O -N -K <<EOF>>$OUTFILE
0.5 0.3
EOF
#psxy -JX -R0/7/-1/1 -Sc0.1 -G255/0/0  -O -K -N <<EOF>>$OUTFILE
#0.5 -0.3
#EOF
		pstext -JX -R0/7/-1/1 -O -K -N <<EOF>>$OUTFILE
1 0.3 6 0 0 CB flipped
EOF
#//1 0.3  6 0 0 CB not flipped (black, not shown here)

set traffic_color = (green purple orange 104/178/255 blue 255/0/0 255/100/0 255/100/100)
		# add marker information for all phases
set x_loc = 1.0
set x_loc2 = 1.5
	##foreach color (`echo $traffic_color`)
		##psxy -JX -R0/7/-1/1 -W/${color}  -O -K <<EOF>>$OUTFILE
##$x_loc 0
##$x_loc2 0
##EOF
##set x_loc = `echo "$x_loc + 1"|bc -l`
##set x_loc2 = `echo "$x_loc2 + 1"|bc -l`
##end #color
		# ONSET symble
		##psxy -JX -R $arrow_parameter -G255/0/0  -O -K <<EOF>>$OUTFILE
##5.4 0 90 0.5
##EOF
		# misfit window symble
		##psxy -JX -R -Wfaint/255/153/51ta -O -K <<EOF>>$OUTFILE
##6.4 0.5
##6.4 -0.5
##EOF

set x_loc = 1.5
##set traffic_file = $work_dir/traffic_file.${EQ}.${STA}.${PHASE}
	##foreach phase_name (`cat $traffic_file |awk '{print $1}'`)
		##pstext -JX -R -O -K <<EOF>>$OUTFILE
##$x_loc 0 7	0 0 LB	$phase_name
##EOF
##set x_loc = `echo "$x_loc + 1"|bc -l`
	##end

#pstext -JX -R -O -K <<EOF>>$OUTFILE
#5.5 0 7 0 0 LB  ONSET
#EOF
#pstext -JX -R -O -K <<EOF>>$OUTFILE
##6.5 0 7 0 0 LB  misfit_win
#EOF
psxy -JX -R -O -K -N -Y0.2i << EOF>>$OUTFILE
EOF

endif

psxy -JX5i/0.5i -R$xmin/$xmax/-1/1  -O -K -N -Y-0.6i <<EOF>>$OUTFILE
EOF


##=======================================
## Plot window information
## 1 
set FLAG_COLOR = 204/255/204
set FLAG_FILE = $DATADIR/$EQ/FLAG_FILE.data
if($BENCHMARK_FLAG == 1) then
cat << EOF >! $FLAG_FILE
$PHASE_BEG -1
$PHASE_END -1
$PHASE_END 1
$PHASE_BEG 1
$PHASE_BEG -1
EOF
psxy $FLAG_FILE -JX5i/0.5i -R$xmin/$xmax/-1/1 -G$FLAG_COLOR -L -O -K -N  >>$OUTFILE
endif
if($BENCHMARK_FLAG == 2) then
cat << EOF >! $FLAG_FILE
$PHASE_WINDOW_BEG -1
$PHASE_WINDOW_END -1
$PHASE_WINDOW_END 1
$PHASE_WINDOW_BEG 1
$PHASE_WINDOW_BEG -1
EOF
psxy $FLAG_FILE -JX5i/0.5i -R$xmin/$xmax/-1/1 -G$FLAG_COLOR -L -O -K -N  >>$OUTFILE
endif
if($BENCHMARK_FLAG == 3) then
cat << EOF >! $FLAG_FILE
$ONSET -1
$ENDSET -1
$ENDSET 1
$ONSET 1
$ONSET -1
EOF
psxy $FLAG_FILE -JX5i/0.5i -R$xmin/$xmax/-1/1 -G$FLAG_COLOR -L -O -K -N  >>$OUTFILE
endif

if($BENCHMARK_FLAG == 4) then
set new_ONSET = `echo "$ONSET -3 "|bc`
set new_ENDSET = `echo "$ENDSET +3 "|bc`
cat << EOF >! $FLAG_FILE
$new_ONSET -1
$new_ENDSET -1
$new_ENDSET 1
$new_ONSET 1
$new_ONSET -1
EOF
psxy $FLAG_FILE -JX5i/0.5i -R$xmin/$xmax/-1/1 -G$FLAG_COLOR -L -O -K -N  >>$OUTFILE
endif
## =======================================


		psxy -JX5i/0.5i -R$xmin/$xmax/-1/1   -O -K <<EOF>>$OUTFILE
EOF

	# ================== plot the trace =====================
		set long_color = 0/0/0
		set long_color_flag = `echo ddd |awk '{if('$CMT_polar_prediction' >= -0.15 && '$CMT_polar_prediction' <= 0.15 ) print "green"; else print "hahaha"}'`
##echo "sta $sta long color is $long_color_flag"
		if($long_color_flag == "green" ) then
		set long_color = 178/102/255
		endif
		set is_ppp = `echo $sta |grep PPP`
		if($is_ppp == "" ) then
			set a = 1
		else
			set long_color = green
		endif

		##if($quality_flag > -1)then
		# if record has small polar prediction and is not flipped
		##if( $long_color_flag == "green" && $is_ppp == ""  ) then
			##set ppp_polar_flag = `cat $final_event |grep -w $EQ |grep -w $sta |grep -w $PHASE |awk 'NR==1 {print $13}'`
##echo "$sta in not flipped, polar = $ppp_polar_flag"
			##if($ppp_polar_flag == 1) then
				##set quality_flag = 1
			##else
				##set quality_flag = -1
			##endif
		##endif
		##if( $long_color_flag == "green" && $is_ppp != ""  ) then
			##set new_sss = `echo $sta|rev|cut -c 5-|rev`
			##set ppp_polar_flag = `cat $final_event |grep -w $EQ |grep -w $new_sss |grep -w $PHASE |awk 'NR==1 {print $13}'`
##echo "$sta is flipped, polar = $ppp_polar_flag"
			##if($ppp_polar_flag == -1) then
			##set quality_flag = 1
			##else
				##set quality_flag = -1
			##endif
		##endif
		####endif

# ==========================================================
	#	add traffic phase system symbal
set traffic_file = $work_dir/traffic_file.${EQ}.${PHASE}
if(! -e $traffic_file) then
set kkk = 1
else 
set traffic_num = `cat $traffic_file | awk -v sss=$STA '$1==sss {print $0}'|wc -l`
set NUM = 1
while ($NUM <= $traffic_num)
	set TMP = `cat $traffic_file |awk -v sss=$STA '$1==sss {print $0}' |awk -v dd=$NUM 'NR==dd {print $0 }' `
	set tPHASE = $TMP[2]
	set rel_time = $TMP[4]
#//echo "$STA $tPHASE $rel_time"
#//set too_close_flag = $TMP[4]
## if relative time too big, skip
set flag_rel_time = `echo $rel_time |awk '{ if ($1 > 100 || $1 < -100 ) print "HHH"}'`
if( $flag_rel_time == "HHH" ) then
@ NUM ++
	continue
endif


# for SKS and sSKS, make sure distance is between 80 - 125
if( $tPHASE == "SKS" || $tPHASE == "sSKS") then
set fff_dist_within_range = `echo $DIST |awk '{if ($1 >80 && $1 <125) print "Y"}'`
if($fff_dist_within_range != "Y") then
@ NUM ++
continue
endif
endif
# for SKKS and sSKKS, make sure distance is between > 100
if( $tPHASE == "SKKS" || $tPHASE == "sSKKS") then
set fff_dist_within_range = `echo $DIST |awk '{if ($1 > 100) print "Y"}'`
if($fff_dist_within_range != "Y") then
@ NUM ++
continue
endif
endif



	set flag = $TMP[4]
	##set color = ${traffic_color[${NUM}]}
	set color = blue
if($tPHASE == "SKS" || $tPHASE == "sSKS" || $tPHASE == "SKKS" ||$tPHASE == "sSKKS") then
	set color = 255/178/102
endif
		psxy -JX -R -W0.8p/${color} -O -K <<EOF>>$OUTFILE
$rel_time -1
$rel_time 1
EOF
## add name on top

set convert_code = $SHELL_DIR/NAMING_CONVENTION.py
set new_phase_name = `python $convert_code $tPHASE`

		pstext -JX -R -O -K << EOF>>$OUTFILE
$rel_time 0.7 3.5 0 0 LB $new_phase_name
EOF

@ NUM ++
	end #while
endif

	#	add traffic phase system symbal
# ==========================================================
		#  ===================== add vertical lines  =====================================

		psxy -JX -R -W4/204/71/228 -O -K <<EOF >> $OUTFILE
0 -1
0 1
EOF

		psxy $long -JX -R -W/$long_color  -O -K >>$OUTFILE

		## for records with small polar prediction, we plot its flipping too
		##if( $long_color_flag == "green" ) then
				##set long_PPP = $work_dir/${EQ}.${sta}_PPP.${PHASE}.${COMP}.long
				##set emp_PPP = $work_dir/${EQ}.${sta}_PPP.${PHASE}.${COMP}.emp
				##set gau_PPP = $work_dir/${EQ}.${sta}_PPP.${PHASE}.${COMP}.gau
	##psxy $long_PPP -JX5i/0.5i -R$xmin/$xmax/-1/1 -W/blue  -O -K >>$OUTFILE
		##endif


		# add a dot to indicate whether it is fliped or not
		if( $polar_flag == -1 ) then  # dark for not flip red for flipped recrod
			set flip_color = red
		set x_loc = `echo "$xmin + 40"|bc`
		psxy -JX -R -Sc0.25 -G$flip_color  -O -K -N <<EOF >> $OUTFILE
$x_loc 1
EOF
		else if ($polar_flag == 1) then
			set flip_color = 0/0/0
		else 
			set flip_color = yellow
		endif

		set quality_color = green 
		if($quality_flag == 2 || $quality_flag  == 3 ) then
		set quality_color = red
		endif
		##set x_loc = `echo "$xmin + 45"|bc`
		##pstext -JX -R -G$quality_color -O -K -N << EOF >>$OUTFILE
##$x_loc 1 8 0 0 LB $quality_flag
##EOF


		set emp_color = red 
		psxy $emp -JX -R -W/$emp_color -O -K>>$OUTFILE

#//psxy $phase -JX -R -W/blue -O -K >> $OUTFILE


		## add best fit gaussian here
		psxy $gau -JX -R -W/orange -O -K>>$OUTFILE


		# this is the not stretched empirical waveform
##psxy $orig_emp -JX -R -W/green -O -K>>$OUTFILE

foreach loc (-15 -10 -5 5 10 15)
		##psxy -JX -R -Wfaint,204/169/228,- -O -K <<EOF>>$OUTFILE
		psxy -JX -R -Wfaint,204/169/228 -O -K <<EOF>>$OUTFILE
$loc -0.1
$loc  0.1
EOF
end # foreach loc
		# add  little marker
		set imarker = -14
		set imarker_max = 14
		while ( $imarker <= $imarker_max)
		##psxy -JX -R -Wfaint/204/169/228ta -O -K <<EOF>>$OUTFILE
		psxy -JX -R -Wfaint/204/169/228 -O -K <<EOF>>$OUTFILE
$imarker 0.05
$imarker -0.05
EOF
@ imarker ++
			end
		#END  ===================== add vertical lines  =====================================

		# add ONSET 
		psxy -JX -R $arrow_parameter -G255/0/0  -O -K <<EOF>>$OUTFILE
$dt_obs_prem 0 90 0.5
EOF
		# add SNR window
		psxy -JX -R -Wfaint/0/0/255ta -O -K <<EOF>>$OUTFILE
$NOISE_BEG 0.5
$NOISE_BEG -0.5
EOF
		psxy -JX -R -Wfaint/0/0/255ta -O -K <<EOF>>$OUTFILE
$NOISE_END 0.5
$NOISE_END -0.5
EOF


		# add misfit measurement onset and endset
		set misfit_beg = $ONSET
		set misfit_end = $ENDSET
		psxy -JX -R -Wthinner/255/153/51ta -O -K <<EOF>>$OUTFILE
$misfit_beg 0.4
$misfit_beg -0.4
EOF
		psxy -JX -R -Wthinner/255/153/51ta -O -K <<EOF>>$OUTFILE
$misfit_end 0.4
$misfit_end -0.4
EOF

		# add horizontal 0 line
		psxy -JX -R -Wfaint,204/169/228,. -O -K <<EOF>>$OUTFILE
$xmin 0
$xmax 0
EOF



# ================== add a sphere global view of EQ--sta geometry =================
set MID = `csh $SHELL_DIR/find_mid_point_on_sphere.sh $SHELL_DIR $EQ_lat $EQ_lon $STA_lat $STA_lon|awk '{print $0}'`
set MID_lat = $MID[1]
set MID_lon = $MID[2]
##set MID_lat = `awk 'NR==1 {print $1}' MID`
##set MID_lon = `awk 'NR==1 {print $2}' MID`
# add the small global map showing the gcp of this cross-section
set MAP = -R0/360/-90/90
set PROJ = -JG${MID_lon}/${MID_lat}/0.5i
set land = "255/225/160"
set sea = 103/204/1
pscoast $MAP $PROJ -Dc -A400000 -B90g45 -W2 -G$land    -O -K -P >>$OUTFILE
psxy  $MAP $PROJ -:   -W0.001/"red" -O -P -K  <<EOF >>$OUTFILE
$EQ_lat $EQ_lon
$STA_lat $STA_lon
EOF
# add EQ and STA location
psxy $MAP $PROJ -: -Sa0.1	-W/"red" -O -P -K << EOF>>$OUTFILE
$EQ_lat $EQ_lon
EOF
psxy $MAP $PROJ -: -Si0.1  -W/"red"	-O -K -P  << EOF>>$OUTFILE
$STA_lat $STA_lon
EOF
#END ================== add a sphere global view of EQ--sta geometry =================

	# ================= add info section ===================
	pstext -JX2i/0.5i -R0/1/0/1 -O -K -N -P -X5.2i <<END>>$OUTFILE
	0 1.0  5 0 0 LB  ----------------------
	0 0.8 5 0 0 LB $STA $NET $DIST $STA_lat $STA_lon misfit: $misfit_pre/$misfit/$misfit_bak weight: $record_weight
	0 0.6  5 0 0 LB CCC3: $CCC3 CCC:$ccc SNR:$SNR dt:$dt_obs_prem gaufactor: $record_gaussian_factor
	0 0.4 5 0 0 LB stretch_ccc: $best_stretch_ccc factor: $best_stretch_coeff tstar: $tstar ccc: $tstar_ccc
	0 0.2  5 0 0 LB AZ:$AZ  polar: $CMT_polar_prediction 1T: $one_period
	0 0.0 5 0 0 LB Misfit2T: $misfit2T_pre/$misfit2T_bak Misfit3T: $misfit3T_pre/$misfit3T_bak SNR3/4: $SNR_peak_trough / $SNR_peak_ratio
END



	# add pdfmarker
if($quality_flag >= 1) then
cat  << EOF >> $OUTFILE
[ 
/T ( ${EQ}_${STA} )
/V /${EQ}_${STA}
/FT /Btn
/Rect [-150 0 -30 100] % — position
/F 4 /H /O
/BS << /W 1 /S /S >>
/MK << /CA (8) /BC [ 0 ] /BG [ 1 ] >>  % Colors
/DA (/ZaDb 0 Tf 1 0 0 rg) % — size and colors
/AP << /N << /${EQ}_${STA} /null >> >> % — checkbox value
/Subtype /Widget
/ANN pdfmark
EOF
else 
cat  << EOF >> $OUTFILE
[ 
/T ( ${EQ}_${STA} )
/FT /Btn
/Rect [-150 0 -30 100] % — position
/F 4 /H /O
/BS << /W 1 /S /S >>
/MK << /CA (8) /BC [ 0 ] /BG [ 1 ] >>  % Colors
/DA (/ZaDb 0 Tf 1 0 0 rg) % — size and colors
/AP << /N << /${EQ}_${STA} /null >> >> % — checkbox value
/Subtype /Widget
/ANN pdfmark
EOF
endif

cat  << EOF >> $OUTFILE
[ 
/Subtype /Widget
/Rect [-1700 0 -1600 100] % — position
/T ( ${EQ}_value_${STA} )
/FT /Tx
/DA (/Helv 10 Tf 0 0 1 rg ) % size and colors
/V (0.0              ${EQ}_value_${STA} )
/AA <<
>>
/ANN pdfmark
EOF


	pstext -JX -R -O -K -N -P -X-5.2i <<END>>$OUTFILE
END


##//echo "current record is $current_record record num is $record_num ploted record is $ploted_record"
##//echo "plot is $ploted_record record is $record_num"

	if( $ploted_record == $record_num || $current_record == $record_num ) then
		psxy  -JX5i/0.5i -R$xmin/$xmax/-1/1 -Ba10f5S -O  -N -P <<EOF>>$OUTFILE
EOF
		cat $OUTFILE >> $tmp_ps
		/bin/rm $OUTFILE 2>&/dev/null
	else if ( $current_record == $PLOT_PER_PAGE ) then
		psxy  -JX5i/0.5i -R$xmin/$xmax/-1/1 -Ba10f5S -O  -N -P <<EOF>>$OUTFILE
EOF
		@ PAGE ++
		@ ploted_record ++
		set current_record = 1
		cat $OUTFILE >> $tmp_ps
		/bin/rm $OUTFILE 2>&/dev/null
	else 
		@ current_record ++
		@ ploted_record ++
	endif
end #sta foreach

ps2pdf $tmp_ps $OUTFILE_pdf
#to_hongyu $PLOTDIR_main
/bin/rm $tmp_ps

