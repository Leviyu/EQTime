#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>

int define_new_phase_window_len(new_RECORD* my_record, new_INPUT* my_input)
{
	int phase_len;
	if( strcmp(my_input->PHASE,"S") || strcmp(my_input->PHASE,"ScS"))
		phase_len = 10;
	else if (strcmp(my_input->PHASE,"ScSScS") || strcmp(my_input->PHASE,"SS") || strcmp(my_input->PHASE,"SSS") || strcmp(my_input->PHASE,"Sdiff"))
		phase_len = 30;


	my_input->phase_len = phase_len;
	my_record->phase_len = phase_len;
	my_record->npts_phase = (int) (phase_len / my_input->delta);



	return 0;
}
