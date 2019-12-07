#include "../c01_tools/hongyulib.h"
#include "../c02_empirical_wavelet/ESF.h"

int main(int argc, char* argv[])
{
	puts("======================== C CODE BEGIN HERE ==============");
	char EQ_name[200];
	char PHASE_name[200];
	char logfile_name[200];
	double delta;

	// read in EQ and PHASE
	FILE* in;
	in = fopen(argv[1], "r");
	fscanf(in, "%s %s %lf ",EQ_name, PHASE_name, &delta);

	// read in input info
	new_INPUT my_input;
	initiate_input(&my_input);
	strcpy(my_input.EQ,EQ_name);
	strcpy(my_input.PHASE,PHASE_name);
	sprintf(logfile_name, "logfile.%s.%s",my_input.EQ, my_input.PHASE);
	my_input.logfile = logfile_name;
	my_input.out_logfile = fopen(my_input.logfile,"w");
	my_input.delta = delta;

	printf("---> Working on Reading in info, EQ: %s PHASE %s \n", EQ_name, PHASE_name);
	read_input_info(&my_input);
	my_input.sta_num =  count_file_num(my_input.event_file);
	printf("Total record number is %d \n",my_input.sta_num);

	new_RECORD my_record[my_input.sta_num];
	read_eventstation_list(my_record, &my_input);
	read_record_for_phase(my_record, &my_input);
	empirical_source_function(my_record,&my_input);
	output_final_event_info(my_record, &my_input);
	free_record_memory(my_record, &my_input);
	puts("======================== C CODE END HERE ==============");
	return 0;
}


