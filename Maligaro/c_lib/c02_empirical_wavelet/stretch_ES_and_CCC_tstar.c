
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

		//my_record[ista].phase_beg = my_input->phase_beg + my_record[ista].dt_picked_shift;
		//read_phase_window(&my_record[ista],my_input);

		if(strcmp(my_input->stretch_flag,"tstar")==0 && my_record[ista].best_stretch_coefficient != 0 && my_record[ista].best_stretch_coefficient <1 )
		{
			coeff_min = 0.1;
			coeff_max = 10.0;
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
				coeff_max = 20.1;
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
		//printf(" Working on sta %d  %s \n ", ista, my_record[ista].name);	
		//output_array1("current_ES_tmp", current_ES,npts_phase);
		//output_array1("current_phase_win_tmp", my_record[ista].phase_win,npts_phase);

		stretch_ES_find_best_match_for_given_interval(&my_record[ista], current_ES, my_record[ista].phase_win, npts_phase, coeff_min, coeff_max, coeff_array[0], &best_ccc, &best_coeff,&best_time_shift,stretched_ES, my_input);

		//char kkk[500];
		////sprintf(kkk,"t_star.%s",my_record[ista].name);
		//output_array1(kkk,my_record[ista].phase_win,npts_phase);

		for(count = 1; count < max_num  ; count ++)
		{

			coeff_min = best_coeff - coeff_array[count-1]/2;
			coeff_max = best_coeff + coeff_array[count-1]/2;
			coeff_delta = coeff_array[count];
			stretch_ES_find_best_match_for_given_interval(&my_record[ista], current_ES, my_record[ista].phase_win, npts_phase, coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff,&best_time_shift,stretched_ES, my_input);
			//printf(" sta %s  tstar  %d / %d , coeff %lf ccc %lf \n", my_record[ista].name, count, max_num , best_coeff,
					//best_ccc);
			//if(best_ccc > 0.99)
				//break;
		}

		double shift_time = best_time_shift*my_input->delta;
		//printf(" -- tstar sta %s shifttime %lf \n", my_record[ista].name,
				//shift_time);
		// if reprocessing_flag is 1, we dont allow big shift
		// if shift is greater then 3sec, then we hardwire it to be 0
		//if(my_input->Reprocessing_Flag == 1 && 	fabs(shift_time) > 5 )
		//{
	//printf(" --> tstar STA %s shifted %lf \n", my_record[ista].name, shift_time);
			//shift_time = 0;
		//}
		//double shift_time_max = 1000;
		my_record[ista].phase_beg -= shift_time;



		// update phase win
		read_phase_window(&my_record[ista],my_input);

		my_record[ista].best_tstar_ccc = best_ccc;
		my_record[ista].best_tstar= best_coeff;

		for(j=0;j<npts_phase;j++)
			my_record[ista].stretched_ES_win[j] = stretched_ES[j];

		//printf("For ista %d stretching best ccc %lf best coeff %lf \n", ista, best_ccc, best_coeff);
	}



	return 0;
}

