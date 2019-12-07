#include "ESF.h"



void redefine_beyon_wind_flag(new_RECORD* my_record, new_INPUT* my_input, 
		double* current_ES,double* EW_new)
{
	printf("---> working on redefine_beyon_wind_flag\n");

	// 1. redefine the beyond_window_flag
	int ista;
	for(ista = 0; ista < my_input->sta_num ; ista++)
	{
		if( my_record[ista].dt_obs_prem > my_input->mask_win_max  )
			my_record[ista].quality = -1;
	}
	return ;
	

	//2. loop and iterative stack
	int loop_num;
	int loop_num_max = 5;
	for(loop_num = 2; loop_num <= loop_num_max ; loop_num ++)
	{
		printf(" loop %d / %d \n", loop_num , loop_num_max);

		my_input->iteration_flag = loop_num;
		empirical_source_for_each_record(my_record,my_input,current_ES,loop_num);
		get_ONSET_ENDSET_for_each_record_origional_phase(my_record,my_input,current_ES);
	}

	// 3. stretch
	strcpy(my_input->stretch_flag,"stretch");
	stretch_ES_and_CCC(my_record, my_input, current_ES);

	// 4.tstar
	strcpy(my_input->stretch_flag,"tstar");
	stretch_ES_and_CCC_tstar(my_record, my_input, EW_new);


	//6.stretch
	get_ONSET_ENDSET_for_each_record_stretched(my_record,my_input);

	//7.define goodness
	define_goodness_of_record(my_record, my_input);

	//8. define stretch
	define_stretch_EW_ONSET(my_record,my_input);

	//9. output ES
	output_long_phase_window(my_record, my_input);
	output_ES_for_each_record(my_record, my_input);


}




