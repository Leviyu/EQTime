#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>

int read_in_modified_S_ES(new_INPUT* my_input, double* array)
{
puts("---> for ScS, SS, ScSScS, SSS, we read in modified S");
	int i;
	int npts_phase;
	int hilbert(double* array_in, int npts, double* array_out); 
	int amplitudeloc(double* array, int len, int* max_amp_loc, double* amplitude, int flag);
	int normalize_array();
	int output_array1();
	npts_phase = (int) (my_input->phase_len / my_input->delta );


	// read in origional S ES
	FILE* in;
	double S_ES[10000];
	int S_count;
	in = fopen("S_ES.out","r");
	for(S_count=0;S_count < 10000 ; S_count++)
	{
		if(feof(in))
		{
			break;
		}
		else
			fscanf(in,"%lf",&S_ES[S_count]);

	}
	fclose(in);


	// read in bp S ES
	double S_ES_bp[10000];
	int S_bp_count;
	in = fopen("S_ES_bp.out","r");
	for(S_bp_count=0;S_bp_count < 10000 ; S_bp_count++)
	{
		if(feof(in))
		{
			break;
		}
		else
			fscanf(in,"%lf",&S_ES_bp[S_bp_count]);
	}
	fclose(in);


	// normalize
	normalize_array(S_ES, S_count);
	normalize_array(S_ES_bp, S_bp_count);



	// find the delay time useing peak difference between S_ES bp and S_ES
	int max_loc_S_ES;
	int max_loc_S_ES_bp;
	double AMP;
	amplitudeloc(S_ES, S_count,&max_loc_S_ES, &AMP,0);
	amplitudeloc(S_ES_bp, S_bp_count,&max_loc_S_ES_bp, &AMP,0);
	my_input->bp_delay_time = fabs(max_loc_S_ES - max_loc_S_ES_bp) * my_input->delta;
	printf("---> S is bp using same filter with SS, the ONSET difference between bp and non S is %lf  ===================================\n", my_input->bp_delay_time);
	// output delay time into delay_time.out
	double S_ES_delay_time[10];
	S_ES_delay_time[0] = my_input->bp_delay_time;
	output_array1("S_ES_bp_delay_time.out",S_ES_delay_time,1);

	

	//if(strcmp(my_input->phase,"Sdiff")==0)
		//return 0;



	// if S_count < npts_phase padd with last value  to make them the same
	if(S_count < npts_phase )
	{
		for(i=0;i<npts_phase - S_count;i++)
			array[i]=S_ES[i];
		for(i = npts_phase - S_count; i< npts_phase; i++)
			array[i] = S_ES[npts_phase - S_count-1];
	}
	// if S_count > npts_phase get rid of the ones that is at the end
	if(S_count >= npts_phase )
	{
		for(i= 0 ; i< npts_phase ;i++)
			array[i]=S_ES[i];
	}


	//find the point between ONSTE and peak
	if(strcmp(my_input->phase, "S") != 0)
	{
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
	}



	// if phase is ScS be it
	if( strcmp(my_input->phase, "ScS")== 0)
	{
		output_array1("S_ES_for_ScS.out", array,npts_phase);
		return 0;
	}

	// if phase is SS we do a 1 times (90 degree phase shift)hilbert transfrom to it 
	if( strcmp(my_input->phase, "SS")== 0 ||  strcmp(my_input->phase, "ScSScS")== 0 )
	{
		double H1[npts_phase];

		hilbert(array, npts_phase, H1);
		for(i = 0;i<npts_phase; i++)
			array[i] = H1[i];

		output_array1("S_ES_for_SS.out", array,npts_phase);
		output_array1("S_ES_for_ScSScS.out", array,npts_phase);

		return 0;
	}
	// if phase is SSS we do a -1* to it
	if(strcmp(my_input->phase, "SSS")== 0)
	{
		for(i=0;i<npts_phase;i++)
			array[i] = array[i]*-1;
		output_array1("S_ES_for_SSS.out", array,npts_phase);
		return 0;
	}

	return 0;


}

