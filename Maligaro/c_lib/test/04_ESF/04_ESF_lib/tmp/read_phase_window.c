#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>

int read_phase_window(double* long_win,double long_beg, double long_len, double phase_beg, double phase_len,double* phase_win, new_INPUT* my_input)
{

	int npts_phase_beg = (int)((phase_beg - long_beg)/my_input->delta);
	int i;
	int npts_phase_len = (int)(phase_len/my_input->delta);
	for(i=0;i<npts_phase_len;i++)
	{
		phase_win[i]=long_win[npts_phase_beg+i];
	}

	return 0;
}
