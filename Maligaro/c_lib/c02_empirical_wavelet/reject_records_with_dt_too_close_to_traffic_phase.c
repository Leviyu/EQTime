#include "ESF.h"


void reject_records_with_dt_too_close_to_traffic_phase(new_RECORD* my_record, new_INPUT* my_input)
{
	int count;

	double time_diff;
	for(count = 0; count < my_record->num_traffic; count++)
	{
		// if phase contrains SKS or SKKS, we skip
		if( strcmp( my_record->traffic_phase[count], "SKS") == 0 ||
			strcmp( my_record->traffic_phase[count], "SKKS") == 0 ||
			strcmp( my_record->traffic_phase[count], "sSKS") == 0 || 
			strcmp( my_record->traffic_phase[count], "sSKKS") == 0 )
				continue;
		if( strcmp( my_record->PHASE, "S") == 0 && 
				strcmp(my_record->traffic_phase[count],"ScS") == 0 )
			continue;
		if( strcmp( my_record->PHASE, "P") == 0 && 
				strcmp(my_record->traffic_phase[count],"PcP") == 0 )
			continue;

		if( strcmp( my_record->traffic_phase[count], my_record->depth_phase) == 0  )
			continue;

		time_diff = fabs( my_record->traffic_time[count] - my_record->dt_obs_prem );
		if( time_diff < my_record->traffic_range_sec)
			my_record->quality = -1;

	}




}
