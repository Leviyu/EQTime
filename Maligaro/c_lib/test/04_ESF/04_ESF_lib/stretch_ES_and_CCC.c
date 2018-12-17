#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>
// stretch ES to find the best waveform to match each record
int stretch_ES_and_CCC(new_RECORD* my_record, new_INPUT* my_input, double* current_ES)
{
	fprintf(my_input->out_logfile, "---> stretch_ES_and_CCC For Stretching ");
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
	

	//FILE* out2;


	for(ista=0;ista<my_input->sta_num;ista++)
	{
		if( my_record[ista].quality == -1)
			continue;

//printf("ista %d \n", ista);
			coeff_min = 0.1;
			coeff_max = 6.0;
			coeff_array[0] = 1;
			coeff_array[1] = 0.5;
			coeff_array[2] = 0.25;
			coeff_array[3] = 0.1;
			coeff_array[4] = 0.03;
			coeff_array[5] = 0.01;
			//coeff_array[6] = 0.02;
			//coeff_array[7] = 0.01;

		stretch_ES_find_best_match_for_given_interval(&my_record[ista], current_ES, my_record[ista].phase_win, npts_phase, coeff_min, coeff_max, coeff_array[0], &best_ccc, &best_coeff,&best_time_shift,stretched_ES, my_input);

		int max_num = 4;

		int count;
		for(count = 1; count < max_num  ; count ++)
		{
			coeff_min = best_coeff - coeff_array[count-1]/2;
			coeff_max = best_coeff + coeff_array[count-1]/2;
			coeff_delta = coeff_array[count];

			stretch_ES_find_best_match_for_given_interval(&my_record[ista], current_ES, my_record[ista].phase_win, npts_phase, coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff,&best_time_shift,stretched_ES, my_input);
		}



		double shift_time = best_time_shift*my_input->delta;
		// shift the phase window
			if( fabs(shift_time) > 30.0)
				shift_time = 0;
			my_record[ista].phase_beg -= shift_time;
			if(my_record[ista].phase_beg < my_record[ista].long_beg) 
				my_record[ista].phase_beg += shift_time;
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

