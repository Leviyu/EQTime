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

int get_ONSET_ENDSET_for_each_record(new_RECORD* my_record, new_INPUT* my_input, int phase_flag, double phase_len)
{
	puts("---> get_ONSET_ENDSET_for_each_record Begin ");
	int npts_phase;
	npts_phase = (int) (phase_len / my_input->delta );
	int ista,i;
	int npts_peak;
	double AMP = 0;
	int npts_ONSET = 1;
	int npts_ENDSET = 1;
	double dt_ONSET = 0;
	double dt_ENDSET = 0;
	if (strcmp(my_input->phase, "SS") ==0 || strcmp(my_input->phase, "ScSScS") ==0)
	{
			int npts_peak_ONSET = my_input->npts_peak_ONSET;

			FILE* out_SS_peak_zero;
			char out_name[200];
			sprintf(out_name,"event.npts_%s_peak_zero",my_input->phase);
			out_SS_peak_zero = fopen(out_name,"w");


			for(ista = 0; ista < my_input->sta_num;ista++)
			{
				// skip those with nan SNR
				if( my_record[ista].T[phase_flag].SNR != my_record[ista].T[phase_flag].SNR  )
					continue;
				
				for(i = 0; i<npts_phase;i++)
				{
					if( fabs( my_record[ista].T[phase_flag].stretched_ES_win[i] ) >= AMP )
					{
						AMP = fabs( my_record[ista].T[phase_flag].stretched_ES_win[i] );
						npts_peak = i;
					}
				}
			// set ONSET begtime with a noise level
			double noise_level = AMP * 0.05;
			int npts_zero;
			// go from peak forward to the onset time where value is smaller then noise level
			for( i = npts_peak; i> 1 ; i--)
			{
				if(fabs( my_record[ista].T[phase_flag].stretched_ES_win[i] ) < fabs(noise_level) )
				{
					npts_zero = i;
					break;
				}
			}
				npts_ONSET = (int)(npts_zero - npts_peak_ONSET*my_record[ista].T[phase_flag].best_stretch_coefficient);
//printf("sta name is %s , npts_peak is %d npts_zero is %d \n", my_record[ista].name, npts_peak, npts_zero);

				double SS_zero_point_time = my_record[ista].T[phase_flag].phase_beg + npts_zero * my_input->delta;
				// store phase peak time and npts
				my_record[ista].T[phase_flag].time_phase_peak = SS_zero_point_time;
				my_record[ista].T[phase_flag].npts_phase_peak = npts_zero;
				//my_record[ista].T[phase_flag].npts_SS_peak_zero = npts_peak - npts_tmp; 
				//fprintf(out_SS_peak_zero,"%s %lf\n", my_record[ista].name, SS_zero_point_time);

				//my_record[ista].T[phase_flag].time_phase_peak = my_record[ista].T[phase_flag].phase_beg + npts_peak * my_input->delta;
				//my_record[ista].T[phase_flag].npts_phase_peak = npts_peak;
				//my_record[ista].T[phase_flag].npts_SS_peak_zero = npts_peak - npts_tmp; 
				//fprintf(out_SS_peak_zero,"%s %lf\n", my_record[ista].name, SS_zero_point_time);

				// convert npts ONSET into dt
				dt_ONSET = my_record[ista].T[phase_flag].phase_beg + npts_ONSET * my_input->delta ;
				int npts_peak_ONSET_tmp = npts_peak - npts_ONSET;
				dt_ENDSET = my_record[ista].T[phase_flag].phase_beg + ( npts_peak+ npts_peak_ONSET_tmp) * my_input->delta ;

				
				// store ONSET into dt_obs_prem
				my_record[ista].T[phase_flag].dt_obs_prem = dt_ONSET;;
				my_record[ista].T[phase_flag].ONSET = dt_ONSET;;
				my_record[ista].T[phase_flag].ENDSET = dt_ENDSET;;
			}
			fclose(out_SS_peak_zero);
	}
	else 
	{
		for(ista = 0; ista < my_input->sta_num;ista++)
		{
			// skip those with nan SNR
			if( my_record[ista].T[phase_flag].SNR != my_record[ista].T[phase_flag].SNR  )
				continue;
			// use max value as the phase peak
				// assumption is that the phase peak will always be the first peak

			AMP = 0;
			for(i = 0; i<npts_phase;i++)
			{
				if( fabs( my_record[ista].T[phase_flag].stretched_ES_win[i] ) >= AMP )
				{
					AMP = fabs( my_record[ista].T[phase_flag].stretched_ES_win[i] );
					npts_peak = i;
				}
			}
		
			// store phase peak time and npts
			my_record[ista].T[phase_flag].time_phase_peak = my_record[ista].T[phase_flag].phase_beg + npts_peak * my_input->delta;
			my_record[ista].T[phase_flag].npts_phase_peak = npts_peak;



			// set ONSET begtime with a noise level
			double noise_level = AMP * 0.1;

			// go from peak forward to the onset time where value is smaller then noise level
			for( i = npts_peak; i> 1 ; i--)
			{
				if(fabs( my_record[ista].T[phase_flag].stretched_ES_win[i] ) < fabs(noise_level) )
				{
					npts_ONSET = i;
					break;
				}
			}
			// convert npts ONSET into dt
			dt_ONSET = my_record[ista].T[phase_flag].phase_beg + npts_ONSET * my_input->delta ;
			int npts_peak_ONSET = npts_peak - npts_ONSET;
			dt_ENDSET = my_record[ista].T[phase_flag].phase_beg + ( npts_peak+ npts_peak_ONSET) * my_input->delta ;
	//printf("sta is %s ONSET time is %lf phase beg is %lf npts_onset is %d * my_input->delta =  %lf  \n",my_record[ista].name,  dt_ONSET, my_record[ista].T[phase_flag].phase_beg, npts_ONSET, npts_ONSET * my_input->delta);
			
			// store ONSET into dt_obs_prem
			my_record[ista].T[phase_flag].dt_obs_prem = dt_ONSET;;
			my_record[ista].T[phase_flag].ONSET = dt_ONSET;;
			my_record[ista].T[phase_flag].ENDSET = dt_ENDSET;;
		}
	}


		return 0;
}



