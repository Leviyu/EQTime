
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>

/******************************************************************
 * This is a c script for output current empirical source
 *
 *	Input:
 *			1. 
 *			2.
 *
 *
 *	Output:
 *
 *
 *	DATE:				Keywords:
 *	Reference:
******************************************************************/

int output_current_ES_for_phase_third(new_INPUT* my_input, double* new_ES)
{
fprintf(my_input->out_logfile, "--> output_current_ES_for_phase 3rd");

	int output_array2( char* output_name, double* array1,double* array2,int file_num, int normalize_flag);
// output ES here 
	int nerr,i;
	char out_file[200];
	char out_file1[200];
	int npts_phase = (int)(my_input->phase_len / my_input->delta );
	float beg  = 0;
	float del = my_input->delta;
	sprintf(out_file,"%s_ES.sac",my_input->PHASE);

	float yarray[npts_phase];
	double yarray_double[npts_phase];
	double xarray[npts_phase];
	for(i=0; i<npts_phase ;i++)
	{
		xarray[i] = i * my_input->delta;
		yarray[i] = new_ES[i];
		yarray_double[i] = new_ES[i];
	}
	sprintf(out_file1,"%s_ES.third.out",my_input->PHASE);

	output_array2(out_file1, xarray, yarray_double, npts_phase,0);
	//output_array1(out_file1, new_ES, npts_phase);



	return 0;
}

