#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>

int stretch_ES_find_best_match_for_given_interval(new_RECORD* my_record, double* ES_win, double* phase_win, int npts_phase, double coeff_min, double coeff_max, double coeff_delta, double* best_ccc, double* best_coeff,
		int* best_time_shift, double* best_ES, new_INPUT* my_input)
{
	double coeff = 0;

	int NUM;
	int NUM_max; // number of stretching needed

	if(coeff_delta <= 0)
		coeff_delta = 0.01;


	NUM_max = (int)( (coeff_max - coeff_min)/coeff_delta)+1;

	//stretch and CCC
	double** stretched_ES;		// 2D array to store the stretched ES
	double stretched_CCC[NUM_max];
	int stretched_timeshift[NUM_max];
	double stretched_coefficient[NUM_max];
	stretched_ES = (double**)malloc(sizeof(double*)*NUM_max);
	for(NUM = 0; NUM < NUM_max; NUM++)
		stretched_ES[NUM] = (double*)malloc(sizeof(double)*npts_phase);

	for(NUM = 0; NUM < NUM_max; NUM ++)
	{
		coeff = coeff_min + NUM*coeff_delta;

		if(coeff <= 0)
		{
			stretched_CCC[NUM] = 0;
			stretched_coefficient[NUM] = coeff;
			stretched_timeshift[NUM] = 0;
			continue;
		}

		stretched_coefficient[NUM] = coeff;
		double tmp_ES[npts_phase];
		//1. stretch ES
		if(strcmp(my_input->stretch_flag,"stretch")==0)
			stretch_ES_function(ES_win, npts_phase, coeff, tmp_ES);
		else if(strcmp(my_input->stretch_flag,"tstar")==0 && my_record->best_stretch_coefficient != 0 && my_record->best_stretch_coefficient <=1 )
			stretch_ES_function(ES_win, npts_phase, coeff, tmp_ES);
		else 
			tstar_ES_function(ES_win, npts_phase, coeff, tmp_ES);

			// store tmp_ES into stretched_ES
			int i;
			for(i=0;i<npts_phase;i++)
				stretched_ES[NUM][i] = tmp_ES[i];

		// only using the 10% to 100% part of both record and phase to CC
		double record_tmp[npts_phase];
		double ES_tmp[npts_phase];

			construct_array_with_main_lobe(tmp_ES, &npts_phase,ES_tmp);
			construct_array_with_main_lobe(phase_win, &npts_phase,record_tmp);

		//2. ccc stretched ES with record
		double ccc;
		int npts_shift, npts_tmp,ccc_flag;
		ccc_flag=1;


		// shift and ccc comes from ccc between padded record and ES
		CCC(ES_tmp,npts_phase,record_tmp, npts_phase, &npts_shift, &ccc, ccc_flag);  
		//store info
		stretched_CCC[NUM] = ccc;
		stretched_coefficient[NUM] = coeff;
		stretched_timeshift[NUM] = npts_shift;
		//printf("--->working on coeff %lf ccc %lf time shift %d \n",coeff, ccc, npts_shift);
	}

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

	*best_ccc = stretched_CCC[index_max];
	*best_time_shift = stretched_timeshift[index_max];
	*best_coeff = stretched_coefficient[index_max];
	//printf("best coeff is %lf \n", *best_coeff);
	int count;
	for(count=0;count<npts_phase;count++)
		best_ES[count] = stretched_ES[index_max][count];

	for(NUM = 0; NUM < NUM_max; NUM++)
		free(stretched_ES[NUM]) ;

	return 0;
}
