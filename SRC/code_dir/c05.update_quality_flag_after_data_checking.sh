#/bin/csh


# ====================================================================
# This is a csh script for updating the eventinfo after data checking manually
# 
# Input: 1. EQ
#		 2. PHASE
#		 3. data checking finished for phase
#
#
#
# Output: updated quality flag for each record
#
#
# DATE: Oct 2014					Keywords:
#
# Reference:
# ===================================================================



set EQ = $1
set PHASE = $2
set filter = $3
set PLOTDIR = $4
set DATADIR = $5
set COMP = $6


set pdf_file = $PLOTDIR/$EQ/${EQ}_${PHASE}_${COMP}_${filter}_catalog_all_record.pdf
set good_list = $DATADIR/$EQ/good_STA_list
pdftk $pdf_file dump_data_fields | grep Value|awk -F"_" '{print $2}' >$good_list

set eventinfo = $DATADIR/$EQ/eventinfo.${EQ}.${PHASE}.${COMP}
set TMP = $DATADIR/$EQ/eventinfo.tmp1
rm $TMP > & /dev/null
# =================================================================
#		Go through every STA and update quality flag
# =================================================================
foreach STA (`cat $eventinfo |awk '{print $1}'`)
		set flag = `grep -w $STA $good_list |awk '{print $1}'`
##echo $STA flag is $flag
		if($flag == "") then
		grep -w $STA $eventinfo |awk 'NR==1 {$14=0; print $0}'>> $TMP
		else
		grep -w $STA $eventinfo |awk 'NR==1 {$14=1; print $0}'>> $TMP
		endif
		end #foreach STA
cp $TMP $eventinfo


if ( $PHASE == "ScS" || $PHASE == "SS" || $PHASE == "SSS" || $PHASE == "ScSScS" ) then
set too_close = $DATADIR/${EQ}/too_close_phase_for_${PHASE}
if(! -e $too_close ) then
exit 0
endif
cat /dev/null > $TMP
		foreach STA (`cat $eventinfo |awk '{print $1}'`)
				set exist_flag = `grep -w $STA $too_close|awk 'NR==1 {print $1}'`
##echo "flag is $exist_flag"
				if(  $exist_flag == $STA ) then
				grep -w $STA $eventinfo |awk  'NR==1 {$14=0; print $0} ' >> $TMP
				else 
				grep -w $STA $eventinfo |awk  'NR==1 {print $0} ' >> $TMP
				endif
				end #foreach STA
cp $TMP $eventinfo
endif # PHASE


exit 0
