
#include "hongyulib.h"
#include "ESF.h"
int read_record_for_phase(new_RECORD* my_record, new_INPUT* my_input)
{
	printf("---> Read record for phase Begin Here \n");
	int i;
	reprocessing_get_prem_shift_time(my_record,my_input);

	for(i=0; i<my_input->sta_num;i++)
	{
		//read for each station
		printf("read record %d /%d %s \n", i,my_input->sta_num, my_record[i].name);
		read_record_for_phase_for_one_sta(&my_record[i], my_input);
	}
	return 0;
}


int read_record_for_phase_for_one_sta(new_RECORD* my_record, new_INPUT* my_input)
{

	char sac_file[200] = "hello";
	char instrument[10];
	strcpy(my_record->COMP,my_input->COMP);
	// find sac file name whether it is BH or HH station
	find_instrument_type(my_record,instrument);
//printf("sta %s instrument type is %s \n", my_record->name, instrument);
	// get sac file name
	sprintf(sac_file,"%s.%s.%s.%s%s.sac",my_record->EQ,my_record->NET,my_record->name,instrument,my_record->COMP);
	
	//  =================== get the prem time for current phase ==============================
	//  We grab the PREM time from the taup_time result in directory, if not exist, we calculate, first try -sta -evt, second try -dist
	//  For minor and major phase
	//	PREM time for minor phase stays to be the first result that has a travel time small then 180 degree
	//	PREM time for mahor phase stays to be the first result that has a travel time greater then 180 degree
	//
	char taup_command[400];
	double prem_tmp;

	my_record->quality = 0;
	my_record->weight = 0;
	my_record->traffic_phase_nearby = 0;
	my_record->SNR2 = 0;

	//sprintf(taup_command,"csh %s/code_dir/c104.get_taup_time_for_phase.sh %s  %s %s |awk 'NR==1 {print $1}' ", my_input->PWD, my_record->EQ, my_record->name, my_record->PHASE);
	sprintf(taup_command,"get_taup_time %s  %s %s ",  my_record->EQ, my_record->name, my_record->PHASE);
	shell_pipe_double(taup_command, &prem_tmp);
	my_record->prem = prem_tmp;

	if(prem_tmp == 0)
		my_record->quality = -1;

	//printf(" === Working on %s  taup time %lf \n", sac_file, prem_tmp);

	// do travel time traffic checking 

	//int EW_travel_time_traffic_checking(new_RECORD* my_record, new_INPUT* my_input)
	
	// lets get depth phase for current station
	//void get_depth_phase_for_current_station( new_RECORD* my_record)
	get_depth_phase_for_current_station(my_record);



	//printf("now initiate phase \n");
	int traffic_flag = EW_travel_time_traffic_checking(my_record, my_input);
	initiate_phase(my_record,prem_tmp,sac_file,  my_input);

	return 0;
}






