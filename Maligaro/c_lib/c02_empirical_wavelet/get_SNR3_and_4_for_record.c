#include "hongyulib.h"
#include "ESF.h"




int get_SNR3_and_4_for_record(double* phase_win,int phase_npts, double* noise_win,int noise_npts, double* SNR3, double* SNR4, new_INPUT* my_input)
{
	int iphase, inoise;

	// 1. find the peak of noise/phase
	// int amplitudeloc(double* array, int len, int* max_amp_loc, double*
	//// amplitude, int flag)
	int npts_phase_max;
	int npts_noise_max;
	double noise_amp;
	double phase_amp;
	int flag = 1;
	amplitudeloc(phase_win, phase_npts, &npts_phase_max, &phase_amp, flag);
	amplitudeloc(noise_win, noise_npts, &npts_noise_max, &noise_amp, flag);
	if(noise_amp == 0)
		noise_amp = 1;
	*SNR4 = fabs( phase_amp / noise_amp);

	double phase_to_trough_array[20];
	int noise_len = 80;
	int search_inc = 4;
	double EW_period = my_input->EW_period;
	int npts_period = (int) (EW_period / my_input->delta);
	double beg_position;
	double end_position;
	int npts_beg;
	int npts_end;
	int i;
	int count = 0;
	double max_noise_trough_amp = 0;
	for(count = 0; count < 20; count ++)
	{
		beg_position = 0 + count * search_inc ;
		end_position = EW_period + count* search_inc;

		if(end_position >= 80 )
			break;

		npts_beg = (int) (beg_position / my_input->delta);
		npts_end = (int) (end_position / my_input->delta);
		
		// store one period data into array
		double tmp_phase_win[npts_period];
		for(i = 0 ; i< npts_period ; i++)
			tmp_phase_win[i] = noise_win[npts_beg + i];
	
		// get peak amplitude
		double tmp_npts_noise_max;
		double tmp_noise_amp;
		amplitudeloc(tmp_phase_win, npts_period,&tmp_npts_noise_max, &tmp_noise_amp,flag);
		// find trough amplitude in one period window
		int npts_noise_trough = 0;
		double noise_trough_amp = 0;
		for(inoise = tmp_npts_noise_max+1; inoise < noise_npts -1 ; inoise ++)
		{
			if( noise_win[inoise+1] - noise_win[inoise] > 0 )
			{
				npts_noise_trough = inoise;
				noise_trough_amp = fabs(tmp_noise_amp - noise_win[inoise]);
				break;
			}
		}
		if( noise_trough_amp > max_noise_trough_amp )
			max_noise_trough_amp = noise_trough_amp;

	}


	// now we calculate the peak-to-trough amplitude
	int npts_phase_trough = 0;
	double phase_trough_amp = 0;
	for(iphase = npts_phase_max+1; iphase < phase_npts -1 ; iphase ++)
	{
		if( phase_win[iphase+1] - phase_win[iphase] > 0 )
		{
			npts_phase_trough = iphase;
			phase_trough_amp = fabs(phase_amp - phase_win[iphase]);
			break;
		}
	}

	
	if(max_noise_trough_amp == 0)
		max_noise_trough_amp = 1;
	*SNR3 = fabs(phase_trough_amp / max_noise_trough_amp);


	return 0;
}
