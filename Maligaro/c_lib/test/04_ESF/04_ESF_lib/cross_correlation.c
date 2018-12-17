#include<stdio.h>
#include<stdlib.h>
#include<math.h>

/*************************************************************
* This C function calculate the cross correlation coefficient of two traces
*	
*	double* x 			Master trace
*	double* y 			shifting trace
*	int npts_x, npts_y 	points of x and y
*	shift 				the shifted points of traces
*	doublle* ccc		ccc returned
*	flag				1 == return the positive ccc -- we dont flip the shifting trace
*						-1 return the negative ccc	 -- we flip the shifting trace
*						0 returns the bigger one of the above two number
*
*	Algorithm used is below:
*				SUM{ (x - x_average)*(y - y_average) }
*	CCC =    ---------------------------------------------
*				sqrt{SUM[(x - x_ave)**2]*SUM[(y - y_ave)**2 ]}
*
*	Hongyu DATE: July 29 2014
*	Key words: ccc
*************************************************************/
int CCC_posi(double* x, int npts_x, double* y, int npts_y, int* shift, double* ccc , int flag);
int CCC( double* x, int npts_x, double* y, int npts_y, int*shift, double* ccc, int flag)
{
	int i,j;
	double x_tmp[npts_x +1];
	double y_tmp[npts_y +1];
	int shift_posi, shift_nega;
	double ccc_posi, ccc_nega;


	//for positive ccc ---- we dont flip the trace
	for( i = 0; i < npts_x; i++)
		x_tmp[i] = x[i];

	for(i = 0;i<npts_y; i++)
		y_tmp[i] = y[i];

	CCC_posi(x_tmp,npts_x, y_tmp, npts_y, &shift_posi, &ccc_posi, 1);

	// for negative ccc --- we flip the trace
	for( i = 0; i < npts_x; i++)
		x_tmp[i] = x[i];

	for(i = 0;i<npts_y; i++)
		y_tmp[i] = -1.0* y[i];

	CCC_posi(x_tmp,npts_x, y_tmp, npts_y, &shift_nega, &ccc_nega, 1);
	ccc_nega = -1*ccc_nega;


//printf("positive CCC is %lf negative CCC is %lf\n", ccc_posi, ccc_nega);
	if ( flag == 1 )
	{
		*shift = shift_posi;
		*ccc = ccc_posi;
	}
	else if (flag == -1)
	{
		*shift = shift_nega;
		*ccc = ccc_nega;
	}
	else	
	{
		if( fabs(ccc_posi) >= fabs(ccc_nega))
		{
			*shift = shift_posi;
			*ccc = ccc_posi;
		}
		else if ( fabs(ccc_posi) < fabs(ccc_nega) ) 
		{	
			*shift = shift_nega;
			*ccc = ccc_nega;
		}
	}

	return 0;

}



// flag == 1 return the positive ccc
// flag == -1 return the negative ccc

int CCC_posi(double* x, int npts_x, double* y, int npts_y, int* shift, double* ccc , int flag)
{
	int i,j,count, i_sum;
	int i_x, i_y;
	double x_ave=0;
	double y_ave = 0;
	int npts_shift;
	double stdsum_x = 0;
	double stdsum_y = 0;
	double y_flip[npts_y+1];
	double denom;


	// go for positive ccc
	// 1. check if array contain nan value
	for(i=0; i< npts_x; i++)
	{
		if( x[i] != x[i] )
		{
			//printf("x[%d] contain nan value!\n", i);
			x[i] = 0;
		}
		x_ave += x[i];
	}
	x_ave = x_ave / npts_x;


	for(i=0; i< npts_y; i++)
	{
		if( y[i] != y[i] )
		{
			//printf("y[%d] contain nan value!\n", i);
			y[i] = 0;
		}
		y_ave += y[i];
	}
	y_ave = y_ave / npts_y;


	double ccc_posi[npts_x+ npts_y ];
	for(i = 0; i< npts_x + npts_y ; i ++)
		ccc_posi[i] = 0;
	stdsum_x = 0;
	stdsum_y = 0;

	for(i=0; i< npts_x; i++)
	{
		stdsum_x += pow( (x[i] - x_ave),2);
	}

	for(i=0; i< npts_y; i++)
	{
		stdsum_y += pow( (y[i] - y_ave),2);
	}

	denom = sqrt(stdsum_x * stdsum_y);
//printf("denom is %lf\n", denom);


	for(npts_shift = -1* ( npts_y - 1) ; npts_shift < npts_x  -1; npts_shift ++)
	{
		for( i_x = 0; i_x < npts_x; i_x ++)
		{
			i_y = i_x - npts_shift;

			if(i_y < 0 || i_y > npts_y -1)
				continue;
			else 
				ccc_posi[npts_shift + npts_y - 1] += ( x[i_x] - x_ave) * (y[i_y] - y_ave) / denom;
		}
	}


	double max_ccc_posi = 0;
	int npts_shift_posi;

	if(flag == 1)
	{
		for(i = 0; i< npts_x + npts_y -1 ; i++)
		{
	//if( fabs(ccc_posi[i]) > fabs( max_ccc_posi)  )
			if( ccc_posi[i] >  max_ccc_posi )
			{
				max_ccc_posi = ccc_posi[i];
				npts_shift_posi = -1*(npts_y - 1 - i);
				//printf(" posti npts is %d max ccc is %lf \n", i, max_ccc_posi);
			}
		}

	*shift = npts_shift_posi;
	*ccc = max_ccc_posi;
	}
	else if (flag == -1)
	{
		max_ccc_posi =0;
		for(i = 0; i< npts_x + npts_y -1 ; i++)
		{
	//if( fabs(ccc_posi[i]) > fabs( max_ccc_posi)  )
			if( ccc_posi[i] <  max_ccc_posi )
			{
				max_ccc_posi = ccc_posi[i];
				npts_shift_posi = -1*(npts_y - 1 - i);
				//printf(" posti npts is %d max ccc is %lf \n", i, max_ccc_posi);
			}
		}

	*shift = npts_shift_posi;
	*ccc = max_ccc_posi;
	}

	return 0;
}











