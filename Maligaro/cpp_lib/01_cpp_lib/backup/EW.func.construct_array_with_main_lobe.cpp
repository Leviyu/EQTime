#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
/******************************************************************
 * This is a c script for get the 10% ~ 100% AMP part of a array (old_ES) and 
 * fix the amp from 0~1 and then pad the rest of the trace with zeros (zeroes does not affect ccc)
 *  then the array is normalized to 0 ~ 1.0
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

int construct_array_with_main_lobe(double* array_in, int* npts_in, double* array_out)
{
	//puts("---> construct array with main lobe");
	int amplitudeloc(double* array, int len, int* max_amp_loc, double* amplitude, int flag);
	int npts_peak_min, npts_peak_max, npts_peak;
	double peak_max, peak_min, value_peak;
	int count, flag;
	double cut_min = 0.2;
	double cut_max = 1.0;
	double array_tmp[*npts_in];


	// get the peak value and loc
	amplitudeloc(array_in, *npts_in, &npts_peak_max, &peak_max,1);
	//amplitudeloc(array_in, *npts_in, &npts_peak_min, &peak_min,-1);

	// normalize and flip and array and set the flag
	if(value_peak == 0)
		value_peak = 1;
	npts_peak = npts_peak_max;
	value_peak = peak_max;
	for(count = 0; count< *npts_in; count ++)
	{
		array_tmp[count] = array_in[count]/value_peak;
	}

	// find the ONSET and ENDSET of array_tmp
	if(npts_peak == 0)
		npts_peak = 10;
	int ONSET=0;
	int ENDSET=*npts_in-1;
//printf("npts peak pf current record is %d peak value is %lf npts_in is %d  \n", npts_peak, value_peak,*npts_in);
	// look backward to find ONSET
	ONSET = npts_peak;
	for( count = npts_peak; count > 1; count-- )
	{
		//printf("coutn is %d array_tmp is %lf \n",count,array_tmp[count]);
		if(fabs(array_tmp[count]) < cut_min )
		{
			ONSET = count;
			break;
		}
	}
//printf("ONSET : %d \n", ONSET);
	// look forward to find ENDSET
	ENDSET = npts_peak;
	for( count = npts_peak; count < *npts_in-2  ; count++  )
	{
		if(fabs(array_tmp[count]) < cut_min )
		{
			ENDSET = count;
			break;
		}
	}
//printf("ENDSET : %d \n", ENDSET);

//printf("npts_peak is %d ONSET is %d ENDSET is %d npts_in is %d \n\n",npts_peak , ONSET , ENDSET, *npts_in);


	// if ONSET ENDSET too close, set all as 0
	if( abs(ENDSET - ONSET) < 15)
	{
		for(count = 0; count < *npts_in; count ++)
				array_out[count] = 0.000000001;
		//printf("---> ERROR for record ENDSET ONSET too close");
		return 0;
	}

	//define array out
	for(count = 0; count < *npts_in; count ++)
	{
		if(count <= ONSET || count >= ENDSET)
			array_out[count] = 0;
		else 
		{
			// fix this 10%~100% part to 0-1
			array_out[count] = value_peak/fabs(value_peak) *( array_tmp[count] - cut_min)/(1-cut_min);
		}
	}
//printf("ONSET AND ENDSET is %d %d\n", ONSET ,ENDSET);
//output_array1("test1111",array_out,*npts_in);

	//puts("---> construct array with main lobe done \n");
	return 0;

}
