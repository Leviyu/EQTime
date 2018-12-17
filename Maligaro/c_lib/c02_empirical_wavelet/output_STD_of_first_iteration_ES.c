
#include "ESF.h"
// calculate and output the STD of ES
int output_STD_of_first_iteration_ES(new_RECORD* my_record,new_INPUT*  my_input,double* current_ES)
{
	double amplitude();
	int stretch_ES_function();
	int normalize_array();
	int phase_flag = 0;
	int ista;
	int pcount;
	int num;
	int npts_phase = (int)(my_input->phase_len / my_input->delta);
	double STD[npts_phase];
	double stretch_tmp[npts_phase];
	int ii;
	double stretch_factor_record;


	for(pcount = 0; pcount < npts_phase ; pcount ++)
	{
		STD[pcount] = 0;
		num = 0;
		for(ista = 0; ista < my_input->sta_num; ista ++)
		{
			if( my_record[ista].quality == -1 || my_record[ista].weight == 0)
				if ( my_input->Reprocessing_Flag != 1)
					continue;
			// calculate the std
			double polar_flag;
			if(my_record[ista].polar_flag == 0)
				polar_flag = 1;
			else 
				polar_flag = my_record[ista].polar_flag;
			STD[pcount] += pow( (current_ES[pcount] - my_record[ista].phase_win[pcount] ) ,2 );
			//printf("current STD is %lf \n", STD[pcount]);
			num = num + 1;
		}
		STD[pcount] = sqrt( STD[pcount] / num );
	}


	// output 
	FILE* out;
	char out_file[200];
	sprintf(out_file,"%s_STD.first_iteration.out", my_input->PHASE);
	out = fopen(out_file, "w" );

	for(pcount = 0; pcount < npts_phase ; pcount++)
		fprintf(out," %lf %10.3lf \n",pcount*my_input->delta, current_ES[pcount] + STD[pcount]);

	for(pcount = npts_phase-1; pcount >= 0 ; pcount--)
		fprintf(out," %lf %10.3lf \n", pcount*my_input->delta, current_ES[pcount] - STD[pcount]);

	fclose(out);
	return 0;
}

