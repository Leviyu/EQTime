#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>

int read_noise_window_relative_to_S(new_RECORD* my_record, new_INPUT* my_input)
{

	char noise_tmp_file[200];
	sprintf(noise_tmp_file,"noise_tmp_file_for_S_Sdiff.data");

	FILE* in;

	if(strcmp(my_record->PHASE,"S")==0 )
	{
		in = fopen(noise_tmp_file,"w");

	}
	double long_beg = my_record->long_beg;
	double long_len = my_record->long_len;
	double phase_beg = my_record->noise_beg;
	double phase_len = my_record->noise_len;

	int npts_phase_beg = (int)((phase_beg - long_beg)/my_input->delta);
	int i;
	int npts_phase_len = (int)(phase_len/my_input->delta);
	for(i=0;i<npts_phase_len;i++)
	{
		my_record->noise_win[i]=my_record->long_win[npts_phase_beg+i];
	}





	return 0;
}
