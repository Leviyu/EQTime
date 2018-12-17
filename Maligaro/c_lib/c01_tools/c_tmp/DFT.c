#include<hongyulib.h>
/******************************************************************
 * This is a c script  impliment fourier transfrom to a array
 *
 *	Input: 
 *	double* 			x
 *	int 				npts
 *
 *	Output:
 *	double*				X
 *
 * NOTES:
 *
 * Assume input is f(x)
 *
 * 	X(k) = SUM( x(n)*exp(-j*2*pi*n/N*k)   )
 *
 *
 *	DATE:Nov 4 2014				Keywords: fourier transform
******************************************************************/
#define PI	3.1415926


void DFT(double* inreal, double* inimag, double* outreal, double* outimag, int n)
{
	int k;
	for (k = 0; k < n; k++)
	{  /* For each output element */
			double sumreal = 0;
			double sumimag = 0;
			int t;

			for (t = 0; t < n; t++)
			{  /* For each input element */
				double angle = 2 * PI * t * k / n;
				sumreal +=  inreal[t] * cos(angle) + inimag[t] * sin(angle);
				sumimag += -inreal[t] * sin(angle) + inimag[t] * cos(angle);
			}
			outreal[k] = sumreal;
			outimag[k] = sumimag;
	}
}



void IDFT(double* inreal, double* inimag, double* outreal, double* outimag, int n)
{
	int k;
	for (k = 0; k < n; k++)
	{  /* For each output element */
			double sumreal = 0;
			double sumimag = 0;
			int t;

			for (t = 0; t < n; t++)
			{  /* For each input element */
				double angle = -2 * PI * t * k / n;
				sumreal +=  inreal[t] * cos(angle) + inimag[t] * sin(angle);
				sumimag += -inreal[t] * sin(angle) + inimag[t] * cos(angle);
				//sumreal +=  inreal[t] * cos(angle) - inimag[t] * sin(angle);
				//sumimag +=  inreal[t] * sin(angle) + inimag[t] * cos(angle);
			}
			outreal[k] = sumreal/n;
			outimag[k] = sumimag/n;
	}
}
