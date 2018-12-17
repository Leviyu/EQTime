#include "hongyulib.h"
/******************************************************************
 * This is a c script for getting the max AMP and location of array
 *
 *	Input:
 *	double*				array		
 *	int 				len
 *
 *	Output:
 *	int*				max_amp_loc
 *	double*				amplitude
 *	int 				flag
 *		flag == -1		return the negative amplitude and loc
 *		flag == 1		return the positive amplitude and loc
 *		flag == 0		return the greater AMP between negative and positive AMP and loc
 *
 *	DATE:				Keywords:
 *	Reference:
******************************************************************/

int amplitudeloc(double* array, int len, int* max_amp_loc, double* amplitude, int flag)
{
	double AMP_nega = 0;
	double AMP_posi = 0;
	int icount;
	int max_loc;
	int min_loc;

	for(icount = 0; icount < len ; icount ++)
	{
		if( array[icount] > AMP_posi )
		{
			AMP_posi = array[icount];
			max_loc = icount;
		}
		if( array[icount] < AMP_nega )
		{
			AMP_nega = array[icount];
			min_loc = icount;
		}
	}


	// determine which kind of AMP to return
	if(flag == 1)
	{
		*max_amp_loc = max_loc;
		*amplitude = AMP_posi;
	}
	else if(flag == -1)
	{
		*max_amp_loc = min_loc;
		*amplitude = AMP_nega;
	}
	if(flag == 0)
	{
		if( fabs(AMP_posi) >= fabs(AMP_nega) )
		{
			*max_amp_loc = max_loc;
			*amplitude = AMP_posi;
		}
		else 
		{
			*max_amp_loc = min_loc;
			*amplitude = AMP_nega;
		}
	}

	return 0;
}

