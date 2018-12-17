#include "hongyulibcpp.h"

// if flag = 1, we use tstar 
// if flag = 2, we use stretching

int stretch_record_find_best_match_for_given_interval(double* record,
		double* phase_win, int npts_phase, double coeff_min, double coeff_max, 
		double coeff_delta, double* best_ccc, double* best_coeff,
		int* best_time_shift, double* best_ES, int flag)
{
//printf(" ---> stretch_record_find_best_match_for_given_interval \n");
	double coeff = 0;

	int count;
	double ES_win[npts_phase];
	int i;
	double record_tmp[npts_phase];
	double ES_tmp[npts_phase];
	double gaussian_func(double a, double b, double c, double d, double x);
	//define standard gaussian
	for(count = 0; count < npts_phase; count ++)
		ES_win[count] = record[count];

	output_array1("origional_gaussian",ES_win,npts_phase);

	if(coeff_min < 0)
		coeff_min = 0.1;
	if(coeff_max <= coeff_min)
	{
		coeff_max = coeff_min + 1;
		coeff_delta = 0.5;
	}

	int NUM;
	int NUM_max; // number of stretching needed
	NUM_max = (int)( (coeff_max - coeff_min)/coeff_delta)+1;
	//NUM_max = 100;

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
			continue;
		stretched_coefficient[NUM] = coeff;
//
//printf("--> tstar beg for %lf \n", coeff);
		double tmp_ES[npts_phase];
		//1. stretch ES
		if(flag == 1)
			tstar_ES_function(ES_win, npts_phase, coeff, tmp_ES);
		else if(flag == 2)
			stretch_ES_function(ES_win, npts_phase, coeff, tmp_ES);
		//output_array("stretched_gaussian",tmp_ES,npts_phase);
		

			// store tmp_ES into stretched_ES
			for(i=0;i<npts_phase;i++)
				stretched_ES[NUM][i] = tmp_ES[i];

		// only using the 10% to 100% part of both record and phase to CC

//printf("here1");
			construct_array_with_main_lobe(tmp_ES, &npts_phase,ES_tmp);
//printf("here2");
			construct_array_with_main_lobe(phase_win, &npts_phase,record_tmp);
//printf("here3");

//printf("--> tstar end \n\n");
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

	if(*best_ccc == 0)
		*best_ccc = 0.5;
	if(*best_coeff == 0)
		*best_coeff = 0.1;

	for(count=0;count<npts_phase;count++)
		best_ES[count] = stretched_ES[index_max][count];


	return 0;
}
