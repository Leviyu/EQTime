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


int get_phase_amplitude(new_PHASE* T)
{
	double amp_posi = 0;
	double amp_nega = 0;
	int npts_phase = T->npts_phase;	
	int i;
	for(i=0 ; i<npts_phase; i++)
	{
		if( T->phase_win[i] > amp_posi )
			amp_posi = T->phase_win[i];

		if(T->phase_win[i] < amp_nega )
			amp_nega = T->phase_win[i];
	}

	// store DEPMIN DEPMAX
	T->DEPMIN = amp_nega;
	T->DEPMAX = amp_posi;

//printf("DEPMINMAZX Is %15.10lf %15.10lf\n", amp_nega, amp_posi);

	// find the max value and store into amplitude
	if ( amp_posi >= amp_nega )
	{
		T->amplitude = amp_posi;
	}
	else if ( amp_posi <= amp_nega )
	{
		T->amplitude = amp_nega;
	}

	return 0;
}
