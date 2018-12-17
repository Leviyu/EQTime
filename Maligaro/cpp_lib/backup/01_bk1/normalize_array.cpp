#include<hongyulib.h>


int normalize_array(double* array, int len)
{
	double amplitude(double* array, int len);
	double AMP = amplitude(array,len);
	int icount;
	for(icount = 0; icount < len ; icount ++)
	{
		array[icount] = array[icount] / AMP;
	}
	return 0;
}




