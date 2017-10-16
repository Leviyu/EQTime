#!/bin/csh
# if writing as SAC output, uncomment this line, and
# the right I/O line in the code:
##gfortran mk_tstar_cz.f /usr/local/sac/lib/sacio.a

##gfortran mk_tstar_cz.f 
gfortran mk_tstar.f 
set NPTS = $1
set DELTA = $2
set TSTAR = $3


set TSTAR = `echo $TSTAR| awk '{printf "%.2f", $1}'`


set out = `echo $TSTAR |awk '{printf "TSTAR_%.2f_ASCII",$1 }'`
if($TSTAR == 0 ) then
set out = TSTAR_0.00_ASCII
endif

##echo "working on TSTAR $TSTAR"
##a.out << END >! $out
a.out << END >! $out
TSTAR_${TSTAR}
$NPTS
$DELTA
$TSTAR
END
