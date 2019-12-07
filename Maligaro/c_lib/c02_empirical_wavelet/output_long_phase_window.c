
#include "hongyulib.h"
#include "ESF.h"

int output_long_phase_window(new_RECORD* my_record, new_INPUT* my_input)
{
	printf("--->Output long phase window! \n");

	int output_long_phase_window_for_each(new_RECORD* my_record, new_INPUT* my_input);
	int count;
	for(count = 0; count < my_input->sta_num; count ++)
	{
		if(my_record[count].beyong_window_flag == -1)
			continue;
		output_long_phase_window_for_each(&my_record[count], my_input);
	}
	return 0;
}

int output_long_phase_window_for_each(new_RECORD* my_record, new_INPUT* my_input)
{
	char long_win_name[200];
	char phase_win_name[200];
	double x_long[100000];
	double x_phase[100000];


	// normalize both long and phase window with amp of long
	int max_loc;
	double amp;
	amplitudeloc(my_record->long_win, (int)( my_record->long_len / my_input->delta) ,&max_loc, &amp, 0);	
	int max_loc2;
	double amp2;
	amplitudeloc(my_record->phase_win,my_record->npts_phase,&max_loc2, &amp2,1);
	if(amp == 0 )
		amp =1;
	if(amp2 == 0)
		amp2 = 1;

	// when S gets close to ScS, we make this differently
	// when phase is ScS and the distance > 76 and the peak location is within 
	// the first 7second then we get rid of the first 7 second and find the amplitude
	int npts_skip = 50;
	if( max_loc2 < npts_skip )
	{
		double new_cut[3000];
		int kkk;
		for(kkk = 0; kkk< my_record->npts_phase -npts_skip; kkk++)
		{
			new_cut[kkk] = my_record->phase_win[kkk+npts_skip];
		}
		amplitudeloc(new_cut, my_record->npts_phase -npts_skip, &max_loc2, &amp2,1);
	}
	else if( max_loc2 > my_record->npts_phase - npts_skip )
	{
		double new_cut[3000];
		int kkk;
		for(kkk = 0; kkk< my_record->npts_phase -npts_skip; kkk++)
		{
			new_cut[kkk] = my_record->phase_win[kkk];
		}
		amplitudeloc(new_cut, my_record->npts_phase -npts_skip, &max_loc2, &amp2,1);
	}
		

	my_record->long_amplitude = fabs(amp) / amp2;
	if( my_record->long_amplitude == 0)
		my_record->long_amplitude =1;

	if(amp == 0)
		amp = 1;
	int i;
	// ======================================================
	// if long_win / phase_win ratio <= 2, we normalize long window to 1
	int increment = 5;

	// if is PHASE P, then make it 2
	if(strstr(my_input->PHASE,"P") != NULL )
		increment = 1;


	int new_npts_phase = (int) (my_record->npts_phase / increment);
	int new_npts_long = (int)(my_record->npts_long / increment);
	if(my_record->long_amplitude <=2 )
	{
			for(i=0;i<new_npts_phase; i++)
			{
				my_record->phase_win[i] = my_record->phase_win[i*increment] / fabs(amp);
				x_phase[i] = my_record->phase_beg + i*increment*my_input->delta;
			}
			for(i=0;i<new_npts_long;i++)
			{
				my_record->long_win[i] = my_record->long_win[i*increment] / fabs(amp);
				my_record->long_orig[i] = my_record->long_orig[i*increment] / fabs(amp);
				x_long[i] = my_record->long_beg + i*increment*my_input->delta;
			}
	}
	// ======================================================
	// if long_win / phase_win ratio > 2, we normalize phase win to 1
	if(my_record->long_amplitude >2 )
	{
			//for(i=0;i<my_record->npts_phase;i++)
			for(i=0;i<new_npts_phase;i++)
			{
				my_record->phase_win[i] = my_record->phase_win[i*increment] / fabs(amp2) *0.5;
				x_phase[i] = my_record->phase_beg + i*increment*my_input->delta;
			}
			//for(i=0;i<my_record->npts_long;i++)
			for(i=0;i<new_npts_long;i++)
			{
				my_record->long_win[i] = my_record->long_win[i*increment] / fabs(amp2) * 0.5;
				my_record->long_orig[i] = my_record->long_orig[i*increment] / fabs(amp2) * 0.5;
				x_long[i] = my_record->long_beg + i*increment*my_input->delta;
			}
	}


	sprintf(long_win_name,"%s.%s.%s.%s.long", my_record->EQ,my_record->name,my_record->PHASE,my_record->COMP);
	sprintf(phase_win_name,"%s.%s.%s.%s.phase", my_record->EQ,my_record->name,my_record->PHASE,my_record->COMP);
	//output_array2(long_win_name,x_long,my_record->long_orig, my_record->npts_long, 1);
	


	output_array2(long_win_name,x_long,my_record->long_orig, new_npts_long, 1);
	output_array2(phase_win_name,x_phase,my_record->phase_win, new_npts_phase, 1);
	//output_array2(long_win_name,x_long,my_record->long_orig, my_record->npts_long, 1);
	//output_array2(phase_win_name,x_phase,my_record->phase_win, my_record->npts_phase, 1);


	return 0;
}
