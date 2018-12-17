#include "hongyulibcpp.h"

int tstar_ES_function(double* current_ES, int npts_phase, double coeff,double*  tmp_ES)
{
	
	double delta = 0.1;

	int ts_npts = npts_phase;
	double TSTAR[ts_npts];
	double conv_result[npts_phase];

	t_star_find_uniq(coeff, TSTAR, ts_npts, delta);
	normalize_array(TSTAR,ts_npts);


	//do convolution between tstar and current_ES
	convolution2(current_ES, npts_phase, TSTAR,  conv_result);

	//normalize_array(conv_result,ts_npts+npts_phase);
	normalize_array(conv_result,npts_phase);

	int count;
	for(count = 0; count < npts_phase; count ++)
		tmp_ES[count] = conv_result[count];

	return 0;
}
