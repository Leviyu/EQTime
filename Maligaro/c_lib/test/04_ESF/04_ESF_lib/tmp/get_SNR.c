#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>

int get_SNR(new_PHASE* T)
{
	//use area undre the curve to evaluate SNR
	// if SNR is nan then this record is really bad

	int i;
	double noise_signal = 0;
	double phase_signal = 0;

	


	for(i=0;i<T->npts_phase;i++)
	{
		phase_signal += fabs(T->phase_win[i]);
	}

	for(i=0;i<T->npts_noise;i++)
	{
		noise_signal += fabs(T->noise_win[i]);
	}
	//printf("noise signal is %lf phase signa; is %lf npts noise phase %d %d\n", noise_signal, phase_signal, T->npts_noise,T->npts_phase);

	if( noise_signal == 0 )
	{
		puts("ERROR: noise_signal is 0 SNR problem!");
		double SNR = 0.0/0.0;
		T->SNR = SNR;
		//getchar();
	}
	else
	{
		double SNR = ( phase_signal / T->phase_len  ) / ( noise_signal / T->noise_len)  ;
		T->SNR = SNR;
	}
return 0;
}

