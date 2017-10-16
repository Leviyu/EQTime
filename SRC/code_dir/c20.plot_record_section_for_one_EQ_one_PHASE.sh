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
set MIN = -15
set MAX = 30
##set AMP = 0.1 		# the amplitude of each record in the plots

cd $DATADIR/$EQ
gmtset ANNOT_FONT_SIZE_PRIMARY = 10p
gmtset COLOR_BACKGROUND = white
gmtset COLOR_FOREGROUND = white
gmtset COLOR_NAN = white
gmtset LABEL_FONT_SIZE = 15p


set OUT = $PLOTDIR/$EQ/Record_section_${EQ}.ps
set OUT_pdf = $PLOTDIR/$EQ/Record_section_${EQ}.pdf
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



pstext -JX6i/1i -R0/10/0/10 -Y10.5i -K -N -P << EOF >> $OUT
0 8 12 0 0 LB Record Section (only good records: $num_good_record)
0 6 12 0 0 LB EQ: 	 $EQ
0 4 12 0 0 LB PHASE: $PHASE
0 2 12 0 0 LB filter: $FLAG AMP $AMP
EOF

set RANGE = $MIN/$MAX/$DISTMIN/$DISTMAX
set SCALE = X1.5i/8.5i 
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
psxy -R$RANGE -J$SCALE -Ba10f5:"Seconds":/a${Y_INC}f${Y_FLAG}WSne:"Distance (Degree)": -Y-8.5i -O -K -N -P <<EOF>> $OUT
EOF
# add vertical line for checking
psxy -J$SCALE -R$RANGE -W1/red -O -K -N -P << EOF >>$OUT
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





	psxy $record -J$SCALE -R$RANGE -O -K -N -P <<EOF >> $OUT
EOF
##psxy $emp_win -W/red  -J$SCALE -R$RANGE -O -K -N -P <<EOF >> $OUT
##EOF
##psxy $phase_win -W/green -J$SCALE -R$RANGE -O -K -N -P <<EOF >> $OUT
##EOF
##pstext -JX -R -O -K -N<< EOF >> $OUT
##$MAX $DIST 7 0 0 CB $stretch_factor
##EOF
##pstext -J$SCALE -R$RANGE -O -K -N -P << EOF >> $OUT
##$MAX $DIST 10 0 0 CB weight: $weight $STA
##EOF
end #foreach STA
## add first iteration EW plot in red at the bottom of PLOT1
set first_EW = $work_dir/${PHASE}_ES.first_iteration.out
set first_EW_STD = $work_dir/${PHASE}_STD.first_iteration.out
set first_EW_xmin = `cat $first_EW|minmax -C |awk '{print $1}'`
set first_EW_xmax = `cat $first_EW|minmax -C |awk '{print $2}'`
psxy $first_EW_STD -G192/192/192 -L -JX1.5i/0.6i -R$first_EW_xmin/$first_EW_xmax/-1.1/1.1 -Y-1.2i -O -K -N -P << EOF >> $OUT
EOF
psxy $first_EW -W/red -JX1.5i/0.6i -R$first_EW_xmin/$first_EW_xmax/-1.1/1.1  -O -K -N -P << EOF >> $OUT
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
psxy -R$RANGE -J$SCALE -Ba10f5/a${Y_INC}f${Y_FLAG}wsne -X1.7i -O -K -N -P <<EOF>> $OUT
EOF
# add vertical line for checking
psxy -J$SCALE -R$RANGE -W1/red -O -K -N -P << EOF >>$OUT
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

	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 - '$dt', $2/'$record_max'*'$AMP'+'$DIST'}' $long   > $record
	##awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 - '$dt', $2*'$AMP'+'$DIST'}' $long   > $record
	##awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 - '$dt', $2*'$AMP'+'$DIST'}' $long   > $record
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 - '$dt', $2*'$AMP'+'$DIST'}' $phase   > $phase_win
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 - '$dt', $2*'$AMP'+'$DIST'}' $emp   > $emp_win





	psxy $record -J$SCALE -R$RANGE -O -K -N -P <<EOF >> $OUT
EOF

##psxy $emp_win -W/red  -J$SCALE -R$RANGE -O -K -N -P <<EOF >> $OUT
##EOF
##psxy $phase_win -W/green -J$SCALE -R$RANGE -O -K -N -P <<EOF >> $OUT
##EOF
##pstext -JX -R -O -K -N<< EOF >> $OUT
##$MAX $DIST 7 0 0 CB $stretch_factor
##EOF


##pstext -J$SCALE -R$RANGE -O -K -N -P << EOF >> $OUT
##$MAX $DIST 10 0 0 CB weight: $weight $STA
##EOF

end #foreach STA
## add first EW plot in red at the bottom of PLOT2
set first_EW = $work_dir/${PHASE}_ES.first.out
set first_EW_STD = $work_dir/${PHASE}_STD.first.out
set first_EW_xmin = `cat $first_EW|minmax -C |awk '{print $1}'`
set first_EW_xmax = `cat $first_EW|minmax -C |awk '{print $2}'`
psxy $first_EW_STD -G192/192/192 -L -JX1.5i/0.6i -R$first_EW_xmin/$first_EW_xmax/-1.1/1.1 -Y-1.2i -O -K -N -P << EOF >> $OUT
EOF
psxy $first_EW -W/red -JX1.5i/0.6i -R$first_EW_xmin/$first_EW_xmax/-1.1/1.1  -O -K -N -P << EOF >> $OUT
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
#	Plottint record section for stretched records (aligned) PLOT3
#	
###############################################################
###############################################################
##echo "___> working on $EQ $PHASE $FLAG "
psxy -R$RANGE -J$SCALE -Ba10f5/a${Y_INC}f${Y_FLAG}wsne -X1.7i -O -K -N -P <<EOF>> $OUT
EOF
# add vertical line for checking
psxy -J$SCALE -R$RANGE -W1/red -O -K -N -P << EOF >>$OUT
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





	##psxy $record -J$SCALE -R$RANGE -O -K -N -P <<EOF >> $OUT
	##EOF
##psxy $emp_win -W/red  -J$SCALE -R$RANGE -O -K -N -P <<EOF >> $OUT
##//EOF
psxy $phase_stretch_win -W/blue -J$SCALE -R$RANGE -O -K -N -P <<EOF >> $OUT
EOF
##pstext -JX -R -O -K -N<< EOF >> $OUT
##$MAX $DIST 7 0 0 CB $stretch_factor
##EOF


##pstext -J$SCALE -R$RANGE -O -K -N -P << EOF >> $OUT
##$MAX $DIST 10 0 0 CB weight: $weight $STA
##EOF

end #foreach STA

## add second EW plot in red at the bottom of PLOT3
set first_EW = $work_dir/${PHASE}_ES.second.out
set first_EW_STD = $work_dir/${PHASE}_STD.second.out
set first_EW_xmin = `cat $first_EW|minmax -C |awk '{print $1}'`
set first_EW_xmax = `cat $first_EW|minmax -C |awk '{print $2}'`
psxy $first_EW_STD -G192/192/192 -L -JX1.5i/0.6i -R$first_EW_xmin/$first_EW_xmax/-1.1/1.1 -Y-1.2i -O -K -N -P << EOF >> $OUT
EOF
psxy $first_EW -W/red -JX1.5i/0.6i -R$first_EW_xmin/$first_EW_xmax/-1.1/1.1  -O -K -N -P << EOF >> $OUT
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
#	Plottint record section for origional records (not aligned ) PLOT4
#	but with t* E.W. and gaussian fit
#	
###############################################################
###############################################################
##echo "___> working on $EQ $PHASE $FLAG "
psxy -R$RANGE -J$SCALE -Ba10f5/a${Y_INC}f${Y_FLAG}wsne -X1.7i -O -K -N -P <<EOF>> $OUT
EOF
# add vertical line for checking
psxy -J$SCALE -R$RANGE -W1/red -O -K -N -P << EOF >>$OUT
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
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 - '$dt', $2*'$AMP'+'$DIST'}' $long   > $record
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 - '$dt', $2*'$AMP'+'$DIST'}' $phase   > $phase_win
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 - '$dt', $2*'$AMP'+'$DIST'}' $emp   > $emp_win
	awk '{if($1 > '$dt_min' && $1 < '$dt_max') print $1 - '$dt', $2*'$AMP'+'$DIST'}' $gau   > $gau_win





	psxy $record -J$SCALE -R$RANGE -O -K -N -P <<EOF >> $OUT
EOF
	psxy $emp_win -W/red  -J$SCALE -R$RANGE -O -K -N -P <<EOF >> $OUT
EOF
	psxy $gau_win -W/purple  -J$SCALE -R$RANGE -O -K -N -P <<EOF >> $OUT
EOF
##psxy $phase_win -W/green -J$SCALE -R$RANGE -O -K -N -P <<EOF >> $OUT
##EOF
	pstext -JX -R -O -K -N<< EOF >> $OUT
$MAX $DIST 7 0 0 CB $tstar_factor
EOF

## add travel time onset arrow here
set arrow_parameter = "-Svh0.005i/0.08i/0.01i"
	psxy -JX -R $arrow_parameter -G255/0/0  -O -K <<EOF>>$OUT
$onset_time $DIST 90 0.5
EOF

##pstext -J$SCALE -R$RANGE -O -K -N -P << EOF >> $OUT
##$MAX $DIST 10 0 0 CB weight: $weight $STA
##EOF

end #foreach STA





psxy -J$SCALE -R$RANGE -O -N -P << EOF >> $OUT
EOF

end 
#foreach end 

ps2pdf $OUT $OUT_pdf
rm $OUT













# =======================================
# add travel time curve
# =======================================
##echo "Computing travel time curves"
set int_EQ_DEP = `printf "%.0f" $EQ_DEP`

set MODEL     =   prem
set Pgroup    =  ( P PcP PP PKP PKIKP PKiKP Pdiff PKKP PPP )
set pPgroup   =  ( pP pPdiff pPP pPcP)
set Sgroup    =  ( S Sdiff SS SSS SSSS  )
set SVgroup   =  ( SKS SKKS SKKKS )
set SHgroup   =  ( ScS ScSScS )
set sSgroup   =  ( sS sSdiff sSS sSSS  )
set sSVgroup   = ( sSKS sSKKS )
set sSHgroup   = ( sSdiff sScS )
set sPgroup   =  ( sP sPdiff )
set pSgroup   =  ( pS pSS pSdiff pSKS pSKKS )
set SPgroup   =  ( ScP SP PS PcS SKP PKS )
set pSPgroup  =  ( pScP pPS )

# setting the pha`taup_curveses that we want to add
set p_or_s = `echo $PHASE |cut -c 1-1`
if($PHASE == "T" && $p_or_s == "S") then
	set phase_list = ( $Sgroup $SHgroup $sSgroup $sSHgroup )
else 
	set phase_list = ( $Sgroup $SVgroup $sSgroup $sSVgroup )
endif

set loc = 1
foreach phase_name  (`echo $phase_list`)

	if( $PHASE == "SSSSm" ||\
	$PHASE == "SSSm" ||\
	$PHASE == "ScSScSm" ||\
	$PHASE == "ScSScSScSm" ||\
	$PHASE == "SSSSSm" ) then
	continue

	##set new_string_l = `echo "$%PHASE-1"|bc `
	##set new_phase_name = `echo $PHASE |cut -c 1-$new_string_l`
	##taup_curve -mod prem -h $EQ_DEP -ph $phase_name -rel $new_phase_name
	##awk '{if(NR>1 && $1 > '$DISTMIN' && $1 < '$DISTMAX' && $2 > '$MIN' && $2 < '$MAX' ) print $0}' taup_curve.gmt > $gmt_curve


	endif
	##echo "--> adding travel time curve for $phase_name"
	set gmt_curve = $work_dir/gmt_curve.${phase_name}
	taup_curve -mod prem -h $EQ_DEP -ph $phase_name -rel $PHASE 
	awk '{if(NR>1 && $1 > '$DISTMIN' && $1 < '$DISTMAX' && $2 > '$MIN' && $2 < '$MAX' ) print $0}' taup_curve.gmt > $gmt_curve



	set x_loc = `awk '{ if(NR == '$loc' ) print $2}' $gmt_curve`
	set y_loc = `awk '{if(NR == '$loc') print $1}' $gmt_curve`
	if($x_loc == "" || $y_loc == "") then
	continue
	endif
	##echo " x y loc is $x_loc $y_loc "
@ loc ++
	psxy $gmt_curve -J$SCALE -R$RANGE -O -: -W/red -K -N -P << EOF >> $OUT
EOF

	pstext -J$SCALE -R$RANGE -O -Wred -K -N -P << EOF >> $OUT
$x_loc $y_loc 10 0 0 CB $phase_name
EOF


end # phase_name


psxy -J$SCALE -R$RANGE -O -N -P << EOF >> $OUT
EOF

end 
#foreach end 

ps2pdf $OUT $OUT_pdf
rm $OUT

