#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>
int read_record_for_phase(new_RECORD* my_record, new_INPUT* my_input)
{
	int i,j,k;
	int read_record_for_phase_for_one_sta(new_RECORD* my_record, new_INPUT* my_input);

	// read in each phase for current phase
	for(i=0; i<my_input->sta_num;i++)
	{
		//read for each station
		read_record_for_phase_for_one_sta(&my_record[i], my_input);
	}
	return 0;
}


int read_record_for_phase_for_one_sta(new_RECORD* my_record, new_INPUT* my_input)
{
	double shell_pipe(char* command);
	int find_instrument_type();
	int phase_flag = 0;	
	char sac_file[100];
	char instrument[10];

	strcpy(my_record->COMP,my_input->comp);


	// find sac file name whether it is BH or HH station
	find_instrument_type(my_record,instrument);

	// get sac file name
	sprintf(sac_file,"%s.%s.%s.%s%s.sac",my_record->eq_name,my_record->NET,my_record->name,instrument,my_record->COMP);

	// if is S and dist >80 < 90 then read in SV component and output it 

	
	// get the prem time for current phase
	char taup_command[400];
	double prem_tmp;

	sprintf(taup_command,"taup_time -mod prem -h %lf -ph %s  -deg %lf| awk 'NR==6 {print $4}' ",my_record->eq_dep, my_input->phase, my_record->DIST);
	prem_tmp = shell_pipe(taup_command);

	// initiate current phase
	initiate_phase(&my_record->T[phase_flag],prem_tmp,sac_file,  my_input, my_record->name);

	//output long window and phase window
	output_long_phase_window(my_record,phase_flag, my_input);	

printf("SNR for sta %s is %lf\n",my_record->name,my_record->T[phase_flag].SNR);
	return 0;
}
