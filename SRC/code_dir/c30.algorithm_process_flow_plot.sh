#!/bin/tcsh
# ====================================================================
# This is a csh script to plot record section for one EQ for one PHASE
# 
# Input:
#
#
#
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

# set alignment window
set MIN = -25
set MAX = 40
##set AMP = 0.1 		# the amplitude of each record in the plots

cd $DATADIR/$EQ
gmtset ANNOT_FONT_SIZE_PRIMARY = 5p
gmtset COLOR_BACKGROUND = white
gmtset COLOR_FOREGROUND = white
gmtset COLOR_NAN = white
gmtset LABEL_FONT_SIZE = 5p


set TAG1 = "Define phase window and stack 1st E.W."
set TAG2 = "Shift phase window"
set TAG3 = "Aligh phase window and restack"
set TAG4 = "Stretch records to match E.W. and restack"
set TAG5 = "t* operate E.W.S to match record"
set TAG6 = "Find best fitting Gaussian to t*(E.W.S)"
set TAG7 = "Define ONSET on Gaussian"
set TAG8 = "ONSET time is hardwired to records"


##=======================================
## Plot window information attribute
## 1 
set FLAG_COLOR = 255/204/153
set FLAG_INC = 1
## =============

set OUT = $PLOTDIR/$EQ/algorithm_process_flow.ps
set OUT_pdf = $PLOTDIR/$EQ/Algorithm_process_flow.pdf
set OUTFILE = $OUT
cat /dev/null > $OUT
set work_dir = $DATADIR/$EQ
set PHASE_LIST = `cat $work_dir/INFILE|grep -w PHASE_LIST | awk 'NR==1 {print $0}'|cut -d ' ' -f2-`
## foreach begin
foreach PHASE (`echo $PHASE_LIST`)
	echo "---> working on Record Profile for $EQ $PHASE "
set INFILE_PHASE = $work_dir/INFILE_${PHASE}
set PHASE = `grep -w "<PHASE>" $INFILE_PHASE |awk 'NR==1 {print $2}'`
set COMP  = `grep -w "<COMP>" $INFILE_PHASE |awk 'NR==1 {print $2}'`
set FLAG  = `grep -w "<filter_flag>" $INFILE_PHASE |awk 'NR==1 {print $2}'`
set DISTMIN = `grep -w "<DIST_MIN_MAX>" $INFILE_PHASE |awk 'NR==1 {print $2}'`
set DISTMAX = `grep -w "<DIST_MIN_MAX>" $INFILE_PHASE |awk 'NR==1 {print $3}'`
if($DISTMIN == "non" ) then
	set DISTMIN = 100
endif
if($DISTMAX == "non" ) then
	set DISTMAX = 100
endif

set DISTMID = `echo "( $DISTMIN + $DISTMAX ) /2"|bc -l`
set event_info = $work_dir/eventinfo.${EQ}.${PHASE}.${COMP}
set EQ_DEP = `cat $event_info |awk 'NR==1 {print $10}'`
set num_good_record = `cat $event_info | awk '$14==1 {print $0}'| wc -l`

set DIST_DELTA = `echo "$DISTMAX - $DISTMIN"|bc -l`
set AMP = `echo "$DIST_DELTA /6 *0.2"|bc -l`
set AMP = `printf "%.3f" $AMP`
echo "AMP is $AMP"


pstext -JX6i/1i -R0/10/0/10 -Y7.5i -K -N << EOF >> $OUT
0 6 6 0 0 LB Record Section (only good records: $num_good_record)
0 5 6 0 0 LB EQ: 	 $EQ
0 4 6 0 0 LB PHASE: $PHASE
EOF

set RANGE = $MIN/$MAX/$DISTMIN/$DISTMAX
set SCALE = X1.2i/6i 
set Y_INC = 1
set Y_FLAG = 1
set DELTA_DIST = `echo "$DISTMAX - $DISTMIN"|bc`
if($DELTA_DIST > 50 ) then
set Y_INC = 5
set Y_FLAG = 5
endif 

###############################################################
#	
###############################################################
#	Plottint record section for origional records PLOT1
#	
###############################################################
###############################################################
##echo "___> working on $EQ $PHASE $FLAG "
psxy -R$RANGE -J$SCALE -Ba10f5:"Seconds (Relative to PREM prediction)":/a${Y_INC}f${Y_FLAG}WSne:"Distance (Degree)": -Y-6i -O -K -N  <<EOF>> $OUT
EOF


set TAG_Y = `echo "$DISTMAX + 1.5*$AMP"|bc -l`
set TAG_Y1 = `echo "$DISTMAX + 1*$AMP"|bc -l`
set X_P = -25
set FONT_SIZE = 5
pstext -R$RANGE -J$SCALE -O -K -N<< EOF >> $OUT
$X_P $TAG_Y $FONT_SIZE 0 0 LB  Step #1
$X_P $TAG_Y1 $FONT_SIZE 0 0 LB $TAG1
EOF



foreach STA (`cat $event_info |awk '{print $1}'`)
	set TMP = `grep -w $STA $event_info`
	set quality_flag = $TMP[14]
	##set dt = $TMP[19]
	set dt = 0
	set DIST = $TMP[3]
	set weight = $TMP[30]
	set stretch_factor = $TMP[22]
set INFILE = $DATADIR/$EQ/INFILE_${PHASE}
set PHASE_BEG = `cat $INFILE |grep -w PHASE_BEG |awk '{print $2}'`
set PHASE_LEN = `cat $INFILE |grep -w PHASE_LEN |awk '{print $2}'`
set PHASE_END = `echo "$PHASE_BEG + $PHASE_LEN"|bc -l `
##=======================================
## Plot window information
## 1 
set XMIN_K = `echo "$FLAG_INC*$AMP + $DIST"|bc -l`
set XMAX_K = `echo "-1*$FLAG_INC*$AMP + $DIST"|bc -l`
##echo "YMIN K Is $XMIN_K $XMAX_K"
psxy -J$SCALE -R$RANGE -G$FLAG_COLOR -L -O -K -N <<EOF >>$OUT
$PHASE_BEG $XMIN_K
$PHASE_END $XMIN_K
$PHASE_END $XMAX_K
$PHASE_BEG $XMAX_K
$PHASE_BEG $XMIN_K
EOF
## =======================================
end # STA
# add vertical line for checking
psxy -J$SCALE -R$RANGE -W1,purple,- -O -K -N  << EOF >>$OUT
0 $DISTMIN
0 $DISTMAX
EOF

foreach STA (`cat $event_info |awk '{print $1}'`)
	set TMP = `grep -w $STA $event_info`
	set quality_flag = $TMP[14]
	##set dt = $TMP[19]
	set dt = 0
	set DIST = $TMP[3]
	set weight = $TMP[30]
	set stretch_factor = $TMP[22]
	set long = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.long
	set phase = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.phase
	set emp = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.emp

	##echo "---> Working on $STA $PHASE"
	# cut long to alignment window
	set dt_min = `echo "$dt+ $MIN"|bc -l`
	set dt_max = `echo "$dt+ $MAX"|bc -l`
	set record = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.data
	set phase_win = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.phase_win
	set emp_win = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.emp_win


	##awk -v min=$dt_min '{if($1>min) print $0}' $long | awk -v max=$dt_max '{if($1<max) print $0}' > $record
	##awk '{if($1 > '$dt_min' && $1 < '$dt_max') printf "%15.3f %15.3f\n",$1-'$dt', $2*'$AMP'+'$DIST'}' $long   > $record
	set record_min = `cat $long |minmax -C |awk '{print $3}'`
	set record_max = `cat $long |minmax -C |awk '{print $4}'`
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 - '$dt', $2/'$record_max'*'$AMP'+'$DIST'}' $long   > $record
	##awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 - '$dt', $2*'$AMP'+'$DIST'}' $long   > $record
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 - '$dt', $2*'$AMP'+'$DIST'}' $phase   > $phase_win
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 - '$dt', $2*'$AMP'+'$DIST'}' $emp   > $emp_win





	psxy $record -J$SCALE -R$RANGE -O -K -N  <<EOF >> $OUT
EOF
##psxy $emp_win -W/red  -J$SCALE -R$RANGE -O -K -N  <<EOF >> $OUT
##EOF
##psxy $phase_win -W/green -J$SCALE -R$RANGE -O -K -N  <<EOF >> $OUT
##EOF
##pstext -JX -R -O -K -N<< EOF >> $OUT
##$MAX $DIST 7 0 0 LB $stretch_factor
##EOF
##pstext -J$SCALE -R$RANGE -O -K -N  << EOF >> $OUT
##$MAX $DIST 10 0 0 LB weight: $weight $STA
##EOF
end #foreach STA
## add first iteration EW plot in red at the bottom of PLOT1
set first_EW = $work_dir/${PHASE}_ES.first_iteration.out
set first_EW_STD = $work_dir/${PHASE}_STD.first_iteration.out
set first_EW_xmin = `cat $first_EW|minmax -C |awk '{print $1}'`
set first_EW_xmax = `cat $first_EW|minmax -C |awk '{print $2}'`
psxy $first_EW_STD -G192/192/192 -L -JX1.0i/0.6i -R$first_EW_xmin/$first_EW_xmax/-1.1/1.1 -Y-1.2i -O -K -N  << EOF >> $OUT
EOF
psxy $first_EW -W/red -JX -R$first_EW_xmin/$first_EW_xmax/-1.1/1.1  -O -K -N  << EOF >> $OUT
EOF
pstext -J -R -N -O -K << EOF >> $OUT
$first_EW_xmin -1.2 7 0 0 LB First Iteration stacked E.W. with STD
$first_EW_xmin -1.6 7 0 0 LB all records
EOF


psxy -JX -R -O -K -Y1.2i << EOF >> $OUT
EOF

###############################################################
#	
###############################################################
#	Plottint record section for origional records (aligned) PLOT2
#	
###############################################################
###############################################################
##echo "___> working on $EQ $PHASE $FLAG "
psxy -R$RANGE -J$SCALE -Ba10f5/a${Y_INC}f${Y_FLAG}wSne -X1.3i -O -K -N  <<EOF>> $OUT
EOF

pstext -R$RANGE -J$SCALE -O -K -N<< EOF >> $OUT
$X_P $TAG_Y $FONT_SIZE 0 0 LB  Step #2
$X_P $TAG_Y1 $FONT_SIZE 0 0 LB $TAG2
EOF

foreach STA (`cat $event_info |awk '{print $1}'`)
	set TMP = `grep -w $STA $event_info`
	set quality_flag = $TMP[14]
	if( $quality_flag == -1) then
		continue
		endif
	##set dt = $TMP[19]
	set dt = $TMP[19]
	set DIST = $TMP[3]
	set weight = $TMP[30]
	set PHASE_WINDOW_BEG = $TMP[29]
	set INFILE = $DATADIR/$EQ/INFILE_${PHASE}
	set PHASE_LEN = `cat $INFILE |grep -w PHASE_LEN |awk '{print $2}'`
	set PHASE_WINDOW_END = `echo "$PHASE_WINDOW_BEG + $PHASE_LEN"|bc -l`
	if($weight == 0 ) then
	continue
	endif
##=======================================
## Plot window information
## 2
set XMIN_K = `echo "$FLAG_INC*$AMP + $DIST"|bc -l`
set XMAX_K = `echo "-1*$FLAG_INC*$AMP + $DIST"|bc -l`
##echo "YMIN K Is $XMIN_K $XMAX_K"
psxy -J$SCALE -R$RANGE -G$FLAG_COLOR -L -O -K -N <<EOF >>$OUT
$PHASE_WINDOW_BEG $XMIN_K
$PHASE_WINDOW_END $XMIN_K
$PHASE_WINDOW_END $XMAX_K
$PHASE_WINDOW_BEG $XMAX_K
$PHASE_WINDOW_BEG $XMIN_K
EOF
## =======================================
end # STA
# add vertical line for checking
psxy -J$SCALE -R$RANGE -W1,purple,- -O -K -N  << EOF >>$OUT
0 $DISTMIN
0 $DISTMAX
EOF

foreach STA (`cat $event_info |awk '{print $1}'`)
	set TMP = `grep -w $STA $event_info`
	set quality_flag = $TMP[14]
	if( $quality_flag == -1) then
		continue
		endif
	##set dt = $TMP[19]
	set dt = $TMP[19]
	set DIST = $TMP[3]
	set weight = $TMP[30]
	set PHASE_WINDOW_BEG = $TMP[29]
	set INFILE = $DATADIR/$EQ/INFILE_${PHASE}
	set PHASE_LEN = `cat $INFILE |grep -w PHASE_LEN |awk '{print $2}'`
	set PHASE_WINDOW_END = `echo "$PHASE_WINDOW_BEG + $PHASE_LEN"|bc -l`
	if($weight == 0 ) then
	continue
	endif
	set stretch_factor = $TMP[22]
	set long = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.long
	set phase = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.phase
	set emp = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.emp

	##echo "---> Working on $STA $PHASE"
	# cut long to alignment window
	set dt_min = `echo "$dt+ $MIN"|bc -l`
	set dt_max = `echo "$dt+ $MAX"|bc -l`
	set record = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.data
	set phase_win = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.phase_win
	set emp_win = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.emp_win
	##awk -v min=$dt_min '{if($1>min) print $0}' $long | awk -v max=$dt_max '{if($1<max) print $0}' > $record
	##awk '{if($1 > '$dt_min' && $1 < '$dt_max') printf "%15.3f %15.3f\n",$1-'$dt', $2*'$AMP'+'$DIST'}' $long   > $record
	set record_min = `cat $long |minmax -C |awk '{print $3}'`
	set record_max = `cat $long |minmax -C |awk '{print $4}'`

	## make sure we dont make the amplitude too big
	##set record_max = `echo $record_max |awk '{if($1 < 1 ) print 1; else print $1}' `

	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 , $2/'$record_max'*'$AMP'+'$DIST'}' $long   > $record
	##awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 - '$dt', $2*'$AMP'+'$DIST'}' $long   > $record
	##awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 - '$dt', $2*'$AMP'+'$DIST'}' $long   > $record
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 - '$dt', $2*'$AMP'+'$DIST'}' $phase   > $phase_win
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 - '$dt', $2*'$AMP'+'$DIST'}' $emp   > $emp_win





	psxy $record -J$SCALE -R$RANGE -O -K -N  <<EOF >> $OUT
EOF

##psxy $emp_win -W/red  -J$SCALE -R$RANGE -O -K -N  <<EOF >> $OUT
##EOF
##psxy $phase_win -W/green -J$SCALE -R$RANGE -O -K -N  <<EOF >> $OUT
##EOF
##pstext -JX -R -O -K -N<< EOF >> $OUT
##$MAX $DIST 7 0 0 LB $stretch_factor
##EOF


##pstext -J$SCALE -R$RANGE -O -K -N  << EOF >> $OUT
##$MAX $DIST 10 0 0 LB weight: $weight $STA
##EOF

end #foreach STA





###############################################################
#	
###############################################################
#	Plottint record section for stretched records (aligned) PLOT3
#	
###############################################################
###############################################################
##echo "___> working on $EQ $PHASE $FLAG "
psxy -R$RANGE -J$SCALE -Ba10f5/a${Y_INC}f${Y_FLAG}wsne -X1.3i -O -K -N  <<EOF>> $OUT
EOF

pstext -R$RANGE -J$SCALE -O -K -N<< EOF >> $OUT
$X_P $TAG_Y $FONT_SIZE 0 0 LB  Step #3
$X_P $TAG_Y1 $FONT_SIZE 0 0 LB $TAG3
EOF

foreach STA (`cat $event_info |awk '{print $1}'`)
	set TMP = `grep -w $STA $event_info`
	set quality_flag = $TMP[14]
	if( $quality_flag == -1) then
		continue
		endif
	##set dt = $TMP[19]
	set dt = $TMP[19]
	set DIST = $TMP[3]
	set weight = $TMP[30]
	if($weight == 0 ) then
	continue
	endif
set INFILE = $DATADIR/$EQ/INFILE_${PHASE}
set PHASE_BEG = `cat $INFILE |grep -w PHASE_BEG |awk '{print $2}'`
set PHASE_LEN = `cat $INFILE |grep -w PHASE_LEN |awk '{print $2}'`
set PHASE_END = `echo "$PHASE_BEG + $PHASE_LEN"|bc -l `

##=======================================
## Plot window information
## 1 
set XMIN_K = `echo "$FLAG_INC*$AMP + $DIST"|bc -l`
set XMAX_K = `echo "-1*$FLAG_INC*$AMP + $DIST"|bc -l`
##echo "YMIN K Is $XMIN_K $XMAX_K"
psxy -J$SCALE -R$RANGE -G$FLAG_COLOR -L -O -K -N <<EOF >>$OUT
$PHASE_BEG $XMIN_K
$PHASE_END $XMIN_K
$PHASE_END $XMAX_K
$PHASE_BEG $XMAX_K
$PHASE_BEG $XMIN_K
EOF
## =======================================
end #STA 
# add vertical line for checking
psxy -J$SCALE -R$RANGE -W1,purple,- -O -K -N  << EOF >>$OUT
0 $DISTMIN
0 $DISTMAX
EOF

foreach STA (`cat $event_info |awk '{print $1}'`)
	set TMP = `grep -w $STA $event_info`
	set quality_flag = $TMP[14]
	if( $quality_flag == -1) then
		continue
		endif
	##set dt = $TMP[19]
	set dt = $TMP[19]
	set DIST = $TMP[3]
	set weight = $TMP[30]
	if($weight == 0 ) then
	continue
	endif
	set stretch_factor = $TMP[22]
	set long = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.long
	set phase = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.phase
	set phase_stretch = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.phase_stretch
	set emp = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.emp

	##echo "---> Working on $STA $PHASE"
	# cut long to alignment window
	set dt_min = `echo "$dt+ $MIN"|bc -l`
	set dt_max = `echo "$dt+ $MAX"|bc -l`
	set record = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.data
	set phase_win = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.phase_win
	set phase_stretch_win = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.phase_stretc_win
	set emp_win = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.emp_win
	##awk -v min=$dt_min '{if($1>min) print $0}' $long | awk -v max=$dt_max '{if($1<max) print $0}' > $record
	##awk '{if($1 > '$dt_min' && $1 < '$dt_max') printf "%15.3f %15.3f\n",$1-'$dt', $2*'$AMP'+'$DIST'}' $long   > $record
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 - '$dt', $2*'$AMP'+'$DIST'}' $long   > $record
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 - '$dt', $2*'$AMP'+'$DIST'}' $phase   > $phase_win
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 - '$dt', $2*'$AMP'+'$DIST'}' $phase_stretch   > $phase_stretch_win
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 - '$dt', $2*'$AMP'+'$DIST'}' $emp   > $emp_win





	##psxy $record -J$SCALE -R$RANGE -O -K -N  <<EOF >> $OUT
	##EOF
##psxy $emp_win -W/red  -J$SCALE -R$RANGE -O -K -N  <<EOF >> $OUT
##//EOF
psxy $record -W -J$SCALE -R$RANGE -O -K -N  <<EOF >> $OUT
EOF
##pstext -JX -R -O -K -N<< EOF >> $OUT
##$MAX $DIST 7 0 0 LB $stretch_factor
##EOF


##pstext -J$SCALE -R$RANGE -O -K -N  << EOF >> $OUT
##$MAX $DIST 10 0 0 LB weight: $weight $STA
##EOF

end #foreach STA

## add first EW plot in red at the bottom of PLOT3
set first_EW = $work_dir/${PHASE}_ES.first.out
set first_EW_STD = $work_dir/${PHASE}_STD.first.out
set first_EW_xmin = `cat $first_EW|minmax -C |awk '{print $1}'`
set first_EW_xmax = `cat $first_EW|minmax -C |awk '{print $2}'`
psxy $first_EW_STD -G192/192/192 -L -JX1i/0.6i -R$first_EW_xmin/$first_EW_xmax/-1.1/1.1 -Y-1.2i -O -K -N  << EOF >> $OUT
EOF
psxy $first_EW -W/red -JX -R$first_EW_xmin/$first_EW_xmax/-1.1/1.1  -O -K -N  << EOF >> $OUT
EOF
pstext -J -R -N -O -K << EOF >> $OUT
$first_EW_xmin -1.2 7 0 0 LB Iteratively stacked E.W. with STD
$first_EW_xmin -1.6 7 0 0 LB weight(SNR, ccc) > 0 
EOF
psxy -JX -R -O -K -Y1.2i << EOF >> $OUT
EOF


###############################################################
#	
###############################################################
#	Plottint record section for origional records (not aligned ) PLOT4
#	but with t* E.W. and gaussian fit
#	
###############################################################
###############################################################
##echo "___> working on $EQ $PHASE $FLAG "
psxy -R$RANGE -J$SCALE -Ba10f5/a${Y_INC}f${Y_FLAG}wsne -X1.3i -O -K -N  <<EOF>> $OUT
EOF
# add vertical line for checking
psxy -J$SCALE -R$RANGE -W1,purple,- -O -K -N  << EOF >>$OUT
0 $DISTMIN
0 $DISTMAX
EOF

pstext -R$RANGE -J$SCALE -O -K -N<< EOF >> $OUT
$X_P $TAG_Y $FONT_SIZE 0 0 LB  Step #4
$X_P $TAG_Y1 $FONT_SIZE 0 0 LB $TAG4
EOF

foreach STA (`cat $event_info |awk '{print $1}'`)
	set TMP = `grep -w $STA $event_info`
	set quality_flag = $TMP[14]
	if( $quality_flag != 1) then
	continue
	endif
	##set dt = $TMP[19]
	set dt = 0
	set onset_time = $TMP[19]
	set DIST = $TMP[3]
	set weight = $TMP[30]
	set stretch_factor = $TMP[22]
	set long = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.long
	set phase = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.phase
	set phase_stretch = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.phase_stretch
	set emp = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.emp

	##echo "---> Working on $STA $PHASE"
	# cut long to alignment window
	set dt_min = `echo "$dt+ $MIN"|bc -l`
	set dt_max = `echo "$dt+ $MAX"|bc -l`
	set record = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.data
	set phase_win = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.phase_win
	set phase_stretch_win = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.phase_stretc_win
	set emp_win = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.emp_win
	##awk -v min=$dt_min '{if($1>min) print $0}' $long | awk -v max=$dt_max '{if($1<max) print $0}' > $record
	##awk '{if($1 > '$dt_min' && $1 < '$dt_max') printf "%15.3f %15.3f\n",$1-'$dt', $2*'$AMP'+'$DIST'}' $long   > $record
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 - '$dt', $2*'$AMP'+'$DIST'}' $long   > $record
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 - '$dt', $2*'$AMP'+'$DIST'}' $phase   > $phase_win
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 - '$dt', $2*'$AMP'+'$DIST'}' $phase_stretch   > $phase_stretch_win
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 - '$dt', $2*'$AMP'+'$DIST'}' $emp   > $emp_win



	psxy $phase_stretch_win -J$SCALE -R$RANGE -O -K -N  <<EOF >> $OUT
EOF
##psxy $phase_win -W/green -J$SCALE -R$RANGE -O -K -N  <<EOF >> $OUT
##EOF

##pstext -J$SCALE -R$RANGE -O -K -N  << EOF >> $OUT
##$MAX $DIST 10 0 0 LB weight: $weight $STA
##EOF

end #foreach STA
## add second EW plot in red at the bottom of PLOT3
set first_EW = $work_dir/${PHASE}_ES.second.out
set first_EW_STD = $work_dir/${PHASE}_STD.second.out
set first_EW_xmin = `cat $first_EW|minmax -C |awk '{print $1}'`
set first_EW_xmax = `cat $first_EW|minmax -C |awk '{print $2}'`
psxy $first_EW_STD -G192/192/192 -L -JX1i/0.6i -R$first_EW_xmin/$first_EW_xmax/-1.1/1.1 -Y-1.2i -O -K -N  << EOF >> $OUT
EOF
psxy $first_EW -W/red -JX -R$first_EW_xmin/$first_EW_xmax/-1.1/1.1  -O -K -N  << EOF >> $OUT
EOF
pstext -J -R -N -O -K << EOF >> $OUT
$first_EW_xmin -1.2 7 0 0 LB  Stretched records stacked E.W. with STD
$first_EW_xmin -1.6 7 0 0 LB weight (SNR, stretching_ccc) > 0 
EOF
psxy -JX -R -O -K -Y1.2i << EOF >> $OUT
EOF


###############################################################
#	
###############################################################
#	Plottint record section for origional records (not aligned ) PLOT5
#	but with t* E.W. and gaussian fit
#	
###############################################################
###############################################################
##echo "___> working on $EQ $PHASE $FLAG "
psxy -R$RANGE -J$SCALE -Ba10f5/a${Y_INC}f${Y_FLAG}wSne -X1.3i -O -K -N  <<EOF>> $OUT
EOF

pstext -R$RANGE -J$SCALE -O -K -N<< EOF >> $OUT
$X_P $TAG_Y $FONT_SIZE 0 0 LB  Step #5
$X_P $TAG_Y1 $FONT_SIZE 0 0 LB $TAG5
EOF

### plot the window background
foreach STA (`cat $event_info |awk '{print $1}'`)
	set TMP = `grep -w $STA $event_info`
	set quality_flag = $TMP[14]
	if( $quality_flag != 1) then
	continue
	endif
	##set dt = $TMP[19]
	set dt = 0
	set onset_time = $TMP[19]
	set DIST = $TMP[3]
	set weight = $TMP[30]
	set stretch_factor = $TMP[22]
	set tstar_factor = $TMP[35]
	set ONSET = $TMP[33]
	set ENDSET = $TMP[34]


## =======================================

set XMIN_K = `echo "1*$FLAG_INC*$AMP + $DIST"|bc -l`
set XMAX_K = `echo "-1*$FLAG_INC*$AMP + $DIST"|bc -l`
##echo "YMIN K Is $XMIN_K $XMAX_K"
set NEW_ONSET = `echo "$ONSET -3"|bc -l`
set NEW_ENDSET = `echo "$ENDSET +3"|bc -l`
psxy -J$SCALE -R$RANGE -G204/255/255 -L -O -K -N <<EOF >>$OUT
$NEW_ONSET $XMIN_K
$NEW_ENDSET $XMIN_K
$NEW_ENDSET $XMAX_K
$NEW_ONSET $XMAX_K
$NEW_ONSET $XMIN_K
EOF
set XMIN_K = `echo "1*$FLAG_INC*$AMP + $DIST"|bc -l`
set XMAX_K = `echo "-1*$FLAG_INC*$AMP + $DIST"|bc -l`
##echo "YMIN K Is $XMIN_K $XMAX_K"
psxy -J$SCALE -R$RANGE -G153/255/153 -L -O -K -N <<EOF >>$OUT
$ONSET $XMIN_K
$ENDSET $XMIN_K
$ENDSET $XMAX_K
$ONSET $XMAX_K
$ONSET $XMIN_K
EOF
## =======================================
end #foreach STA
# add vertical line for checking
psxy -J$SCALE -R$RANGE -W1,purple,- -O -K -N  << EOF >>$OUT
0 $DISTMIN
0 $DISTMAX
EOF

foreach STA (`cat $event_info |awk '{print $1}'`)
	set TMP = `grep -w $STA $event_info`
	set quality_flag = $TMP[14]
	if( $quality_flag != 1) then
	continue
	endif
	##set dt = $TMP[19]
	set dt = 0
	set onset_time = $TMP[19]
	set DIST = $TMP[3]
	set weight = $TMP[30]
	set stretch_factor = $TMP[22]
	set tstar_factor = $TMP[35]
	set long = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.long
	set phase = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.phase
	set emp = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.emp
	set gau = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.gau
	set ONSET = $TMP[33]
	set ENDSET = $TMP[34]

	##echo "---> Working on $STA $PHASE"
	# cut long to alignment window
	set dt_min = `echo "$dt+ $MIN"|bc -l`
	set dt_max = `echo "$dt+ $MAX"|bc -l`
	set record = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.data
	set phase_win = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.phase_win
	set emp_win = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.emp_win
	set gau_win = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.gau_win
	##awk -v min=$dt_min '{if($1>min) print $0}' $long | awk -v max=$dt_max '{if($1<max) print $0}' > $record
	##awk '{if($1 > '$dt_min' && $1 < '$dt_max') printf "%15.3f %15.3f\n",$1-'$dt', $2*'$AMP'+'$DIST'}' $long   > $record
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 , $2*'$AMP'+'$DIST'}' $long   > $record
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 , $2*'$AMP'+'$DIST'}' $phase   > $phase_win
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 , $2*'$AMP'+'$DIST'}' $emp   > $emp_win
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 , $2*'$AMP'+'$DIST'}' $gau   > $gau_win





	psxy $record -J$SCALE -R$RANGE -O -K -N  <<EOF >> $OUT
EOF
	psxy $emp_win -W/red  -J$SCALE -R$RANGE -O -K -N  <<EOF >> $OUT
EOF
##psxy $phase_win -W/green -J$SCALE -R$RANGE -O -K -N  <<EOF >> $OUT
##EOF

set TSTAR_X_P = 30
set TSTAR_Y_P = `echo "$DIST + $AMP*0.6"|bc -l`

pstext -JX -R -O -K -N<< EOF >> $OUT
$TSTAR_X_P  $TSTAR_Y_P 5 0 0 LB t*: $tstar_factor
EOF


##pstext -J$SCALE -R$RANGE -O -K -N  << EOF >> $OUT
##$MAX $DIST 10 0 0 LB weight: $weight $STA
##EOF

end #foreach STA


###############################################################
#	
###############################################################
#	Plottint record section for origional records (not aligned ) PLOT6
#	but with t* E.W. and gaussian fit
#	
###############################################################
###############################################################
##echo "___> working on $EQ $PHASE $FLAG "
psxy -R$RANGE -J$SCALE -Ba10f5/a${Y_INC}f${Y_FLAG}wSne -X1.3i -O -K -N  <<EOF>> $OUT
EOF
# add vertical line for checking
psxy -J$SCALE -R$RANGE -W1,purple,- -O -K -N  << EOF >>$OUT
0 $DISTMIN
0 $DISTMAX
EOF

pstext -R$RANGE -J$SCALE -O -K -N<< EOF >> $OUT
$X_P $TAG_Y $FONT_SIZE 0 0 LB  Step #6
$X_P $TAG_Y1 $FONT_SIZE 0 0 LB $TAG6
EOF

foreach STA (`cat $event_info |awk '{print $1}'`)
	set TMP = `grep -w $STA $event_info`
	set quality_flag = $TMP[14]
	if( $quality_flag != 1) then
	continue
	endif
	##set dt = $TMP[19]
	set dt = 0
	set onset_time = $TMP[19]
	set DIST = $TMP[3]
	set weight = $TMP[30]
	set stretch_factor = $TMP[22]
	set tstar_factor = $TMP[35]
	set long = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.long
	set phase = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.phase
	set emp = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.emp
	set gau = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.gau

	##echo "---> Working on $STA $PHASE"
	# cut long to alignment window
	set dt_min = `echo "$dt+ $MIN"|bc -l`
	set dt_max = `echo "$dt+ $MAX"|bc -l`
	set record = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.data
	set phase_win = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.phase_win
	set emp_win = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.emp_win
	set gau_win = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.gau_win
	##awk -v min=$dt_min '{if($1>min) print $0}' $long | awk -v max=$dt_max '{if($1<max) print $0}' > $record
	##awk '{if($1 > '$dt_min' && $1 < '$dt_max') printf "%15.3f %15.3f\n",$1-'$dt', $2*'$AMP'+'$DIST'}' $long   > $record
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 , $2*'$AMP'+'$DIST'}' $long   > $record
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 , $2*'$AMP'+'$DIST'}' $phase   > $phase_win
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 , $2*'$AMP'+'$DIST'}' $emp   > $emp_win
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 , $2*'$AMP'+'$DIST'}' $gau   > $gau_win





	psxy $emp_win -W/red  -J$SCALE -R$RANGE -O -K -N  <<EOF >> $OUT
EOF
	psxy $gau_win -W/blue  -J$SCALE -R$RANGE -O -K -N  <<EOF >> $OUT
EOF
##psxy $phase_win -W/green -J$SCALE -R$RANGE -O -K -N  <<EOF >> $OUT
##EOF


##pstext -J$SCALE -R$RANGE -O -K -N  << EOF >> $OUT
##$MAX $DIST 10 0 0 LB weight: $weight $STA
##EOF

end #foreach STA

###############################################################
#	
###############################################################
#	Plottint record section for origional records (not aligned ) PLOT7
#	but with t* E.W. and gaussian fit
#	
###############################################################
###############################################################
##echo "___> working on $EQ $PHASE $FLAG "
psxy -R$RANGE -J$SCALE -Ba10f5/a${Y_INC}f${Y_FLAG}wSne -X1.3i -O -K -N  <<EOF>> $OUT
EOF
# add vertical line for checking
psxy -J$SCALE -R$RANGE -W1,purple,- -O -K -N  << EOF >>$OUT
0 $DISTMIN
0 $DISTMAX
EOF

pstext -R$RANGE -J$SCALE -O -K -N<< EOF >> $OUT
$X_P $TAG_Y $FONT_SIZE 0 0 LB  Step #7
$X_P $TAG_Y1 $FONT_SIZE 0 0 LB $TAG7
EOF

foreach STA (`cat $event_info |awk '{print $1}'`)
	set TMP = `grep -w $STA $event_info`
	set quality_flag = $TMP[14]
	if( $quality_flag != 1) then
	continue
	endif
	##set dt = $TMP[19]
	set dt = 0
	set onset_time = $TMP[19]
	set DIST = $TMP[3]
	set weight = $TMP[30]
	set stretch_factor = $TMP[22]
	set tstar_factor = $TMP[35]
	set long = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.long
	set phase = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.phase
	set emp = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.emp
	set gau = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.gau

	##echo "---> Working on $STA $PHASE"
	# cut long to alignment window
	set dt_min = `echo "$dt+ $MIN"|bc -l`
	set dt_max = `echo "$dt+ $MAX"|bc -l`
	set record = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.data
	set phase_win = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.phase_win
	set emp_win = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.emp_win
	set gau_win = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.gau_win
	##awk -v min=$dt_min '{if($1>min) print $0}' $long | awk -v max=$dt_max '{if($1<max) print $0}' > $record
	##awk '{if($1 > '$dt_min' && $1 < '$dt_max') printf "%15.3f %15.3f\n",$1-'$dt', $2*'$AMP'+'$DIST'}' $long   > $record
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 , $2*'$AMP'+'$DIST'}' $long   > $record
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 , $2*'$AMP'+'$DIST'}' $phase   > $phase_win
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 , $2*'$AMP'+'$DIST'}' $emp   > $emp_win
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 , $2*'$AMP'+'$DIST'}' $gau   > $gau_win





	psxy $record -J$SCALE -R$RANGE -O -K -N  <<EOF >> $OUT
EOF
	psxy $gau_win -W/blue  -J$SCALE -R$RANGE -O -K -N  <<EOF >> $OUT
EOF
##psxy $phase_win -W/green -J$SCALE -R$RANGE -O -K -N  <<EOF >> $OUT
##EOF

## add travel time onset arrow here
set arrow_parameter = "-Svh0.005i/0.08i/0.01i"
	psxy -JX -R $arrow_parameter -G255/0/0  -O -K <<EOF>>$OUT
$onset_time $DIST 90 0.5
EOF

##pstext -J$SCALE -R$RANGE -O -K -N  << EOF >> $OUT
##$MAX $DIST 10 0 0 LB weight: $weight $STA
##EOF

end #foreach STA


###############################################################
#	
###############################################################
#	Plottint record section for origional records (not aligned ) PLOT8
#	but with t* E.W. and gaussian fit
#	
###############################################################
###############################################################
##echo "___> working on $EQ $PHASE $FLAG "
psxy -R$RANGE -J$SCALE -Ba10f5/a${Y_INC}f${Y_FLAG}wSne -X1.3i -O -K -N  <<EOF>> $OUT
EOF
# add vertical line for checking
psxy -J$SCALE -R$RANGE -W1,purple,- -O -K -N  << EOF >>$OUT
0 $DISTMIN
0 $DISTMAX
EOF

pstext -R$RANGE -J$SCALE -O -K -N<< EOF >> $OUT
$X_P $TAG_Y $FONT_SIZE 0 0 LB  Step #8
$X_P $TAG_Y1 $FONT_SIZE 0 0 LB $TAG8
EOF

foreach STA (`cat $event_info |awk '{print $1}'`)
	set TMP = `grep -w $STA $event_info`
	set quality_flag = $TMP[14]
	if( $quality_flag != 1) then
	continue
	endif
	##set dt = $TMP[19]
	set dt = 0
	set onset_time = $TMP[19]
	set DIST = $TMP[3]
	set weight = $TMP[30]
	set stretch_factor = $TMP[22]
	set tstar_factor = $TMP[35]
	set long = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.long
	set phase = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.phase
	set emp = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.emp
	set gau = $work_dir/${EQ}.${STA}.${PHASE}.${COMP}.gau

	##echo "---> Working on $STA $PHASE"
	# cut long to alignment window
	set dt_min = `echo "$dt+ $MIN"|bc -l`
	set dt_max = `echo "$dt+ $MAX"|bc -l`
	set record = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.data
	set phase_win = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.phase_win
	set emp_win = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.emp_win
	set gau_win = $work_dir/record_section.${EQ}.${STA}.${PHASE}.${COMP}.${FLAG}.gau_win
	##awk -v min=$dt_min '{if($1>min) print $0}' $long | awk -v max=$dt_max '{if($1<max) print $0}' > $record

	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 , $2*'$AMP'+'$DIST'}' $long   > $record
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 , $2*'$AMP'+'$DIST'}' $phase   > $phase_win
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 , $2*'$AMP'+'$DIST'}' $emp   > $emp_win
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 , $2*'$AMP'+'$DIST'}' $gau   > $gau_win



	psxy $record -J$SCALE -R$RANGE -O -K -N  <<EOF >> $OUT
EOF
##psxy $phase_win -W/green -J$SCALE -R$RANGE -O -K -N  <<EOF >> $OUT
##EOF

## add travel time onset arrow here
set arrow_parameter = "-Svh0.005i/0.08i/0.01i"
	psxy -JX -R $arrow_parameter -G255/0/0  -O -K <<EOF>>$OUT
$onset_time $DIST 90 0.5
EOF

##pstext -J$SCALE -R$RANGE -O -K -N  << EOF >> $OUT
##$MAX $DIST 10 0 0 LB weight: $weight $STA
##EOF

end #foreach STA



## add explanation to all the FLAG in the plot
psxy -JX -R -O -K -Y-1.3i -X-3i << EOF >> $OUT
EOF

# purple PREM time
psxy -JX1.5i/1i -R0/10/0/10 -O -W1,purple,- -K -N << EOF >> $OUT
1 8
1 10
EOF
# horizontal phase window
psxy -JX -R -O -K -W15,255/128/0 -N << EOF >> $OUT
0 7
2 7
EOF

# horizontal tight window
psxy -JX -R -O -K -W15,153/255/153 -N << EOF >> $OUT
0 5
2 5
EOF

# horizontal tight window + 3sec
psxy -JX -R -O -K -W15,204/255/255 -N << EOF >> $OUT
0 3
2 3
EOF



## add travel time onset arrow here
set arrow_parameter = "-Svh0.005i/0.08i/0.01i"
	psxy -JX -R $arrow_parameter -G255/0/0  -O -K <<EOF>>$OUT
1 2 90 0.5
EOF

### add text
pstext -JX -R -O -K << EOF >> $OUT
4 9 5 0 0 LB PREM time
4 7 5 0 0 LB phase window
4 5 5 0 0 LB CCC window
4 3 5 0 0 LB Misfit window
4 1 5 0 0 LB ONSET
EOF


## add some more explanation

## add explanation to all the FLAG in the plot
psxy -JX -R -O -K  -X1.5i << EOF >> $OUT
EOF

# recod
psxy -JX -R -O -K -W2 -N << EOF >> $OUT
0 9
2 9
EOF
#  add E.W.
psxy -JX -R -O -K -W2,red -N << EOF >> $OUT
0 7
2 7
EOF

# gaussian
psxy -JX -R -O -K -W2,blue -N << EOF >> $OUT
0 5
2 5
EOF

### add text
pstext -JX -R -O -K << EOF >> $OUT
4 9 5 0 0 LB Record
4 7 5 0 0 LB E.W. (stretched,t* operated)
4 5 5 0 0 LB Best fitting Gaussian function
EOF



psxy -J$SCALE -R$RANGE -O -N -P << EOF >> $OUT
EOF

end 
#foreach end 

ps2pdf $OUT $OUT_pdf
rm $OUT










##psxy -J$SCALE -R$RANGE -O -N -P << EOF >> $OUT
##EOF

##end 
###foreach end 

##ps2pdf $OUT $OUT_pdf
##rm $OUT

