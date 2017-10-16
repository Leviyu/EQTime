#!/bin/tcsh
# This script makes big taup_file for each EQ-STA including all possible phases
# INPUT:
# 	EQ
# 	STA
# 	eventStation
# OUTPUT:
# 	big_taup time file named after taup_time.${EQ}.${STA} under current
# 	directory

set EQ = $1
set STA = $2
set eventStation = $3
set file_directory = $4

set TMP = `grep -w $STA $eventStation|awk 'NR==1 {print $0}'`
set eq_lat = $TMP[11]
set eq_lon = $TMP[12]
set eq_dep = $TMP[13]
set sta_lat = $TMP[9]
set sta_lon = $TMP[10]


set taup_time_file = $file_directory/taup_time.${EQ}.${STA}
set common_S_list = S,ScS,SS,SSS,Sdiff,ScSScS
set multi_S_phase = SSSS,SSSSS,SSSSSS
set multi_ScS_phase = ScSScSScS,ScSScSScSScS,ScSScSScSScSScS

set s_common_S_list = sS,sScS,sSS,sSSS,sSdiff,sScSScS
set s_multi_S_phase = sSSSS,sSSSSS,sSSSSSS
set s_multi_ScS_phase = sScSScSScS,sScSScSScSScS,sScSScSScSScSScS
set SKS_group = SKS,SKKS,sSKS,sSKKS

set P_list = P,PcP,Pdiff
set pP_list = pP,pPcP,pPdiff

set traffic_list = ${common_S_list},${s_common_S_list},${multi_S_phase},${s_multi_S_phase},${s_multi_ScS_phase},${s_multi_ScS_phase},${P_list},${pP_list},${SKS_group}

## if taup_file for current main_PHASE does not exist, create it
if(! -e $taup_time_file ) then
	taup_time -mod prem -ph $traffic_list -h $eq_dep -sta $sta_lat $sta_lon -evt $eq_lat $eq_lon >> $taup_time_file
endif
