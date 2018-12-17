
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

int output_current_ES_for_phase(new_INPUT* my_input, double* new_ES)
{
puts("--> output_current_ES_for_phase");
	int sac2xy(char* sac_file, double begin_time, double length, double* out_array);
	int output_array1();
	int normalize_array(double* array, int len);
	char command[300]= "csh bp_S_ES.sh";
	char sac_file[300] = "S_ES_bp.sac";

// output ES here 
	int nerr,i;
	char out_file[200];
	char out_file1[200];
	int npts_phase = (int)(my_input->phase_len / my_input->delta );
	float beg  = 0;
	float del = my_input->delta;
	sprintf(out_file,"%s_ES.sac",my_input->phase);

	float yarray[npts_phase];
	for(i=0; i<npts_phase ;i++)
		yarray[i] = new_ES[i];
	wsac1(out_file,yarray, &npts_phase, &beg,&del, &nerr, strlen( out_file ));
	sprintf(out_file1,"%s_ES.out",my_input->phase);
	output_array1(out_file1, new_ES, npts_phase);


	// bp filter S empirical wavelet and write to S_ES_bp.sac
	if( strcmp(my_input->phase,"S") ==0 ) 
	{
		system(command);
		// read in sac file and print as S_ES_bp.out

		double sac_array[10000];
		double length = my_input->phase_len;

//printf("length is %lf npts_phase is %d\n", length, npts_phase);
		sac2xy(sac_file, 0, length, sac_array);

		// write out
		char out_file2[200];
		sprintf(out_file2,"%s_ES_bp.out",my_input->phase);
		output_array1(out_file2,sac_array,npts_phase);
	}


	return 0;
}

