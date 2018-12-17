
#include "ESF.h"
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

int output_current_ES_for_phase_second(new_INPUT* my_input, double* new_ES)
{
fprintf(my_input->out_logfile,"--> output_current_ES_for_phase");

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
	wsac1(out_file,yarray, &npts_phase, &beg,&del, &nerr, strlen( out_file ));
	sprintf(out_file1,"%s_ES.second.out",my_input->PHASE);
	output_array2(out_file1, xarray, yarray_double, npts_phase,0);
	//output_array1(out_file1, new_ES, npts_phase);


	if(strcmp(my_input->PHASE,"S") ==0  || strcmp(my_input->PHASE,"P") ==0 )
	{
		strcpy(out_file1, "main_ES.out");
		output_array2(out_file1, xarray, yarray_double, npts_phase,0);
	}

	/*

	char command[300]= "csh bp_S_ES.sh";
	char sac_file[300] = "S_ES_bp.sac";
	// bp filter S empirical wavelet and write to S_ES_bp.sac
	if( strcmp(my_input->PHASE,"S") ==0 ) 
	{
		system(command);
		// read in sac file and print as S_ES_bp.out

		double sac_array[10000];
		double length = my_input->phase_len;

//printf("length is %lf npts_phase is %d\n", length, npts_phase);
		sac2xy(sac_file, 0, length, sac_array);

		// write out
		char out_file2[200];
		sprintf(out_file2,"%s_ES_bp.out",my_input->PHASE);
		output_array1(out_file2,sac_array,npts_phase);
	}

*/

	return 0;
}

