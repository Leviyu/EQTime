
#include "hongyulib.h"
#include "ESF.h"
int tstar_ES_function(double* current_ES, int npts_phase, double coeff,double*  tmp_ES, char* tstar_lib)
{
	
	double delta = 0.1;
	int normalize_array();
	int convolution2();

	int ts_npts = npts_phase;
	double TSTAR[ts_npts];
	double conv_result[npts_phase];

	t_star_find_uniq(coeff, TSTAR, ts_npts, delta,tstar_lib);
	normalize_array(TSTAR,ts_npts);
	
	//do convolution between tstar and current_ES
	//convolution(current_ES, npts_phase, TSTAR, ts_npts, conv_result);
	convolution2(current_ES, npts_phase, TSTAR,  conv_result);

	//normalize_array(conv_result,ts_npts+npts_phase);
	normalize_array(conv_result,npts_phase);

	// find the peak of both ES and tstar and then match them
	int npts_ES = 0;
	int npts_tstar = 0;

	int count = 0;
	double tmp_amp = 0;
	for(count = 0; count < npts_phase; count ++)
	{
		if( current_ES[count] > tmp_amp)
		{
			tmp_amp = current_ES[count];
			npts_ES = count;
		}
	}
	count = 0;
	tmp_amp = 0;
	for(count = 0; count < npts_phase; count ++)
	{
		if( conv_result[count] > tmp_amp)
		{
			tmp_amp = conv_result[count];
			npts_tstar = count;
		}
	}

	for(count = 0; count < npts_phase; count ++)
	{
		int new_count = count + npts_tstar - npts_ES;
		if( new_count < 0)
		{
			//new_count = 0;
			tmp_ES[count] = 0.00001;
			continue;
		}
		if(new_count >= npts_phase) 
		{
			tmp_ES[count] = 0.00001;
			continue;
		}
		tmp_ES[count] = conv_result[new_count];
	}

	return 0;
}
