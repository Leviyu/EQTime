#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>
int read_input_info(new_INPUT* my_input)
{
fprintf(my_input->out_logfile,"---> Read INFILE_PHASE info\n");
	// read in input file
	FILE* in;
	char INFILE_name[200];
	sprintf(INFILE_name,"INFILE_%s",my_input->PHASE);
	in=fopen(INFILE_name,"r");
	char buff[200];
	char read_tmp[200];
	char tmp[200];
	int N=200,count;


	int file_num = count_file_num(INFILE_name);
//printf("file name is %s file num is %d \n",INFILE_name,file_num);

	for(count = 0;count < file_num; count ++)
	{
		fgets(read_tmp,N, in);              // line
		sscanf(read_tmp,"%s" ,buff);
//printf("read in %s \n",buff);
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

	// output input
	//FILE* out;
	//out=fopen("input.out","w");
	//fprintf(out,"EQ is %s\n",my_input->EQ);
	//fprintf(out,"eventfile %s\n",my_input->event_file);
	//fprintf(out,"phase is  %s\n",my_input->PHASE);
	//fprintf(out,"comp is %s\n",my_input->COMP);
	//fprintf(out,"ve_or_di is %s\n",my_input->velocity_or_displacement);
	//fprintf(out,"long_beg_len is %lf %lf \n",my_input->long_beg, my_input->long_len);
	//fprintf(out,"phase_beg_len is %lf %lf \n",my_input->phase_beg, my_input->phase_len);
	//fprintf(out,"noise_beg_len is %lf %lf \n",my_input->noise_beg, my_input->noise_len);
	//fprintf(out,"dist_min_max is %lf %lf \n",my_input->distmin, my_input->distmax);
	//fclose(out);	

	return 0;
}
