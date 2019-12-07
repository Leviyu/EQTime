
#include "hongyulib.h"
#include "ESF.h"
int read_record_for_phase_for_stacking(new_RECORD* my_record, new_INPUT* my_input)
{
	puts("---> Read record for phase reading in sac ");
	int i,j,k;
	int read_record_for_phase_for_one_sta(new_RECORD* my_record, new_INPUT* my_input);


	for(i=0; i<my_input->sta_num;i++)
	{
		//read for each station
		read_record_for_phase_for_one_sta_for_stacking(&my_record[i], my_input);
	}
	puts("---> Read record for phase reading in sac DONE");
	return 0;
}
int read_record_for_phase_for_one_sta_for_stacking(new_RECORD* my_record, new_INPUT* my_input)
{
	double shell_pipe(char* command);
	int find_instrument_type();
	char sac_file[100];
	char instrument[10];

	strcpy(my_record->COMP,my_input->COMP);


	// find sac file name whether it is BH or HH station
	find_instrument_type(my_record,instrument);

	// get sac file name
	sprintf(sac_file,"%s.%s.%s.%s%s.sac",my_record->EQ,my_record->NET,my_record->name,instrument,my_record->COMP);

	
	// get the prem time for current phase
	char taup_command[400];
	double prem_tmp;
	char phase_tmp[200];
	strcpy(phase_tmp,my_input->PHASE);
	prem_tmp = my_input->TIME_REF;

	// initiate current phase
	initiate_phase(my_record,prem_tmp,sac_file,  my_input);


	// normalize long win
	normalize_array(my_record->long_win, (int) (my_input->long_len / my_input->delta));


	return 0;
}






