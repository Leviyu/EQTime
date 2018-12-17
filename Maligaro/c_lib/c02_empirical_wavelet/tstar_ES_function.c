
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

	//printf("tstar for %lf begin \n", coeff);
	t_star_find_uniq(coeff, TSTAR, ts_npts, delta,tstar_lib);
	//printf("tstar for %lf end\n\n", coeff);
	normalize_array(TSTAR,ts_npts);
	
	//do convolution between tstar and current_ES
	//convolution(current_ES, npts_phase, TSTAR, ts_npts, conv_result);
	convolution2(current_ES, npts_phase, TSTAR,  conv_result);
	 //int convolution2(double* array1, int npts1, double* array2, double* array_out)

	//normalize_array(conv_result,ts_npts+npts_phase);
	normalize_array(conv_result,npts_phase);
	//char conv_file[200];
	//sprintf(conv_file,"conv_file_%lf",coeff);
	//output_array1(conv_file,conv_result, ts_npts+npts_phase-1);
	//output_array1(conv_file,conv_result, npts_phase);
	//sprintf(conv_file,"current_file_%lf",coeff);
	//output_array1(conv_file,current_ES, npts_phase);

	int count;
	for(count = 0; count < npts_phase; count ++)
		tmp_ES[count] = conv_result[count];

	return 0;
}
