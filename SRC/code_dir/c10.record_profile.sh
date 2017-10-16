#!/bin/csh

# ==================================================
#	This Script plot the record profile of EQ
#	
#	Hongyu DATE: 
#	Key words: 
# ==================================================


# ====================================================
# 			define input parameter of shell 
# ====================================================
set EQ = $1
set DATADIR = $2
set PLOTDIR = $3
set C_DIR = $4
set SHELL_DIR = $5
set PHASE = $6
set COMP = $7
set filter_flag = $8
set DIR = $9

set work_dir = $DATADIR/$EQ
set SRCDIR = $SHELL_DIR/ESF_C_S

set input = $work_dir/input
set ESinfo = $work_dir/eventinfo.${EQ}.${PHASE}.${COMP}.${filter_flag}

set DISTMIN = `grep -w DIST_MIN_MAX $DIR/INFILE_${PHASE} | awk 'NR==1 {print $2}'`
set DISTMAX = `grep -w DIST_MIN_MAX $DIR/INFILE_${PHASE} | awk 'NR==1 {print $3}'`
if($DISTMIN == "non") then
set DISTMIN = 100
endif
if($DISTMAX == "non") then
set DISTMAX = 100
endif

set OUTFILE = $PLOTDIR/Record_profile.${EQ}.${PHASE}.${COMP}.ps
set OUTFILE_pdf = $PLOTDIR/Record_profile.${EQ}.${PHASE}.${COMP}.pdf


# gmt setting and Page Setting 
gmtset TICK_PEN = 0.2p
gmtset ANNOT_FONT_SIZE_PRIMARY = 5p
gmtset FRAME_PEN = 0.4p
gmtset ANNOT_OFFSET_PRIMARY = 0.1c
gmtset PAPER_MEDIA = A4

# add header info 
pstext -JX6i/1i -R0/10/0/10 -K -N -P -Y10.5i << EOF > $OUTFILE
5 5 15 0 0 CB Record Profilf of $EQ for PHASE: $PHASE COMP: $COMP
EOF

psxy -JX -R -O -Y-8i -K -N -P << EOF >> $OUTFILE
EOF


set record_num = `cat $ESinfo |wc -l`
foreach sta (`cat $ESinfo |awk '{print $1}'`)
set long = $work_dir/${EQ}.${sta}.${PHASE}.${COMP}.long
##set emp = $work_dir/${EQ}.${sta}.${PHASE}.${COMP}.emp
##set orig_emp = $work_dir/${EQ}.${sta}.${PHASE}.${COMP}.orig.emp
##set CMT_polar_prediction_file = $work_dir/eventinfo.polarity.${PHASE}.${COMP}
##set CMT_polar_tmp  = `grep -w $sta $CMT_polar_prediction_file |awk 'NR==1 {print $2}'` 
##set CMT_polar_prediction = `printf "%.2f" $CMT_polar_tmp`

if( ! -e $long ) then
echo "$long does not exist! ========"
continue
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
set prem = $infoES[15]
set phase_amplitude = $infoES[16]
set ccc = $infoES[17]
set SNR = $infoES[18]
set dt_obs_prem = $infoES[19]
##echo "STA $STA dt_obs_prem is $dt_obs_prem"
set best_stretch_ccc = $infoES[21]
set best_stretch_coeff = $infoES[22]
set misfit = $infoES[23]

set NOISE_BEG = $infoES[27]
set NOISE_LEN = $infoES[28]
set NOISE_END = `echo "$NOISE_BEG + $NOISE_LEN"|bc -l`

set record_weight = $infoES[30]

set ONSET = $infoES[33]
set ENDSET = $infoES[34]

if($PHASE == "ScS") then
awk '{if($1>-70 && $1 < 70) print $0}' $long > $work_dir/long_tmp
mv $work_dir/long_tmp $long
endif

set xy_tmp = `minmax -C $long`
set xmin = $xy_tmp[1]
set xmax = $xy_tmp[2]
set ymin = $xy_tmp[3]
set ymax = $xy_tmp[4]

# shift the record relative to the distance
set long_tmp = $work_dir/long_tmp.data
rm $long_tmp > & /dev/null
awk -v dist=$DIST '{print $1,$2+dist}' $long > $long_tmp


	# ================== plot the trace =====================
		set long_color = 0/0/0
		set long_color_flag = `echo ddd |awk '{if('$CMT_polar_prediction' > -0.3 && '$CMT_polar_prediction' < 0.3 ) print green}'`
		if($long_color_flag == "green" ) then
		set long_color = 0/255/0
		endif
		set RANGE = -R$xmin/$xmax/$DISTMIN/$DISTMAX
		set FRAME = -JX5i/8i
		psxy $long_tmp $FRAME  $RANGE -W/$long_color  -O -K -N -P >>$OUTFILE


end 


ps2pdf $OUTFILE $OUTFILE+pdf
rm $OUTFILE
