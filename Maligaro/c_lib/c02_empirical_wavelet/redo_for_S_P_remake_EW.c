#include "ESF.h"



void redo_for_S_P_remake_EW(new_RECORD* my_record, new_INPUT* my_input)
{

	if( strcmp(my_input->PHASE, "S") == 0 || strcmp(my_input->PHASE, "P") == 0)
		printf("---> working on redo_for_S_P_remake_EW \n");
	else
		return ;

	int npts_phase = (int)(my_input->phase_len / my_input->delta);
	double EW[npts_phase];
	double stretched_EW[npts_phase];
	int ista;
	int count;
	int max_loc;
	double amp;

	
	// initiate
	for(count = 0; count < npts_phase ; count++ )
	{
		EW[count] = 0;
		stretched_EW[count] = 0;
	}

	for(ista = 0; ista < my_input->sta_num ; ista++)
	{
		my_record[ista].beyong_window_flag = 0;
		read_phase_window(&my_record[ista], my_input);
		if(my_record[ista].quality <= 0)
			continue;
		//int amplitudeloc(double* array, int len, int* max_amp_loc, double* amplitude, int flag) 
		amplitudeloc(my_record[ista].phase_win , npts_phase, & max_loc , & amp, 1 );
		for(count = 0; count < npts_phase ; count++)
			EW[count] += my_record[ista].phase_win[count] / amp * my_record[ista].weight;
		//char sta_file[200];
		//sprintf(sta_file,"sta_%s",my_record[ista].name);
		//printf(" MMMMM output stat %s sta_file %s \n", my_record[ista].name, sta_file);
		//output_array1(sta_file,my_record[ista].phase_win, npts_phase);

	}
	//int output_array1( char* output_name, double* array1,int file_num)
	//output_array1("my_EW",EW,npts_phase);


	/*

	// 0. use all good records to stack new EW
	int loop_num;
	int loop_num_max = 2;
	for(loop_num = 2; loop_num <= loop_num_max ; loop_num ++)
	{
		printf(" loop %d / %d \n", loop_num , loop_num_max);

		my_input->iteration_flag = loop_num;
		empirical_source_for_each_record(my_record,my_input,EW,loop_num);
		get_ONSET_ENDSET_for_each_record_origional_phase(my_record,my_input,EW);
		restack_ES_for_phase_code_choice(my_record,my_input,EW);
	}

	store_ES_into_record(my_record,my_input,EW);
	find_best_match_gaussian_for_iterative_ES(my_record,my_input,EW);
	output_STD_of_first_ES(my_record,my_input, EW);
	output_current_ES_for_phase(my_input, EW);

	
	// 1. stretched records to EW
	strcpy(my_input->stretch_flag, "stretch");
	stretch_ES_and_CCC(my_record,my_input,EW);
	*/


	// 2. stretch records to restack
	stretch_record_restack_ES_based_on_code_choice(my_record,my_input,stretched_EW);
	output_STD_of_second_ES(my_record,my_input, stretched_EW);
	output_current_ES_for_phase_second(my_input, stretched_EW);

	output_array1("my_EW",stretched_EW,npts_phase);

	//3. tstar
	strcpy(my_input->stretch_flag,"tstar");
	stretch_ES_and_CCC_tstar(my_record, my_input, stretched_EW);

	get_ONSET_ENDSET_for_each_record_stretched(my_record,my_input);
	define_goodness_of_record(my_record, my_input);
	output_STD_of_third_ES(my_record,my_input, stretched_EW);
	output_current_ES_for_phase_third(my_input, stretched_EW);


	define_stretch_EW_ONSET( my_record, my_input);
	output_ES_for_each_record(my_record, my_input);
	//redefine_beyon_wind_flag(my_record,my_input,stretched_EW,stretched_EW);
}




