#include<stdlib.h>
#include<stdio.h>
#include<hongyulib.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>

// calculate and output the STD of ES
int output_STD_of_ES_with_stretch_record(new_RECORD* my_record,new_INPUT*  my_input,double* current_ES)
{
	fprintf(my_input->out_logfile, "---> output_STD_of_ES Begin");
	double amplitude();
	int phase_flag = 0;
	int ista;
	int pcount;
	int num;
	int npts_phase = (int)(my_input->phase_len / my_input->delta);
	double STD[npts_phase];
	FILE* out2;
	char out2_file[200];
	sprintf(out2_file,"%s_STD.out.xy", my_input->PHASE);
	int ii,jj,kk;
	out2 = fopen(out2_file,"w");

	double stretch_factor_record;
	double stretch_tmp[npts_phase];
	normalize_array(current_ES, npts_phase);

	for(pcount = 0; pcount < npts_phase ; pcount ++)
	{
		STD[pcount] = 0;
		num = 0;
		for(ista = 0; ista < my_input->sta_num; ista ++)
		{
			if(my_record[ista].quality != -1 && fabs(my_record[ista].SNR) > my_input->SNR_CUT && fabs(my_record[ista].best_stretch_ccc) > my_input->CCC_CUT )
			{
				// calculate the std
				double polar_flag;
				if(my_record[ista].polar_flag == 0)
					polar_flag = 1;
				else 
					polar_flag = my_record[ista].polar_flag;

				stretch_factor_record = 1/my_record[ista].best_stretch_coefficient;
				//stretch phase win using stretch factor
				stretch_ES_function(my_record[ista].phase_win, npts_phase, stretch_factor_record, stretch_tmp);
				normalize_array(stretch_tmp, npts_phase);

				// output stretched record into out2
				//for(ii = 0; ii < npts_phase; ii++)
					//fprintf(out2,"%lf %lf %d\n", ii *my_input->delta, stretch_tmp[ii],1);
				

				STD[pcount] += pow( (current_ES[pcount] - stretch_tmp[pcount] ) ,2 );
				num = num + 1;
			}	
		}
		STD[pcount] = sqrt( STD[pcount] / num );
	}

	for(ista = 0; ista < my_input->sta_num; ista ++)
		{
			if(my_record[ista].quality != -1 && fabs(my_record[ista].SNR) > my_input->SNR_CUT && fabs(my_record[ista].best_stretch_ccc) > my_input->CCC_CUT )
			{
				// calculate the std
				double polar_flag;
				if(my_record[ista].polar_flag == 0)
					polar_flag = 1;
				else 
					polar_flag = my_record[ista].polar_flag;

				stretch_factor_record = 1/my_record[ista].best_stretch_coefficient;
				//stretch phase win using stretch factor
				stretch_ES_function(my_record[ista].phase_win, npts_phase, stretch_factor_record, stretch_tmp);
				normalize_array(stretch_tmp, npts_phase);

				// output stretched record into out2
				for(ii = 0; ii < npts_phase; ii++)
					fprintf(out2,"%lf %lf %d\n", ii *my_input->delta, stretch_tmp[ii],1);
				

			}	
		}
	fclose(out2);



	// output 
	FILE* out;
	char out_file[200];
	sprintf(out_file,"%s_STD.out", my_input->PHASE);
	out = fopen(out_file, "w" );

	for(pcount = 0; pcount < npts_phase ; pcount++)
		fprintf(out," %lf %10.3f \n",pcount* my_input->delta, current_ES[pcount] + STD[pcount]);

	for(pcount = npts_phase-1; pcount >= 0 ; pcount--)
		fprintf(out," %lf %10.3f \n", pcount*my_input->delta, current_ES[pcount] - STD[pcount]);

	fclose(out);
	return 0;
}

