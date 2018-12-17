
#include "hongyulib.h"
#include "ESF.h"
int read_phase_window_original_long_win(new_RECORD* my_record, 
		new_INPUT* my_input,double* phase_win_from_long_orig)
{

	double long_beg = my_record->long_beg;
	//double long_len = my_record->long_len;
	double phase_beg = my_record->phase_beg;
	double phase_len = my_record->phase_len;

	int npts_phase_beg = (int)((phase_beg - long_beg)/my_input->delta);
	int i;
	int npts_phase_len = (int)(phase_len/my_input->delta);
	for(i=0;i<npts_phase_len;i++)
	{
		phase_win_from_long_orig[i]=my_record->long_orig[npts_phase_beg+i];
	}

	return 0;
}
