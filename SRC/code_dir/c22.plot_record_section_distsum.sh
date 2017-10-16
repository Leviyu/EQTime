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
##set AMP = 0.1 		# the amplitude of each record in the plots

cd $DATADIR/$EQ
gmtset ANNOT_FONT_SIZE_PRIMARY = 10p
gmtset LABEL_FONT_SIZE = 15p

echo "---> Working on c22.plot_record_section_distsum.sh "

set OUT = $PLOTDIR/$EQ/Record_section_${EQ}.ps
set OUT_pdf = $PLOTDIR/$EQ/Record_section_distsum_${EQ}.pdf
set OUTFILE = $OUT
cat /dev/null > $OUT

set work_dir = $DATADIR/$EQ
set PHASE_LIST = `cat $work_dir/INFILE|grep -w PHASE_LIST | awk 'NR==1 {print $0}'|cut  -f2-`

## foreach begin
foreach PHASE (`echo $PHASE_LIST`)
	##echo "---> working on Record Profile for $EQ $PHASE "
set INFILE_PHASE = $work_dir/INFILE_${PHASE}
set PHASE = `grep -w "<PHASE>" $INFILE_PHASE |awk 'NR==1 {print $2}'`
set COMP  = `grep -w "<COMP>" $INFILE_PHASE |awk 'NR==1 {print $2}'`
set FLAG  = `grep -w "<filter_flag>" $INFILE_PHASE |awk 'NR==1 {print $2}'`
set DISTMIN = `grep -w "<DIST_MIN_MAX>" $INFILE_PHASE |awk 'NR==1 {print $2}'`
set DISTMAX = `grep -w "<DIST_MIN_MAX>" $INFILE_PHASE |awk 'NR==1 {print $3}'`
set DISTDELTA = `grep -w "<DIST_DELTA>" $INFILE_PHASE |awk 'NR==1 {print $2}'`

set MIN = `grep -w "<LONG_BEG>" $INFILE_PHASE |awk 'NR==1 {print $2}'`
set LEN = `grep -w "<LONG_LEN>" $INFILE_PHASE |awk 'NR==1 {print $2}'`

set MAX = `echo "$MIN + $LEN"|bc -l`

if($PHASE == "ScS" )  then
set MIN = -100 
set MAX = 100

endif


if($DISTMIN == "non" ) then
	set DISTMIN = 100
endif
if($DISTMAX == "non" ) then
	set DISTMAX = 100
endif

set DISTMID = `echo "( $DISTMIN + $DISTMAX ) /2"|bc -l`
set event_info = $DATADIR/$EQ/eventinfo.${EQ}.${PHASE}.${COMP}
set EQ_DEP = `cat $event_info |awk 'NR==1 {print $10}'`
set num_good_record = `cat $event_info | awk '$14==1 {print $0}'| wc -l`

set DIST_DELTA = `echo "$DISTMAX - $DISTMIN"|bc -l`
set AMP = `echo "$DIST_DELTA /6 *0.15"|bc -l`
set AMP = `printf "%.3f" $AMP`



pstext -JX6i/1i -R0/10/0/10 -Y10.5i -K -N -P << EOF >> $OUT
0 8 12 0 0 LB Record Section Distance Sum delta: $DISTDELTA
0 6 12 0 0 LB EQ: 	 $EQ
0 4 12 0 0 LB PHASE: $PHASE
0 2 12 0 0 LB filter: $FLAG AMP $AMP
EOF

set RANGE = $MIN/$MAX/$DISTMIN/$DISTMAX
set SCALE = X6i/8.5i 
##echo "---> Range is $RANGE Scale is $SCALE"

##echo "___> working on $EQ $PHASE $FLAG "
psxy -R$RANGE -J$SCALE -Ba10f5:"Seconds":/a5f1WSne:"Distance (Degree)": -Y-8.5i -O -K -N -P <<EOF>> $OUT
EOF
# add vertical line for checking
psxy -J$SCALE -R$RANGE -W1/green -O -K -N -P << EOF >>$OUT
0 $DISTMIN
0 $DISTMAX
EOF

set NUM = 0

while ($NUM < 100 )
	set distsum_file = $DATADIR/${EQ}/${EQ}.${PHASE}.${COMP}.distsum.${NUM}
	if(! -e $distsum_file ) then
@ NUM ++
	continue
	endif

	set stack_file = $DATADIR/${EQ}/distsum.stack_num.${EQ}.${PHASE}
	set stack_num = `cat $stack_file | awk '$1=='$NUM' {print $2}'`


	set long_tmp = $DATADIR/$EQ/long.tmp
	set current_dist = `echo "$DISTMIN + $NUM * $DISTDELTA"|bc -l `
	awk '{print $1,$2*'$AMP'+'$current_dist'}' $distsum_file > $long_tmp

	psxy $long_tmp -J$SCALE -R$RANGE -O -K -N -P <<EOF >> $OUT
EOF

set MAX_loc = `echo "$MAX + 10"|bc`

	pstext -JX -R -O -K -N -P << EOF >> $OUT
$MAX_loc $current_dist 10 0 0 CB $stack_num
EOF

##pstext -J$SCALE -R$RANGE -O -K -N -P << EOF >> $OUT
##$MAX $DIST 10 0 0 CB weight: $weight $STA
##EOF

@ NUM ++

end #while 
# =======================================
# add travel time curve
# =======================================
##echo "Computing travel time curves"
set int_EQ_DEP = `printf "%.0f" $EQ_DEP`

set MODEL     =   prem
set Pgroup    =  ( P PcP PP PKP PKIKP PKiKP Pdiff PKKP PPP )
set pPgroup   =  ( pP pPdiff pPP pPcP)
set Sgroup    =  ( S Sdiff SS SSS SSSS  SSSSS SSSSSS )
set SVgroup   =  ( SKS SKKS SKKKS )
set SHgroup   =  ( ScS ScSScS ScSScSScS ScSScSScSScS ScSScSScSScSScS  )
set sSgroup   =  ( sS sSdiff sSS sSSS  sSSSS sSSSSS sSSSSSS)
set sSVgroup   = ( sSKS sSKKS )
set sSHgroup   = ( sSdiff sScS sScSScS sScSScSScS sScSScSScSScS)
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

	##echo "--> adding travel time curve for $phase_name"
	set gmt_curve = $DATADIR/$EQ/gmt_curve.${phase_name}
	if(`echo $PHASE | grep m  ` == "" ) then
	taup_curve -mod prem -h $EQ_DEP -ph $phase_name -rel $PHASE 
	else
	set main_PHASE = `echo $PHASE | sed 's/.$//' `
	taup_curve -mod prem -h $EQ_DEP -ph $phase_name -rel $main_PHASE
	endif
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

