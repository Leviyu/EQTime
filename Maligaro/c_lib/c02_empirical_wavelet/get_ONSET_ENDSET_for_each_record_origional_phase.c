
#include "hongyulib.h"
#include "ESF.h"
/******************************************************************
 *
 *  The goal of this script is to define a misfit window for PHASE
 *  and calcualte the misfit and update the SNR
******************************************************************/

int get_ONSET_ENDSET_for_each_record_origional_phase(new_RECORD* my_record,
		new_INPUT* my_input, double* current_ES)
{
	printf("---> get_ONSET_ENDSET_for_each_record Begin for origional EW \n");
	int npts_phase;
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
		if(my_record[ista].beyong_window_flag == -1)
			continue;


		// use max value as the phase peak
		// assumption is that the phase peak will always be the first peak
		AMP = 0;
		for(i = 0; i<npts_phase;i++)
		{
			if( fabs( current_ES[i] ) >= AMP )
			{
				AMP = fabs( current_ES[i] );
				npts_peak = i;
			}
		}
	
		// store phase peak time and npts
		my_record[ista].time_phase_peak = my_record[ista].phase_beg + npts_peak * my_input->delta;
		my_record[ista].npts_phase_peak = npts_peak;

		// set ONSET begtime with a noise level
		double noise_level = AMP * 0.1;

		// go from peak forward to the onset time where value is smaller then noise level
		for( i = npts_peak; i> 1 ; i--)
		{
			if(fabs( current_ES[i] ) < fabs(noise_level) )
			{
				npts_ONSET = i;
				break;
			}
		}

		//npts_ENDSET = npts_peak + (npts_peak - npts_ONSET);
		for( i = npts_peak; i< npts_phase ; i++)
		{
			if(fabs( my_record[ista].stretched_ES_win[i] ) < fabs(noise_level) )
			{
				npts_ENDSET = i;
				break;
			}
		}

		// convert npts ONSET into dt
		dt_ONSET = my_record[ista].phase_beg + npts_ONSET * my_input->delta ;
		int npts_peak_ONSET = npts_peak - npts_ONSET;
		dt_ENDSET = my_record[ista].phase_beg + ( npts_peak+ npts_peak_ONSET) * my_input->delta ;
		
		// store ONSET into dt_obs_prem
		my_record[ista].dt_obs_prem = dt_ONSET;
		my_record[ista].ONSET = dt_ONSET;
		my_record[ista].ENDSET = dt_ENDSET;



		double extra_time = 3;
		npts_ONSET = npts_ONSET - (int)(extra_time / my_input->delta); 
		if(npts_ONSET < 0 )
			npts_ONSET = 0;
	

		// ===========================================================
		//	get SNR measurement 
		// ===========================================================
		// using misfit window to set SNR_new
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
		if( noise_signal == 0 )
		{
			puts("ERROR: noise_signal is 0 SNR problem!");
			my_record[ista].SNR = 0;
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

		}


		return 0;
}


