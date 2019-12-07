#include "hongyulib.h"
#include "ESF.h"
/******************************************************************
 * This is a c script to define the onset of each record
 * we find the best fit gaussian relative the stretched_ES_win of each record
 * then we define onset one gaussian and hardwire this time on record
 *
 *  we stretcg gaussian function to fit stretched_ES_win
 *
 *	DATE:				Keywords:
 *	Reference:
******************************************************************/

int define_stretch_EW_ONSET(new_RECORD* my_record, new_INPUT* my_input)
{
	printf("---> define_stretch_EW_ONSET use gaussian to fit stretched_ES_win for each record\n");
	int npts_phase;
	npts_phase = (int) (my_input->phase_len / my_input->delta );
	int ista,i;
	int npts_peak;
	double AMP = 0;
	int npts_ONSET = 1;
	int npts_ENDSET = 1;
	double dt_ONSET = 0;
	double dt_ENDSET = 0;
	double coeff_min, coeff_max, coeff_delta;
	double stretched_gaussian[npts_phase];
	int maxloc;
	double amp;
	double gaussian_threshold = 0.02;
	double best_ccc, best_coeff;
	int best_time_shift;

	int count;
	int npts_gaussian_onset = 0;
	int npts_gaussian_endset = 0;
	int npts_onset = 0;
	int npts_endset = 0;



	for(ista = 0; ista < my_input->sta_num;ista++)
	{
		if(my_record[ista].beyong_window_flag == -1)
			continue;

		// we try to find the best gaussian fit to stretched_ES_win
		coeff_min = 1;
		coeff_max = 90;
		coeff_delta = 10;
		stretch_gaussian_find_best_match_for_given_interval( my_record[ista].stretched_ES_win, npts_phase, coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff,&best_time_shift,stretched_gaussian);

		// search smaller window with delta 0.1
		coeff_min = best_coeff - coeff_delta / 2;
		coeff_max = best_coeff + coeff_delta / 2;
		coeff_delta = 3;
		stretch_gaussian_find_best_match_for_given_interval( my_record[ista].stretched_ES_win, npts_phase, coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff,&best_time_shift,stretched_gaussian);

		// search smaller window with delta 0.1
		coeff_min = best_coeff - coeff_delta / 2;
		coeff_max = best_coeff + coeff_delta / 2;
		coeff_delta = 1.5;
		stretch_gaussian_find_best_match_for_given_interval( my_record[ista].stretched_ES_win, npts_phase, coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff,&best_time_shift,stretched_gaussian);

		// search smaller window with delta 0.01
		coeff_min = best_coeff - coeff_delta / 2;
		coeff_max = best_coeff + coeff_delta / 2;
		coeff_delta = 0.5;
		stretch_gaussian_find_best_match_for_given_interval( my_record[ista].stretched_ES_win, npts_phase, coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff,&best_time_shift,stretched_gaussian);

		// search smaller window with delta 0.01
		coeff_min = best_coeff - coeff_delta / 2;
		coeff_max = best_coeff + coeff_delta / 2;
		coeff_delta = 0.2;
		stretch_gaussian_find_best_match_for_given_interval( my_record[ista].stretched_ES_win, npts_phase, coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff,&best_time_shift,stretched_gaussian);

		// search smaller window with delta 0.01
		coeff_min = best_coeff - coeff_delta / 2;
		coeff_max = best_coeff + coeff_delta / 2;
		//##coeff_delta = 0.1;
		coeff_delta = 0.1;
		stretch_gaussian_find_best_match_for_given_interval( my_record[ista].stretched_ES_win, npts_phase, coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff,&best_time_shift,stretched_gaussian);

		// search smaller window with delta 0.01
		coeff_min = best_coeff - coeff_delta / 2;
		coeff_max = best_coeff + coeff_delta / 2;
		//##coeff_delta = 0.1;
		coeff_delta = 0.05;
		stretch_gaussian_find_best_match_for_given_interval( my_record[ista].stretched_ES_win, npts_phase, coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff,&best_time_shift,stretched_gaussian);

		// search smaller window with delta 0.01
		coeff_min = best_coeff - coeff_delta / 2;
		coeff_max = best_coeff + coeff_delta / 2;
		//##coeff_delta = 0.1;
		coeff_delta = 0.02;
		stretch_gaussian_find_best_match_for_given_interval( my_record[ista].stretched_ES_win, npts_phase, coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff,&best_time_shift,stretched_gaussian);

		// search smaller window with delta 0.01
		coeff_min = best_coeff - coeff_delta / 2;
		coeff_max = best_coeff + coeff_delta / 2;
		//##coeff_delta = 0.1;
		coeff_delta = 0.01;
		stretch_gaussian_find_best_match_for_given_interval( my_record[ista].stretched_ES_win, npts_phase, coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff,&best_time_shift,stretched_gaussian);



		int jjj;
		//store stretched gaussian into my_record
		for(count = 0 ; count < npts_phase; count++)
		{
			jjj = count + best_time_shift;
			if(jjj < 0)
				jjj = 0;
			if(jjj >npts_phase -1)
				jjj = npts_phase -1;
			my_record[ista].stretched_gaussian_win[count] = stretched_gaussian[jjj];
		}


		normalize_array( my_record[ista].stretched_gaussian_win, npts_phase);
		amplitudeloc(my_record[ista].stretched_gaussian_win,npts_phase,&maxloc, &amp,1);
		// check from gaussian peak to 0.05 amp
		for(count = maxloc; count >=0; count--)
		{
			if(my_record[ista].stretched_gaussian_win[count] < gaussian_threshold * amp)
			{
				npts_gaussian_onset = count;
				break;
			}
		}
		for(count = maxloc; count < npts_phase; count++)
		{
			if(my_record[ista].stretched_gaussian_win[count] < gaussian_threshold * amp)
			{
				npts_gaussian_endset = count;
				break;
			}
		}
		npts_ONSET = npts_gaussian_onset;
		// get ONSET ENDSET with 0.1 amplitude
		for(count = maxloc; count >=0; count--)
		{
			if(my_record[ista].stretched_gaussian_win[count] < 0.1 * amp)
			{
				npts_onset = count;
				break;
			}
		}
		for(count = maxloc; count < npts_phase; count++)
		{
			if(my_record[ista].stretched_gaussian_win[count] < 0.1 * amp)
			{
				npts_endset = count;
				break;
			}
		}



		// convert npts ONSET into dt
		dt_ONSET = my_record[ista].phase_beg + npts_onset * my_input->delta ;
		dt_ENDSET = my_record[ista].phase_beg + npts_endset * my_input->delta ;
		npts_ENDSET = npts_peak + (npts_peak - npts_ONSET);
		int npts_peak_ONSET = npts_peak - npts_ONSET;
		//dt_ENDSET = my_record[ista].phase_beg + ( npts_peak+ npts_peak_ONSET) * my_input->delta ;
		
		// store ONSET into dt_obs_prem
		my_record[ista].dt_obs_prem = my_record[ista].phase_beg + npts_gaussian_onset * my_input->delta ;
		my_record[ista].ONSET = dt_ONSET;
		my_record[ista].ENDSET = dt_ENDSET;
		my_record[ista].record_gaussian_factor = best_coeff;
		my_record[ista].emp_gaussian_factor = my_input->emp_gaussian_factor;


		// apply a width-dependent gaussian function
		// if record_gaussian_factor is greater then 30, apply a time delay
		if( my_record[ista].record_gaussian_factor > 30 )
		{
			double time_delay = width_dependent_gaussian_time_delay(my_record[ista].record_gaussian_factor);
			if(time_delay < 0 || time_delay > 3)
				time_delay = 0;
			my_record[ista].dt_obs_prem += time_delay;
		}



		


		// gaussian misfit
		my_record[ista].record_gaussian_area = 0;
		int kkk;
		for(kkk = 0; kkk < npts_phase ; kkk ++	)
		{
			my_record[ista].record_gaussian_area += stretched_gaussian[kkk];
		}
		if( my_input->emp_gaussian_area == 0 )
			my_input->emp_gaussian_area = 1;

		my_record[ista].gaussian_misfit =  ( my_record[ista].record_gaussian_area - my_input->emp_gaussian_area ) / my_input->emp_gaussian_area;





	}



		return 0;
}


