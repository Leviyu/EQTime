
#include "ESF.h"
int read_input_info(new_INPUT* my_input)
{
	printf("---> Read INFILE_PHASE info\n");
	// read in input file
	FILE* in;
	char INFILE_name[200];
	sprintf(INFILE_name,"INFILE_%s",my_input->PHASE);
	in=fopen(INFILE_name,"r");
	char buff[200];
	char read_tmp[200];
	int N=200,count;


	int file_num = count_file_num(INFILE_name);

	for(count = 0;count < file_num; count ++)
	{
		fgets(read_tmp,N, in);              // line
		sscanf(read_tmp,"%s" ,buff);
		if (strcmp(buff,"<COMP>") == 0)
			sscanf(read_tmp,"%s %s" ,buff, my_input->COMP);
		else if (strcmp(buff,"<filter_flag>") == 0)
			sscanf(read_tmp,"%s %s" ,buff, my_input->filter_flag);
		else if (strcmp(buff,"<Frequency_range>") == 0) // frequency range
			sscanf(read_tmp,"%s %lf %lf" ,buff, &my_input->filter_min, &my_input->filter_max);
		else if (strcmp(buff,"<LONG_BEG>") == 0)
			sscanf(read_tmp,"%s %lf" ,buff, &my_input->long_beg);
		else if (strcmp(buff,"<LONG_LEN>") == 0)
			sscanf(read_tmp,"%s %lf" ,buff, &my_input->long_len);
		else if (strcmp(buff,"<PHASE_BEG>") == 0)
			sscanf(read_tmp,"%s %lf" ,buff, &my_input->phase_beg);
		else if (strcmp(buff,"<PHASE_LEN>") == 0)
			sscanf(read_tmp,"%s %lf" ,buff, &my_input->phase_len);
		else if (strcmp(buff,"<NOISE_BEG>") == 0)
			sscanf(read_tmp,"%s %lf" ,buff, &my_input->noise_beg);
		else if (strcmp(buff,"<NOISE_LEN>") == 0)
			sscanf(read_tmp,"%s %lf" ,buff, &my_input->noise_len);
		else if (strcmp(buff,"<DIST_MASK_BEG>") == 0)
			sscanf(read_tmp,"%s %lf" ,buff, &my_input->DIST_MASK_BEG);
		else if (strcmp(buff,"<DIST_MASK_END>") == 0)
			sscanf(read_tmp,"%s %lf" ,buff, &my_input->DIST_MASK_END);
		else if (strcmp(buff,"<velocity_or_displacement>") == 0)
			sscanf(read_tmp,"%s %s" ,buff, my_input->velocity_or_displacement);
		else if (strcmp(buff,"<SNR_CUT>") == 0)
			sscanf(read_tmp,"%s %lf" ,buff, &my_input->SNR_CUT);
		else if (strcmp(buff,"<CCC_CUT>") == 0)
			sscanf(read_tmp,"%s %lf" ,buff, &my_input->CCC_CUT);
		else if (strcmp(buff,"<POLAR_SOLUTION>") == 0)
			sscanf(read_tmp,"%s %d" ,buff, &my_input->POLAR_SOLUTION);
		else if (strcmp(buff,"<SNR_phase>") == 0)
			sscanf(read_tmp,"%s %d %s" ,buff, &my_input->SNR_phase_flag, my_input->SNR_phase);
		else if (strcmp(buff,"<DIST_DELTA>") == 0)
			sscanf(read_tmp,"%s %lf " ,buff, &my_input->DIST_DELTA);
		else if (strcmp(buff,"<DELTA>") == 0)
			sscanf(read_tmp,"%s %lf " ,buff, &my_input->delta);
		else if (strcmp(buff,"<DIST_MIN_MAX>") == 0)
			sscanf(read_tmp,"%s %lf %lf " ,buff, &my_input->distmin, &my_input->distmax);
		else if (strcmp(buff,"<STACK_FLAG>") == 0)
			sscanf(read_tmp,"%s %d " ,buff, &my_input->STACK_FLAG);
		else if (strcmp(buff,"<TIME_REF>") == 0)
			sscanf(read_tmp,"%s %lf " ,buff, &my_input->TIME_REF);
		else if (strcmp(buff,"<AREA_RADIUS>") == 0)
			sscanf(read_tmp,"%s %lf " ,buff, &my_input->AREA_RADIUS);
		else if (strcmp(buff,"<AREA_LAT>") == 0)
			sscanf(read_tmp,"%s %lf %lf %lf  " ,buff, &my_input->lat_beg, &my_input->lat_end, &my_input->lat_delta);
		else if (strcmp(buff,"<AREA_LON>") == 0)
			sscanf(read_tmp,"%s %lf %lf %lf  " ,buff, &my_input->lon_beg, &my_input->lon_end, &my_input->lon_delta);
	}
	fclose(in);

	char event_file[200];
	sprintf(event_file,"eventStation.%s.%s.%s",my_input->EQ,my_input->PHASE,my_input->COMP);
	strcpy(my_input->event_file,event_file);
	char eventinfo_file[200];
	sprintf(eventinfo_file,"eventinfo.%s.%s.%s",my_input->EQ,my_input->PHASE,my_input->COMP);
	strcpy(my_input->eventinfo_file,eventinfo_file);


	// read INFILE information
	FILE* in2;
	char INFILE_name2[200];
	sprintf(INFILE_name2,"INFILE");
	in2=fopen(INFILE_name2,"r");


	file_num = count_file_num(INFILE_name2);

	for(count = 0;count < file_num; count ++)
	{
		fgets(read_tmp,N, in2);              // line
		sscanf(read_tmp,"%s" ,buff);
		if (strcmp(buff,"<T_star_lib>") == 0)
			sscanf(read_tmp,"%s %s" ,buff, my_input->tstar_lib);
		else if (strcmp(buff,"<ED_CHECK_FILE>") == 0)
			sscanf(read_tmp,"%s %s" ,buff, my_input->ed_checked_file);
		else if (strcmp(buff,"<ED_CHECK_FLAG>") == 0)
			sscanf(read_tmp,"%s %d " ,buff, &my_input->ED_CHECKED_FLAG);
		else if (strcmp(buff,"<Fix_missing_sta_flag>") == 0)
			sscanf(read_tmp,"%s %d " ,buff, &my_input->Fix_missing_sta_flag);
		else if (strcmp(buff,"<Reprocessing_Flag>") == 0)
			sscanf(read_tmp,"%s %d " ,buff, &my_input->Reprocessing_Flag);
		else if (strcmp(buff,"<PLOT_FLAG>") == 0)
			sscanf(read_tmp,"%s %d " ,buff, &my_input->PLOT_FLAG);
		else if (strcmp(buff,"<velocity_or_displacement>") == 0)
			sscanf(read_tmp,"%s %s" ,buff, my_input->velocity_or_displacement);
	}

	fclose(in2);

	// initiate mask_win_min & mask_win_max
	if(strcmp(my_input->PHASE, "Sdiff") == 0)
	{
		my_input->mask_win_min = -15;
		my_input->mask_win_max = 25;
	}
	else
	{
		my_input->mask_win_min = -15;
		my_input->mask_win_max = 20;
	}


	// initiate traffic_phase_file
	char traffic_file_name[200];
	sprintf(traffic_file_name,"traffic_file.%s.%s",my_input->EQ, my_input->PHASE);
	my_input->traffic_phase_file = fopen(traffic_file_name,"w");


	return 0;
}
