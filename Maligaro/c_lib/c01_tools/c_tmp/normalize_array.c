#include "hongyulib.h"


int normalize_array(double* array, int len)
{
	double AMP;



	// find the positive amp of array
	AMP = 0;	
	int i;
	for(i = 0 ; i < len ; i++)
	{
		if(array[i] > AMP)
			AMP = array[i];
	}


	int icount;
	for(icount = 0; icount < len ; icount ++)
	{
		array[icount] = array[icount] / AMP;
	}
	return 0;
}




