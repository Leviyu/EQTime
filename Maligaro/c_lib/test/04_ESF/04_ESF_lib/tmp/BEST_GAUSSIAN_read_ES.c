#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<ESF.h>
/******************************************************************
 * This is a c script for 
 *
 *	Input:
 *	path		----- ES file path
 *	ES			----- ES store array
 *	len			----- ES length
 *
 *
 *	DATE: Sep 20 2014				Keywords:
******************************************************************/

int BEST_GAUSSIAN_read_ES(char* path, double* ES, int* len)
{
	int normalize_array();
	int count_file_num(char *filename);
	int file_line = count_file_num(path);
	*len = file_line;

	FILE* in;
	in = fopen(path,"r");
	int icount;
	for(icount = 0;icount< file_line; icount ++)
	{
		fscanf(in,"%lf\n",&ES[icount]);
	}

	fclose(in);


	// make all ES have positive peak
	normalize_array(ES,*len);	

	
	double AMP_posi = 0;
	double AMP_nega = 0;
	for(icount = 0; icount < *len ; icount ++)
	{
		if( ES[icount] > AMP_posi )
		{
			AMP_posi = ES[icount];
		}
		if( ES[icount] < AMP_nega )
		{
			AMP_nega = ES[icount];
		}
	}

	if( fabs(AMP_posi) < fabs(AMP_nega) )
	{
		for(icount = 0; icount < *len; icount++)
			ES[icount] = -1*ES[icount];
	}


	return 0;
}

