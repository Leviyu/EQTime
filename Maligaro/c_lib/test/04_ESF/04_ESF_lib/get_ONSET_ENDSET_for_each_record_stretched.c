#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>
/******************************************************************
 * This is a c script for determine the  ONSET and ENDSET of each record
 *
 * The ONSET is determined by using the stretched_ES_win 
 *
 * if PHASE is S / ScS/ SSS/ Sdiff
 * 	we use 0,1 AMP as its ONSET
 *
 * if PHASE is SS or ScSScS
 * 	we first find the peak 
 * 	then we find the first zero point in front of peak, this point is the peak in the origional S Empirical Wavelet
 * 	then we go back the amout of time between S peak and S ONSET modified by stretching factor to find the ONSET 
 *
 * Then we read in S_ES and find the ONSET
 *
 *	Input:
 *
 *
 *	Output:
 *
 *
 *	DATE:				Keywords:
 *	Reference:
******************************************************************/

int get_ONSET_ENDSET_for_each_record_stretched(new_RECORD* my_record, new_INPUT* my_input)
{
	fprintf(my_input->out_logfile,"---> get_ONSET_ENDSET_for_each_record Begin for stretched EW\n");
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
		// skip those bad record
		if( my_record[ista].quality == -1 )
			continue;
		// use max value as the phase peak
			// assumption is that the phase peak will always be the first peak

		AMP = 0;
		for(i = 0; i<npts_phase;i++)
		{
			if( fabs( my_record[ista].stretched_ES_win[i] ) >= AMP )
			{
				AMP = fabs( my_record[ista].stretched_ES_win[i] );
				npts_peak = i;
			}
		}
	
		// store phase peak time and npts
		my_record[ista].time_phase_peak = my_record[ista].phase_beg + npts_peak * my_input->delta;
		my_record[ista].npts_phase_peak = npts_peak;



		// set ONSET begtime with a noise level
		double amp_crit = 0.1;
		double noise_level = AMP * amp_crit;;

		// go from peak forward to the onset time where value is smaller then noise level
		for( i = npts_peak; i> 1 ; i--)
		{
			if(fabs( my_record[ista].stretched_ES_win[i] ) < fabs(noise_level) )
			{
				npts_ONSET = i;
				break;
			}
		}



		// convert npts ONSET into dt
		dt_ONSET = my_record[ista].phase_beg + npts_ONSET * my_input->delta ;
		npts_ENDSET = npts_peak + (npts_peak - npts_ONSET);
		int npts_peak_ONSET = npts_peak - npts_ONSET;
		dt_ENDSET = my_record[ista].phase_beg + ( npts_peak+ npts_peak_ONSET) * my_input->delta ;
		
		// store ONSET into dt_obs_prem
		my_record[ista].dt_obs_prem = dt_ONSET;
		my_record[ista].ONSET = dt_ONSET;
		my_record[ista].ENDSET = dt_ENDSET;




//printf("sta is %s ONSET time is %lf phase beg is %lf npts_onset is %d * my_input->delta =  %lf  \n",my_record[ista].name,  dt_ONSET, my_record[ista].phase_beg, npts_ONSET, npts_ONSET * my_input->delta);

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

		return 0;
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
