#!/bin/csh
# ====================================================================
# This is a csh script for plotting the ES infomation of a given phase
# 	including
# 	1. ES with STD
# 	2. S transformed phase if exists
# 	3. Histogram info of 
# 		CCC SNR stratch_factor
# 
# DATE: Nov 7 2014			Keywords:
# ===================================================================

set EQ = $1
set DIR = $2
set SRCDIR = $3
set PLOTDIR = $4/$EQ
set work_dir = $5
set OUT = $PLOTDIR/${EQ}_ES_statistics.ps
set OUT_pdf = $PLOTDIR/${EQ}_ES_statistics.pdf
set eventStation = $work_dir/${EQ}/eventStation.${EQ}

rm $OUT > & /dev/null

echo "----> Working on c07.plot_phase_statistics.sh"
set PHASE_LIST = `cat $work_dir/INFILE|grep -w PHASE_LIST | awk 'NR==1 {print $0}'|cut -d ' ' -f2-`

set TMP = `awk 'NR==2 {print $0}' $eventStation`
set EQ_mag = $TMP[16]
set EQ_lat = $TMP[11]
set EQ_lon = $TMP[12]
set EQ_dep = $TMP[13]
set ymin = -1.2
set ymax = 1.2



cd $work_dir/${EQ}
gmtset ANNOT_FONT_SIZE_PRIMARY = 9p
gmtset LABEL_FONT_SIZE = 9p
gmtset ANNOT_FONT_SIZE_PRIMARY = 5p
gmtset ANNOT_FONT_SIZE_SECONDARY = 5p

foreach PHASE ( $PHASE_LIST)
	set INFILE = $work_dir/INFILE_${PHASE}
	set velocity_or_displacement = `grep -w velocity_or_displacement $INFILE |awk 'NR==1 {print $2}'`
	set flag = `grep -w filter_flag $INFILE | awk '{print $2}'`
	set COMP = `grep -w COMP $INFILE | awk '{print $2}'`
	set event = eventinfo.${EQ}.${PHASE}.${COMP}
	if( -e $event) then
echo "PLOT Empirical Wavelet Info for PHASE: $PHASE $flag"
		else 
		continue
		endif
	set OUT_tmp = $work_dir/${EQ}/out.tmp_${PHASE}_${flag}.ps
	#  ==================  prepare the plot ==========================
	pstext -JX6i/1i -R0/10/0/10 -K -N -P  -Y11i << EOF >! $OUT_tmp
	0 0   15 0 0 LB Empirical Source Plot of EQ: $EQ for PHASE:$PHASE in $velocity_or_displacement  
	0 -3  10 0 0 LB EQ_lat: $EQ_lat EQ_lon:$EQ_lon EQ_dep: $EQ_dep MAG: $EQ_mag filter:$flag
EOF

	set ES = $work_dir/${EQ}/${PHASE}_ES.second.out
	set STD = $work_dir/${EQ}/${PHASE}_STD.second.out
	if ( -e $ES) then	
	set tmp = `minmax -C $ES`
	set xmin = $tmp[1]
	set xmax = $tmp[2]

	psxy $STD -JX5i/2i -R$xmin/$xmax/$ymin/$ymax -Y-2.5i -O -L -G192/192/192 -K -P >>$OUT_tmp
	psxy $ES -JX -R$xmin/$xmax/$ymin/$ymax  -O -K -P >>$OUT_tmp
	

# add horizontal 0 line
	psxy -JX -R -O -Wfaint/204/169/228ta -K -P <<EOF>> $OUT_tmp
	$xmin 0 
	$xmax 0
EOF

echo "Histogram for $PHASE begin"
#  =======add histogram info of SNR CCC stretch factor ====================
set SNR_file = SNR_file.tmp
set CCC_file = CCC_file.tmp
set stretch_CCC_file = stretch_CCC_file.tmp
set weight_file = weight.tmp
set stretch_coeff = stretch_coeff.tmp
set dT_file  = dT_file.tmp
set misfit_file = misfitfile.tmp
awk '$14==1 {print $18}' $event > $SNR_file
awk '$14==1 {print $17}' $event > $CCC_file
awk '$14==1 {print $21}' $event > $stretch_CCC_file
awk '$14==1 {print $30}' $event > $weight_file
awk '$14==1 {print $22}' $event > $stretch_coeff
awk '$14==1 {print $19}' $event > $dT_file
awk '$14==1 {print $23}' $event > $misfit_file

# =========================================================================
#		
#		Add the histogram info for SNR
#
# =========================================================================
set INFILE = $SNR_file
set CPT_MAX = 10
set XMIN = 0
set XMAX = 25
set XINC =  1
set XNUM =  4 


set TEXT = SNR
set XLABEL = $TEXT 
set YLABEL = 'Frequency'
set TMP = histogram.tmp
awk '{ if($1 != "") print $1}' $INFILE > $TMP
set tmp = `minmax -C $TMP`


pshistogram  $TMP  -W$XINC -IO >! histo.tmp
set sta_num = `cat $TMP |wc -l`

set YMAX = `minmax -C histo.tmp |awk '{print $4*1.2}'`
set YNUM = ` echo $YMAX | awk '{print int(0.2*$1) }' `
set YTICK = ` echo $YNUM | awk '{print int(1.0*$1 / 2.0) }' `

pshistogram $TMP -R${XMIN}/${XMAX}/0/$YMAX -Ba${XNUM}f${XINC}:"${XLABEL}":/a${YNUM}f${YTICK}:"${YLABEL}":WS -JX5.0i/1.2i -W$XINC -L0.5p -G50/50/250   -Y-1i   -O -K  <<EOF>> $OUT_tmp
EOF

# =========================================================================
#		
#		Add the histogram info for CCC
#
# =========================================================================
set INFILE = $CCC_file
set XINC =  0.1
set XNUM =  0.2
set XMIN =  -1
set XMAX =  1


set TEXT = CCC
set XLABEL = $TEXT 
set YLABEL = 'Frequency'
set TMP = histogram.tmp
awk '{ if($1 != "") print $1}' $INFILE > $TMP
set tmp = `minmax -C $TMP`


pshistogram  $TMP  -W$XINC -IO >! histo.tmp
set sta_num = `cat $TMP |wc -l`

set YMAX = `minmax -C histo.tmp |awk '{print $4*1.2}'`
set YNUM = ` echo $YMAX | awk '{print int(0.2*$1) }' `
set YTICK = ` echo $YNUM | awk '{print int(1.0*$1 / 2.0) }' `

pshistogram $TMP -R${XMIN}/${XMAX}/0/$YMAX -Ba${XNUM}f${XINC}:"${XLABEL}":/a${YNUM}f${YTICK}:"${YLABEL}":WS -JX5.0i/1.2i -W$XINC -L0.5p -G50/50/250   -Y-1.2i   -O -K  <<EOF>> $OUT_tmp
EOF

# =========================================================================
#		
#		Add the histogram info for weight
#
# =========================================================================
set INFILE = $weight_file
set XMIN = 0
set XMAX = 1
set XINC =  0.1
set XNUM =  0.2


set TEXT = Weight
set XLABEL = $TEXT 
set YLABEL = 'Frequency'
set TMP = histogram.tmp
awk '{ if($1 != "") print $1}' $INFILE > $TMP
set tmp = `minmax -C $TMP`


pshistogram  $TMP  -W$XINC -IO >! histo.tmp
set sta_num = `cat $TMP |wc -l`

set YMAX = `minmax -C histo.tmp |awk '{print $4*1.2}'`
set YNUM = ` echo $YMAX | awk '{print int(0.2*$1) }' `
set YTICK = ` echo $YNUM | awk '{print int(1.0*$1 / 2.0) }' `

pshistogram $TMP -R${XMIN}/${XMAX}/0/$YMAX -Ba${XNUM}f${XINC}:"${XLABEL}":/a${YNUM}f${YTICK}:"${YLABEL}":WS -JX5.0i/1.2i -W$XINC -L0.5p -G50/50/250   -Y-1.2i   -O -K  <<EOF>> $OUT_tmp
EOF
# =========================================================================
#		
#		Add the histogram info for dt_residual
#
# =========================================================================
set INFILE = $dT_file
set XMIN = -20 
set XMAX = 20
set XINC =  1
set XNUM =  5


set TEXT = dT_residual 
set XLABEL = $TEXT 
set YLABEL = 'Frequency'
set TMP = histogram.tmp
awk '{ if($1 != "") print $1}' $INFILE > $TMP
set tmp = `minmax -C $TMP`


pshistogram  $TMP  -W$XINC -IO >! histo.tmp
set sta_num = `cat $TMP |wc -l`

set YMAX = `minmax -C histo.tmp |awk '{print $4*1.2}'`
set YNUM = ` echo $YMAX | awk '{print int(0.2*$1) }' `
set YTICK = ` echo $YNUM | awk '{print int(1.0*$1 / 2.0) }' `

pshistogram $TMP -R${XMIN}/${XMAX}/0/$YMAX -Ba${XNUM}f${XINC}:"${XLABEL}":/a${YNUM}f${YTICK}:"${YLABEL}":WS -JX5.0i/1.2i -W$XINC -L0.5p -G50/50/250   -Y-1.2i   -O -K <<EOF>> $OUT_tmp
EOF

# =========================================================================
#		
#		Add the histogram info for stretch_coefficient
#
# =========================================================================
set INFILE = $stretch_coeff
set XMIN = 0.5 
set XMAX = 2.5
set XINC = 0.1 
set XNUM =  0.5


set TEXT = Stretch_coeff
set XLABEL = $TEXT 
set YLABEL = 'Frequency'
set TMP = histogram.tmp
awk '{ if($1 != "") print $1}' $INFILE > $TMP
set tmp = `minmax -C $TMP`


pshistogram  $TMP  -W$XINC -IO >! histo.tmp
set sta_num = `cat $TMP |wc -l`

set YMAX = `minmax -C histo.tmp |awk '{print $4*1.2}'`
set YNUM = ` echo $YMAX | awk '{print int(0.2*$1) }' `
set YTICK = ` echo $YNUM | awk '{print int(1.0*$1 / 2.0) }' `

pshistogram $TMP -R${XMIN}/${XMAX}/0/$YMAX -Ba${XNUM}f${XINC}:"${XLABEL}":/a${YNUM}f${YTICK}:"${YLABEL}":WS -JX5.0i/1.2i -W$XINC -L0.5p -G50/50/250   -Y-1.2i   -O -K <<EOF>> $OUT_tmp
EOF
# =========================================================================
#		
#		Add the histogram info for misfit
#
# =========================================================================
set INFILE = $misfit_file
set XMIN = 0
set XMAX = 1.5
set XINC = 0.05 
set XNUM =  0.2


set TEXT = Misfit
set XLABEL = $TEXT 
set YLABEL = 'Frequency'
set TMP = histogram.tmp
awk '{ if($1 != "") print $1}' $INFILE > $TMP
set tmp = `minmax -C $TMP`


pshistogram  $TMP  -W$XINC -IO >! histo.tmp
set sta_num = `cat $TMP |wc -l`

set YMAX = `minmax -C histo.tmp |awk '{print $4*1.2}'`
set YNUM = ` echo $YMAX | awk '{print int(0.2*$1) }' `
set YTICK = ` echo $YNUM | awk '{print int(1.0*$1 / 2.0) }' `

pshistogram $TMP -R${XMIN}/${XMAX}/0/$YMAX -Ba${XNUM}f${XINC}:"${XLABEL}":/a${YNUM}f${YTICK}:"${YLABEL}":WS -JX5.0i/1.2i -W$XINC -L0.5p -G50/50/250   -Y-1.2i   -O -K <<EOF>> $OUT_tmp
EOF

psxy -JX -R -O  -N -P <<EOF>>$OUT_tmp
EOF

cat $OUT_tmp  >> $OUT
##cat $OUT_tmp >>$work_dir/${EQ}/dont_know_why.ps
	endif ## ES exist
end		# phase
##mv $work_dir/${EQ}/dont_know_why.ps $OUT

ps2pdf $OUT $OUT_pdf
rm $OUT
exit 0
