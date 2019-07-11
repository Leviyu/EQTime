#include "hongyulib.h"
/*************************************************************
* This C function find the proper t star operator 
*	
*	INPUT:
*		double		t_star   tstar coefficient 
*		int 		npts  number of points of t star array
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

int t_star_find_uniq(double t_star , double* tstar_array, int npts, double delta,char* tstar_lib)
{
	char command[1000];
	sprintf(command, "cd %s; csh make_tstar.sh  %d %lf %.2lf", tstar_lib, npts, delta, t_star);

	char in_file[600];
	sprintf(in_file,"%s/TSTAR_%.2lf_ASCII",tstar_lib,t_star);

	FILE* in;
	in = fopen(in_file,"r");
	int count;
	char tmp1[200];
	char tmp2[200];

	fscanf(in,"%s %s",tmp1,tmp2);
	fscanf(in,"%s %s",tmp1,tmp2);
	for(count = 0; count < npts; count++)
	{
		fscanf(in,"%lf",&tstar_array[count]);
		//printf("%lf\n",tstar_array[npts]);
	}

	fclose(in);
	return 0;
}
/*
#define PI 3.1415926

int binfd(int LX, int* LX1, int* order);
int t_star_find_uniq(double t_star , double* tstar_array, int npts_out, double delta)
{
	int npts = npts_out;
	int lx1 = 0;
	int np = 0;

	bindf(npts, &lxa, &np);

	
	int lhalf = lx1;
	lx1 = 2*lx1;
	np = np+1;
	int lcent = lhalf+1;
	double tt = lx1*delta;
	double df = 1.0/tt;
	double fn = df*lhalf;

	int count;
	double f;
	double c;
	double c2;
	double r;
	double ai;

	
	for(count = 2; count < lcent; count ++)
	{
		f = (count-1)*df;
		c = 2*f*t_star*(log(f) - PI*8/t_star+PI*0.65);
		c2 = exp(-1*PI*f*t_star);
		r = c2*cos(c);
		ai = c2*sin(c);
	}



	return 0;
}


int COOL(int NN,double* DATAI, double SIGNI)
{
	double TEMPR;
	double TEMPI;
	double MMAX;

	int N = pow(2,NN+1);
	int J = 1;
	int I;
	for(I = 1; I < N; I = I+2)
	{
		if(I -J < 0)
		{
			TEMPR = DATAI(J);
			TEMPI = DATAI(J+1);
			DATAI(J) = DATAI(I);
			DATAI(J+1) = DATAI(I+1);
			DATAI(I) = TEMPR;
			DATAI(I+1) = TEMPI;
		}
		else 
		{
			M = N/2;
		}


		if(J - M <= 0)
		{
			J = J+M;
			MMAX =2;

		}
		else
		{
			J = J -M;
			M = M/2;
		}

		if(M-2 <0)
		{
			J = J+M;
			MMAX = 2;

		}
		else
		{

		}


	}




	return 0;
}


// LX is the input npts
// LX1 is 2^ npts
// npts is the order 
int binfd(int LX, int* LX1, int* order)
{
	int count;
	int max_npts;
	for(count = 1; count < 20; count ++)
	{

		max_npts = pow(2,count);
		if( LX > max_npts )
			coutinue;
		else
		{
			*LX1 = max_npts;
			*order = count;
			break;
		}
	}
	return 0;
}


*/

/*
int t_star_find_uniq(double t_star , double* tstar_array, int npts_out)
{
	int npts = 2*npts_out;
	//int npts = 200;

	void IDFT(double* inreal, double* inimag, double* outreal, double* outimag, int n);
	//DFT(double* inreal, double* inimag, double* outreal, double* outimag, int n);


	int N = 100000;
	double real1;
	double imag1;

	double array_in_real[N];
	double array_in_imag[N];
	double array_out_real[N];
	double array_out_imag[N];


	// define frequency
	double w;
	int count;

	double w_beg = 0;
	//double w_end = 0.002;
	//double w_end = 2;
	double w_end = 10;
	//double w_beg = 0.01;
	//double w_end = 0.05;

	double w_delta = (w_end - w_beg) /  npts;

	for(count = 0; count < npts ; count ++)
	{
		w = w_beg + count*w_delta;
		array_in_real[count] = exp(-2*3.1415926*w*t_star / 2);
		array_in_imag[count] = 0;
	}


	//IDFT( array_in_real, array_in_imag, array_out_real, array_out_imag , npts );
	IDFT( array_in_real, array_in_imag, tstar_array, array_out_imag , npts );
	//IDFT( array_in_real, array_in_imag, array_out_real, tstar_array , npts);

	// make sure that we only have the first part
	//for(count = 0; count < npts; count ++)
	//{
		//if(count < npts_out)
		//tstar_array[count] = 0;
		//else 
		//tstar_array[count] *= 2;

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

*/
