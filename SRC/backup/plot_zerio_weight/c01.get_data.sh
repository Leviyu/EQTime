# 1. get all the zero weight eventinfo
set PWD = `pwd`
cd $PWD/DATADIR
set eventinfo = $PWD/DATADIR/eventinfo.zeroweight
set tmpevent = $PWD/DATADIR/tmp.event
cd /DATA1/EQTime/DATADIR

cat POST2*/1*/eventinfo.1*.T >! $eventinfo
cat POST2*/2*/eventinfo.2*.T >> $eventinfo

cat $eventinfo |awk '$30==0 {print $0}' >! $tmpevent
mv $tmpevent $eventinfo
exit 0
