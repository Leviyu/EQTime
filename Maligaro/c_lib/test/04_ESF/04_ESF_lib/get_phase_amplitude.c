#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>

/******************************************************************
 * This is a c script to get phase window amp 
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


int get_phase_amplitude(new_RECORD* my_record)
{
	double amp_posi = 0;
	double amp_nega = 0;
	int npts_phase = my_record->npts_phase;	
	int i;
	for(i=0 ; i<npts_phase; i++)
	{
		if( my_record->phase_win[i] > amp_posi )
			amp_posi = my_record->phase_win[i];

		if(my_record->phase_win[i] < amp_nega )
			amp_nega = my_record->phase_win[i];
	}


//printf(" STA %s DEPMINMAZX Is %15.10lf %15.10lf\n", my_record->name,amp_nega, amp_posi);

	// find the max value and store into amplitude
	my_record->amplitude = amp_posi;

	double AMP = amp_posi;

	//normalize according to the AMP we have here
	if(my_record->amplitude == 0)
	{
		my_record->quality = -1;
		return 1;
	}
	else
	{
		//normalize phase window to 1 realtive to positive AMP

		for(i=0 ; i<my_record->npts_phase; i++)
		{
			my_record->phase_win[i] *= 1/AMP;
		}
		for(i=0 ; i<my_record->npts_long; i++)
		{
			my_record->long_win[i] *= 1/AMP;
		}
		for(i=0 ; i<my_record->npts_noise; i++)
		{
			my_record->noise_win[i] *= 1/AMP;
		}

	}



	return 0;
}
