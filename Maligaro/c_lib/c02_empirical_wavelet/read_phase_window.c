
#include "hongyulib.h"
#include "ESF.h"
int read_phase_window(new_RECORD* my_record, new_INPUT* my_input)
{
	if(my_input->iteration_flag == 0 && my_input->ED_CHECKED_FLAG ==1)
	{
		char command[100];
		sprintf(command,"cat %s |grep -w %s |grep -w %s |grep -w %s|awk 'NR==1 {print $19}'", my_input->ed_checked_file, my_record->EQ, my_record->PHASE,my_record->name);
		double dt_shift =  shell_pipe(command);
		if(dt_shift != dt_shift )
			dt_shift = 0;
		if(dt_shift != 0 )
			my_record->checked_to_be_good = 1;

	}

	double long_beg = my_record->long_beg;
	double long_len = my_record->long_len;
	double phase_beg = my_record->phase_beg;
	double phase_len = my_record->phase_len;

	int npts_phase_beg = (int)((phase_beg - long_beg)/my_input->delta);
	int i;
	int npts_phase_len = (int)(phase_len/my_input->delta);
	double current_time_relative_to_prem;
	for(i=0;i<npts_phase_len;i++)
	{
		current_time_relative_to_prem =  phase_beg + i*my_input->delta;
		 //zero out value out side of [-15, 27] window
		 double back_time_window = 0;
		 if(my_input->EW_period == 0) 
			 back_time_window = my_input->mask_win_max + 7;
		 else
		 	back_time_window = my_input->mask_win_max + my_input->EW_period;

		 if(my_record->beyong_window_flag == 1)
		 {
		 	back_time_window = my_input->mask_win_max + my_input->EW_period;
		 }


		if( current_time_relative_to_prem < my_input->mask_win_min || 
				current_time_relative_to_prem > back_time_window)
			my_record->phase_win[i] = 0;
		else
			my_record->phase_win[i]=my_record->long_win[npts_phase_beg+i];
	}

	return 0;
}
