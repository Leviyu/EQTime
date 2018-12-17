#include "hongyulibcpp.h"


int normalize_array(double* array, int len)
{
	double amplitude(double* array, int len);
	double AMP = amplitude(array,len);
	int icount;
	for(icount = 0; icount < len ; icount ++)
	{
		array[icount] = array[icount] / AMP;
	}
	return 0;
}

// =====
//	Flag:
//	1 ~ normalized to positive peak
//	-1 ~ normalized to negative peak
//	0 ~ same as normalize_array
// =====
int normalize_array_with_flag(double* array, int len, int flag)
{
	//int amplitudeloc(double* array, int len, int* max_amp_loc, double* amplitude, int flag)
	int max_loc;
	double amp;

	amplitudeloc(array, len, &max_loc , &amp,flag);
	if(amp == 0)
		amp = 1;

	int icount;
	for(icount = 0; icount < len ; icount ++)
	{
		array[icount] /=  fabs(amp);
	}
	return 0;
}


