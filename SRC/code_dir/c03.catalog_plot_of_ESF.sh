#!/bin/csh

# ==================================================
#	This Script plot the catalog plot of ESF
# ==================================================


# ====================================================
# 			define input parameter of shell 
# ====================================================
set EQ = $1
set DATADIR = $2
set PLOTDIR_main = $3
set PLOTDIR = $PLOTDIR_main/$EQ
set C_DIR = $4
set SHELL_DIR = $5
set PHASE = $6
set DIR = $7


set INFILE = $DATADIR/$EQ/INFILE_${PHASE}
set COMP = `grep -w COMP $INFILE |awk 'NR==1 {print $2}'`
set filter_flag = `grep -w filter_flag $INFILE |awk 'NR==1 {print $2}'`
set velocity_or_displacement = `grep -w velocity_or_displacement $INFILE |awk 'NR==1 {print $2}'`
set PHASE_BEG = `cat $INFILE |grep -w PHASE_BEG |awk 'NR==1 {print $2}'`
set PHASE_LEN = `cat $INFILE |grep -w PHASE_LEN |awk 'NR==1 {print $2}'`
set ED_CHECK_FILE1 = `cat $DATADIR/INFILE |grep -w ED_CHECK_FILE |awk 'NR==1 {print $2}'`
set PHASE_END = `echo "$PHASE_BEG + $PHASE_LEN"|bc -l `
set work_dir = $DATADIR/$EQ
set input = $work_dir/input
set ESinfo = $work_dir/eventinfo.${EQ}.${PHASE}.${COMP}
set ESinfo_tmp = $work_dir/eventinfo.${EQ}.${PHASE}.${COMP}.tmp


cd $work_dir
# =========================================================== # 
# Implement ML algorithm to make prediction
cp $SHELL_DIR/ML_model/* .
sed "s/^[ \t]*//" -i $ESinfo
python3 make_prediction.py $ESinfo 
# outfile is saved as pred
set out_pred = ./pred_out
cat ./pred |awk 'NR>1 {print $0}' >! $out_pred
paste $ESinfo $out_pred >! $ESinfo_tmp
mv $ESinfo_tmp $ESinfo
cd -
# =========================================================== # 


set BENCHMARK_FLAG = `cat $DATADIR/$EQ/INFILE |grep -w BENCHMARK_FLAG|awk '{print $2}'`
set reprocessing_flag = `cat $DATADIR/$EQ/INFILE |grep -w Reprocessing_Flag|awk '{print $2}'`

set ed_check_file = ./dfsdf
#set ed_check_file = $ED_CHECK_FILE1

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


echo "=============================================="
echo "---> Begin Catalog Plot for $EQ PHASE: $PHASE "

set tmp_ps = $PLOTDIR/tmp.ps
/bin/rm $tmp_ps 2>&/dev/null
set PWD = `pwd`
## ====================================================
# Here we make sure that if STA and STA_PPP are both checked
# we reject STA_PPP
set tmp_event = $work_dir/tmp.m.eventinfo
cat /dev/null >! $tmp_event
foreach sta (`cat $ESinfo |awk '{print $1}' `)
	set is_PPP = `echo $sta|grep PPP`
	if($is_PPP == "" )then
		cat $ESinfo |awk -v sss=$sta '$1==sss {print $0}' >> $tmp_event
		continue
	endif
	## deal with origional sta
	set sta_ppp = `echo $sta |rev |cut -c 5-|rev`
	## find sta quality
	set sta_quality = `cat $ESinfo |awk -v sss=$sta '$1==sss {print $14}'|awk 'NR==1 {print $1}'`
	set sta_ppp_quality = `cat $ESinfo |awk -v sss=$sta_ppp '$1==sss {print $14}'|awk 'NR==1 {print $1}'`
	if($sta_quality >= 1 && $sta_ppp_quality >= 1) then
		## reject STA_PPP
		cat $ESinfo |awk -v sss=$sta '$1==sss {$14=-1;print $0}' >> $tmp_event
		else
		cat $ESinfo |awk -v sss=$sta '$1==sss {print $0}' >> $tmp_event
	endif


end
cp $tmp_event $ESinfo

# ====================================================
# 			set output pdf file and plot parameter
# ====================================================
# sort according to $14 quality flag of recrods
set ESinfo_tmp = $work_dir/eventinfo.${EQ}.${PHASE}.${COMP}.${filter_flag}_tmp
mv $ESinfo $ESinfo_tmp
# =============================================================================== fff NULL
if($fff == "") then

## seperate eventinfo into good and bad
set good_event_sta_list = $work_dir/event.good.list
set bad_event_sta_list = $work_dir/bad.good.list
cat $ESinfo_tmp |awk '$14>=1 {print $1}' |awk -F"_" '{print $1}'|sort|uniq >! $good_event_sta_list
cat $ESinfo_tmp |awk '$14<1 {print $1}' | awk -F"_" '{print $1}' |sort|uniq>! $bad_event_sta_list
cat /dev/null >! $ESinfo
# reconstruct ESinfo
set eventinfo_bad_tmp_list = $work_dir/tmp.eventinfo.bad_list
set eventinfo_good_tmp_list = $work_dir/tmp.eventinfo.good_list
cat /dev/null >! $eventinfo_bad_tmp_list
cat /dev/null >! $eventinfo_good_tmp_list
# pop in good station and their counterpart
foreach sta (`cat $good_event_sta_list |awk '{print $1}'`)
	cat $ESinfo_tmp | awk -v sss=$sta '$1==sss {print $0}'|awk 'NR==1 {print $0}' >> $eventinfo_good_tmp_list
	set sta_PPP = ${sta}_PPP
	set is_PPP_exist_flag = `cat $ESinfo_tmp |grep $sta_PPP |awk 'NR==1 {print $1}'`
	if( $is_PPP_exist_flag != "" ) then
		cat $ESinfo_tmp |awk -v sss=$sta_PPP '$1==sss {print $0}'|awk 'NR==1 {print $0}' >> $eventinfo_good_tmp_list
	endif
end
cat $eventinfo_good_tmp_list |sort -n -k 3 >> $ESinfo
# pop in bad station and their counterpart
foreach sta (`cat $bad_event_sta_list |awk '{print $1}'`)
	set exist_in_good_event = `cat $good_event_sta_list |grep -w $sta|awk 'NR==1 {print $1}'`
	if($exist_in_good_event != "" ) then
		continue
	endif

	cat $ESinfo_tmp | awk -v sss=$sta '$1==sss {print $0}'|awk 'NR==1 {print $0}' >> $eventinfo_bad_tmp_list
	set sta_PPP = ${sta}_PPP
	set is_PPP_exist_flag = `cat $ESinfo_tmp |grep $sta_PPP |awk 'NR==1 {print $1}'`
	if( $is_PPP_exist_flag != "" ) then
		cat $ESinfo_tmp |awk -v sss=$sta_PPP '$1==sss {print $0}'|awk 'NR==1 {print $0}' >> $eventinfo_bad_tmp_list
	endif
end
cat $eventinfo_bad_tmp_list |sort -n -k 3 >> $ESinfo


# =============================================================================== fff
# =============================================================================== fff not NULL
# if current exist in ed checked file, then we put anything that is not checked by ed into 
# the new eventinfo first and then the good pick 
else if($fff != "" ) then
echo "-> This EQ has been checked, we arrange by bad + good"

set eventinfo_step1 = $work_dir/eventinfo.step1
cat /dev/null >! $eventinfo_step1
## if is checked EQ, first step: hardwire the quality_flag in the eventinfo
set checked_sta_list = $work_dir/checked.sta_list
cat $ed_check_file |grep -w $EQ |grep -w $PHASE |awk '{print $1}' >! $checked_sta_list
foreach sta (`cat $ESinfo_tmp |awk '{print $1}'`)
	set is_sta_checked = `cat $checked_sta_list |grep -w $sta |awk 'NR==1 {print $1}'`
	if($is_sta_checked == "" ) then
		cat $ESinfo_tmp |awk -v sss=$sta '$1==sss {print $0}'|awk '{$14=-1;print $0}'>> $eventinfo_step1
	else
		# if there is traffic nearby, we reject it
		set traffic_nearby = `cat $ESinfo_tmp |awk -v sss=$sta 'sss=$1 {print $0}' |awk 'NR==1 {print $45}'`
		if($traffic_nearby == 1) then
			cat $ESinfo_tmp |awk -v sss=$sta '$1==sss {print $0}'|awk '{$14=-1;print $0}'>> $eventinfo_step1
		else 
			cat $ESinfo_tmp |awk -v sss=$sta '$1==sss {print $0}'|awk '{$14=1;print $0}'>> $eventinfo_step1
		endif
	endif
end 
cp $eventinfo_step1 $ESinfo_tmp


# step two is the same as not-checked EQ, but good records is put at later part
## seperate eventinfo into good and bad
set good_event_sta_list = $work_dir/event.good.list
set bad_event_sta_list = $work_dir/bad.good.list
cat $ESinfo_tmp |awk '$14>=1 {print $1}' |awk -F"_" '{print $1}' |sort| uniq>! $good_event_sta_list
cat $ESinfo_tmp |awk '$14<1 {print $1}' | awk -F"_" '{print $1}'|sort| uniq>! $bad_event_sta_list
cat /dev/null >! $ESinfo
# reconstruct ESinfo
# pop in bad station and their counterpart
set eventinfo_bad_tmp_list = $work_dir/tmp.eventinfo.bad_list
set eventinfo_good_tmp_list = $work_dir/tmp.eventinfo.good_list
cat /dev/null >! $eventinfo_bad_tmp_list
cat /dev/null >! $eventinfo_good_tmp_list
foreach sta (`cat $bad_event_sta_list |awk '{print $1}'`)
	set exist_in_good_event = `cat $good_event_sta_list |grep -w $sta|awk 'NR==1 {print $1}'`
	if($exist_in_good_event != "" ) then
		continue
	endif

	cat $ESinfo_tmp | awk -v sss=$sta '$1==sss {print $0}'|awk 'NR==1 {print $0}' >> $eventinfo_bad_tmp_list
	set sta_PPP = ${sta}_PPP
	set is_PPP_exist_flag = `cat $ESinfo_tmp |grep $sta_PPP |awk 'NR==1 {print $1}'`
	if( $is_PPP_exist_flag != "" ) then
		cat $ESinfo_tmp |awk -v sss=$sta_PPP '$1==sss {print $0}'|awk 'NR==1 {print $0}' >> $eventinfo_bad_tmp_list
	endif
end
cat $eventinfo_bad_tmp_list |sort -n -k 3 >> $ESinfo

# pop in good station and their counterpart
foreach sta (`cat $good_event_sta_list |awk '{print $1}'`)
	cat $ESinfo_tmp | awk -v sss=$sta '$1==sss {print $0}'|awk 'NR==1 {print $0}' >> $eventinfo_good_tmp_list
	set sta_PPP = ${sta}_PPP
	set is_PPP_exist_flag = `cat $ESinfo_tmp |grep $sta_PPP |awk 'NR==1 {print $1}'`
	if( $is_PPP_exist_flag != "" ) then
		cat $ESinfo_tmp |awk -v sss=$sta_PPP '$1==sss {print $0}'|awk 'NR==1 {print $0}' >> $eventinfo_good_tmp_list
	endif
end
cat $eventinfo_good_tmp_list |sort -n -k 3 >> $ESinfo
# =============================================================================== fff not NULL
endif


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
set long = $work_dir/${EQ}.${sta}.${PHASE}.${COMP}.long
set emp = $work_dir/${EQ}.${sta}.${PHASE}.${COMP}.emp
set phase = $work_dir/${EQ}.${sta}.${PHASE}.${COMP}.phase
set gau = $work_dir/${EQ}.${sta}.${PHASE}.${COMP}.gau
set orig_emp = $work_dir/${EQ}.${sta}.${PHASE}.${COMP}.orig.emp

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
if($quality_flag == "") then
set quality_flag = -1
endif

set prem = $infoES[15]
set phase_amplitude = $infoES[16]
set ccc = $infoES[17]
set SNR = $infoES[18]
set SNR2 = $infoES[31]
set dt_obs_prem = $infoES[19]
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
set ML_quality = $infoES[55]
set ML_flag = $infoES[56]
#set ML_quality = $quality_flag
#set ML_flag = 1
#set ML_quality = 0
#set ML_flag = 0

set xy_tmp = `minmax -C $long`
set xmin = $xy_tmp[1]
set xmax = $xy_tmp[2]
set ymin = $xy_tmp[3]
set ymax = $xy_tmp[4]
# we compute surface wave time window and ignore everything within the window
## Now lets deal with the traffic phase part
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
		pstext -JX -R0/7/-1/1 -O -K -N <<EOF>>$OUTFILE
1 0.3 6 0 0 CB flipped
EOF

set traffic_color = (green purple orange 104/178/255 blue 255/0/0 255/100/0 255/100/100)
		# add marker information for all phases
set x_loc = 1.0
set x_loc2 = 1.5

set x_loc = 1.5
psxy -JX -R -O -K -N -Y0.2i << EOF>>$OUTFILE
EOF

endif

psxy -JX5i/0.5i -R$xmin/$xmax/-1/1  -O -K -N -Y-0.6i <<EOF>>$OUTFILE
EOF


##=======================================
## Plot window information
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

		#  ===================== add vertical lines  =====================================

		psxy -JX -R -W4/204/71/228 -O -K <<EOF >> $OUTFILE
0 -1
0 1
EOF

		psxy $long -JX -R -W/$long_color  -O -K >>$OUTFILE


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


		set emp_color = red 
		psxy $emp -JX -R -W/$emp_color -O -K>>$OUTFILE
		## add best fit gaussian here
		psxy $gau -JX -R -W/orange -O -K>>$OUTFILE

foreach loc (-15 -10 -5 5 10 15)
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

echo "$STA $ML_quality $ML_flag"

	# add pdfmarker
if($ML_quality >= 1) then
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

# add value 
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

if( $ML_flag == 0) then
# Here we add another tiny box to indicate if ML model disagree with original pick
cat  << EOF >> $OUTFILE
[ 
/T ( ${EQ}_ML_${STA}_ML_disagree )
/V /${EQ}_ML_${STA}_ML_disagree
/FT /Btn
/Rect [-200 50 -160 100] % — position
/F 4 /H /O
/BS << /W 1 /S /S >>
/MK << /CA (8) /BC [ 0 ] /BG [ 1 ] >>  % Colors
/DA (/ZaDb 0 Tf 1 0 0 rg) % — size and colors
/AP << /N << /${EQ}_ML_${STA}_ML_disagree /null >> >> % — checkbox value
/Subtype /Widget
/ANN pdfmark
EOF
else 
cat  << EOF >> $OUTFILE
[ 
/T ( ${EQ}_ML_${STA}_ML_disagree )
/FT /Btn
/Rect [-200 50 -160 100] % — position
/F 4 /H /O
/BS << /W 1 /S /S >>
/MK << /CA (8) /BC [ 0 ] /BG [ 1 ] >>  % Colors
/DA (/ZaDb 0 Tf 1 0 0 rg) % — size and colors
/AP << /N << /${EQ}_ML_${STA}_ML_disagree /null >> >> % — checkbox value
/Subtype /Widget
/ANN pdfmark
EOF

endif



	pstext -JX -R -O -K -N -P -X-5.2i <<END>>$OUTFILE
END


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
/bin/rm $tmp_ps

