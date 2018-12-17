
#include "ESF.h"
#include "hongyulib.h"
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
	int npts_peak_min, npts_peak_max, npts_peak;
	double peak_max, peak_min, value_peak;
	int count, flag;
	double cut_min = 0.2;
	double cut_max = 1.0;
	double array_tmp[*npts_in];
	int kkk;
	int kkk_time;
	int current_npts;
	int cut_time_sum;
	double new_cut[*npts_in];


	// get the peak value and loc
	amplitudeloc(array_in, *npts_in, &npts_peak_max, &peak_max,1);


	// if peak is within the first or last 10sec window, we make a new array
	// with first and last 10sec masked and re find peak
	//printf(" peak max is %d \n", npts_peak_max);
	int skip_npts = 80;
	if(npts_peak_max < skip_npts || npts_peak_max > *npts_in - skip_npts)
	{
		double tmpArray[*npts_in];
		for(kkk = 0; kkk < *npts_in ; kkk++)
		{
			if( kkk < skip_npts || kkk > *npts_in - skip_npts)
				tmpArray[kkk] = 0;
			else
				tmpArray[kkk] = array_in[kkk];
		}
		amplitudeloc(tmpArray, *npts_in, &npts_peak_max, &peak_max,1);

	}




	// sometimes the array has peak value at the front or the end, 
	// we creat an algorithm to make sure that the peak is always chosen to be somewhere in
	// the middle of the trace, if it is at the end points, we chop off 10points/1sec and redo the process
	int npts_cut = npts_peak_max;
	if( npts_cut == 0 || npts_cut == *npts_in-1)
	//if( npts_cut < 100 || npts_cut > *npts_in-1 - 100)
	{
	current_npts = *npts_in;
	kkk_time = 1;
	cut_time_sum = 0;
	while( npts_cut == 0 || npts_cut == current_npts-1)
	// we change it in a way that the npts_cut, which is the peak location, 
	// are not allowed to be in the first or last 10sec of the window
	//while( npts_cut <100 || npts_cut > current_npts-1 - 100 )
	{
		// chop off 2* 10 points
		// lets try chop off 2* 20
		current_npts = *npts_in - kkk_time * 40;
		if( current_npts < 40 )
			break;
		for(kkk = 0; kkk< current_npts; kkk++) 
			new_cut[ kkk] = array_in[kkk+10*kkk_time];
		amplitudeloc(new_cut, current_npts, &npts_cut, &peak_max,1);
		//if( npts_cut == current_npts -1 || npts_cut == 0)
		cut_time_sum = npts_cut + 10*kkk_time;
		//printf("  kkk %d current peak npts is %d  npts_cut %d \n",kkk_time, cut_time_sum, npts_cut);
		kkk_time += 1;
	}
	npts_peak_max = cut_time_sum;
	}


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

	// sometimes the ONSET does not find the correct ONSET cause 
	// the signal never to below threshold 0.1
	// we make a hard rule:
	// if  peak - ONSET  > ENDSET - peak + 5sec then
	// ONSET = peak - (ENDSET - peak)
	//printf(" ONSET ENDSET  %d %d \n", npts_peak - ONSET , (ENDSET - npts_peak ));
	////printf(" first point %lf \n", array_in[ONSET]);	

	if(  fabs(npts_peak - ONSET -  (ENDSET - npts_peak )) > 20  )
	{
		//ONSET = npts_peak - (ENDSET - npts_peak);
		cut_min = 0.4;
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

	}

//printf("npts_peak is %d ONSET is %d ENDSET is %d npts_in is %d \n\n",npts_peak , ONSET , ENDSET, *npts_in);


	// if ONSET ENDSET too close, set all as 0
	if( abs(ENDSET - ONSET) < 13)
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
//#printf("ONSET AND ENDSET is %d %d\n", ONSET ,ENDSET);
//output_array1("test1111",array_out,*npts_in);

	//puts("---> construct array with main lobe done \n");
	return 0;

}
