#include "hongyulib.h"
/******************************************************************
 * This is a c script  impliment convolution
 *
 *	Input: 
 *	double* 			array1
 *	int 				npts1
 *	array u	
 *
 *
 *	double& 			array2
 *	int					npts2
 *	array v
 *
 *	Output:
 *	double*				array_out
 *
 *
 *  
 *
 * NOTES:
 *
 *	Reference: http://www.mathworks.com/help/matlab/ref/conv.html
******************************************************************/
#define PI	3.1415926

int convolution(double* array1, int npts1, double* array2,int npts2, double* array_out)
{
//npts1 signal array1
//npts2 kernel array2

	int i,j,n,k;



	for(n = 0; n < npts1 + npts2 -1 ; n ++)
	{

		array_out[n] = 0;

		int kmin;
		int kmax;

		kmin = (n>= npts2 -1)? n - (npts2 -1) : 0;
		kmax = (n< npts1 -1)? n : npts1 -1;

		for(k = kmin; k <= kmax ; k++)
		{
			array_out[n] += array1[k] * array2[n - k ];
		}

	}



	return 0;
}


int convolution2(double* array1, int npts1, double* array2, double* array_out)
{
//npts1 signal array1
//npts2 kernel array2

	int i,j,n,k;
	double array_tmp[2*npts1-1];

	for(n = 0; n < 2 *npts1  -1 ; n ++)
	{

		array_tmp[n] = 0;

		int kmin;
		int kmax;

		kmin = (n>= npts1 -1)? n - (npts1 -1) : 0;
		kmax = (n< npts1 -1)? n : npts1 -1;
//printf("kmin kmax is %d %d \n",kmin, kmax);
//printf("k n-1 is %d %d \n",k,n-k);

		for(k = kmin; k <= kmax ; k++)
		{
			array_tmp[n] += array1[k] * array2[n - k ];
		}

	}

	//puts("working on conv2");
	for(n = 0; n< npts1; n++ )
	{
		array_out[n] = array_tmp[n];
		//printf("tmp %lf out %lf \n", array_tmp[n*2], array_out[n]);
	}


	return 0;
}
