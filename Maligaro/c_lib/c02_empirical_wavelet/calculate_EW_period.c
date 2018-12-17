
#include "hongyulib.h"
#include "ESF.h"
/******************************************************************
 * This is a c script to calculate the EW_period
 *
 *	Input:
 *
 *
 *	Output:
 *
 *
 *	DATE:				Keywords:
 *	Reference:
******************************************************************/

int calculate_EW_period(new_INPUT* my_input, double* EW)
{

//int amplitudeloc(double* array, int len, int* max_amp_loc, double* amplitude, int flag)

	int npts = (int)(my_input->phase_len / my_input->delta);
	int max_amp_loc = 0;
	double amp;
	int flag = 1;
	amplitudeloc(EW,npts,&max_amp_loc,&amp,flag);

	if( max_amp_loc == 0 || max_amp_loc == npts -1  )
	{	
		my_input->EW_period = 10;
		return 0;
	}

	// 
	double threshold = 0.1;
	int npts_threshold = max_amp_loc;
	int count = 0;

	for(count = npts_threshold; count > 1 ; count --)
	{
		if( EW[count] <= amp * threshold )
		{
			break;
		}
	}

	double dt_onset_peak = ( max_amp_loc - count ) * my_input->delta;

	if( dt_onset_peak > 10 )
		dt_onset_peak = 10;


	my_input->EW_period = 2*dt_onset_peak;
	
	return 0;
}
