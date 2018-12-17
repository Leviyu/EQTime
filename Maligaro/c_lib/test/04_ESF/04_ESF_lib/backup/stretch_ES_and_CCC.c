#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>
// stretch ES to find the best waveform to match each record
int stretch_ES_and_CCC(new_RECORD* my_record, new_INPUT* my_input, double* current_ES)
{
	puts("---> stretch_ES_and_CCC Begin");
	int npts_phase = (int)(my_input->phase_len / my_input->delta);
	int ista;
	int construct_array_with_main_lobe(double* array_in, int* npts_in, double* array_out);
	int amplitudeloc(double* array, int len, int* max_amp_loc, double* amplitude, int flag);
	

	//FILE* out2;


	for(ista=0;ista<my_input->sta_num;ista++)
	{
		// skip those with nan SNR
		if( my_record[ista].quality == -1)
			continue;

	//char out2_file[300];
	//sprintf(out2_file,"coeff_ccc_%s_%s",my_record[ista].name,my_record[ista].PHASE);
	//out2 = fopen(out2_file,"w");
//printf("stretch ES and CCc for sta: %s \n", my_record[ista].name);

		//stretch and CCC
		int NUM;
		double coeff;		// stretch coefficient
		int NUM_max = 20;
		double** stretched_ES;		// 2D array to store the stretched ES
		double stretched_CCC[NUM_max];
		double stretched_timeshift[NUM_max];
		double stretched_coefficient[NUM_max];
		stretched_ES = (double**)malloc(sizeof(double*)*NUM_max);
		for(NUM = 0; NUM < NUM_max; NUM++)
			stretched_ES[NUM] = (double*)malloc(sizeof(double)*npts_phase);

		for(NUM = 0; NUM < NUM_max; NUM ++)
		{
			coeff = 0.5 + NUM*0.5;
			double tmp_ES[npts_phase];
			//1. stretch ES
			stretch_ES_function(current_ES, npts_phase, coeff, tmp_ES);

				// store tmp_ES into stretched_ES
				int i;
				for(i=0;i<npts_phase;i++)
					stretched_ES[NUM][i] = tmp_ES[i];

			// only using the 10% to 100% part of both record and phase to CC
			double record_tmp[npts_phase];
			double ES_tmp[npts_phase];

				construct_array_with_main_lobe(tmp_ES, &npts_phase,ES_tmp);
				construct_array_with_main_lobe(my_record[ista].phase_win, &npts_phase,record_tmp);
					// output for DEBUG
					//char out_name[200];
					//sprintf(out_name,"Padded_ES.%s",my_record[ista].name);
					//output_array1(out_name,ES_tmp, npts_phase);
					//sprintf(out_name,"Padded_Record.%s",my_record[ista].name);
					//output_array1(out_name, record_tmp, npts_phase);

			//2. ccc stretched ES with record
			double ccc, ccc_tmp;
			int npts_shift, npts_tmp,ccc_flag;
			ccc_tmp = my_record[ista].ccc;
			ccc_flag=1;


			// shift and ccc comes from ccc between padded record and ES
			CCC(ES_tmp,npts_phase,record_tmp, npts_phase, &npts_shift, &ccc, ccc_flag);  
			//store info
			stretched_CCC[NUM] = ccc;
			stretched_coefficient[NUM] = coeff;
			stretched_timeshift[NUM] = npts_shift*my_input->delta;
//fprintf(out2,"%lf %lf \n",coeff, ccc);
		}
		// find the best CCC and store the stretch coefficient and the best ccc
		//find the max ccc
		int j;
		int index_max;
		double max_value=0;
		for(j=0; j<NUM_max; j++)
		{
			if( fabs(stretched_CCC[j]) > max_value )
			{
				max_value = fabs(stretched_CCC[j]);
				index_max = j;
			}
		}

		double best_ccc = stretched_CCC[index_max];
		double shift_time = stretched_timeshift[index_max];
		double best_coeff = stretched_coefficient[index_max];


//printf("sta is %s best  ccc is  %lf  \n",my_record[ista].name , best_ccc);

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
			//if( best_ccc > 0)
				//my_record[ista].polar_flag = 1;
			//else if (best_ccc < 0)
				//my_record[ista].polar_flag = -1;
		my_record[ista].best_stretch_coefficient = best_coeff;
		for(j=0;j<npts_phase;j++)
			my_record[ista].stretched_ES_win[j] = stretched_ES[index_max][j];



		//free stretched_ES
		for(NUM=0; NUM<NUM_max;NUM++)
			free(stretched_ES[NUM]);
		free(stretched_ES);
	}




	return 0;
}

