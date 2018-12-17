#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>
// calculate and output the STD of ES
int output_STD_of_third_ES(new_RECORD* my_record,new_INPUT*  my_input,double* current_ES)
{
	fprintf(my_input->out_logfile,"---> output_STD_of_ES 3rd Begin\n");
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

	double weight;
	double weight_sum = 0;

	for(pcount = 0; pcount < npts_phase ; pcount ++)
	{
		STD[pcount] = 0;
		num = 0;
		weight_sum = 0 ;
		for(ista = 0; ista < my_input->sta_num; ista ++)
		{
			if(my_record[ista].quality == 1 )
			{
				// calculate the std
				//weight = my_record[ista].weight;
				weight = get_weight_from_SNR_CCC( my_record[ista].SNR, my_record[ista].best_tstar_ccc);
				//STD[pcount] += pow( (current_ES[pcount] - my_record[ista].phase_win[pcount] ) ,2 ) * weight;
				STD[pcount] += pow( (current_ES[pcount] - my_record[ista].stretched_phase_win[pcount] ) ,2 ) * weight;
				weight_sum += weight;
				num = num + 1;
			}	
		}
		if(weight_sum == 0)
		{
			STD[pcount] = 0;
			continue;
		}
		STD[pcount] = sqrt( STD[pcount] / weight_sum );
	}


	FILE* out2;
	char out2_file[200];
	sprintf(out2_file,"%s_STD.out.third.xy", my_input->PHASE);
	out2 = fopen(out2_file,"w");
	for(ista = 0; ista < my_input->sta_num; ista ++)
		{
			if(my_record[ista].quality == 1 )
			{
				// calculate the std

				// output stretched record into out2
				for(ii = 0; ii < npts_phase; ii++)
					fprintf(out2,"%lf %lf %d\n", ii *my_input->delta, my_record[ista].stretched_phase_win[ii],1);
					//fprintf(out2,"%lf %lf %d\n", ii *my_input->delta, my_record[ista].phase_win[ii],1);
			}	
		}
	fclose(out2);

	// output 
	FILE* out;
	char out_file[200];
	sprintf(out_file,"%s_STD.third.out", my_input->PHASE);
	out = fopen(out_file, "w" );

	for(pcount = 0; pcount < npts_phase ; pcount++)
		fprintf(out," %lf %10.3lf \n",pcount*my_input->delta, current_ES[pcount] + STD[pcount]);

	for(pcount = npts_phase-1; pcount >= 0 ; pcount--)
		fprintf(out," %lf %10.3lf \n", pcount*my_input->delta, current_ES[pcount] - STD[pcount]);

	fclose(out);
	return 0;
}

