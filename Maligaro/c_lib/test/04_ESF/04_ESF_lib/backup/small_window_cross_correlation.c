#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>
/******************************************************************
 * This is a c script to use small window of that PHASE to do cross-correlation
 * 	when doing cross-correlation, we cut the ES and phase_window from 0.1 to 1 and 
 * 	padd the rest points with zeros.
 * 	This way we will have both the ccc and npts_shift
 *
 *	Input:
 *		
 *
 *
 *	Output:
 *
 *
 *	DATE:				Keywords:
 *	Reference:
******************************************************************/

int small_window_cross_correlation(double* ES, double* phase_win, int npts_phase, int* npts_shift, int* ccc, int ccc_flag)
{
	

	// 1. find the peak for ES and phase_window and cut them into new array
	double new_ES[npts_phase];
	double new_PHASE[npts_phase];

	// convert the ES and phase_win to 0.1 ~ 1 part and normalized to 0 ~ 1 
	//int construct_array_with_main_lobe(double* array_in, int* npts_in, double* array_out)
	construct_array_with_main_lobe(ES, &npts_phase, new_ES);
	construct_array_with_main_lobe(phase_win, &npts_phase, new_PHASE);

	//2. cross-correlation the new ES and PHASE
	









	return 0;
}
