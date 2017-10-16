#!/bin/csh

# ==================================================
#	This Script plot the catalog plot of ESF
#	
#	Hongyu DATE: 
#	Key words: 
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


set INFILE = $DATADIR/$EQ/INFILE_${PHASE}
set COMP = `grep -w COMP $INFILE |awk 'NR==1 {print $2}'`
set filter_flag = `grep -w filter_flag $INFILE |awk 'NR==1 {print $2}'`
set velocity_or_displacement = `grep -w velocity_or_displacement $INFILE |awk 'NR==1 {print $2}'`
set work_dir = $DATADIR/$EQ
set input = $work_dir/input
##set check_info = $DIR/storage_dir/${EQ}_${PHASE}_STA_LIST
set ESinfo = $work_dir/eventinfo.${EQ}.${PHASE}.${COMP}
set ESinfo_tmp = $work_dir/eventinfo.${EQ}.${PHASE}.${COMP}.tmp

echo "=============================================="
echo "---> Begin Catalog Plot for $EQ PHASE: $PHASE "


## put the sta list in order
##	1. alorithm thinks good, i picked
##	2. algorithm thinks good, i didnot pick
##	3. bad
##if(-e $check_info ) then
##cat /dev/null >  $ESinfo_tmp
##foreach STA (`cat $ESinfo |awk '{print $1}'`)
##set line = `grep -w $STA $ESinfo | awk 'NR==1 {print $0}'`
##set quality = $line[14]
##set pick_flag = `grep -w $STA $check_info |awk 'NR==1 {print $1}'`
##if( $quality == 1 && $pick_flag == $STA ) then
##echo $line >> $ESinfo_tmp
##endif
##end
##foreach STA (`cat $ESinfo |awk '{print $1}'`)
##set line = `grep -w $STA $ESinfo | awk 'NR==1 {print $0}'`
##set quality = $line[14]
##set pick_flag = `grep -w $STA $check_info |awk 'NR==1 {print $1}'`
##if( $quality == 1 && $pick_flag != $STA ) then
##echo $line >> $ESinfo_tmp
##endif
##end
##foreach STA (`cat $ESinfo |awk '{print $1}'`)
##set line = `grep -w $STA $ESinfo | awk 'NR==1 {print $0}'`
##set quality = $line[14]
##set pick_flag = `grep -w $STA $check_info |awk 'NR==1 {print $1}'`
##if( $quality != 1 && $pick_flag != $STA ) then
##echo $line >> $ESinfo_tmp
##endif
##end

##cp $ESinfo_tmp $ESinfo
##endif # checkinfo



set tmp_ps = $PLOTDIR/tmp.ps
rm $tmp_ps 2>&/dev/null
set PWD = `pwd`
# ====================================================
# 			set output pdf file and plot parameter
# ====================================================
set OUTFILE_pdf = $PLOTDIR/${EQ}_${PHASE}_${COMP}_${filter_flag}_catalog_all_record.pdf


# sort according to $14 quality flag of recrods
set ESinfo_tmp = $work_dir/eventinfo.${EQ}.${PHASE}.${COMP}.${filter_flag}_tmp
mv $ESinfo $ESinfo_tmp
awk '$14==1 {print $0}' $ESinfo_tmp > $ESinfo
awk '$14!=1 {print $0}' $ESinfo_tmp >> $ESinfo
/bin/rm $ESinfo_tmp > & /dev/null

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
# =============================

# establish a file containing the records that should be thrown away because of being too close to other phases only for good records


# ====================================================
# 	plot record one by one
# ====================================================
set record_num = `cat $ESinfo |wc -l`
foreach sta (`cat $ESinfo |awk '{print $1}'`)
##echo "---> Working on plotting $sta PHASE is $PHASE"
set long = $work_dir/${EQ}.${sta}.${PHASE}.${COMP}.long
set emp = $work_dir/${EQ}.${sta}.${PHASE}.${COMP}.emp
set gau = $work_dir/${EQ}.${sta}.${PHASE}.${COMP}.gau
set orig_emp = $work_dir/${EQ}.${sta}.${PHASE}.${COMP}.orig.emp
set CMT_polar_prediction_file = $work_dir/eventinfo.polarity.${PHASE}.${COMP}
set CMT_polar_tmp  = `grep -w $sta $CMT_polar_prediction_file |awk 'NR==1 {print $2}'` 
set CMT_polar_prediction = `printf "%.2f" $CMT_polar_tmp`
##echo "---> sta $sta cmt $CMT_polar_prediction"

if( ! -e $long ) then
echo "$long does not exist! ========"
continue
endif
if(! -e $emp ) then
echo "$emp does not exist! ========="
##continue
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
##set tstar = $best_stretch_coeff
set misfit = $infoES[23]
set misfit2 = $infoES[32]

set NOISE_BEG = $infoES[27]
set NOISE_LEN = $infoES[28]
set NOISE_END = `echo "$NOISE_BEG + $NOISE_LEN"|bc -l`

set record_weight = $infoES[30]

set ONSET = $infoES[33]
set ENDSET = $infoES[34]


##for ScS cut from -80 to 80 sec
if($PHASE == "ScS") then
awk '{if($1>-70 && $1 < 70) print $0}' $long > $work_dir/long_tmp
mv $work_dir/long_tmp $long
endif

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
		0 0.5 11 0 0 CB Catalog Plot for $EQ PHASE: $PHASE COMP:$COMP PAGE:$PAGE in $velocity_or_displacement filter: $filter_flag
		0 0.1 10 0 0 CB EQ_lat: $EQ_lat EQ_lon: $EQ_lon EQ_depth: $EQ_dep Mw: $EQ_mag 
END
		pstext -JX -R -O -K -N -P -X-0.3i <<END>>$OUTFILE
END
		# add flipping flag instruction
		psxy -JX -R0/7/-1/1 -Sc0.1 -G0/0/0 -Y-0.3i -O -N -K <<EOF>>$OUTFILE
0.5 0.3
EOF
		psxy -JX -R0/7/-1/1 -Sc0.1 -G255/0/0  -O -K -N <<EOF>>$OUTFILE
0.5 -0.3
EOF
		pstext -JX -R0/7/-1/1 -O -K -N <<EOF>>$OUTFILE
1 0.3  6 0 0 CB not flipped
1 -0.3 6 0 0 CB flipped
EOF

set traffic_color = (green purple orange 104/178/255 blue )
		# add marker information for all phases
set x_loc = 1.0
set x_loc2 = 1.5
	foreach color (`echo $traffic_color`)

		psxy -JX -R0/7/-1/1 -W/${color}  -O -K <<EOF>>$OUTFILE
$x_loc 0
$x_loc2 0
EOF
set x_loc = `echo "$x_loc + 1"|bc -l`
set x_loc2 = `echo "$x_loc2 + 1"|bc -l`
end #color
		# ONSET symble
		psxy -JX -R $arrow_parameter -G255/0/0  -O -K <<EOF>>$OUTFILE
5.4 0 90 0.5
EOF
		# misfit window symble
		psxy -JX -R -Wfaint/255/153/51ta -O -K <<EOF>>$OUTFILE
6.4 0.5
6.4 -0.5
EOF

set x_loc = 1.5
set traffic_file = $work_dir/traffic_file.${EQ}.${STA}.${PHASE}
	foreach phase_name (`cat $traffic_file |awk '{print $1}'`)
		pstext -JX -R -O -K <<EOF>>$OUTFILE
$x_loc 0 7	0 0 LB	$phase_name
EOF
set x_loc = `echo "$x_loc + 1"|bc -l`
	end

		pstext -JX -R -O -K <<EOF>>$OUTFILE
5.5 0 7 0 0 LB  ONSET
EOF
		pstext -JX -R -O -K <<EOF>>$OUTFILE
6.5 0 7 0 0 LB  misfit_win
EOF

endif

	# ================== plot the trace =====================
		set long_color = 0/0/0
		set long_color_flag = `echo ddd |awk '{if('$CMT_polar_prediction' > -0.15 && '$CMT_polar_prediction' < 0.15 ) print "green"; else print "hahaha"}'`
##echo "sta $sta long color is $long_color_flag"
		if($long_color_flag == "green" ) then
		set long_color = 178/102/255
		endif
		psxy $long -JX5i/0.5i -R$xmin/$xmax/-1/1 -W/$long_color  -O -K -Y-0.6i>>$OUTFILE

		# add a dot to indicate whether it is fliped or not
		if( $polar_flag == -1 ) then  # dark for not flip red for flipped recrod
			set flip_color = red
		else if ($polar_flag == 1) then
			set flip_color = 0/0/0
		else 
			set flip_color = yellow
		endif
		set x_loc = `echo "$xmin + 40"|bc`
		psxy -JX -R -Sc0.1 -G$flip_color  -O -K -N <<EOF >> $OUTFILE
$x_loc 1
EOF


		set emp_color = red 
		psxy $emp -JX -R -W/$emp_color -O -K>>$OUTFILE


		## add best fit gaussian here
		psxy $gau -JX -R -W/orange -O -K>>$OUTFILE


		# this is the not stretched empirical waveform
##psxy $orig_emp -JX -R -W/green -O -K>>$OUTFILE

		#  ===================== add vertical lines  =====================================
		psxy -JX -R -W2/204/169/228 -O -K <<EOF>>$OUTFILE
0 -1
0 1
EOF
foreach loc (-15 -10 -5 5 10 15)
		psxy -JX -R -Wfaint,204/169/228,.- -O -K <<EOF>>$OUTFILE
$loc -0.3
$loc  0.3
EOF
end # foreach loc
		# add  little marker
		set imarker = -14
		set imarker_max = 14
		while ( $imarker <= $imarker_max)
		psxy -JX -R -Wfaint/204/169/228ta -O -K <<EOF>>$OUTFILE
$imarker 0.2
$imarker -0.2
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

	#	add traffic phase system symbal
##set traffic_file = traffic_file.${EQ}.${STA}.${PHASE}
if(! -e $traffic_file) then
set kkk = 1
else 
set traffic_num = `cat $traffic_file |wc -l`
set NUM = 1
while ($NUM <= $traffic_num)
	set TMP = `cat $traffic_file |awk 'NR=='$NUM' {print $0 }' `
	set tPHASE = $TMP[1]
	set rel_time = $TMP[3]
	set flag = $TMP[4]
	set color = ${traffic_color[${NUM}]}
		psxy -JX -R -Wfaint/${color}ta -O -K <<EOF>>$OUTFILE
$rel_time -1
$rel_time 1
EOF
@ NUM ++
	end #while
endif


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
pscoast $MAP $PROJ -Dc -A4000000 -B90g45 -W2 -G$land    -O -K -P >>$OUTFILE
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
	0 1.0  7 0 0 LB  ----------------------
	0 0.75 7 0 0 LB $STA $NET $DIST $STA_lat $STA_lon 
	0 0.5  7 0 0 LB ccc:$ccc SNR:$SNR SNR2: $SNR2  dt:$dt_obs_prem 
	0 0.25 7 0 0 LB stretch_ccc: $best_stretch_ccc factor: $best_stretch_coeff tstar: $tstar ccc: $tstar_ccc
	0 0.0  7 0 0 LB AZ:$AZ misfit:$misfit $misfit2 weight: $record_weight polar: $CMT_polar_prediction
END

	# add pdfmarker
if($quality_flag == 1) then
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
/T ( ${EQ}_problem_${STA} )
/FT /Btn
/Rect [-1700 0 -1600 100] % — position
/F 4 /H /O
/BS << /W 1 /S /S >>
/MK << /CA (8) /BC [ 0 ] /BG [ 1 ] >>  % Colors
/DA (/ZaDb 0 Tf 1 0 0 rg) % — size and colors
/AP << /N << /${EQ}_problem_${STA} /null >> >> % — checkbox value
/Subtype /Widget
/ANN pdfmark
EOF


	pstext -JX -R -O -K -N -P -X-5.2i <<END>>$OUTFILE
END


##echo "current record is $current_record record num is $record_num ploted record is $ploted_record"

	if( $ploted_record == $record_num ) then
		psxy  -JX5i/0.5i -R$xmin/$xmax/-1/1 -Ba10f5S -O  -N -P <<EOF>>$OUTFILE
EOF
		cat $OUTFILE >> $tmp_ps
		rm $OUTFILE 2>&/dev/null
	else if ( $current_record == $PLOT_PER_PAGE ) then
		psxy  -JX5i/0.5i -R$xmin/$xmax/-1/1 -Ba10f5S -O  -N -P <<EOF>>$OUTFILE
EOF
		@ PAGE ++
		@ ploted_record ++
		set current_record = 1
		cat $OUTFILE >> $tmp_ps
		rm $OUTFILE 2>&/dev/null
	else 
		@ current_record ++
		@ ploted_record ++
	endif
end #sta foreach

ps2pdf $tmp_ps $OUTFILE_pdf
/bin/rm $tmp_ps

