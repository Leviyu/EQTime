#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>
#include "hongyulib.h"

// single my_record
int EW_travel_time_traffic_checking(new_RECORD* my_record, new_INPUT* my_input)
{

	char traffic_phase[100][20];
	int iphase = 0;
	char taup_command[1000];
	double prem_tmp;
	double traffic_range_sec = 0;
	char upper_flag[200];


	if(strcmp(my_input->PHASE,"ScS") == 0)
	{
		iphase = 0;
		strcpy(traffic_phase[iphase],"S");
		iphase++;
		strcpy(traffic_phase[iphase],"sS");
		iphase++;
		strcpy(traffic_phase[iphase],"sSS");
		iphase++;
		strcpy(traffic_phase[iphase],"SS");
		iphase++;
		traffic_range_sec = 5;
	}
	else if(strcmp(my_input->PHASE,"S") == 0)
	{
		iphase = 0;
		strcpy(traffic_phase[iphase],"ScS");
		iphase++;
		strcpy(traffic_phase[iphase],"sS");
		iphase++;
		traffic_range_sec = 5;
	}
	else if(strcmp(my_input->PHASE,"Sdiff") == 0)
	{
		iphase = 0;
		strcpy(traffic_phase[iphase],"SKS");
		iphase++;
		traffic_range_sec = 5;
	}
	else if(strcmp(my_input->PHASE,"SS") == 0)
	{
		iphase = 0;
		strcpy(traffic_phase[iphase],"ScS");
		iphase++;
		strcpy(traffic_phase[iphase],"sScS");
		iphase++;
		traffic_range_sec = 20;
	}
	else if(strcmp(my_input->PHASE,"SSS") == 0)
	{
		iphase = 0;
		strcpy(traffic_phase[iphase],"ScS");
		iphase++;
		strcpy(traffic_phase[iphase],"sScS");
		iphase++;
		strcpy(traffic_phase[iphase],"sSS");
		iphase++;
		strcpy(traffic_phase[iphase],"ScSScS");
		iphase++;
		strcpy(traffic_phase[iphase],"sScSScS");
		iphase++;
		traffic_range_sec = 20;
	}
	else if(strcmp(my_input->PHASE,"ScSScS") == 0 )
	{
		iphase = 0;
		strcpy(traffic_phase[iphase],"SS");
		iphase++;
		strcpy(traffic_phase[iphase],"sSS");
		iphase++;
		strcpy(traffic_phase[iphase],"SSS");
		iphase++;
		strcpy(traffic_phase[iphase],"sSSS");
		iphase++;
		strcpy(traffic_phase[iphase],"SSSS");
		iphase++;
		strcpy(traffic_phase[iphase],"SSSSS");
		iphase++;
		traffic_range_sec = 30;
	}
	else if(strcmp(my_input->PHASE,"SSSS") == 0 )
	{
		iphase = 0;
		strcpy(traffic_phase[iphase],"ScSScS");
		iphase++;
		strcpy(traffic_phase[iphase],"ScSScSScS");
		iphase++;
		strcpy(traffic_phase[iphase],"sScSScS");
		iphase++;
		strcpy(traffic_phase[iphase],"sScSScSScS");
		iphase++;
		traffic_range_sec = 40;
	}
	else if(strcmp(my_input->PHASE,"SSSSS") == 0 )
	{
		iphase = 0;
		strcpy(traffic_phase[iphase],"SSSm");
		iphase++;
		strcpy(traffic_phase[iphase],"SSSS4");
		iphase++;
		strcpy(traffic_phase[iphase],"ScSScS");
		iphase++;
		strcpy(traffic_phase[iphase],"sScSScS");
		iphase++;
		strcpy(traffic_phase[iphase],"ScSScSScS");
		iphase++;
		strcpy(traffic_phase[iphase],"sScSScSScS");
		iphase++;
		traffic_range_sec = 40;
	}
	else if(strcmp(my_input->PHASE,"ScSScSScSm") == 0)
	{
		iphase = 0;
		strcpy(traffic_phase[iphase],"ScSScSScSScS");
		iphase++;
		strcpy(traffic_phase[iphase],"sScSScSScSScS");
		iphase++;
		strcpy(traffic_phase[iphase],"SSSSm");
		iphase++;
		strcpy(traffic_phase[iphase],"SSSSSm");
		iphase++;
		strcpy(traffic_phase[iphase],"SSSSSSm");
		iphase++;
		traffic_range_sec = 40;
	}
	else if(strcmp(my_input->PHASE,"ScSScSScS") == 0)
	{
		iphase = 0;
		strcpy(traffic_phase[iphase],"SSS");
		iphase++;
		strcpy(traffic_phase[iphase],"SSSS");
		iphase++;
		strcpy(traffic_phase[iphase],"SSSSS");
		iphase++;
		strcpy(traffic_phase[iphase],"SSSSSS");
		iphase++;
		strcpy(traffic_phase[iphase],"sSSSS");
		iphase++;
		strcpy(traffic_phase[iphase],"sSSSSS");
		iphase++;
		strcpy(traffic_phase[iphase],"sSSS");
		iphase++;
		traffic_range_sec = 40;
	}
	else if(strcmp(my_input->PHASE,"ScSScSScSScSm") == 0)
	{
		iphase = 0;
		strcpy(traffic_phase[iphase],"ScSScSScSScSScS");
		iphase++;
		strcpy(traffic_phase[iphase],"ScSScSScSScSScSScS");
		iphase++;
		strcpy(traffic_phase[iphase],"SSSSSm");
		iphase++;
		strcpy(traffic_phase[iphase],"SSSSSSm");
		iphase++;
		strcpy(traffic_phase[iphase],"sScSScSScSScSScS");
		iphase++;
		strcpy(traffic_phase[iphase],"sScSScSScSScSScSScS");
		iphase++;
		traffic_range_sec = 40;
	}
	else if(strcmp(my_input->PHASE,"ScSScSScSScS") == 0)
	{
		iphase = 0;
		strcpy(traffic_phase[iphase],"sScSScSScS");
		iphase++;
		strcpy(traffic_phase[iphase],"ScSScSScS");
		iphase++;
		strcpy(traffic_phase[iphase],"SSSm");
		iphase++;
		strcpy(traffic_phase[iphase],"SSSSm");
		iphase++;
		strcpy(traffic_phase[iphase],"SSSSSm");
		iphase++;
		strcpy(traffic_phase[iphase],"SSSSSSm");
		iphase++;
		traffic_range_sec = 40;
	}
	else if(strcmp(my_input->PHASE,"SSSm") == 0)
	{
		iphase = 0;
		strcpy(traffic_phase[iphase],"SSSS");
		iphase++;
		strcpy(traffic_phase[iphase],"SSSSS");
		iphase++;
		strcpy(traffic_phase[iphase],"SSSSSS");
		iphase++;
		strcpy(traffic_phase[iphase],"ScSScSScS");
		iphase++;
		strcpy(traffic_phase[iphase],"sSSSSS");
		iphase++;
		strcpy(traffic_phase[iphase],"sSSSSSS");
		iphase++;
		traffic_range_sec = 40;
	}
	else if(strcmp(my_input->PHASE,"SSSSm") == 0)
	{
		iphase = 0;
		strcpy(traffic_phase[iphase],"SSSSS");
		iphase++;
		strcpy(traffic_phase[iphase],"SSSSSS");
		iphase++;
		strcpy(traffic_phase[iphase],"ScSScSScSScS");
		iphase++;
		strcpy(traffic_phase[iphase],"sScSScSScSScS");
		iphase++;
		traffic_range_sec = 40;
	}
	else if(strcmp(my_input->PHASE,"SSSSSm") == 0)
	{
		iphase = 0;
		strcpy(traffic_phase[iphase],"ScSScSScSScS");
		iphase++;
		strcpy(traffic_phase[iphase],"ScSScSScSScSScS");
		iphase++;
		strcpy(traffic_phase[iphase],"sScSScSScSScS");
		iphase++;
		strcpy(traffic_phase[iphase],"sScSScSScSScSScS");
		iphase++;
		strcpy(traffic_phase[iphase],"SSSSSS");
		iphase++;
		traffic_range_sec = 40;
	}
	else if(strcmp(my_input->PHASE,"SSSSSS") == 0)
	{
		iphase = 0;
		strcpy(traffic_phase[iphase],"ScSScSScS");
		iphase++;
		strcpy(traffic_phase[iphase],"ScSScS");
		iphase++;
		strcpy(traffic_phase[iphase],"SSSm");
		iphase++;
		strcpy(traffic_phase[iphase],"SSSSm");
		iphase++;
		strcpy(traffic_phase[iphase],"SSSSSm");
		iphase++;
		traffic_range_sec = 40;
	}
	else if(strcmp(my_input->PHASE,"SSSSSSm") == 0)
	{
		iphase = 0;
		strcpy(traffic_phase[iphase],"ScSScSScSScSScS");
		iphase++;
		strcpy(traffic_phase[iphase],"sScSScSScSScSScS");
		iphase++;
		strcpy(traffic_phase[iphase],"ScSScSScSScS");
		iphase++;
		strcpy(traffic_phase[iphase],"sScSScSScSScS");
		iphase++;
		traffic_range_sec = 40;
	}
	else if(strcmp(my_input->PHASE,"ScSScSScSScSScS") == 0)
	{
		iphase = 0;
		strcpy(traffic_phase[iphase],"ScSScSScSScSm");
		iphase++;
		strcpy(traffic_phase[iphase],"sScSScSScSScSm");
		iphase++;
		strcpy(traffic_phase[iphase],"ScSScSScSm");
		iphase++;
		strcpy(traffic_phase[iphase],"sScSScSScSm");
		iphase++;
		strcpy(traffic_phase[iphase],"SSSS");
		iphase++;
		strcpy(traffic_phase[iphase],"SSSSS");
		iphase++;
		strcpy(traffic_phase[iphase],"SSSSSS");
		iphase++;
		traffic_range_sec = 40;
	}
	else if(strcmp(my_input->PHASE,"ScSScSScSScSScSm") == 0)
	{
		iphase = 0;
		strcpy(traffic_phase[iphase],"ScSScSScSScSScSScS");
		iphase++;
		strcpy(traffic_phase[iphase],"sScSScSScSScSScSScS");
		iphase++;
		strcpy(traffic_phase[iphase],"sScSScSScSScSScS");
		iphase++;
		traffic_range_sec = 40;
	}
	else if(strcmp(my_input->PHASE,"ScSScSScSScSScSScS") == 0)
	{
		iphase = 0;
		strcpy(traffic_phase[iphase],"ScSScSScSScSm");
		iphase++;
		strcpy(traffic_phase[iphase],"sScSScSScSScSm");
		iphase++;
		strcpy(traffic_phase[iphase],"ScSScSScSScSScSm");
		iphase++;
		strcpy(traffic_phase[iphase],"sScSScSScSScSScSm");
		iphase++;
		strcpy(traffic_phase[iphase],"SSSSm");
		iphase++;
		strcpy(traffic_phase[iphase],"SSSSSm");
		iphase++;
		strcpy(traffic_phase[iphase],"SSSSSSm");
		iphase++;
		traffic_range_sec = 40;
	}
	else if(strcmp(my_input->PHASE,"ScSScSScSScSScSScSm") == 0)
	{
		iphase = 0;
		strcpy(traffic_phase[iphase],"ScSScSScSScSScSScSScS");
		iphase++;
		strcpy(traffic_phase[iphase],"sScSScSScSScSScSScSScS");
		iphase++;
		traffic_range_sec = 40;
	}


	// now output the traffic_phase list
	char traffic_file[500];
	FILE* out;
	sprintf(traffic_file, "traffic_file.%s.%s.%s", my_record->EQ, my_record->name, my_record->PHASE);
	out = fopen(traffic_file,"w");

	// now compute and find all the traffic phase PREM time and decide if there is overlap
	int count;
	for(count = 0 ; count < iphase;  count++)
	{
		//printf("traffic phase is %s \n", traffic_phase[count]);
		sprintf(taup_command,"csh %s/code_dir/c100.get_taup_time_for_record.sh %s %s %s |awk 'NR==1 {print $1}' ", my_input->PWD, my_record->EQ, my_record->name, traffic_phase[count]);
		shell_pipe_double(taup_command, &prem_tmp);

		// compare the traffic_phase time with record PREM time, if close enough, we clasify it as bad
		if( fabs(my_record->prem - prem_tmp) <  traffic_range_sec) 
		{
			

			// for uppder going phase, we just use it for visual reference 
			sprintf(upper_flag,"%c",my_record->PHASE[0]);
			if(strcmp(upper_flag,"s")==0 || strcmp(upper_flag, "p") ==0)
				my_record->quality = 0;
			else 
				my_record->quality = -1;


			// we have some expection here
			// for S, we dont wanna to throw any traffic record since S is usually good
			if(strcmp(my_record->PHASE, "S") == 0 || strcmp(my_record->PHASE, "Sdiff") == 0|| strcmp(my_record->PHASE, "P") == 0|| strcmp(my_record->PHASE, "Pdiff") == 0)
				my_record->quality = 0;


			// phase_name, absolute_PREM, relative_PREM, flag
			// flag  too_close / good
			fprintf(out,"%15s %10.2lf %10.2lf %10s\n",traffic_phase[count], prem_tmp, prem_tmp - my_record->prem , "too_close");
		}
		else
		{
			fprintf(out,"%15s %10.2lf %10.2lf %10s\n",traffic_phase[count], prem_tmp, prem_tmp - my_record->prem , "good");
		}

	}

	

	fclose(out);

	return 0;
}






