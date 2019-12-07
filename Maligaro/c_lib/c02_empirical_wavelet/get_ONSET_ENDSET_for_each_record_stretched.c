
#include "hongyulib.h"
#include "ESF.h"
/******************************************************************
 * This is a c script for determine the  ONSET and ENDSET of each record
 *
 * The ONSET is determined by using the stretched_ES_win 
 *
 *  Changes:
 *  the one period window has been changed to be:
 *  [peak_to_amp_threshold_in_front_of_peak,
 *  peak_to_amp_threshold_at_back_of_peak]
 *
******************************************************************/

int get_ONSET_ENDSET_for_each_record_stretched(new_RECORD* my_record, new_INPUT* my_input)
{
	printf("---> get_ONSET_ENDSET_for_each_record Begin for stretched EW\n");
	int npts_phase;
	int count;

	npts_phase = (int) (my_input->phase_len / my_input->delta );
	int ista,i;
	int npts_peak;
	double AMP = 0;
	int npts_ONSET = 1;
	int npts_ENDSET = 1;
	double dt_ONSET = 0;
	double dt_ENDSET = 0;

	for(ista = 0; ista < my_input->sta_num;ista++)
	{
		if(my_record[ista].beyong_window_flag == -1 ) 
			continue;
		// use max value as the phase peak
		// assumption is that the phase peak will always be the first peak

		AMP = my_input->EW_AMP;
		npts_peak = my_input->npts_EW_peak  ;

		if(npts_peak == 0)
			npts_peak = 100;
		if(npts_peak == npts_phase -1)
			npts_peak = npts_phase -50;
	
		// store phase peak time and npts
		my_record[ista].time_phase_peak = my_record[ista].phase_beg + npts_peak * my_input->delta;
		my_record[ista].npts_phase_peak = npts_peak;

		// set ONSET begtime with a noise level
		double amp_crit = 0.1;
		double noise_level = AMP * amp_crit;

		// go from peak forward to the onset time where value is smaller then noise level
		// instead of use stretched ES win, we should be using phase_win
		for( i = npts_peak; i> 1 ; i--)
		{
			//printf(" amp is %lf \n", my_record[ista].stretched_ES_win[i] );
			//if(fabs( my_record[ista].stretched_ES_win[i] ) < fabs(noise_level) )
			if(fabs( my_record[ista].stretched_ES_win[i] ) < fabs(noise_level) )
			{
				npts_ONSET = i;
				break;
			}
		}

		// convert npts ONSET into dt
		dt_ONSET = my_record[ista].phase_beg + npts_ONSET * my_input->delta ;
printf(" sta %s dt_onset %lf phase beg %lf dff %lf \n",my_record[ista].name,dt_ONSET, my_record[ista].phase_beg, npts_ONSET * my_input->delta);
printf(" peak %d \n", npts_peak);


		// npts_ENDSET is find the same way as npts_ONSET
		//npts_ENDSET = npts_peak + (npts_peak - npts_ONSET);
		for( i = npts_peak; i< npts_phase ; i++)
		{
			if(fabs( my_record[ista].stretched_ES_win[i] ) < fabs(noise_level) )
			{
				npts_ENDSET = i;
				break;
			}
		}


		if( npts_ONSET == npts_ENDSET )
		{
			npts_ONSET = npts_peak -20;
			npts_ENDSET = npts_peak -20;
			if(npts_ONSET < 0 )
				npts_ONSET = 0;
			if(npts_ENDSET > npts_phase-1)
				npts_ENDSET = npts_phase -1;
		}


		int npts_peak_ONSET = npts_peak - npts_ONSET;
		dt_ENDSET = my_record[ista].phase_beg + ( npts_peak+ npts_peak_ONSET) * my_input->delta ;
	
		// store ONSET into dt_obs_prem
		my_record[ista].dt_obs_prem = dt_ONSET;
		my_record[ista].ONSET = dt_ONSET;
		my_record[ista].ENDSET = dt_ENDSET;

		int d_npts;

		int npts_one_period = dt_ENDSET - dt_ONSET;


// ===========================================================
//	get misfit measurement misfit
// ===========================================================

		// ==================================
		// when we calculate the misfit, we want to use the un-masked long
		// window to do the calculation, thus, we need to reconstruct the
		// phase_win from original long_win
		double phase_win_from_long_orig[npts_phase];
		read_phase_window_original_long_win(&my_record[ista],my_input, phase_win_from_long_orig);
		// ==================================
		double misfit_ES=0;
		double misfit_record=0;
		double misfit = 0;
		double misfit_diff = 0;
		misfit = 0;
		misfit_diff = 0;
		double ES_amp;
		double record_amp;
		int amp_loc;
		amplitudeloc(my_record[ista].stretched_ES_win, npts_phase, &amp_loc , &ES_amp,1);
		amplitudeloc(phase_win_from_long_orig , npts_phase, &amp_loc , &record_amp,1);
		if(ES_amp == 0 ) ES_amp = 1;
		if(record_amp == 0 ) record_amp = 1;
		for(count = npts_ONSET; count < npts_ENDSET; count++)
		{
			misfit_diff += fabs( my_record[ista].stretched_ES_win[count] / ES_amp -  
					phase_win_from_long_orig[count] / record_amp);
		}

		d_npts = (npts_ENDSET - npts_ONSET);
		if(d_npts == 0)
			d_npts =1;
		misfit = misfit_diff / d_npts;
		my_record[ista].misfit = misfit;

// ===========================================================
//	get misfit measurement misfit_pre
// ===========================================================
		//for empirical wavelet and for records
		misfit_ES=0;
		misfit_record=0;
		misfit = 0;
		misfit_diff = 0;
		double misfit_pre_2T = 0;
		double misfit_pre_3T = 0;
		double misfit_pre_4T = 0;
		int npts_beg;
		npts_beg = 2*npts_ONSET - npts_ENDSET;
		if(npts_beg < 0 )
			npts_beg = 0;
		int npts_end;
		npts_end = npts_ONSET;
		for(count = npts_beg; count < npts_end; count++)
		{
			misfit_diff += fabs( my_record[ista].stretched_ES_win[count] / ES_amp -  
					phase_win_from_long_orig[count] / record_amp);
		}
		d_npts = npts_end - npts_beg;
		if(d_npts == 0)
			d_npts =1;
		misfit = misfit_diff / d_npts;
		my_record[ista].misfit_pre = misfit;
		// get misfit_pre2T
		npts_beg = npts_ONSET - 2*(npts_ENDSET - npts_ONSET);
		npts_end = npts_ONSET - 1*(npts_ENDSET - npts_ONSET);
		if( npts_beg < 0) npts_beg = 0;
		if( npts_end >= npts_phase) npts_end = npts_phase -1;

		misfit_diff = 0;
		for(count = npts_beg; count < npts_end; count++)
		{
			misfit_diff += fabs( my_record[ista].stretched_ES_win[count] / ES_amp -  
					phase_win_from_long_orig[count] / record_amp);
		}
		d_npts = npts_end - npts_beg;
		if(d_npts == 0)
			d_npts =1;
		misfit = misfit_diff / d_npts;
		my_record[ista].misfit_pre2T = misfit;
		// get misfit_pre3T
		npts_beg = npts_ONSET - 3*(npts_ENDSET - npts_ONSET);
		npts_end = npts_ONSET - 2*(npts_ENDSET - npts_ONSET);
		if( npts_beg < 0) npts_beg = 0;
		if( npts_end >= npts_phase) npts_end = npts_phase -1;
		misfit_diff = 0;
		for(count = npts_beg; count < npts_end; count++)
		{
			misfit_diff += fabs( my_record[ista].stretched_ES_win[count] / ES_amp -  
					phase_win_from_long_orig[count] / record_amp);
		}
		d_npts = npts_end - npts_beg;
		if(d_npts == 0)
			d_npts =1;
		misfit = misfit_diff / d_npts;
		my_record[ista].misfit_pre3T = misfit;


// ===========================================================
//	get misfit measurement misfit_bak
// ===========================================================
		//for empirical wavelet and for records
		misfit_ES=0;
		misfit_record=0;
		misfit = 0;
		misfit_diff = 0;
		npts_beg = npts_ENDSET;
		npts_end = 2*npts_ENDSET - npts_ONSET;
		if( npts_end > npts_phase )
			npts_end = npts_phase;
		for(count = npts_beg; count < npts_end; count++)
		{
			misfit_diff += fabs( my_record[ista].stretched_ES_win[count] / ES_amp -  
					phase_win_from_long_orig[count] / record_amp);
		}

		d_npts = npts_end - npts_beg;
		if(d_npts == 0)
			d_npts =1;
		misfit = misfit_diff / d_npts;
		my_record[ista].misfit_bak = misfit;
		// get misfit_bak2T
		npts_beg = npts_ENDSET + 1*(npts_ENDSET - npts_ONSET);
		npts_end = npts_ENDSET + 2*(npts_ENDSET - npts_ONSET);
		if( npts_beg < 0) npts_beg = 0;
		if( npts_end >= npts_phase) npts_end = npts_phase -1;
		misfit_diff = 0;
		for(count = npts_beg; count < npts_end; count++)
		{
			misfit_diff += fabs( my_record[ista].stretched_ES_win[count] / ES_amp -  
					phase_win_from_long_orig[count] / record_amp);
		}
		d_npts = npts_end - npts_beg;
		if(d_npts == 0)
			d_npts =1;
		misfit = misfit_diff / d_npts;
		my_record[ista].misfit_bak2T = misfit;
		// get misfit_bak3T
		npts_beg = npts_ENDSET + 2*(npts_ENDSET - npts_ONSET);
		npts_end = npts_ENDSET + 3*(npts_ENDSET - npts_ONSET);
		if( npts_beg < 0) npts_beg = 0;
		if( npts_end >= npts_phase) npts_end = npts_phase -1;
		misfit_diff = 0;
		for(count = npts_beg; count < npts_end; count++)
		{
			misfit_diff += fabs( my_record[ista].stretched_ES_win[count] / ES_amp -  
					phase_win_from_long_orig[count] / record_amp);
		}
		d_npts = npts_end - npts_beg;
		if(d_npts == 0)
			d_npts =1;
		misfit = misfit_diff / d_npts;
		my_record[ista].misfit_bak3T = misfit;



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
		
		int value_noise_npts = 0;
		for(i=0;i<my_record[ista].npts_noise;i++)
		{
			if( my_record[ista].noise_win[i] == 0)
				continue;
			value_noise_npts ++;
			noise_signal += fabs(my_record[ista].noise_win[i]);
		}
		if(value_noise_npts < 100)
			my_record[ista].noise_too_short_flag = 1;


		if( noise_signal == 0 || npts_ONSET == npts_ENDSET)
		{
			puts("ERROR: noise_signal is 0 SNR problem!");
			my_record[ista].quality = -1;
			continue;
		}
		else
		{
			double SNR_sig = phase_signal / (npts_ENDSET - npts_ONSET);
			double SNR_noi = noise_signal / value_noise_npts;
			double SNR = SNR_sig/SNR_noi;
			my_record[ista].SNR_sig = SNR_sig;
			my_record[ista].SNR_noi = SNR_noi;
			my_record[ista].SNR = SNR;
		}


// ===========================================================
//	get misfit measurement misfit2
// ===========================================================
		//printf("==> get misfit2 \n");
		misfit_diff = 0;
		misfit_ES = 0;
		for(count = npts_ONSET; count < npts_ENDSET; count++)
		{
			misfit_diff += fabs(my_record[ista].stretched_ES_win[count]/ ES_amp
					- phase_win_from_long_orig[count] / record_amp	);
		}


		//##my_record[ista].misfit2 = misfit;
		d_npts = (npts_ENDSET - npts_ONSET);
		if(d_npts == 0)
			d_npts =1;
		misfit = misfit_diff / d_npts;
		my_record[ista].misfit2 = misfit;
// ===========================================================
//	get new CCC for misfit2 window
// ===========================================================
	//1. get records window
	int new_npts = npts_ENDSET - npts_ONSET ;
	double record_y[new_npts];
	double EW_y[new_npts];
	//record_y = (double*)malloc(sizeof(double)*new_npts);
	//EW_y = (double*)malloc(sizeof(double)*new_npts);
	int iii = 0;
	for(count = npts_ONSET; count < npts_ENDSET; count++)
	{
		record_y[iii] = my_record[ista].phase_win[count];
		EW_y[iii] = my_record[ista].stretched_ES_win[count];
		iii++;
	}
	int ccc_flag = 1;
	int npts_shift = 0;
	double new_ccc;
	CCC(record_y,new_npts,EW_y, new_npts, &npts_shift, &new_ccc, ccc_flag);
	my_record[ista].CCC3 = new_ccc;




// ===========================================================
//	get SNR3 which is the peak to trough SNR
// ===========================================================
	if( my_input->Reprocessing_Flag == 1 )
	{
		double SNR3 = 0;
		double SNR4 = 0;
		int npts_noise = (int)(my_input->noise_len / my_input->delta);
		get_SNR3_and_4_for_record(my_record[ista].phase_win, npts_phase, my_record[ista].noise_win, npts_noise, &SNR3, &SNR4, my_input);

		my_record[ista].SNR3 = SNR3;
		my_record[ista].SNR4 = SNR4;
	}


	}


		return 0;
}

