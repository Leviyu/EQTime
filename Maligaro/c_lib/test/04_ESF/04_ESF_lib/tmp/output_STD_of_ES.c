#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>
// calculate and output the STD of ES
int output_STD_of_ES(new_RECORD* my_record,new_INPUT*  my_input,double* current_ES)
{
	puts("---> output_STD_of_ES Begin");
	double amplitude();
	int phase_flag = 0;
	int ista;
	int pcount;
	int num;
	int npts_phase = (int)(my_input->phase_len / my_input->delta);
	double STD[npts_phase];


	// get AMP of ES and record
	double AMP_ES = amplitude(current_ES,npts_phase);

//printf("AMP of ES is %lf \n", AMP_ES);


	for(pcount = 0; pcount < npts_phase ; pcount ++)
	{
		STD[pcount] = 0;
		num = 0;
		for(ista = 0; ista < my_input->sta_num; ista ++)
		{
			double AMP_record = amplitude( my_record[ista].T[phase_flag].phase_win,npts_phase);
			if(my_record[ista].T[phase_flag].SNR == my_record[ista].T[phase_flag].SNR && AMP_ES != 0 && AMP_record != 0 && my_record[ista].T[phase_flag].polar_flag != 0 
					&& my_record[ista].T[phase_flag].SNR > 2.0 && fabs(my_record[ista].T[phase_flag].best_stretch_ccc) > 0.94)
			{
				// calculate the std
				STD[pcount] += pow( (current_ES[pcount] / AMP_ES - my_record[ista].T[phase_flag].phase_win[pcount] / AMP_record * my_record[ista].T[phase_flag].polar_flag ) ,2 );
				num = num + 1;
			}	
		}
		STD[pcount] = sqrt( STD[pcount] / num );
	}



	// output 
	FILE* out;
	char out_file[200];
	sprintf(out_file,"%s_STD.out", my_input->phase);
	out = fopen(out_file, "w" );

	for(pcount = 0; pcount < npts_phase ; pcount++)
		fprintf(out," %d %10.3f \n",pcount+1, current_ES[pcount] + STD[pcount]);

	for(pcount = npts_phase-1; pcount >= 0 ; pcount--)
		fprintf(out," %d %10.3f \n", pcount+1, current_ES[pcount] - STD[pcount]);

	fclose(out);
	return 0;
}

