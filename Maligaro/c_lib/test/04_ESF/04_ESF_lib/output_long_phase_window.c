#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<hongyulib.h>
#include<sacio.h>
#include<ESF.h>


int output_long_phase_window(new_RECORD* my_record, new_INPUT* my_input)
{
	fprintf(my_input->out_logfile,"--->Output long phase window!");

	int output_long_phase_window_for_each(new_RECORD* my_record, new_INPUT* my_input);
	int count;
	for(count = 0; count < my_input->sta_num; count ++)
	{
		//printf("output long and phase for %d record\n", count);
			output_long_phase_window_for_each(&my_record[count], my_input);
	}

	return 0;
}

int output_long_phase_window_for_each(new_RECORD* my_record, new_INPUT* my_input)
{
	char long_win_name[200];
	char phase_win_name[200];
	double x_long[100000];
	double x_phase[100000];



	// normalize both long and phase window with phase_win peak
	int max_loc;
	double amp;
	amplitudeloc(my_record->phase_win,  my_record->npts_phase,&max_loc, &amp, 1);
	if(amp == 0)
		amp = 1;

	// get x_long
	int i;
	for(i=0;i<my_record->npts_phase;i++)
	{
		my_record->phase_win[i] = my_record->phase_win[i] / amp;
		x_phase[i] = my_record->phase_beg + i*my_input->delta;
	}
	for(i=0;i<my_record->npts_long;i++)
	{
		my_record->long_win[i] = my_record->long_win[i] / amp;
		x_long[i] = my_record->long_beg + i*my_input->delta;
	}

	sprintf(long_win_name,"%s.%s.%s.%s.long", my_record->EQ,my_record->name,my_record->PHASE,my_record->COMP);
	sprintf(phase_win_name,"%s.%s.%s.%s.phase", my_record->EQ,my_record->name,my_record->PHASE,my_record->COMP);
	output_array2(long_win_name,x_long,my_record->long_win, my_record->npts_long, 1);
	output_array2(phase_win_name,x_phase,my_record->phase_win, my_record->npts_phase, 1);


	return 0;
}
