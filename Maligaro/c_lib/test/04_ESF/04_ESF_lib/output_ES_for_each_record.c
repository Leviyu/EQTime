
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>
/******************************************************************
 * This is a c script to output the ES of each record in seperate file
 * each file is attached time info (relative to PREM)
 *
 *	Input:
 *
 *
 *	Output:
 *
 *
 *	DATE:				Keywords:
 *	Reference:
******************************************************************/

int output_ES_for_each_record(new_RECORD* my_record, new_INPUT* my_input)
{
	fprintf(my_input->out_logfile,"---> output_ES_for_each_record \n");
	int npts_phase;
	npts_phase = (int) (my_input->phase_len / my_input->delta );
	int ista,i;
	char ES_out[200];
	double ES_x[npts_phase],ES_y[npts_phase];
	for(ista = 0; ista < my_input->sta_num;ista++)
	{
		// skip those with nan SNR
		if( my_record[ista].quality == -1)
			continue;
		sprintf(ES_out,"%s.%s.%s.%s.emp",my_record[ista].EQ, my_record[ista].name, my_record[ista].PHASE,my_record[ista].COMP);
		for(i=0;i<npts_phase;i++)
		{
			ES_x[i] = my_record[ista].phase_beg + i*my_input->delta;
			ES_y[i] = my_record[ista].stretched_ES_win[i];
			//ES_y[i] = my_record[ista].stretched_ES_win[i] * my_record[ista].polar_flag;
		}
		output_array2(ES_out,ES_x,ES_y,npts_phase,0);

		// out put the stretched gaussian window
		sprintf(ES_out,"%s.%s.%s.%s.gau",my_record[ista].EQ, my_record[ista].name, my_record[ista].PHASE,my_record[ista].COMP);
		//printf("we out put %s here!!!!!\n", ES_out );
		for(i=0;i<npts_phase;i++)
		{
			ES_x[i] = my_record[ista].phase_beg + i*my_input->delta;
			ES_y[i] = my_record[ista].stretched_gaussian_win[i];
			//ES_y[i] = my_record[ista].stretched_ES_win[i] * my_record[ista].polar_flag;
		}
		output_array2(ES_out,ES_x,ES_y,npts_phase,0);

		// also output EW_win without stretching
		//sprintf(ES_out,"%s.%s.%s.%s.orig.emp",my_record[ista].EQ, my_record[ista].name, my_record[ista].PHASE,my_record[ista].COMP);
		//for(i=0;i<npts_phase;i++)
		//{
			//ES_x[i] = my_record[ista].phase_beg + i*my_input->delta;
			//ES_y[i] = my_record[ista].ES_win[i];
			////ES_y[i] = my_record[ista].stretched_ES_win[i] * my_record[ista].polar_flag;
		//}
		//output_array2(ES_out,ES_x,ES_y,npts_phase,0);

	}

	return 0;
}

