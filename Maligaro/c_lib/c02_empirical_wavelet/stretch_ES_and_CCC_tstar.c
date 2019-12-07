
#include "hongyulib.h"
#include "ESF.h"
// stretch ES to find the best waveform to match each record
int stretch_ES_and_CCC_tstar(new_RECORD* my_record, new_INPUT* my_input, double* current_ES)
{
	printf( "---> stretch_ES_and_CCC for tstar \n");
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
	int max_num = 4;
	int count;


	for(ista=0;ista<my_input->sta_num;ista++)
	{

		if(my_record[ista].beyong_window_flag == -1)
			continue;


		if(strcmp(my_input->stretch_flag,"tstar")==0 && my_record[ista].best_stretch_coefficient != 0 && my_record[ista].best_stretch_coefficient <1 )
		{
			coeff_min = 0.1;
			coeff_max = 5.0;
			coeff_array[0] = 0.5;
			coeff_array[1] = 0.25;
			coeff_array[2] = 0.15;
			coeff_array[3] = 0.1;
			coeff_array[4] = 0.05;
			coeff_array[5] = 0.02;
			coeff_array[6] = 0.01;
			//coeff_array[5] = 0.0;
			max_num = 7;
		}
		else
		{
			if(strstr(my_input->PHASE,"S") != NULL )
			{
				coeff_min = 0.1;
				coeff_max = 22.1;
				coeff_array[0] = 1;
				coeff_array[1] = 0.5;
				coeff_array[2] = 0.3;
				coeff_array[3] = 0.25;
				coeff_array[4] = 0.1;
				coeff_array[5] = 0.05;
				coeff_array[6] = 0.02;
				coeff_array[7] = 0.01;
				//coeff_array[6] = 0.02;
				//coeff_array[7] = 0.01;
				//coeff_array[8] = 0.01;
				max_num = 8;
			}
			else if(strstr(my_input->PHASE,"P") != NULL )
			{
				coeff_min = 0.1;
				coeff_max = 10.1;
				coeff_array[0] = 0.5;
				coeff_array[1] = 0.25;
				coeff_array[2] = 0.1;
				coeff_array[3] = 0.05;
				coeff_array[4] = 0.02;
				coeff_array[5] = 0.01;
				//coeff_array[6] = 0.02;
				//coeff_array[7] = 0.01;
				//coeff_array[6] = 0.02;
				//coeff_array[7] = 0.01;
				//coeff_array[8] = 0.01;
				max_num = 6;
			}
		}

		stretch_ES_find_best_match_for_given_interval(&my_record[ista], current_ES, my_record[ista].phase_win, npts_phase, coeff_min, coeff_max, coeff_array[0], &best_ccc, &best_coeff,&best_time_shift,stretched_ES, my_input);


		for(count = 1; count < max_num  ; count ++)
		{

			coeff_min = best_coeff - coeff_array[count-1]/2;
			coeff_max = best_coeff + coeff_array[count-1]/2;
			coeff_delta = coeff_array[count];
			stretch_ES_find_best_match_for_given_interval(&my_record[ista], current_ES, my_record[ista].phase_win, npts_phase, coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff,&best_time_shift,stretched_ES, my_input);
		}

		double shift_time = best_time_shift*my_input->delta;
		my_record[ista].phase_beg -= shift_time;



		// update phase win
		read_phase_window(&my_record[ista],my_input);

		my_record[ista].best_tstar_ccc = best_ccc;
		my_record[ista].best_tstar= best_coeff;

		for(j=0;j<npts_phase;j++)
			my_record[ista].stretched_ES_win[j] = stretched_ES[j];

	}



	return 0;
}

