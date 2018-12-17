#include "ESF.h"
#include "hongyulib.h"

int empirical_source_function(new_RECORD* my_record, new_INPUT* my_input)
{
	printf("--->  empirical_source_function Begin \n");
	//define a 2D  ES array 
	double ccc,shift_time;
	int npts_phase, i,j,ista, npts_shift;
	npts_phase = (int) (my_input->phase_len / my_input->delta );
	double old_ES[npts_phase];
	double new_ES[npts_phase];
	double current_ES[npts_phase];
	double first_EW[npts_phase];
	int restack_flag;
	int count;

	double** ES;
	ES = (double**)malloc(sizeof(double*)*30);
	for(i=0;i<30;i++)
		ES[i] = (double*)malloc(sizeof(double)*npts_phase);
	
	get_first_EW_for_phase(my_record,my_input,first_EW);
	for(i=0;i<npts_phase; i++ )
		ES[1][i] = first_EW[i];

	puts("============================================\n");
	puts( "==============First CCC loop Begin ==============\n");
	puts("============================================\n");
	my_input->shift_info_out = (char*)malloc(sizeof(char)*200);
	sprintf(my_input->shift_info_out,"shiftinfo.out");
	my_input->out_shift = fopen(my_input->shift_info_out,"w");
	// ===========================================================
	//	2. start loop ccc to get a stable E.W.
	// ===========================================================
	int loop_num;
	int loop_num_max = 5;
	for(loop_num = 1; loop_num <= loop_num_max ; loop_num++)
	{
		my_input->iteration_flag = loop_num;
		printf("---> Working on ccc loop %d \n",loop_num);
		// get current E.W. and store into current_ES and normalize it
		for(i=0;i<npts_phase;i++)
			current_ES[i]=ES[loop_num][i];
		normalize_array(current_ES, npts_phase);

		// calculate Empirical Wavelet Period
		calculate_EW_period(my_input, current_ES);

		//int empirical_source_for_each_record(new_RECORD* my_record, new_INPUT* my_input, double* current_ES);
		empirical_source_for_each_record(my_record,my_input,current_ES, loop_num);

		
		get_ONSET_ENDSET_for_each_record_origional_phase(my_record,my_input,current_ES);
		//stack again to get new ES with weight (SNR, ccc)
		restack_flag = restack_ES_for_phase(my_record,my_input,ES[loop_num +1]);

		// normalize ES
		normalize_array(ES[loop_num + 1], npts_phase);

		// if ccc new_ES & old_ES > 0.99 exit
		for(i = 0; i< npts_phase; i++)
		{
			old_ES[i] = ES[loop_num][i];
			new_ES[i] = ES[loop_num+1][i];
		}
		CCC(old_ES,npts_phase,new_ES, npts_phase, &npts_shift, &ccc, 0);
		printf("  ==> For current iteration old and new ES ccc is %lf\n",ccc);

		// output first iteration EW and STD
		if(loop_num ==1 )
		{
			output_STD_of_first_iteration_ES(my_record,my_input, new_ES);
			output_current_ES_for_phase_first_iteration(my_input, new_ES);
		}

		// for Phase ScS,since using S ES for ccc therefore we dont stack again 
		if(strcmp(my_input->PHASE,"S")!=0 && strcmp(my_input->PHASE,"P")!=0)
		{
			for(i=0;i<npts_phase;i++)
				current_ES[i]=old_ES[i];
			break;
		}
		// if we do missint stations, we dont iterate
		if(my_input->Fix_missing_sta_flag == 1)
		{
			for(i=0;i<npts_phase;i++)
				current_ES[i]=old_ES[i];
			break;
		}

		
		// output ES and STD for each iteration
		output_ES_with_STD_loop(my_record, my_input,current_ES, loop_num);

		if(fabs(ccc) > 0.95 )
			break;
	}
	// the iterative EW construction ends there
	// we need to store the E.W. into every record`s ES_win
	store_ES_into_record(my_record, my_input, current_ES);


	// use 1T to zero out traffic phase
	for(count = 0; count < my_input->sta_num; count++)
	{
		zero_out_traffic_phase(&my_record[count], my_input);
	 	read_noise_window(&my_record[count],  my_input);
	}



	//output long and phase whindow
	//puts("         ---> output_long_phase_win and first ES & STD");
	output_STD_of_first_ES(my_record,my_input, old_ES);
	output_current_ES_for_phase(my_input, old_ES);

	
	//puts("         ---> stretching ");
	// stretch ES to find the best waveform to match each record
	strcpy(my_input->stretch_flag,"stretch");
	// first stretching here ***
	stretch_ES_and_CCC(my_record, my_input, old_ES);

	//puts("         ---> restack ");
	//update ES by stretching records (using stretch factor) and stack again
	double stretch_record_stack[npts_phase];
	stretch_record_restack_ES(my_record, my_input, stretch_record_stack); 
	normalize_array(stretch_record_stack, npts_phase);
	output_STD_of_second_ES(my_record,my_input, stretch_record_stack);
	output_current_ES_for_phase_second(my_input, stretch_record_stack);

	// for non S phase use current_ES
	// for S use stretch_record_stack
	if(strcmp(my_input->PHASE,"S")!=0 && strcmp(my_input->PHASE,"P")!=0)
		find_best_match_gaussian_for_iterative_ES(my_record,my_input,current_ES);
	else
		find_best_match_gaussian_for_iterative_ES(my_record,my_input,stretch_record_stack);


	 //define E.W. to use here
	double EW_new[npts_phase];
	if(strcmp(my_input->PHASE,"S") == 0 || strcmp(my_input->PHASE,"P") == 0 )
	{
		for(count = 0; count < npts_phase; count++)
			EW_new[count] = stretch_record_stack[count];
	}
	else 
	{
		for(count = 0; count < npts_phase; count++)
			EW_new[count] = first_EW[count];
	}
	printf(" fix missing flag is %d \n", my_input->Fix_missing_sta_flag);
	if(my_input->Fix_missing_sta_flag == 1)
	{
		for(count = 0; count < npts_phase; count++)
			EW_new[count] = first_EW[count];
	}


	//puts("         ---> t* ");
	// stretch updated E.W. with tstar and best fit each record 
	strcpy(my_input->stretch_flag,"tstar");
	// second  stretching here ***
	stretch_ES_and_CCC_tstar(my_record, my_input, EW_new);
	//stretch_ES_and_CCC(my_record, my_input, EW_new);

	//find ONSET (arrival time)	 and ENDSET time
	//puts("         ---> get_ONSET_ENDSET_for_each_record_stretched ");
	get_ONSET_ENDSET_for_each_record_stretched(my_record,my_input);

	//define if record is good
	//puts("         ---> define goodness ");
	define_goodness_of_record(my_record, my_input);

	//restack_ES_for_phase_after_tstar(my_record, my_input, EW_new);
	//puts("         ---> output ES STD 3rd ");
	//output_STD_of_third_ES(my_record,my_input, EW_new);
	//output_current_ES_for_phase_third(my_input, EW_new);
	output_STD_of_third_ES(my_record,my_input, stretch_record_stack);
	output_current_ES_for_phase_third(my_input, stretch_record_stack);


	// use gaussian to best fit each t* record and define arrival time
	// third stretching here ***
	//puts("         ---> define stretch ONSET with gaussian ");
	define_stretch_EW_ONSET( my_record, my_input);

	// output ES for each record 
	//puts("         ---> output_ES_for_each_record ");
	output_long_phase_window(my_record, my_input);
	output_ES_for_each_record(my_record, my_input);

	// calculate true one period in displacement
	calculate_true_one_period(my_record,my_input);	


	// for records that have dt outside of allowable window, we redo 
	// the whole process with the new masked window
	// 1. redefine the beyond_window_flag



	// after code choice, we used code picked good records to restack a E.W.
	// and redo the whole process again just for S/P
	//redo_for_S_P_remake_EW(my_record, my_input);

	//redefine_beyon_wind_flag(my_record,my_input,current_ES,EW_new);

	return 0;
}

