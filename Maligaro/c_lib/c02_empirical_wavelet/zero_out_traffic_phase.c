#include "ESF.h"

// this is a function to construct long window with traffic phase padded with zero
int zero_out_traffic_phase( new_RECORD* my_record, new_INPUT* my_input)
{
	//printf("zero out traffic %s \n", my_record->name);
	int count;

	for(count = 0; count < my_record->num_traffic ; count++)
	{

		// if is depth_phase and is close to main phase then 15 second
		// we use 15sec cause we dont have any estimation of one Period here
		// yet
		if( strcmp( my_record->traffic_phase[count], my_record->depth_phase) == 0)
			if( fabs( my_record->traffic_time[count]) < 15  )
				continue;

		// if phase contrains SKS or SKKS, we skip
		if( strcmp( my_record->traffic_phase[count], "SKS") == 0 ||
			strcmp( my_record->traffic_phase[count], "SKKS") == 0 ||
			strcmp( my_record->traffic_phase[count], "sSKS") == 0 || 
			strcmp( my_record->traffic_phase[count], "sSKKS") == 0 )
				continue;

		// for Phase S, if dist > 80 then we dont use traffic anymore
		if( strcmp(my_record->PHASE,"S") == 0 && my_record->DIST > 80)
			continue;
		if( strcmp(my_record->PHASE,"P") == 0 && my_record->DIST > 78)
			continue;

		double what_is_too_far_huawei = 200;
		if( strstr(my_record->PHASE,"P") != NULL )
			what_is_too_far_huawei = 50;
		if(my_record->traffic_time[count] < -1*what_is_too_far_huawei  ||
				my_record->traffic_time[count] > what_is_too_far_huawei )
			continue;

//printf(" zero out %d / %d phase %s \n", count , my_record->num_traffic, my_record->traffic_phase[count]);
		int pad_length = 8;
		int npts_pad_each_side = (int)( pad_length / my_input->delta);
		int npts_min = (int)(-15 / my_input->delta);
		int npts_max_with_1T = (int) (20 / my_input->delta) + my_input->EW_period;

		int npts_beg_pad;
		int npts_end_pad;
		npts_beg_pad  = (int)(( my_record->traffic_time[count] - my_record->long_beg )/my_input->delta 
				+ npts_min );
		npts_end_pad  = (int)(( my_record->traffic_time[count] -my_record->long_beg ) /my_input->delta 
				+ npts_max_with_1T );

		if( npts_beg_pad <= 0 || npts_end_pad > (int)(my_input->phase_len / my_input->delta))
		{
			//printf(" npts problem \n");
			npts_beg_pad = 0;
			npts_end_pad = 0;
		}

		//printf("sta %s , traffic zoom out is %s \n", my_record->name, my_record->traffic_phase[count]);
		//printf("npts beg end is %d %d  \n", npts_beg_pad, npts_end_pad );

		int kkk;
		for(kkk = npts_beg_pad ; kkk< npts_end_pad ; kkk++)
		{
			my_record->long_win[kkk] = 0;
		}
	}



	return 0;
}

