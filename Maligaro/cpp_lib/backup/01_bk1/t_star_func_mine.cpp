#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<complex.h>
#include<hongyulib.h>

/*************************************************************
* This C function find the proper t star operator 
*	
*	INPUT:
*		double		t_star   tstar coefficient 
*		int 		npts  number of points of t star array
*		double 		delta  dt of t star array
*	OUTPUT
*		double* 	tstar_array		output array
*
* 	the attenuation part is defined as 
* 	A(w) = A0(w) * exp(-w*tstar/2)
* 	
* 	the t star is acquire by
*
* 	read( ifft( exp(-w*star/2) )  )
*
* 	where w is the frequency of array
* 	we only consider the range of w [0.01 2]
*	
*	Hongyu DATE: June 20 2014
*	Key words: line
*	reference: Introduction to Seismology (Peter Shearer, P165)
*************************************************************/

#define PI 3.1415926

int t_star_func_mine(double t_star, int npts , double delta, double* tstar_array)
{

	void IDFT(double* inreal, double* inimag, double* outreal, double* outimag, int n);
	//DFT(double* inreal, double* inimag, double* outreal, double* outimag, int n);
	int BINFD(int LX, int* LX1, int* N);
	int lhalf;
	int lx1;
	int np;
	int lcent;

	BINFD(npts, &lx1, &np);
//printf("lx1 np %d %d  \n", lx1, np);
	lhalf = lx1;
	lx1 = lx1*2;
	lcent = lhalf +1;

	double tt;
	tt = lx1*delta;
	double df;
	df = 1/ tt;


	int count;
	double Q_real[10000];
	double Q_imag[10000];
	Q_real[0] = 1;
	Q_imag[0] = 0;
	for(count = 1; count <= lcent; count++)
	{
		double f = count *df;
		double c = 2*f *t_star *( log(f) - 8*PI/t_star + PI*0.65 );
		double c2 = exp(-1*PI*f*t_star);
		double r = c2*cos(c);
		double ai = c2*sin(c);
		Q_real[count] = r;
		//Q_imag[count] = 0;
		Q_imag[count] = ai;
	}

	// do conj function 
	int NM1 = lcent -1;
	//for(count = lcent+1 ; count < np;count++)
	for(count = 1 ; count <= NM1;count++)
	{
		Q_real[lcent+count] = Q_real[lcent - count];
		Q_imag[lcent+count] = -Q_imag[lcent - count];
		//Q_real[count] = Q_real[2*lcent - count];
		//Q_imag[count] = -Q_imag[2*lcent - count];
	}

	Q_imag[lcent] = 0;

	// do fft to Q 
	int N = 100000;
	double array_in_real[N];
	double array_in_imag[N];
	double array_out_real[N];
	double array_out_imag[N];



	//IDFT( array_in_real, array_in_imag, array_out_real, array_out_imag , npts );
	//IDFT( Q_real, Q_imag, array_out_real, array_out_imag , npts );
	IDFT( Q_real, Q_imag, array_out_real, array_out_imag , pow(2,np));
	//IDFT( array_in_real, array_in_imag, array_out_real, tstar_array , npts);



	// construct tstar array
	
	for(count = 0; count < npts; count++)
	{
		tstar_array[count] = exp(PI*t_star)*array_out_real[count];
	}



	// make sure that we only have the first part
	//for(count = 0; count < npts; count ++)
	//{
		//tstar_array[count] = array_out_real[count];
	//}
	
	// make sure its symetric
	//for(count = 0 ; count < (int)(npts/2); count ++ )
	//{
		//tstar_array[count] = array_out_real[count];
	//}
	//for(count = (int)(npts/2) ; count < npts; count ++ )
	//{
		//tstar_array[count] = array_out_real[npts - count];
	//}


	return 0;
}

int BINFD(int LX, int* LX1, int* N)
{
	int count;

	int current_npts;


	if(LX <= 0 )
	{
		printf("ERROR LX is too small!");
		return 1;
	}



	for(count = 0; count < 100; count++)
	{
		
		current_npts = pow(2,count);
		if(current_npts > LX )
		{
			*N = count;
			*LX1 = current_npts;
			break;
		}
	}






	return 0;
}


