#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>

int define_goodness_of_record(new_RECORD* my_record, new_INPUT* my_input)
{
	int count;
	double SNR;
	double ccc;
	double stretch_ccc;
	double stretch_factor;
	double tstar_factor;
	double tstar_ccc;

	double tstar_factor_min = 0;
	double tstar_factor_max = 5;
	double SNR_threshold = 0;
	double CCC_threshold = 0;
	int NUM_station_within_range = 0;
	double SUM_dt_within_range = 0;


	int ista;
	for(ista = 0; ista < my_input->sta_num; ista++)
	{
		if(my_record[ista].quality == -1)
			continue;
		SNR = my_record[ista].SNR;
		ccc = my_record[ista].ccc;
		stretch_ccc = my_record[ista].best_stretch_ccc;
		stretch_factor = my_record[ista].best_stretch_coefficient;
		tstar_factor = my_record[ista].best_tstar;
		tstar_ccc  = my_record[ista].best_tstar_ccc;
		
		


		if(my_record[ista].quality != -1 &&
				fabs(SNR) > my_input->SNR_CUT &&
				fabs(tstar_ccc) > my_input->CCC_CUT)
			my_record[ista].quality = 1;


		if(fabs(ccc) < 0.5)
			my_record[ista].quality = -1;


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
