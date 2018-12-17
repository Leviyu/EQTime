#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<ESF.h>
/******************************************************************
 * This is a c script to creat the Gaussian Family and find the best 
 * matching ES
 *
 *	Input:
 *	double* new_ES 			---- new_ES fixed to 0 -1
 *	int npts_new_ES			---- npts_of new_ES array
 *
 * 	int npts_Gaussian		---- npts of Gaussian we want
 *
 *	OUTPUT
 *	double* ccc				---- best ccc between the best Gaussian and the new ES
 *	double*	best_Gaussian	---- the best Gaussian
 *
 *
 *
 *
 *	DATE: Sep 20 2014				Keywords:
******************************************************************/



int BEST_GAUSSIAN_construct_Gaussian(double* old_ES, int npts_old_ES, int  npts_Gaussian, double* ccc, double* best_Gaussian)
{
	double gaussian_func(double a, double b, double c, double d, double x);
	int CCC( double* x, int npts_x, double* y, int npts_y, int*shift, double* ccc, int flag);
	int output_array1( char* output_name, double* array1,int file_num);
	int amplitudeloc(double* array, int len, int* max_amp_loc, double* amplitude, int flag);



	//define the Gaussian family
	double a = 1;
	double b = 1000 / 2;
	double c;
	double d = 0;

	int icount, jcount, kcount;

	double** Gaussian;
	int NUM_GAUSSIAN = 40;
	Gaussian = (double**)malloc(sizeof(double*)*NUM_GAUSSIAN);
	for(icount = 0 ; icount < NUM_GAUSSIAN; icount ++ )
			Gaussian[icount] = (double*)malloc(sizeof(double)*npts_Gaussian);

	double cc[NUM_GAUSSIAN];
	int npts_shift[NUM_GAUSSIAN];
	// output ccc info
	FILE* out;
	out = fopen("CCC_ES_Gaussian.info","w");

	// the c parameter determines the shape of Gaussian
	for(icount = 0; icount < NUM_GAUSSIAN; icount ++)
	{
		// new gaussian formed.
		c = 20 + 2*icount;
		
		for(jcount = 0; jcount < npts_Gaussian; jcount ++)
		{
			Gaussian[icount][jcount] = gaussian_func(a,b,c,d,jcount);
//printf("Gaussian i  %d j  %d is %lf \n", icount, jcount, Gaussian[icount][jcount]);
		}

		double cut_level = 0.1;
		
		double Gaussian_tmp[npts_Gaussian];
		double ES_tmp[npts_old_ES];
		// initiate
		for(jcount = 0; jcount < npts_Gaussian; jcount ++)
			Gaussian_tmp[jcount] = 0.0;
		for(jcount = 0; jcount < npts_old_ES; jcount ++)
			ES_tmp[jcount] = 0.0;
		

		// construct a new Gaussian with half phase
		// find its peak 
		int max_loc;
		double max_amp;
		amplitudeloc(Gaussian[icount], npts_Gaussian, &max_loc, &max_amp,0);
		for(jcount = max_loc; jcount >=0 ; jcount --)
		{
			if( Gaussian[icount][jcount] > cut_level * 1 )
				Gaussian_tmp[jcount] = Gaussian[icount][jcount];
		}


		// construct a new ES with half phase
		amplitudeloc(old_ES, npts_old_ES, &max_loc, &max_amp,0);
		for(jcount = max_loc; jcount >=0; jcount--)
		{
			if(old_ES[jcount] > cut_level * 1)
				ES_tmp[jcount] = old_ES[jcount];
		}



		// ccc this Gaussian with ES
		CCC(ES_tmp  , npts_old_ES, Gaussian_tmp,  npts_Gaussian, &npts_shift[icount], &cc[icount],0 );


		// output new_ES and Gaussian_tmp
		char out_new_ES[20];
		char out_new_Gaussian[20];

		sprintf(out_new_ES,"output_new_ES_%d", icount);
		sprintf(out_new_Gaussian,"output_new_Gaussian_%d", icount);


		output_array1(out_new_ES,ES_tmp,npts_old_ES);
		output_array1(out_new_Gaussian,Gaussian_tmp,npts_Gaussian);
		
		fprintf(out,"%5d  %7.3lf  %5d \n", icount, cc[icount], npts_shift[icount]);
		
//printf("icount is %d ccc is %lf \n",  icount, cc[icount]);
	}

	fclose(out);	

	// find the best ccc and return it
	double ccc_max = 0;
	int best_index;
	for(icount = 0; icount < NUM_GAUSSIAN ; icount ++)
	{
		
		if(fabs(cc[icount]) > ccc_max)
		{
			ccc_max = cc[icount];
			best_index = icount;
		}
	}


	*ccc = ccc_max;
	for(icount = 0 ; icount< npts_Gaussian; icount++)
		best_Gaussian[icount] = Gaussian[best_index][icount];

	// output the best gaussian
	output_array1("best_Gaussian.out",best_Gaussian,npts_Gaussian);




	return 0;
}
