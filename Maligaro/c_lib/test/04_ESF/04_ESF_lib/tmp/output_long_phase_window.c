#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>

int output_long_phase_window(new_RECORD* my_record, int phase_flag, new_INPUT* my_input)
{
	char long_win_name[200];
	char phase_win_name[200];
	double x_long[100000];
	double x_phase[100000];
	int output_array2();

	sprintf(long_win_name,"%s.%s.%s.%s.long", my_record->eq_name,my_record->name,my_record->T[phase_flag].phase_name,my_record->COMP);
	sprintf(phase_win_name,"%s.%s.%s.%s.phase", my_record->eq_name,my_record->name,my_record->T[phase_flag].phase_name,my_record->COMP);
//printf("OUTPUT: %s %s\n",long_win_name,phase_win_name);

	// get x_long
	int i;
	for(i=0;i<my_record->T[phase_flag].npts_long;i++)
		x_long[i] = my_record->T[phase_flag].long_beg + i*my_input->delta;
	for(i=0;i<my_record->T[phase_flag].npts_phase;i++)
		x_phase[i] = my_record->T[phase_flag].phase_beg + i*my_input->delta;

	output_array2(long_win_name,x_long,my_record->T[phase_flag].long_win, my_record->T[phase_flag].npts_long, 0);
	output_array2(phase_win_name,x_phase,my_record->T[phase_flag].phase_win, my_record->T[phase_flag].npts_phase, 0);


	return 0;
}
