#include "hongyulib.h"
/******************************************************************
 * This is a c script  impliment hilbert transfrom to a array
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
 * 	1. It calculates the FFT of the input sequence, storing the result in a vector x.
 *
 * 	It creates a vector h whose elements h(i) have the values:
 *
 * 	1 for i = 1, (n/2)+1
 *
 * 	2 for i = 2, 3, ... , (n/2)
 *
 * 	0 for i = (n/2)+2, ... , n
 *
 * 	It calculates the element-wise product of x and h.
 *
 * 	It calculates the inverse FFT of the sequence obtained in step 3 and returns the first n elements of the result.
 *
 *	DATE:Nov 4 2014				Keywords: hilbert transfrom
 *	Reference: Mathwork hilbert page
******************************************************************/
#define PI	3.1415926

int hilbert(double* array_in, int npts, double* array_out)
{
	void DFT(double* inreal, double* inimag, double* outreal, double* outimag, int n);
	void IDFT(double* inreal, double* inimag, double* outreal, double* outimag, int n);

	double inimag[npts];
	double outreal[npts];
	double outimag[npts];

	int i;
	for(i=0;i<npts;i++)
	{
		inimag[i] = 0;
		outreal[i] = 0;
		outimag[i] = 0;
	}


	//1.
	DFT(array_in,inimag,outreal,outimag,npts);

	//2.
	double h[npts];
	for(i=0;i<npts;i++)
	{
		if(i == 1 || i == floor(npts/2)+1 )
			h[i] = 1;
		else if(i >=2 && i <= floor(npts/2) )
			h[i] = 2;
		else
			h[i] = 0;
	}

	
	//3.
	double xhreal[npts];
	double xhimag[npts];
	for(i=0;i<npts;i++)
	{
		xhreal[i] = outreal[i]*h[i];
		xhimag[i] = outimag[i]*h[i];
	}


	//4.
	//IDFT(xhreal,xhimag,outreal,outimag,npts);
	IDFT(xhreal,xhimag,outreal,array_out,npts);

	


	return 0;
}


