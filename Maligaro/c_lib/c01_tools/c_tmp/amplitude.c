#include<hongyulib.h>

double amplitude(double* array, int len)
{
	double AMP = 0;
	int icount;



	for(icount = 0; icount < len ; icount ++)
	{
		if( fabs(array[icount]) > AMP )
		{
			AMP = fabs(array[icount]);
		}
	}
	

	return AMP;
}

