#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<ESF.h>
/******************************************************************
 * This is a c script for get the 10% ~ 100% AMP part of a array (old_ES) and 
 * fix the amp from 0~1 and then only keep this part into new_ES
 *
 *	Input:
 *	double* old_ES
 *	int*	npts_old_ES
 *
 *	OUTPUT
 *	double* new_ES				-- new ES with AMP from 0.1 to 1 and fix to 0 to 1
 *	int* npts_new_ES
 *
 *
 *	DATE: Sep 20 2014				Keywords:
******************************************************************/

int BEST_GAUSSIAN_construct_new_ES(double* old_ES, int* npts_old_ES, double* new_ES, int* npts_new_ES)
{
	int amplitudeloc(double* array, int len, int* max_amp_loc, double* amplitude,int flag);
	int output_array1();
	int npts_peak;
	double max_peak;


	// get the peak value and loc
	amplitudeloc(old_ES, *npts_old_ES, &npts_peak, &max_peak,0);
//printf("max loc valie is %lf npts is %d \n", max_peak, npts_peak);


	// construct duplicated new ES
	int npts_ES = 2* npts_peak;
	double ES_tmp[npts_ES];
	int icount;
	for(icount = 0; icount < npts_peak; icount ++)
	{	
		ES_tmp[icount] = old_ES[icount];
	}

	for(icount = npts_peak; icount < npts_ES; icount ++)
	{	
		ES_tmp[icount] = old_ES[2* (npts_peak - 1) - icount];
	}

	output_array1("ES_tmp_out",ES_tmp, npts_ES);

	// cut only the part with AMP from 0.1 to 1
	*npts_new_ES = 0;
	double cut_level = 0.1;
	for(icount = 0; icount < npts_ES; icount ++)
	{
		if(ES_tmp[icount] > cut_level * 1)
		{
			new_ES[*npts_new_ES] = ES_tmp[icount];
			*npts_new_ES = *npts_new_ES +1;
		}
	}
	


	// fix to 0 to 1
	for(icount = 0; icount < *npts_new_ES; icount ++)
	{
			new_ES[icount] =  (new_ES[icount] - cut_level)/ (1- cut_level);
	}

	return 0;

}
