#!/bin/csh
# EJG/PL March 2010
# Hongyu Aug. 2014
#===========================================================
#                 c02.TraveltimeCurve with clean face
#===========================================================
# Here we make travel time curves made by the taup_toolkit.
#
# the f90 code was written by Pei-ying (Patty) Lin at
# ASU. any errors are undoubtedly mine, as I modified things.

# INPUT:  (1) station/event location info and distances from
# -----       master info file
#         (2) plot box bounds: Dist min/max, time min/max
#         (3) some directory location information

# OUTPUT:  (1) a postscript plot for SV
# -----    (2) a postscript plot for SH
#          (3) a postscript plot for Z

set EZ 				= 100
set DISTMIN         = 0
set DISTMAX         = 180
set PHASE           = SS
set COMP            = T
set TIMEMIN         = -1000
set TIMEMAX         = 1000




set DIRSRC          = /home/hongyu_ubuntu/class/Travel_time_plot
set DIRDATA         = $DIRSRC
set DIRPLOT         = /home/hongyu_ubuntu/PLOTDIR/TC01/travel_time

set MODEL     =   prem
set Pgroup    =  ( P PcP PP PKP PKIKP PKiKP Pdiff PKKP PPP )
set pPgroup   =  ( pP pPdiff )
set Sgroup    =  ( S Sdiff SS SSS SSSS  ScSScS)
set SVgroup   =  ( SKS SKKS SKKKS )

##set SHgroup   =  ( ScS ScSScS ScSScSScS ScSScSScSScS ScSScSScSScSScS ScSScSScSScSScSScS ScSScSScSScSScSScSScS ) 
set SHgroup   =  ( ScS ScSScS ScSScSScS )
set sSgroup   =  ( sS sSdiff sSS sSSS  )
set sSVgroup   = ( sSKS sSKKS )
set sSHgroup   = ( sSdiff sScS sScSScS sScSScSScS sScSScSScSScS sScSScSScSScSScS sScSScSScSScSScSScS sScSScSScSScSScSScSScS )

set sPgroup   =  ( sP sPdiff )
set pSgroup   =  ( pS pSS pSdiff pSKS pSKKS )
set SPgroup   =  ( ScP SP PS PcS SKP PKS )
set pSPgroup  =  ( pScP pPS )

# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! HERE
##set Phaselist = (sScS ScSScS  sSS sS )
set Phaselist = ( $Sgroup $sSgroup $SHgroup )
set flag_textphase = y
set PHASECOLOR = red
set wTYPE = 1.0p/"$PHASECOLOR"

# go to the data directory
set DIRPWD = ` pwd `

/bin/rm -rf  $DIRDATA/TC
mkdir $DIRDATA/TC
cd $DIRDATA/TC
# check to see if script is to compute distance bounds
# for plot.  If DISTMIN = -1, code does it.  If yes,
# code will add +/- 10% of distance range of data
set CHECK = ` echo $DISTMIN | awk ' $1<0 { print "Y"}' `
if($CHECK == Y ) then
   set MINMAX = ( ` awk '{print $3}' IINFIL | minmax -C ` )
   set DISTMIN = ` echo $MINMAX | awk '{print int($1 - 0.1*($2 - $1))}' `
   set DISTMAX = ` echo $MINMAX | awk '{print int($2 + 0.1*($2 - $1))}' `
endif

# make lists of seismograms, for plot panels: a separate
# list for each plot to be made


#===========================================================
#             PLOTTING TIME!!!
#===========================================================

# here we make travel time curves with taup
# here, we make them in groups, for different color coding in the
# profile plots that follow. note: the plot gets pretty busy
# pretty quickly, so i don't have all possibilities being plotted
echo "c98: computing travel time curves"


#set Pgroup = ( )
#set pPgroup = ( )
#set Sgroup = ( )
#set SVgroup = ( )
#set SHgroup = ( ScSScSScSScSScSScSScSScS )
#set sSHgroup = ( sScSScSScSScSScSScSScSScS )
#set sSgroup = ( )
#set sSVgroup = ( )
#set sPgroup   =  ( )
#set pSgroup   =  ( )
#set SPgroup   =  ( )
#set pSPgroup  =  ( ) 

# compile code:
f95 $DIRSRC/taupTC_text.f90 -o taupTC_text
f95 $DIRSRC/random_num.f90 -o random_num


# plot size (the f90 sac2xy code assumes these are the dimensions
# we are working with)
set XSIZE = 6
set YSIZE = 8.5 
set ddist0 = `echo $DISTMAX $DISTMIN | awk '{print 0.03*($1-$2)}' `

# crunch some axis attributes for plot:
# well put dist on horiz axis, and time on vertical
set BX    = `echo $TIMEMIN $TIMEMAX | awk '{print $2-$1}' | awk '{ if ( $1/100 >= 20 ) {print 500,100} else if ( $1/100 < 10 ) {print 100, 10} else {print 200, 20} }'`

set BY    = `echo $DISTMIN $DISTMAX | awk '{print (int(int(($2-$1)/10)/5)+1)*5 }' |  awk '{print $1, $1/5}'`
set SCALE = X"$XSIZE"i/-"$YSIZE"i
set RANGE = $TIMEMIN/$TIMEMAX/$DISTMIN/$DISTMAX/
gmtset GRID_PEN_PRIMARY  0.25p,200/200/200
set BAXIS = a"$BX[1]"f"$BX[2]"g"$BX[2]"/a"$BY[1]"f"$BY[2]"g"$BY[2]"
set Y0 = -Y1.5i

# define the output file
if ( $COMP == "ALL" ) then
    set NCOMP = 0
else if ( $COMP == "Z" ) then
    set NCOMP = 1
else if ( $COMP == "R" ) then
    set NCOMP = 2
else if ( $COMP == "T" ) then
    set NCOMP = 3
endif

set iEZ = `echo $EZ | awk '{printf "%d\n",$1}'`
set OUTFILE = $DIRPLOT/TraveltimeCurve_clean.$PHASE.$COMP.$EZ.ps
set OUTFILE_pdf = $DIRPLOT/TraveltimeCurve_clean.$PHASE.$COMP.$EZ.pdf
   
# plot some text
pstext -Jx1i -R0/6/0/9 -K -N -P $Y0 << END >! $OUTFILE
3.0 9.3 15  0 0 CB Travel Time Curve Source Depth $EZ PHASE $PHASE Comp: $COMP
-0.5 4.5 15 90 0 CB Distance (deg)
3.0 -0.7 15  0 0 CB Time after PREM ${PHASE}-wave time (sec)
END

# initiate coords for traces and curves
psbasemap -J$SCALE -R$RANGE -B"$BAXIS"WSne -K -O -P >> $OUTFILE
psxy -JX -R -W0.5p/0/0/255t10_5:0  -O -K <<EOF>>$OUTFILE
0 $DISTMIN
0 $DISTMAX
EOF
# plot travel time curves
cat << EOF >! pstext.temp
EOF
set textDlist = "0"


set iphase = 1 
while ( $iphase <= $#Phaselist )
set phasename = $Phaselist[$iphase]

taup_curve -mod $MODEL -h $EZ -ph $phasename -rel $PHASE
set nline_taup = `wc -l < taup_curve.gmt `

if ($nline_taup >= 2 ) then
 if ( $flag_textphase == "y" ) then  
   set drange_TC = `cat taup_curve.gmt | awk 'NR > 1 {print $1}' | minmax -C`
   set drange_TC_TEXT = `echo $drange_TC $DISTMIN $DISTMAX |  tr ' ' '\n' | sort -n | tr '\n' ' ' | awk '{print $2, $3}'`
   set ddist_PLOT = `echo $DISTMAX  $DISTMIN | awk '{printf "%d \n", $1-$2}'`
   if ( $ddist_PLOT >= 90 ) then 
   if ( $phasename == "P" || $phasename == "S" ||  $phasename == "ScS" || $phasename == "ScSScS" ||  $phasename == "ScP" ||  $phasename == "SKS" ) then
       set drange_TC_TEXT =  `echo $drange_TC_TEXT | awk '{ print $1, $1+5/100*($2-$1)}'`
   else if ( $phasename == "pP" || $phasename == "pS" ) then
       set drange_TC_TEXT =  `echo $drange_TC_TEXT | awk '{ print $1+20/100*($2-$1), $2}'`
   else if ( $phasename == "SS" || $phasename == "SSS" || $phasename == "SSS" ||  $phasename == "SKKS" ||  $phasename == "SKKKS"  ) then
       set drange_TC_TEXT =  `echo $drange_TC_TEXT | awk '{ print $2-20/100*($2-$1), $2}'`
   endif
   endif
   set ddist = `echo $drange_TC_TEXT[1] $drange_TC_TEXT[2] $DISTMIN $DISTMAX $ddist0 | awk '{if ( ($2-$1)/($4-$3) < 0.3 ) {print $5/2} else {print $5}}'` 

   # in linux
   #set randomnum =  `echo | awk '{ srand(systime()); print rand()}'`
   # in MAC
   set randomnum = `random_num`
   # ------------------------------------
   set textD = `echo $drange_TC_TEXT[1] $drange_TC_TEXT[2]  $randomnum | awk '{print $3*($2-$1)+$1}' `
   set trange_TC = `cat taup_curve.gmt | awk 'NR > 1 {print $2}' | minmax -C`
   set trange_TC_TEXT = `echo $trange_TC $TIMEMIN $TIMEMAX |  tr ' ' '\n' | sort -n | tr '\n' ' ' | awk '{print $2, $3}'`
   set textDlist = `echo $textDlist |  tr ' ' '\n' | sort -n | tr '\n' ' '  `
   set logic_recal = `echo $textD $textDlist | awk '{  for (i = 2; i <= NF ; ++i) if ( ($1 - $i) <= '$ddist') {print "y"} }' | head -1 `

   set ii = 1
   while ( $logic_recal == "y"  ) 
       #set randomnum =  `echo | awk '{ srand(systime()*'$ii'); print rand()}'`
       set randomnum = `random_num`
       set textD = `echo $drange_TC_TEXT[1] $drange_TC_TEXT[2]  $randomnum | awk '{print $3*($2-$1)+$1}'`
       set logic_recal = `echo $textD $textDlist | awk ' function abs(x){return (((x < 0.0) ? -x : x) + 0.0)} {for (i = 2; i <= NF ; ++i) if ( abs($1 - $i) <= '$ddist') {print "y"} }' | head -1 ` 

       if ( $ii >= 10 ) then
           set ddist = `echo $ddist | awk '{print $1/2}'`
           set logic_recal = "n"
       endif


@ ii = $ii + 1 

end #end while 



    set textDlist = `echo $textDlist $textD `

    taupTC_text << ! >! textgmt.out
    taup_curve.gmt
    $textD
    $trange_TC_TEXT[1] $trange_TC_TEXT[2]
!
    cat textgmt.out | head -1 | awk '{ print "echo ",$2,$1, "8 0 22 LM '$phasename' | pstext -JX -R -G'$PHASECOLOR' -Sthicker/255 -K -O -N >> '$OUTFILE'"  }' >> pstext.temp
  endif
    if ( $phasename == "SSS" || $phasename == "PPP" ||  $phasename == "sSSS" || $phasename == "SSSS" || $phasename == "SS" || $phasename == "PP") then
       set wTYPE = `echo $wTYPE | awk -Fp/ '{ print $1/2"p/"$2}'`
    endif


    psxy taup_curve.gmt  -JX -R  -: -W$wTYPE -m -K -O >> $OUTFILE

endif
@ iphase ++
end



sort -k 2 -nr  pstext.temp | csh


pstext -JX -R -O  << ! >> $OUTFILE
!
ps2pdf $OUTFILE $OUTFILE_pdf
rm $OUTFILE
# clean up
\rm pstext.temp taup_curve.gmt textgmt.out
\rm taupTC_text
/bin/rm -rf  $DIRDATA/TC
cd $DIRSRC

