#include "ESF.h"

int define_goodness_of_record(new_RECORD* my_record, new_INPUT* my_input)
{
	int count;
	double SNR;
	double ccc;
	double stretch_ccc;
	double stretch_factor;
	double tstar_factor;
	double tstar_ccc;

	double SNR_threshold = 0;
	double CCC_threshold = 0;
	int NUM_station_within_range = 0;
	double SUM_dt_within_range = 0;
	double misfit;
	double misfit_pre;
	double dt_anomaly;


	int ista;
	for(ista = 0; ista < my_input->sta_num; ista++)
	{
		if(my_record[ista].beyong_window_flag == -1)
			continue;

		SNR = my_record[ista].SNR;
		ccc = my_record[ista].ccc;
		stretch_ccc = my_record[ista].best_stretch_ccc;
		stretch_factor = my_record[ista].best_stretch_coefficient;
		tstar_factor = my_record[ista].best_tstar;
		tstar_ccc  = my_record[ista].best_tstar_ccc;
		misfit = my_record[ista].misfit;
		misfit_pre = my_record[ista].misfit_pre;
		dt_anomaly = my_record[ista].dt_obs_prem;

		// here we define the polarity_prediction flag
		my_record[ista].polar_correct_flag = 0;
		if( fabs( my_record[ista].polarity) < 0.1 ) 
		{
			if( my_record[ista].polarity * my_record[ista].polar_flag <  0)
				my_record[ista].polar_correct_flag = 1;
		}
		
		if( my_record[ista].traffic_phase_nearby == 1)
		{
			my_record[ista].quality = -1;
			continue;
		}
		// if is SS and misfit pre is too big, reject
		if(strcmp( my_record[ista].PHASE, "SS") == 0 && my_record[ista].misfit_pre > 0.25 )
		{
			my_record[ista].quality = -1;
			continue;
		}
		if(strcmp( my_record[ista].PHASE, "SS") == 0 && my_record[ista].misfit_pre2T > 0.3 )
		{
			my_record[ista].quality = -1;
			continue;
		}
		// if record is too skinny, stretch_factor < 0.8, reject
		if( my_record[ista].best_stretch_coefficient < 0.8 )
		{
			my_record[ista].quality = -1;
			continue;
		}


		if(my_record[ista].quality != -1 &&
				fabs(SNR) > my_input->SNR_CUT &&
				fabs(tstar_ccc) > my_input->CCC_CUT)
			my_record[ista].quality = 1;

		if(SNR != SNR) 
			my_record[ista].quality = -1;


		if(fabs(ccc) < 0.8)
			my_record[ista].quality = -1;

		if(misfit == 0)
			misfit = 1;

		//if( misfit_pre / misfit > 5.0)
			//my_record[ista].quality = -1;
		if( misfit_pre > 0.3 )
			my_record[ista].quality = -1;

		if( dt_anomaly < my_input->mask_win_min || dt_anomaly > my_input->mask_win_max )
			my_record[ista].quality = -1;

		// here we also make sure that if dt is too close to other phase, then, we mark its quality to be -1
		reject_records_with_dt_too_close_to_traffic_phase(&my_record[ista], my_input);



		if( my_record[ista].quality != 1 )
			continue;


		// Step1 Define a part of the data as quality = 2
		// These data are records that CCC< CCC_threshold or SNR < SNR_threshold
		SNR_threshold = 3.0;
		CCC_threshold = 0.9;
		if(strcmp(my_record[ista].PHASE, "S") == 0)
		{
			SNR_threshold = 2.7;
			CCC_threshold = 0.9;
		}
		else if(strcmp(my_record[ista].PHASE, "ScS") == 0)
		{
			SNR_threshold = 3.0;
			CCC_threshold = 0.85;
		}
		else if(strcmp(my_record[ista].PHASE, "SS") == 0)
		{
			SNR_threshold = 3.0;
			CCC_threshold = 0.8;
		}
		else if(strcmp(my_record[ista].PHASE, "SSS") == 0)
		{
			SNR_threshold = 3.0;
			CCC_threshold = 0.8;
		}
		else if(strcmp(my_record[ista].PHASE, "ScSScS") == 0)
		{
			SNR_threshold = 3.0;
			CCC_threshold = 0.83;
		}
		else if(strcmp(my_record[ista].PHASE, "Sdiff") == 0)
		{
			SNR_threshold = 3.0;
			CCC_threshold = 0.87;
		}

		if( my_record[ista].quality == 1 )
		{
			if(SNR <= SNR_threshold || ccc <= CCC_threshold )
			{
				my_record[ista].quality = 2;
			}
		}


		// step 2 Problem for records that grabed the pulse that is far away from the right pulse caused of better CCC
		// we calculate the average ONSET with a degree of that station, if the ONSET of that station is 15sec away, 
		// then we mark is as quality = 3, which need to be human checked.
		NUM_station_within_range = 0;
		SUM_dt_within_range = 0;
		for(count = 0; count < my_input->sta_num; count ++ )
		{
			if(my_record[count].quality == -1)
				continue;
			if( fabs(my_record[ista].DIST - my_record[count ].DIST) <=0.5 ) 
			{
				NUM_station_within_range ++;
				SUM_dt_within_range += my_record[count].dt_obs_prem;
			}
		}

		if( NUM_station_within_range == 0)
			NUM_station_within_range = 1;

		SUM_dt_within_range = SUM_dt_within_range / NUM_station_within_range;


		if(fabs ( my_record[ista].dt_obs_prem  -  SUM_dt_within_range ) > 15 ) 
			my_record[ista].quality = 3;


				//fabs(tstar_factor) > tstar_factor_min &&
				//fabs(tstar_factor) < tstar_factor_max)
		//if(my_record[ista].quality != -1 &&
				//fabs(SNR) > my_input->SNR_CUT &&
				//fabs(stretch_ccc) > my_input->CCC_CUT &&
				//fabs(ccc) > 0.5)
			//my_record[ista].quality = 1;


//printf("STA %s ccc %lf SNR %lf quality %d \n ", my_record[ista].name, stretch_ccc, stretch_factor, my_record[ista].quality );
	}




	

	return 0;
}
