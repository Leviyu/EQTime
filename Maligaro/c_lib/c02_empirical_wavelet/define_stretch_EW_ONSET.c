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

		// we design gaussian_threshold_funciton, which is a funciton of record_gaussian_factor
		//##my_record[ista].record_gaussian_factor = best_coeff;
		//if( best_coeff < 25  )
			//gaussian_threshold = 0.02;
		//else if( best_coeff < 30 )
			//gaussian_threshold = 0.05;
		//else if( best_coeff < 40 )
			//gaussian_threshold = 0.08;
		//else if( best_coeff < 50)
			//gaussian_threshold = 0.10;
		//else
			//gaussian_threshold = 0.15;
	
		




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


		//my_record[ista].ONSET = dt_ONSET;
		//my_record[ista].ENDSET = dt_ENDSET;



	}

//printf("sta is %s ONSET time is %lf phase beg is %lf npts_onset is %d * my_input->delta =  %lf  \n",my_record[ista].name,  dt_ONSET, my_record[ista].phase_beg, npts_ONSET, npts_ONSET * my_input->delta);


		return 0;
}


/*
		double extra_time = 3;
		npts_ONSET = npts_ONSET - (int)(extra_time / my_input->delta);
		if(npts_ONSET < 0) 
			npts_ONSET = 0;


		// update the SNR using new ES window
		double phase_signal=0;
		double noise_signal=0;
		int i;
		for(i=npts_ONSET;i<npts_ENDSET;i++)
		{
			phase_signal += fabs(my_record[ista].phase_win[i]);
		}

		for(i=0;i<my_record[ista].npts_noise;i++)
		{
			noise_signal += fabs(my_record[ista].noise_win[i]);
		}
		//printf("noise signal is %lf phase signa; is %lf npts noise phase %d %d\n", noise_signal, phase_signal, my_record[ista].npts_noise,my_record[ista].npts_phase);
		if( noise_signal == 0 )
		{
			puts("ERROR: noise_signal is 0 SNR problem!");
			my_record[ista].quality = -1;
			continue;
		}
		else
		{
			double SNR_sig = phase_signal / (npts_ENDSET - npts_ONSET);
			double SNR_noi = noise_signal / (my_record[ista].noise_len/my_input->delta);
			double SNR = SNR_sig/SNR_noi;
			my_record[ista].SNR_sig = SNR_sig;
			my_record[ista].SNR_noi = SNR_noi;
			my_record[ista].SNR = SNR;
		}


// ===========================================================
//	get misfit measurement misfit2
// ===========================================================
		//for empirical wavelet and for records
		double misfit_ES=0;
		double misfit_record=0;
		double misfit = 0;
		int count;
		for(count = npts_ONSET; count < npts_ENDSET; count++)
		{
			misfit_ES +=  fabs( my_record[ista].stretched_ES_win[count] );
			misfit_record +=  fabs( my_record[ista].phase_win[count] );
		}

		//average it with the number of points
		//misfit_ES = misfit_ES / (npts_ENDSET - npts_ONSET);
		//misfit_record = misfit_record / (npts_ENDSET - npts_ONSET);
		//misfit = misfit / (npts_ENDSET - npts_ONSET);
		misfit = (misfit_record - misfit_ES) / misfit_ES;

		my_record[ista].misfit2 = misfit;


	}

	//// output ONSET ENDSET for plotting
	//FILE* out;
	//char out_name[200];
	//sprintf(out_name,"%s_ONSET_ENDSET.out",my_input->PHASE);
	//out = fopen(out_name,"w");
	//for(ista =0; ista < my_input->sta_num; ista ++)
	//{
		//if(my_record[ista].SNR != my_record[ista].SNR )
			//continue;
		//fprintf(out,"%7s %7.3lf %7.3lf %7.3lf %7.3lf\n", my_record[ista].name,  my_record[ista].ONSET, my_record[ista].ENDSET,my_input->S_npts_peak_beg*my_input->delta);
	//}
	////fclose(out);
	//
*/
