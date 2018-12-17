
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>

int output_ES_for_each_record(new_RECORD* my_record, new_INPUT* my_input, int phase_flag, double phase_len)
{
	puts("---> output_ES_for_each_record ");
	int output_array2();
	int npts_phase;
	npts_phase = (int) (phase_len / my_input->delta );
	int ista,i;
	for(ista = 0; ista < my_input->sta_num;ista++)
	{
		// skip those with nan SNR
		if( my_record[ista].T[phase_flag].SNR != my_record[ista].T[phase_flag].SNR  )
			continue;
		char ES_out[200];
		sprintf(ES_out,"%s.%s.%s.%s.emp",my_record[ista].eq_name, my_record[ista].name, my_record[ista].T[phase_flag].phase_name, 
				my_record[ista].COMP);
		double ES_x[npts_phase],ES_y[npts_phase];
//printf("OUTPUT ES FOR each record ista is %d \n", ista);
		for(i=0;i<npts_phase;i++)
		{
			ES_x[i] = my_record[ista].T[phase_flag].phase_beg + i*my_input->delta;
			ES_y[i] = my_record[ista].T[phase_flag].stretched_ES_win[i];
			//ES_y[i] = my_record[ista].T[phase_flag].stretched_ES_win[i] * my_record[ista].T[phase_flag].polar_flag;
		}
		output_array2(ES_out,ES_x,ES_y,npts_phase);
	}

	return 0;
}

