
#include "ESF.h"
// calculate and output the STD of ES
int output_ES_with_STD_loop(new_RECORD* my_record,new_INPUT*  my_input,double* current_ES, int loop_num)
{
	int ista;
	int pcount;
	int num;
	int npts_phase = (int)(my_input->phase_len / my_input->delta);
	double STD[npts_phase];


	for(pcount = 0; pcount < npts_phase ; pcount ++)
	{
		STD[pcount] = 0;
		num = 0;
		for(ista = 0; ista < my_input->sta_num; ista ++)
		{
			if(my_record[ista].quality != -1 && my_record[ista].weight > 0)
			{
				// calculate the std
				double polar_flag;
				if(my_record[ista].polar_flag == 0)
					polar_flag = 1;
				else 
					polar_flag = my_record[ista].polar_flag;
				STD[pcount] += pow( (current_ES[pcount] - my_record[ista].phase_win[pcount] ) ,2 );
				num = num + 1;
			}	
		}
		STD[pcount] = sqrt( STD[pcount] / num );
	}


	// output 
	FILE* out;
	char out_file[200];
	sprintf(out_file,"%s_STD.out.%d", my_input->PHASE,loop_num);
	out = fopen(out_file, "w" );
	for(pcount = 0; pcount < npts_phase ; pcount++)
		fprintf(out," %lf %10.3lf \n",pcount*my_input->delta, current_ES[pcount] + STD[pcount]);
	for(pcount = npts_phase-1; pcount >= 0 ; pcount--)
		fprintf(out," %lf %10.3lf \n", pcount*my_input->delta, current_ES[pcount] - STD[pcount]);
	fclose(out);

	char out_file1[200];
	float yarray[npts_phase];
	double yarray_double[npts_phase];
	double xarray[npts_phase];
	int i;
	for(i=0; i<npts_phase ;i++)
	{
		xarray[i] = i * my_input->delta;
		yarray[i] = current_ES[i];
		yarray_double[i] = current_ES[i];
	}
	sprintf(out_file1,"%s_ES.out.%d",my_input->PHASE,loop_num);

	output_array2(out_file1, xarray, yarray_double, npts_phase,0);


	return 0;
}

