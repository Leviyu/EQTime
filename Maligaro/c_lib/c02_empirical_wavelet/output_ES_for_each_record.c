
#include "hongyulib.h"
#include "ESF.h"
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
	printf("---> output_ES_for_each_record \n");
	int npts_phase;
	npts_phase = (int) (my_input->phase_len / my_input->delta );
	int ista,i;
	char ES_out[200];
	double ES_x[npts_phase],ES_y[npts_phase];
	double EW_amp;
	int EW_amp_loc;
	int flag = 1;

	for(ista = 0; ista < my_input->sta_num;ista++)
	{
		
		if(my_record[ista].beyong_window_flag == -1)
			continue;


		//printf(" %d /%d \n", ista, my_input->sta_num);
		amplitudeloc(my_record[ista].stretched_ES_win,npts_phase, &EW_amp_loc, &EW_amp, flag);
		if(EW_amp == 0)
			EW_amp = 1;
		//printf("record is %s long amp ratio is %lf \n", my_record[ista].name, my_record[ista].long_amplitude);

		sprintf(ES_out,"%s.%s.%s.%s.emp",my_record[ista].EQ, my_record[ista].name, my_record[ista].PHASE,my_record[ista].COMP);
		//for(i=0;i<npts_phase;i++)
		int increment = 5;
		// if is PHASE P, then make it 2
		if(strstr(my_input->PHASE,"P") != NULL )
			increment = 1;

		int new_npts_phase = (int)(npts_phase/increment);
		for(i=0;i<new_npts_phase;i++)
		{
			ES_x[i] = my_record[ista].phase_beg + i*increment*my_input->delta;
			if( my_record[ista].long_amplitude <=2 ) 
				ES_y[i] = my_record[ista].stretched_ES_win[i*increment] /EW_amp / fabs(my_record[ista].long_amplitude );
			else if (my_record[ista].long_amplitude >2 )
				ES_y[i] = my_record[ista].stretched_ES_win[i*increment] /EW_amp *0.5;

			//ES_y[i] = my_record[ista].stretched_ES_win[i] * my_record[ista].polar_flag;
		}
		//printf("output for stretched ES win \n");
		//output_array2(ES_out,ES_x,ES_y,npts_phase,1);
		output_array2(ES_out,ES_x,ES_y,new_npts_phase,1);

		// out put the stretched gaussian window
		sprintf(ES_out,"%s.%s.%s.%s.gau",my_record[ista].EQ, my_record[ista].name, my_record[ista].PHASE,my_record[ista].COMP);
		//printf("we out put %s here!!!!!\n", ES_out );

		//for(i=0;i<npts_phase;i++)
		for(i=0;i<new_npts_phase;i++)
		{
			ES_x[i] = my_record[ista].phase_beg + i*increment*my_input->delta;
			if(my_record[ista].long_amplitude <= 2)
				ES_y[i] = my_record[ista].stretched_gaussian_win[i*increment] / fabs(my_record[ista].long_amplitude) ; 
			else if(my_record[ista].long_amplitude > 2) 
				ES_y[i] = my_record[ista].stretched_gaussian_win[i*increment] *0.5 ; 
			//ES_y[i] = my_record[ista].stretched_ES_win[i] * my_record[ista].polar_flag;
		}
		//printf("output for stretched gau win \n");
		output_array2(ES_out,ES_x,ES_y,new_npts_phase,1);

		// also output EW_win without stretching
		//sprintf(ES_out,"%s.%s.%s.%s.orig.emp",my_record[ista].EQ, my_record[ista].name, my_record[ista].PHASE,my_record[ista].COMP);
		//for(i=0;i<npts_phase;i++)
		//{
			//ES_x[i] = my_record[ista].phase_beg + i*my_input->delta;
			//ES_y[i] = my_record[ista].ES_win[i];
			////ES_y[i] = my_record[ista].stretched_ES_win[i] * my_record[ista].polar_flag;
		//}
		//output_array2(ES_out,ES_x,ES_y,npts_phase,0);
		//printf(" DONE %d /%d \n", ista, my_input->sta_num);

	}

	return 0;
}

