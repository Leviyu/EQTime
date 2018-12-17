#include "ESF.h"
// calculate and output the STD of ES
int output_STD_of_second_ES(new_RECORD* my_record,new_INPUT*  my_input,double* current_ES)
{
	printf("---> output_STD_of_ES second Begin\n");
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
	double weight;
	double weight_sum = 0;
	double stretch_factor_record;




	//int amplitudeloc(double* array, int len, int* max_amp_loc, double* amplitude, int flag) 
	int max_loc;
	double amp_ES;
	double amp_phase;
	amplitudeloc(current_ES, npts_phase, &max_loc, &amp_ES,1);
	if(amp_ES == 0 ) amp_ES = 1;


	for(pcount = 0; pcount < npts_phase ; pcount ++)
	{
		STD[pcount] = 0;
		num = 0;
		weight_sum = 0;
		for(ista = 0; ista < my_input->sta_num; ista ++)
		{

			//if(my_record[ista].quality > 0 )
			//{
				// calculate the std
				//weight = my_record[ista].weight;
				weight = my_record->weight;
				if(weight == 0 && my_input->Reprocessing_Flag != 1)
					continue;
				amplitudeloc(my_record[ista].stretched_phase_win, npts_phase, &max_loc, &amp_phase,1);
				if(amp_phase == 0) amp_phase = 1;
				//normalize_array(my_record[ista].stretched_phase_win,npts_phase);
				STD[pcount] += pow( (current_ES[pcount]/ amp_ES - 
							my_record[ista].stretched_phase_win[pcount]  / amp_phase) ,2 ) * weight;
				weight_sum += weight;
				num = num + 1;
			//}	
		}
		if(num == 0)
		{
			STD[pcount] = 0;
			continue;
		}
		STD[pcount] = sqrt( STD[pcount] / num );
	}


	FILE* out2;
	char out2_file[200];
	sprintf(out2_file,"%s_STD.out.second.xy", my_input->PHASE);
	out2 = fopen(out2_file,"w");
	for(ista = 0; ista < my_input->sta_num; ista ++)
		{
			if(my_record[ista].quality != -1 && my_record[ista].weight > 0)
			{
				// calculate the std

				// output stretched record into out2
				for(ii = 0; ii < npts_phase; ii++)
					fprintf(out2,"%lf %lf %d\n", ii *my_input->delta, my_record[ista].stretched_phase_win[ii],1);
			}	
		}
	fclose(out2);

	// output 
	FILE* out;
	char out_file[200];
	sprintf(out_file,"%s_STD.second.out", my_input->PHASE);
	out = fopen(out_file, "w" );

	for(pcount = 0; pcount < npts_phase ; pcount++)
		fprintf(out," %lf %10.3lf \n",pcount*my_input->delta, current_ES[pcount] / amp_ES + STD[pcount]);

	for(pcount = npts_phase-1; pcount >= 0 ; pcount--)
		fprintf(out," %lf %10.3lf \n", pcount*my_input->delta, current_ES[pcount] / amp_ES - STD[pcount]);

	fclose(out);
	return 0;
}

