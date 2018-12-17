
#include "hongyulib.h"
#include "ESF.h"

int calculate_true_one_period(new_RECORD* my_record, new_INPUT* my_input)
{
	int calculate_true_one_period_for_each(new_RECORD* my_record, new_INPUT* my_input);
	printf("--->Calculate one period in disp ! \n");

	int count;
	for(count = 0; count < my_input->sta_num; count ++)
	{
		if(my_record[count].beyong_window_flag == -1)
			continue;
		calculate_true_one_period_for_each(&my_record[count], my_input);
	}
	return 0;
}

int calculate_true_one_period_for_each(new_RECORD* my_record, new_INPUT* my_input)
{
	int vel_to_disp(double* vel, int npts, double* disp);

	// 2. use [onset - 0.5T , onset + 2.5T] window 
	double t1 = my_record->ENDSET - my_record->ONSET;

	if(t1 > 30 || t1 <= 0 )
		t1 = 10;
	double beg = my_record->ONSET - 0.5 * t1;
	double end = my_record->ONSET + 2.5 * t1;
	int npts_start = (int)( ( beg - my_record->long_beg) / my_input->delta);
	int npts = (int)(3*t1 / my_input->delta);
	double vel[npts];
	double disp[npts];

	//printf("sta %s beg %lf end %lf  1t is %lf\n", my_record->name, beg, end,t1);

	// read from long_win_orig
	int i;
	for( i = 0; i < npts; i++)
		if ( npts_start + i >= npts || npts_start + i < 0)
			vel[i] = 0;
		else
			vel[i] = my_record->long_orig[npts_start + i];
	
	// 1. convert phase_win vel into disp
	vel_to_disp(vel,npts,disp);



	// 3. fit a gaussian function to disp phase_win
	
	int npts_phase = npts;

	double stretched_gaussian[npts_phase];
	double best_ccc;
	double best_coeff;
	int best_time_shift;
	double coeff_min = 10;
	double coeff_max = 80;
	double coeff_delta = 10;
	stretch_gaussian_find_best_match_for_given_interval( disp, npts_phase, coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff,&best_time_shift,stretched_gaussian);
	coeff_min = best_coeff - 5;
	coeff_max = best_coeff + 5;
	coeff_delta = 1;
	stretch_gaussian_find_best_match_for_given_interval( disp, npts_phase, coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff,&best_time_shift,stretched_gaussian);
	coeff_min = best_coeff - 0.5;
	coeff_max = best_coeff + 0.5;
	coeff_delta = 0.1;
	stretch_gaussian_find_best_match_for_given_interval( disp, npts_phase, coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff,&best_time_shift,stretched_gaussian);
	my_input->emp_gaussian_factor = best_coeff;


	//printf(" best coeff is %lf \n", best_coeff);

	// 4. define ONSET and ENDSET on the gaussian 
	// int amplitudeloc(double* array, int len, int* max_amp_loc, double* amplitude, int flag)
	int max_loc;
	double amp;
	amplitudeloc(stretched_gaussian,npts,&max_loc,&amp,1);
	
	int npts_beg;
	int npts_end;
	int j;

	npts_beg = max_loc;
	npts_end = max_loc;
	for(j = max_loc; j > 0; j--)
	{
		if( stretched_gaussian[j] < 0.1 * amp)
		{
			npts_beg = j;
			break;
		}
	}
	for(j = max_loc; j < npts; j++)
	{
		if( stretched_gaussian[j] < 0.1 * amp)
		{
			npts_end = j;
			break;
		}
	}



	// 5. calculate one period
	my_record->one_period = ( npts_end - npts_beg) * my_input->delta;


	// if 1T is too small, it is cause the front or back end does not reach 0.1 threshold
	// thus we make the threshold to 0.3
	if( my_record->one_period  < 8 )
	{
		npts_beg = max_loc;
		npts_end = max_loc;
		for(j = max_loc; j > 0; j--)
		{
			if( stretched_gaussian[j] < 0.3 * amp)
			{
				npts_beg = j;
				break;
			}
		}
		for(j = max_loc; j < npts; j++)
		{
			if( stretched_gaussian[j] < 0.3 * amp)
			{
				npts_end = j;
				break;
			}
		}
		my_record->one_period = ( npts_end - npts_beg) * my_input->delta;
	}

	// if 1T is too small, it is cause the front or back end does not reach 0.1 threshold
	// thus we make the threshold to 0.5
	if( my_record->one_period  < 5 )
	{
		npts_beg = max_loc;
		npts_end = max_loc;
		for(j = max_loc; j > 0; j--)
		{
			if( stretched_gaussian[j] < 0.5 * amp)
			{
				npts_beg = j;
				break;
			}
		}
		for(j = max_loc; j < npts; j++)
		{
			if( stretched_gaussian[j] < 0.5 * amp)
			{
				npts_end = j;
				break;
			}
		}
		my_record->one_period = ( npts_end - npts_beg) * my_input->delta;
	}

	// if 1T is still two small, I guess we can only fix it by use the average 1T of that phase

	


	return 0;
}

int vel_to_disp(double* vel, int npts, double* disp)
{
	int i;
	// set output[0] = 0;
	disp[0] = 0;
	for(i = 0; i < npts -1 ; i++)
	{
		disp[i+1] = vel[i] * 1 + disp[i];
	}
	return 0;
}
