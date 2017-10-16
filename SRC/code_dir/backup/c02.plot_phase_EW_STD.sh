#!/bin/csh
# ====================================================================
# This is a csh script for plotting the ES of chosen phase with their STD in a single
# plot, the goal is to find the good EQ and choose which phase to use by just looking at 
# this plot
# 
# DATE: Nov 7 2014			Keywords:
# ===================================================================

set EQ = $1
set DIR = $2
set SHELL_DIR = $3
set PLOTDIR = $4/${EQ}
set work_dir = $5
set C_DIR = $6
set DIR = $7
set OUT = $work_dir/c02.${EQ}_ES.ps
set OUT_pdf = $PLOTDIR/${EQ}_ES.pdf
set eventStation = $work_dir/${EQ}/eventStation.${EQ}
set PHASE_LIST = `cat $DIR/PHASE_LIST`

echo "---> Working on c02.plot_phase_EW_STD.sh "

cd $work_dir/$EQ
set TMP = `awk 'NR==2 {print $0}' $eventStation`
set EQ_mag = $TMP[16]
set EQ_lat = $TMP[11]
set EQ_lon = $TMP[12]
set EQ_dep = $TMP[13]

set DELTA = `grep -w "<DELTA>" $DIR/INFILE |awk '{print $2}'`


set INFILE = $DIR/INFILE_S
set velocity_or_displacement = `grep -w velocity_or_displacement $INFILE |awk 'NR==1 {print $2}'`


cat << EOF > ${OUT}
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

# prepare the plot
pstext -JX6i/1i -R0/10/0/10 -K -N -P -Y11i << EOF >> $OUT
5 0   13 0 0 CB Empirical Waveform Plot of EQ: $EQ in $velocity_or_displacement
5 -3  10 0 0 CB EQ_lat: $EQ_lat EQ_lon:$EQ_lon EQ_dep: $EQ_dep MAG: $EQ_mag delta:$DELTA
EOF
# ======================
#add explanation to some symbles
# #####################
psxy -JX -R -O -W/red  -K -Y-1i -N -P << EOF >> $OUT
1 1
1.5 1
EOF
pstext -JX -R -O -K -N -P << EOF >> $OUT
2 1 10 0 0 CB S E.W.
EOF
psxy -JX -R -O -W/green  -K  -N -P << EOF >> $OUT
3 1
3.5 1
EOF
##pstext -JX -R -O -K -N -P << EOF >> $OUT
##4 1 10 0 0 LB Hilbert(S E.W.) 
##EOF







set PHASE_NUM = 1
set PROJ = -JX3i/1i

foreach PHASE ( $PHASE_LIST )
	set flag = `grep -w filter_flag 	$DIR/INFILE_${PHASE} | awk '{print $2}'`
	set COMP = `grep -w COMP 			$DIR/INFILE_${PHASE} | awk '{print $2}'`
	set ES = $work_dir/${EQ}/${PHASE}_ES.out
	set STD = $work_dir/${EQ}/${PHASE}_STD.out
	set eventinfo = $work_dir/$EQ/eventinfo.${EQ}.${PHASE}.${COMP}
	set number_of_good_record = `awk '$14==1 {print $0}' $eventinfo |wc -l`

	if ( -e $ES) then	
	set tmp = `minmax -C $ES`
	set xmin = $tmp[1]
	set xmax = $tmp[2]
	set ymin = -1.2
	set ymax = 1.2
	set REG = -R$xmin/$xmax/$ymin/$ymax

	psxy $STD $PROJ $REG -Y-1.1i -O -L -Ba10f5S -G192/192/192 -K -P >>$OUT
	psxy $ES -JX -R -O -K -P >>$OUT


# add horizontal 0 line
	psxy -JX -R -O -Wfaint/204/169/228ta -K -P <<EOF>> $OUT
	$xmin 0 
	$xmax 0
EOF


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

	pstext -JX6i/1i -R0/1/0/1 -Y-0.3i -O -K -N -P << EOF >>$OUT
	0.1 -0.1 8 0 0 LB  Empirical Source of ${PHASE} for ${flag}  good records: $number_of_good_record
EOF
	endif

@ PHASE_NUM++

	if($PHASE_NUM == 7 ) then
	psxy -JX -R -X3.2i -Y6i -O -K -P << EOF>> $OUT
EOF
	endif

end		# phase


ps2pdf $OUT $OUT_pdf
rm $OUT

exit 0

