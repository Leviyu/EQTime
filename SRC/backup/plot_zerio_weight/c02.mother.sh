#!/bin/tcsh



set PWD = `pwd`
set event = $PWD/DATADIR/eventinfo.zeroweight
set log = $PWD/log
/bin/rm -rf $log



foreach EQ (`cat $event |awk '{print $12}'|sort|uniq`)
	foreach PHASE (S ScS SS SSS ScSScS Sdiff)
		set count = `cat $event |grep -w $EQ |grep -w $PHASE|wc -l`

		if( $count <= 0 ) then
			continue
		endif
		echo "--> on $EQ $PHASE $count"
		csh $PWD/c03.catalog_plot_of_ESF.sh $EQ $PHASE >> & $log
	end 
end 
