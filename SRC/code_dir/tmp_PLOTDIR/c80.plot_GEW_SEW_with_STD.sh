#!/bin/tcsh


set PWD = `pwd`
set KK = p1plot3

set BIGDIR = /DATA1/EQTime/DATADIR
cd $BIGDIR
foreach WORKDIR (`ls ${KK}* -d`)
	echo $WORKDIR
	cd $BIGDIR/$WORKDIR
	pwd

foreach EQ (`ls 20* -d`)

	echo "--> on $EQ"



set out = $PWD/tmp_PLOTDIR/$EQ.ps
set out_pdf = $PWD/tmp_PLOTDIR/$EQ.pdf




cd /DATA1/EQTime/DATADIR/$WORKDIR/$EQ

set EQ_lat = `check_event $EQ |grep EQ_LAT_ISC|awk '{print $2}'`
set EQ_lon = `check_event $EQ |grep EQ_LON_ISC|awk '{print $2}'`
set EQ_dep = `check_event $EQ |grep EQ_DEP_ISC|awk '{print $2}'`

pstext -JX6i/1i -R0/1/0/1 -K -N -P -Y9i << EOF >! $out
0 0 10 0 0 LB $EQ LAT/LON/DEP: $EQ_lat $EQ_lon $EQ_dep
EOF
set SHELL_DIR = $PWD/code_dir
# ================== add a sphere global view of EQ--sta geometry =================
##set MID_lat = `awk 'NR==1 {print $1}' MID`
##set MID_lon = `awk 'NR==1 {print $2}' MID`
# add the small global map showing the gcp of this cross-section
set MAP = -R0/360/-90/90
set PROJ = -JG${EQ_lon}/${EQ_lat}/1.5i
set land = "255/225/160"
set sea = 103/204/1
pscoast $MAP $PROJ -Dc -A400000 -B90g45 -W2 -G$land    -O -K -P -Y-1i >>$out
# add EQ and STA location
psxy $MAP $PROJ -: -Sa1.0	-W/"red" -Gred -O -P -K << EOF>>$out
$EQ_lat $EQ_lon
EOF



#END ================== add a sphere global view of EQ--sta geometry =================


foreach PHASE ( S SS SSS Sdiff ScS ScSScS )

	set GEW = ${PHASE}_ES.first_iteration.out
	set GEW_STD = ${PHASE}_STD.first_iteration.out
	set SEW = ${PHASE}_ES.third.out
	set SEW_STD = ${PHASE}_STD.third.out

set PROJ = -JX2i/1.2i
set REG = -R0/40/-1.8/1.8
set num = `cat eventinfo.${EQ}.${PHASE}.T |wc -l`

	psxy $GEW_STD $PROJ $REG  -O -K -N -P -Ggrey -Y-1i << EOF >> $out
EOF

	psxy $GEW $PROJ $REG -O -K -N -P  << EOF >> $out
EOF

	psxy $SEW_STD $PROJ $REG -O -K -N -P -Ggrey -X3i << EOF >> $out
EOF

	psxy $SEW $PROJ $REG -O -K -N -P  << EOF >> $out
EOF
psxy $PROJ $REG -O -K -X-3i << EOF >> $out
EOF

pstext -JX2i/1i -R0/1/0/1 -O -K -N << EOF >>$out
0 0 10 0 0 LB ${PHASE} $num
EOF


end 



ps2pdf $out $out_pdf
rm $out

#to_hongyu $out_pdf


end


end # WORKDIR 



