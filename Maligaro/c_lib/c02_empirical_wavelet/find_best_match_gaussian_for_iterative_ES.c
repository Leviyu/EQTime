#include "ESF.h"


void find_best_match_gaussian_for_iterative_ES(new_RECORD* my_record, new_INPUT* my_input, double* current_ES)
{
	int npts_phase = (int)(my_input->phase_len/ my_input->delta);

// we find the best matching gaussian factor for current stacked EWM for current phase
	double stretched_gaussian[npts_phase];
	double best_ccc;
	double best_coeff;
	int best_time_shift;
	double coeff_min = 1;
	double coeff_max = 90;
	double coeff_delta = 10;
	stretch_gaussian_find_best_match_for_given_interval( current_ES, npts_phase, coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff,&best_time_shift,stretched_gaussian);
	coeff_min = best_coeff - 5;
	coeff_max = best_coeff + 5;
	coeff_delta = 1;
	stretch_gaussian_find_best_match_for_given_interval( current_ES, npts_phase, coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff,&best_time_shift,stretched_gaussian);
	coeff_min = best_coeff - 0.5;
	coeff_max = best_coeff + 0.5;
	coeff_delta = 0.1;
	stretch_gaussian_find_best_match_for_given_interval( current_ES, npts_phase, coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff,&best_time_shift,stretched_gaussian);
	my_input->emp_gaussian_factor = best_coeff;

	my_input->emp_gaussian_area = 0;
	int kkk;
	for( kkk = 0; kkk < npts_phase ; kkk ++ )
	{
		my_input->emp_gaussian_area += stretched_gaussian[kkk];
	}

}
