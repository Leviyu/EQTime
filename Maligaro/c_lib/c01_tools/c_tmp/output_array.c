#include "hongyulib.h"

/*************************************************************
* This C function returns the lines of input file
*	
*	Hongyu DATE: June 20 2014
*	Key words: line
*************************************************************/

double amplitude_tmp(double *p, int npts);

int output_array1( char* output_name, double* array1,int file_num)
{
	FILE* out;
	out=fopen(output_name,"w")	;
	int i;

	for(i = 0;i < file_num ; i++)
	{
		fprintf(out,"%15.8lf\n", array1[i]);
		fflush(out);												// output data from memory immediately
	}
//printf("data is outputed into %s\n", output_name);
	fclose(out);
		 return 0;
}


/***********************************************************
 * This C function output the 2 dimension array
 *
 * output_name ---- output name
 * array1 ---- first array name
 * array2 ---- second array name
 * file_num ---- file num 
 * normalize_flag ----- 0 will normalize the array2 to 1, others will not
 *
 * Hongyu
 * July 20 2014
 *
 * Key words: output array
***********************************************************/
int output_array2( char* output_name, double* array1,double* array2,int file_num, int normalize_flag)
{
	FILE* out;
	out=fopen(output_name,"w")	;
	int i;

	double amp = amplitude_tmp(array2,file_num);

	for(i = 0;i < file_num ; i++)
	{
		if(normalize_flag != 0 )
		{
			fprintf(out,"%10.6lf %15.8lf\n", array1[i], array2[i]);
			fflush(out);												// output data from memory immediately
		}
		else if (normalize_flag == 0 )
		{
			fprintf(out,"%10.6lf %15.8lf\n", array1[i], array2[i]/amp);
			fflush(out);												// output data from memory immediately
		}
/*
 *	Usually, when writing data to file, the program read in data into memory
 * 		when data is stacked to some degree, it then write them into the file
 *
 *	fflush() output the data in memory immediately into file
 */
	}
	fclose(out);
//printf("data is outputed into %s\n", output_name);
		 return 0;
}



/***********************************************************
 * This C function returns the amplitude of a float array.
 *
 * float *p   ---- Array pointer.
 * int   npts ---- Array npts.
 *
 * Shule Yu
 * Feb 21 2014
 *
 * Key words: amplitude, float
***********************************************************/

double amplitude_tmp(double *p, int npts){

    int count;
    double AMP=0.0;

    for (count=0;count<npts;count++){
        if (AMP<fabs(p[count])){
            AMP=fabs(p[count]);
        }
    }

    return AMP;
}
