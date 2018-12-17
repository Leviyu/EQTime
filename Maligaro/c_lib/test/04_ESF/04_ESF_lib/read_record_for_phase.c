#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>
#include<time.h>
#include "hongyulib.h"
int read_record_for_phase(new_RECORD* my_record, new_INPUT* my_input)
{
	fprintf(my_input->out_logfile,"---> Read record for phase Begin Here ");
	int i,j,k;
	int read_record_for_phase_for_one_sta(new_RECORD* my_record, new_INPUT* my_input);

	for(i=0; i<my_input->sta_num;i++)
	{
		//read for each station
		read_record_for_phase_for_one_sta(&my_record[i], my_input);
	}
	return 0;
}


int read_record_for_phase_for_one_sta(new_RECORD* my_record, new_INPUT* my_input)
{

	char sac_file[100];
	char instrument[10];
	strcpy(my_record->COMP,my_input->COMP);
	// find sac file name whether it is BH or HH station
	find_instrument_type(my_record,instrument);

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


	sprintf(taup_command,"csh %s/code_dir/c100.get_taup_time_for_record.sh %s %s %s |awk 'NR==1 {print $1}' ", my_input->PWD, my_record->EQ, my_record->name, my_record->PHASE);
	shell_pipe_double(taup_command, &prem_tmp);
	my_record->prem = prem_tmp;

	if(prem_tmp == 0)
		my_record->quality = 0;

	//printf(" === Working on %s  taup time %lf \n", sac_file, prem_tmp);
	//fprintf(my_input->out_logfile, "SAC %s PREM TIME %lf \n", sac_file, prem_tmp);

	// do travel time traffic checking 

	//int EW_travel_time_traffic_checking(new_RECORD* my_record, new_INPUT* my_input)
	int traffic_flag = EW_travel_time_traffic_checking(my_record, my_input);

	initiate_phase(my_record,prem_tmp,sac_file,  my_input);

	return 0;
}






