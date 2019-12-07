
#include "hongyulib.h"
#include "ESF.h"
/******************************************************************
 * This is a c script to construct first E.W. for PHASES
 *
 *  we use S E.W. for phases, expect
 *
 *  we apply -1*S_EW for : S3 S3m S5 S5m SS 
 *   since we flipped all negative phase, we use S E.W. for all phases
 *   note we applied hilbert transform to SS S4
******************************************************************/

int read_in_modified_S_ES(new_INPUT* my_input, double* array)
{
	printf("--> Read in modified S ES \n");
	int i;
	int npts_phase;
	npts_phase = (int) (my_input->phase_len / my_input->delta );
	double xx[npts_phase];
	double yy[npts_phase];

	char S_ES_input[300] = "main_ES.out";
	FILE* in;
	double S_ES[10000];
	in = fopen(S_ES_input,"r");
	int S_count= count_file_num(S_ES_input);
	double tmp;
	for(i = 0; i < S_count; i++)
	{
		fscanf(in," %lf %lf",&tmp,&S_ES[i]);
	}
	fclose(in);

	// normalize
	normalize_array(S_ES, S_count);



	// if S_count < npts_phase padd with last value  to make them the same
	if(S_count < npts_phase )
	{
		for(i=0;i<S_count;i++)
			array[i]=S_ES[i];
		for(i = S_count ; i< npts_phase; i++)
			array[i] = S_ES[S_count-2];
	}
	// if S_count > npts_phase get rid of the ones that is at the end
	// if S_count > npts_phase, we begin at the peak of S E.W. and go back- and forth-
	// npts_phase / 2 points
	// int amplitudeloc(double* array, int len, int* max_amp_loc, double* amplitude, int flag)
	int max_loc;
	double  max_amp;
	int flag = 1;
	amplitudeloc(S_ES, S_count, &max_loc, &max_amp, flag);
	int new_begin = max_loc - (int)(npts_phase/2);
	if(new_begin <= 0)
		new_begin = 0;

	if(S_count >= npts_phase )
	{
		//for(i= 0 ; i< npts_phase ;i++)
		//for(i= max_loc - (int)(npts_phase/2) ; i<  max_loc - (int)(npts_phase/2) +npts_phase ;i++)
		for(i = 0; i< npts_phase; i++)
			array[i]=S_ES[i+new_begin];
	}


// ====================================================================
//	for S3 S5 SS we use -1* S EW
// ====================================================================
// ====================================================================
		double AMP = 0;
		int npts_peak;
		for(i = 0; i<npts_phase;i++)
		{
			if( fabs( S_ES[i] ) >= AMP )
			{
				AMP = fabs( S_ES[i] );
				npts_peak = i;
			}
		}
		// set ONSET begtime with a noise level
		double noise_level = AMP * 0.1;
		int npts_ONSET = 1;
		double dt_ONSET = 0;

		// go from peak forward to the onset time where value is smaller then noise level
		for( i = npts_peak; i> 1 ; i--)
		{
			if(fabs( S_ES[i] ) < fabs(noise_level) )
			{
				npts_ONSET = i;
				break;
			}
		}
		int npts_peak_ONSET = npts_peak - npts_ONSET;
		my_input->npts_peak_ONSET = npts_peak_ONSET;
		my_input->S_npts_peak_onset = npts_peak_ONSET;
		my_input->S_npts_peak_beg = npts_peak;

		char S_ES_for_PHASE[200];
		sprintf(S_ES_for_PHASE,"S_ES_for_%s.out", my_input->PHASE);
		for(i = 0; i<npts_phase; i++)
		{
			xx[i] = i*my_input->delta;
			yy[i] = array[i];
		}

		output_array2(S_ES_for_PHASE,xx, yy,npts_phase,0);
		// output it as sac
		int nerr;
		char out_file[200];
		sprintf(out_file,"S_ES_for_%s.sac", my_input->PHASE);
		float beg  = 0;
		float del = my_input->delta;
		float yarray[npts_phase];
		for(i=0; i<npts_phase ;i++)
			yarray[i] = array[i];
		wsac1(out_file,yarray, &npts_phase, &beg,&del, &nerr, strlen( out_file ));
	return 0;


}

