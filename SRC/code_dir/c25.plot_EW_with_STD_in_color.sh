#!/bin/csh
# ====================================================================
# This is a csh script to plot every record in format of x y
# then xy2grd the file and plot the summary file in colors
# 
# 
# DATE: Nov 7 2014			Keywords:
# ===================================================================

set EQ = $1
set DIR = $2
set SHELL_DIR = $3
set PLOTDIR = $4/${EQ}
set DATADIR = $5
set C_DIR = $6
set OUT = $PLOTDIR/${EQ}_ES_STD_with_color.ps
set OUT_pdf = $PLOTDIR/${EQ}_ES_STD_with_color.pdf
set eventStation = $DATADIR/${EQ}/eventStation.${EQ}

set work_dir = $DATADIR/$EQ
set PHASE_LIST = `cat $work_dir/INFILE|grep -w PHASE_LIST | awk 'NR==1 {print $0}'|cut  -f2-`

echo "--> working on c25 plot EW_STD with color"
## add psscale
cd $work_dir/
gmtset COLOR_BACKGROUND = white
gmtset ANNOT_FONT_SIZE_SECONDARY = 7p
gmtset LABEL_FONT_SIZE = 7p
gmtset COLOR_FOREGROUND = white
##gmtset COLOR_BACKGROUND = 255/0/0
gmtset COLOR_NAN = white

set TMP = `awk 'NR==2 {print $0}' $eventStation`
set EQ_mag = $TMP[16]
set EQ_lat = $TMP[11]
set EQ_lon = $TMP[12]
set EQ_dep = $TMP[13]

set INFILE = $work_dir/INFILE_S
set velocity_or_displacement = `grep -w velocity_or_displacement $INFILE |awk 'NR==1 {print $2}'`
set DELTA = `grep -w "<DELTA>" $work_dir/INFILE |awk '{print $2}'`


cat << EOF >! ${OUT}
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

foreach PHASE ( `echo $PHASE_LIST`)

pstext -JX6i/1i -R0/10/0/10 -K -N  -Y7.6i << EOF >> $OUT
5 0   13 0 0 CB Empirical Waveform Plot of EQ: $EQ in $velocity_or_displacement PHASE: $PHASE
5 -3  10 0 0 CB EQ_lat: $EQ_lat EQ_lon:$EQ_lon EQ_dep: $EQ_dep MAG: $EQ_mag delta:$DELTA
EOF
# ======================
# add explanation to some symbles
# ======================
psxy -JX -R -O -W/red  -K -Y-0.5i -N  << EOF >> $OUT
EOF
#1 1
#1.5 1
#EOF
##pstext -JX -R -O -K -N  << EOF >> $OUT
##2 1 10 0 0 CB S E.W.
##EOF
psxy -JX -R -O -W/green  -K  -N  << EOF >> $OUT
EOF
#3 1
#3.5 1
#EOF
##pstext -JX -R -O -K -N  << EOF >> $OUT
##4 1 10 0 0 LB Hilbert(S E.W.) 
##EOF
# ======================
#add explanation to some symbles
# ======================






set ymin = -1.2
set ymax = 1.2
	echo "--> Working on $PHASE"
	set flag = `grep -w filter_flag 	$work_dir/INFILE_${PHASE} | awk '{print $2}'`
	set COMP = `grep -w COMP 			$work_dir/INFILE_${PHASE} | awk '{print $2}'`
	set eventinfo = $work_dir/eventinfo.${EQ}.${PHASE}.${COMP}


	set first_ES = $work_dir/${PHASE}_ES.first.out
	set first_STD = $work_dir/${PHASE}_STD.first.out
	set tmp = `minmax -C $first_ES`
	set xmin = $tmp[1]
	set xmax = $tmp[2]
## STD_GRD
	set REG = -R$xmin/$xmax/$ymin/$ymax
	set PROJ = -JX4i/1i
	set STD_grd_space = 0.25
	set STD_grd_space2 = 0.02

## plot the first EW with STD
## plot the first EW with STD
## plot the first EW with STD
## plot the first EW with STD
## plot the first EW with STD
## plot the first EW with STD
	psxy $first_STD $PROJ $REG  -G192/192/192 -L  -O -K -Ba10f5/a1f0.5SW -Y-1.2i  >>$OUT
	psxy $first_ES  $PROJ $REG  -O -K -Ba10f5/a1f0.5SW   >>$OUT
	psxy -JX -R -O -Wfaint/204/169/228ta -K  <<EOF>> $OUT
	$xmin 0 
	$xmax 0
EOF
	set number_of_good_record = `awk '$14!=-1&&$30!=0 {print $0}' $eventinfo |wc -l`

	set first_EW_flag = 		"Iteratively stacked E.W. with STD (all records weight > 0) "
	set first_EW_flag_color = 	"Iteratively stacked E.W. with STD (all records weight > 0) "

	pstext -JX -R -N -O -K << EOF >> $OUT
$xmax -2 8 0 0 RB $first_EW_flag
EOF
## plot first EW with stretched record incolor 
	set STD_xy = $work_dir/${PHASE}_STD.out.first.xy
	set STD_grd = $work_dir/${PHASE}_STD.out.xy.grd
	set CPT = $work_dir/${PHASE}_STD.cpt
	set CPT_MAX = `echo " $number_of_good_record / 1"|bc -l`
	set CPT_INC = `echo " $CPT_MAX / 5"|bc -l`
	makecpt -Cseis  -T1/${CPT_MAX}/$CPT_INC  -I -Z> $CPT
#//echo "makecpt -Cseis  -T1/${CPT_MAX}/$CPT_INC  -I -Z> $CPT"

	xyz2grd  $STD_xy -G$STD_grd -I$STD_grd_space/$STD_grd_space2 $REG -An 
	psxy $PROJ $REG  -O -K -Ba10f5/a1f0.5SW -X5i << EOF >>$OUT
EOF
grdimage $STD_grd -C$CPT $REG -JX  -E300 -N -O -K   >>$OUT

	psxy $PROJ $REG -N -O -K -Ba10f5/a1f0.5SW   <<EOF>>$OUT
EOF
	pstext -N -JX -R -O -K << EOF >> $OUT
$xmax -2 8 0 0 RB $first_EW_flag_color
EOF

	pstext -JX -R -O -K -X-5i << EOF >>$OUT
EOF
psscale -L -N -C$CPT -D8i/1.5i/2i/0.1ih   -O -K  -N300 >>$OUT
	
## plot the second EW with STD
## plot the second EW with STD
## plot the second EW with STD
## plot the second EW with STD
## plot the second EW with STD
## plot the second EW with STD
## plot the second EW with STD
	set second_ES = $work_dir/${PHASE}_ES.second.out
	set second_STD = $work_dir/${PHASE}_STD.second.out
	psxy $second_STD $PROJ $REG -G192/192/192 -L  -O -K -Ba10f5/a1f0.5SW -Y-1.6i   >>$OUT
	psxy $second_ES  $PROJ $REG  -O -K -Ba10f5/a1f0.5SW    >>$OUT
	psxy -JX -R -O -Wfaint/204/169/228ta -K  <<EOF>> $OUT
	$xmin 0 
	$xmax 0
EOF
	set number_of_good_record = `awk '$14!=-1&&$30!=0 {print $0}' $eventinfo |wc -l`
	set second_EW_flag = 		"Stretched Records stacked E.W. with STD (all records weight > 0) "
	set second_EW_flag_color = 	"Stretched Records stacked E.W. with STD (all records weight > 0) "
	pstext -JX -N -R -O -K << EOF >> $OUT
$xmax -2 8 0 0 RB $second_EW_flag
EOF
## plot second EW with stretched record incolor 
	set STD_xy = $work_dir/${PHASE}_STD.out.second.xy
	set STD_grd = $work_dir/${PHASE}_STD.out.xy.grd
	set CPT = $work_dir/${PHASE}_STD.cpt
	set CPT_MAX = `echo " $number_of_good_record / 1"|bc -l`
	set CPT_INC = `echo " $CPT_MAX / 5"|bc -l`
	makecpt -Cseis  -T1/${CPT_MAX}/$CPT_INC  -I -Z> $CPT
	xyz2grd  $STD_xy -G$STD_grd -I$STD_grd_space/$STD_grd_space2 $REG -An 
	psxy $PROJ $REG  -O -K -Ba10f5/a1f0.5SW -X5i << EOF >>$OUT
EOF
grdimage $STD_grd -C$CPT $REG -JX  -E300 -N -O -K   >>$OUT

	psxy $PROJ $REG -N -O -K -Ba10f5/a1f0.5SW   <<EOF>>$OUT
EOF
	pstext -N -JX -R -O -K << EOF >> $OUT
$xmax -2 8 0 0 RB $second_EW_flag_color
EOF

	pstext -JX -R -O -K -X-5i << EOF >>$OUT
EOF
psscale -L -C$CPT -D8i/-0.5i/2i/0.1ih  -O -N -K  -N300 >>$OUT

## plot the third EW with STD
## plot the third EW with STD
## plot the third EW with STD
## plot the third EW with STD
## plot the third EW with STD
	set third_ES = $work_dir/${PHASE}_ES.third.out
	set third_STD = $work_dir/${PHASE}_STD.third.out
psxy $third_STD $PROJ $REG -G192/192/192 -L  -O -K -Ba10f5:"Time (sec)":/a1f0.5SW -Y-1.4i   >>$OUT
psxy $third_ES  $PROJ $REG  -O -K -Ba10f5:"Time (sec)":/a1f0.5SW    >>$OUT
	psxy -JX -R -O -Wfaint/204/169/228ta -K  <<EOF>> $OUT
	$xmin 0 
	$xmax 0
EOF
	set number_of_good_record = `awk '$14==1{print $0}' $eventinfo |wc -l`
	set third_EW_flag = 		"Stretched Records stacked E.W. with STD (just for good records) "
	set third_EW_flag_color = 	"Stretched Records stacked E.W. with STD (just for good records) "
	pstext -JX -N -R -O -K << EOF >> $OUT 
$xmax -2 8 0 0 RB  $third_EW_flag
EOF
## plot thirdEW with stretched record incolor 
	set STD_xy = $work_dir/${PHASE}_STD.out.third.xy
	set STD_grd = $work_dir/${PHASE}_STD.out.xy.grd
	set CPT = $work_dir/${PHASE}_STD.cpt
	set CPT_MAX = `echo " $number_of_good_record / 1"|bc -l`
	set CPT_INC = `echo " $CPT_MAX / 5"|bc -l`
	makecpt -Cseis  -T1/${CPT_MAX}/$CPT_INC  -I -Z> $CPT
	xyz2grd  $STD_xy -G$STD_grd -I$STD_grd_space/$STD_grd_space2 $REG -An 
	psxy $PROJ $REG  -O -K -Ba10f5:"Time (sec)":/a1f0.5SW -X5i << EOF >>$OUT
EOF
grdimage $STD_grd -C$CPT $REG -JX  -E300 -N -O -K   >>$OUT

	psxy $PROJ $REG -N -O -K -Ba10f5:"Time (sec)":/a1f0.5SW   <<EOF>>$OUT
EOF
	pstext -N -JX -R -O -K << EOF >> $OUT
$xmax -2 8 0 0 RB $third_EW_flag_color
EOF

	pstext -JX -R -O -K -X-5i << EOF >>$OUT
EOF



	psxy -JX -R -O -Wfaint/204/169/228ta -K  <<EOF>> $OUT
	$xmin 0 
	$xmax 0
EOF

psscale  -C$CPT -D8i/-0.5i/2i/0.1ih -B:"num of record in each cell ( 0.25 * 0.02)":  -O -K  -N300 >>$OUT

		# add pdfmarker
	cat  << EOF >> $OUT
	[ /Rect [-200 0 -20 200] % — position
	/T (${EQ}_${PHASE}) % — name
	/FT /Btn
	/F 4 /H /O
	/MK << /CA (8) /BC [ 0 ] /BG [ 1 ] >>  % Colors
	/BS << /W 1 /S /S >>
	/DA (/ZaDb 0 Tf 1 0 0 rg) % — size and colors
	/AP << /N << /${EQ}_${PHASE} /null >> >> % — checkbox value
	/Subtype /Widget
	/ANN pdfmark
EOF

	pstext -JX6i/1i -R0/1/0/1 -Y-1.4i -O  -N  << EOF >>$OUT
	0.1 0.1 8 0 0 LB  Empirical Source of ${PHASE} for ${flag}  good records: $number_of_good_record
	0.1 -0.3 8 0 0 LB  All records are stretched to match E.W. of S
EOF
end		# phase


ps2pdf $OUT $OUT_pdf
rm $OUT

exit 0

