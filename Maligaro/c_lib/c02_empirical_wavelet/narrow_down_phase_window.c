
#include "ESF.h"
int narrow_down_phase_window(new_RECORD* my_record, new_INPUT* my_input, int old_npts)
{

	// 1. find the peak of phase window
	int count;
	double max_amp = 0;
	int npts_max = 0;;
	for(count = 0; count < old_npts; count ++)
	{
		if( fabs(my_record->phase_win[count]) > max_amp )
		{
			max_amp = fabs(my_record->phase_win[count]);
			npts_max = count;
		}
	}

	// find tight window
	int npts_half_win = (int)(my_input->phase_len / 2 / my_input->delta);
	int npts_win_beg = npts_max - npts_half_win;

	//printf(" new phase_len %lf old_phase_len %lf  \n", my_input->phase_len, old_npts*my_input->delta);
	//printf("delta is %lf \n", my_input->delta);
	//printf("max npts is %d beg is %d \n", npts_max, npts_win_beg);


	// 3. update tight window
	int new_npts = (int) (my_record->phase_len / my_input->delta);
	double phase_win_tmp[new_npts];
	for(count = 0; count < new_npts; count++)
		phase_win_tmp[count] = my_record->phase_win[npts_win_beg + count];
	for(count = 0; count < new_npts; count++)
		my_record->phase_win[count] = phase_win_tmp[count];

	my_record->phase_beg = my_record->phase_beg + npts_win_beg*my_input->delta;

	return 0;
}
