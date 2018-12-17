
#include "hongyulib.h"
#include "ESF.h"
// stretch ES to find the best waveform to match each record
int stretch_ES_and_CCC(new_RECORD* my_record, new_INPUT* my_input, double* current_ES)
{
	printf( "---> stretch_ES_and_CCC For Stretching \n");
	int npts_phase = (int)(my_input->phase_len / my_input->delta);
	int ista,j;
	double stretched_ES[npts_phase];
	double best_ccc ;
	int best_time_shift;
	double best_coeff ;
	double coeff_min;
	double coeff_max;
	double coeff_delta;
	double coeff_array[50];
	int max_num;
	int count;
	
	for(ista=0;ista<my_input->sta_num;ista++)
	{
		if(my_record[ista].beyong_window_flag == -1)
			continue;

		if(strstr(my_input->PHASE,"S") != NULL )
		{
			coeff_min = 0.1;
			coeff_max = 10.0;
			coeff_array[0] = 1;
			coeff_array[1] = 0.5;
			coeff_array[2] = 0.25;
			coeff_array[3] = 0.1;
			coeff_array[4] = 0.05;
			coeff_array[5] = 0.02;
			coeff_array[6] = 0.01;
			//coeff_array[7] = 0.01;
			//coeff_array[8] = 0.1;
			//coeff_array[9] = 0.05;
			//coeff_array[10] = 0.02;
			//coeff_array[11] = 0.01;
			max_num = 7;
		}
		else if(strstr(my_input->PHASE,"P") != NULL )
		{
			coeff_min = 0.1;
			coeff_max = 5.0;
			coeff_array[0] = 0.3;
			coeff_array[1] = 0.15;
			coeff_array[2] = 0.1;
			coeff_array[3] = 0.05;
			coeff_array[4] = 0.02;
			coeff_array[5] = 0.01;
			//coeff_array[7] = 0.01;
			//coeff_array[8] = 0.1;
			//coeff_array[9] = 0.05;
			//coeff_array[10] = 0.02;
			//coeff_array[11] = 0.01;
			max_num = 6;
		}


		stretch_ES_find_best_match_for_given_interval(&my_record[ista], current_ES, my_record[ista].phase_win, npts_phase, coeff_min, coeff_max, coeff_array[0], &best_ccc, &best_coeff,&best_time_shift,stretched_ES, my_input);

		for(count = 1; count < max_num  ; count ++)
		{
			coeff_min = best_coeff - coeff_array[count-1]/2;
			coeff_max = best_coeff + coeff_array[count-1]/2;
			coeff_delta = coeff_array[count];
			stretch_ES_find_best_match_for_given_interval(&my_record[ista], current_ES, my_record[ista].phase_win, npts_phase, coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff,&best_time_shift,stretched_ES, my_input);
			//printf(" stretching find best %d / %d ccc  %lf \n", count, max_num, best_ccc);
			//if(best_ccc > 0.99)
				//break;
		}

		double shift_time = best_time_shift*my_input->delta;
		// if reprocessing_flag is 1, we dont allow big shift
		// if shift is greater then 3sec, then we hardwire it to be 0
		//if(my_input->Reprocessing_Flag == 1 && 
				//fabs(shift_time) > 5 )
		//{
			//printf(" --> reprocessing_flag STA %s shifted %lf \n", my_record[ista].name, shift_time);
			//shift_time = 0;
		//}
		// shift the phase window
		double shift_time_max = 1000;
		my_record[ista].phase_beg -= shift_time;
		//if( my_record[ista].checked_to_be_good == 1 )
			//shift_time_max = 15;
		//else
			//shift_time_max = 30;

//printf(" ccc stretch station %s is shifted %lf \n", my_record[ista].name, shift_time );
/*
			if( fabs(shift_time) > shift_time_max)
				shift_time = 0;
			if(my_record[ista].phase_beg < my_record[ista].long_beg) 
				my_record[ista].phase_beg += shift_time;


		// count the time difference between E.W. onset to PREM 
	//int amplitudeloc(double* array, int len, int* max_amp_loc, double* amplitude, int flag)	
		int max_loc;
		double max_amp;
		amplitudeloc(current_ES, npts_phase, &max_loc, &max_amp,1);
		double EW_onset_relative_to_PREM = max_loc * my_input->delta + my_input->phase_beg - 5;
		


		my_record[ista].shift_time_recorder += shift_time;
		printf(" sta %s shift %lf total shift %lf EW_onset_relative_to_PREM %lf  \n", my_record[ista].name,
				shift_time, my_record[ista].shift_time_recorder, EW_onset_relative_to_PREM);
		double max_time = 30;
		double dt_shift = -1*my_record[ista].shift_time_recorder + EW_onset_relative_to_PREM;
		if( dt_shift > max_time || dt_shift < -1*max_time)
		{
			printf("%s sta shift to much \n", my_record[ista].name);
			my_record[ista].shift_time_recorder -= shift_time;
			my_record[ista].phase_beg += shift_time;
			my_record[ista].quality = -1;
		}

*/





		// update phase win
		read_phase_window(&my_record[ista],my_input);

		// store this ccc and this stretched ES win
		my_record[ista].best_stretch_ccc = best_ccc;
		my_record[ista].best_stretch_coefficient = best_coeff;


		for(j=0;j<npts_phase;j++)
			my_record[ista].stretched_ES_win[j] = stretched_ES[j];
		//printf("For ista %d stretching best ccc %lf best coeff %lf \n", ista, best_ccc, best_coeff);

	}


	return 0;
}

