
#include "../c01_tools/hongyulib.h"
#include "ESF.h"

int output_long_phase_window_distsum(new_RECORD* my_record, new_INPUT* my_input)
{
	fprintf(my_input->out_logfile,"---> Output long phase window! for distance sum\n");

	double distmin = my_input->distmin;
	double distmax = my_input->distmax;
	double distdelta = my_input->DIST_DELTA;


	int ista;
	int ii;
	int MAX = 100000;
	double distsum[MAX];
	double x_long[MAX];


	FILE* out;
	char out_file[200];
	sprintf(out_file,"distsum.stack_num.%s.%s", my_input->EQ, my_input->PHASE);
	out = fopen(out_file,"w");


	int count;
	for(count = 0; count < 180; count ++)
	{
		for(ii = 0; ii < my_record[2].npts_long; ii++)
		{
			x_long[ii] = 0;
			distsum[ii] = 0;
		}



		double dist_beg = distmin + count * distdelta;
		double dist_end = distmin + (count +1) * distdelta;

		// if distend > distmax break
		if(dist_beg > distmax) break;

		char dist_sum_name[200];


		


		int stack_num = 0;
		for(ista = 0; ista < my_input->sta_num; ista ++)
		{
			 //if(my_record->quality == -1 ) continue;
			double dist = my_record[ista].DIST;
			normalize_array(my_record[ista].long_win, my_record[ista].npts_long);
			if(dist > dist_beg && dist < dist_end)
			{

				for(ii = 0; ii< my_record[ista].npts_long; ii ++)
				{
					if(my_record[ista].long_win[ii] != my_record[ista].long_win[ii])
						continue;

					int shift_npts;
					shift_npts = (int) ( my_record[ista].dt_obs_prem / my_input->delta);
					int npts_new_p = ii + shift_npts;

					distsum[ii] += my_record[ista].long_win[npts_new_p];
				}
					stack_num ++;
			}
		}

		// normalize the long
		int jj;
		//double AMP = amplitude(distsum,my_record[2].npts_long);
		//printf("AMP is %lf \n", AMP);
		//normalize_array(distsum,my_record[2].npts_long);
		//for(jj = 0; jj< my_record[2].npts_long; jj++)
			//distsum[jj] = distsum[jj] / AMP;

		//for(jj = 0; jj< my_record[2].npts_long; jj++)
			//distsum[jj] += dist_beg;

		sprintf(dist_sum_name,"%s.%s.%s.distsum.%d", my_record->EQ,my_record->PHASE,my_record->COMP, count );

		fprintf(out, "%d %d\n", count, stack_num);



	int i;
	for(i=0;i<my_record[2].npts_long;i++)
		x_long[i] = my_record[2].long_beg + i*my_input->delta;
//printf("---> Woring on %lf %lf \n", dist_beg, dist_end);
	output_array2(dist_sum_name,x_long,distsum, my_record[2].npts_long, 0);


	

	}


	fclose(out);

	return 0;
}

