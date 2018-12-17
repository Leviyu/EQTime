#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>

int stretch_ES_function(double* current_ES, int npts_phase, double coeff,double*  tmp_ES)
{
	


	//1. find the max location of current ES
	int i=0;
	int index_max;
	double max_value = 0;
	for(i=0; i<npts_phase; i++)
	{
		if( fabs(current_ES[i]) > max_value)
		{
			max_value = fabs(current_ES[i]);
			index_max = i;
		}
	}

	int npts_delta;			// the npts from max location to insert location
	for(i = 0; i<npts_phase; i++)
	{
		npts_delta = i - index_max;								// i =0 index_max = 5     == -5
		double double_delta_from_max = npts_delta / coeff;		//	coeff - 0.7 		  == -3.5
		double double_delta_from_beg = double_delta_from_max + index_max;
		int lower_num, upper_num;
		if(double_delta_from_beg < 0 )
		{
			lower_num = 0;
			upper_num = 0;
		}
		else if(double_delta_from_beg >= npts_phase )
		{
			lower_num = npts_phase;
			upper_num = npts_phase;
		}
		else 
		{
			lower_num = floor(double_delta_from_beg);
			upper_num = lower_num + 1; 
		}
		
		// linear interpolation between lower and upper point
		tmp_ES[i] = current_ES[lower_num] + (double_delta_from_beg - lower_num)*(current_ES[upper_num] - current_ES[lower_num]); 
	}

	return 0;
}
