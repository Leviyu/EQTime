#include "hongyulib.h"
/******************************************************************
 * This is a c script  impliment hilbert transfrom to a array using convolution algorithm
 *
 *	Input: 
 *	double* 			array_in
 *	int 				npts
 *
 *	Output:
 *	double*				array_out
 *
 * NOTES:
 *
 * Assume input is f(x)
 *
 * Algorithm:
 *
 *	Hilbert(x) = SUM( f(t) / ((x-t)*pi) )
 *
 *
 *
 *
 *	DATE:Nov 4 2014				Keywords: hilbert transfrom
 *	Reference: Mathwork hilbert page
******************************************************************/
#define PI	3.1415926

int hilbert2(double* array_in, int npts, double* array_out)
{
	
	int count,i;

	for(count = 0; count < npts; count ++)
	{
		array_out[count] = 0;
		for(i = 0; i<npts; i++)
			if(count != i)
				array_out[count] += array_in[i]/(count-i)/PI;

		printf("array_out[%d] is %lf \n",count, array_out[count]);

	}

	return 0;
}


