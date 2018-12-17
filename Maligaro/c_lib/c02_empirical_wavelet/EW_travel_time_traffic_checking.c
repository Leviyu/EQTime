#include "ESF.h"
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
		strcpy(traffic_phase[iphase],"sScS");
		iphase++;
		strcpy(traffic_phase[iphase],"SKS");
		iphase++;
		strcpy(traffic_phase[iphase],"sSKS");
		iphase++;
		traffic_range_sec = 15;
	}
	else if(strcmp(my_input->PHASE,"S") == 0)
	{
		iphase = 0;
		strcpy(traffic_phase[iphase],"ScS");
		iphase++;
		strcpy(traffic_phase[iphase],"sS");
		iphase++;
		strcpy(traffic_phase[iphase],"SKS");
		iphase++;
		strcpy(traffic_phase[iphase],"sSKS");
		iphase++;
		traffic_range_sec = 15;
	}
	else if(strcmp(my_input->PHASE,"P") == 0)
	{
		iphase = 0;
		strcpy(traffic_phase[iphase],"PcP");
		iphase++;
		strcpy(traffic_phase[iphase],"pP");
		iphase++;
		strcpy(traffic_phase[iphase],"PP");
		iphase++;
		strcpy(traffic_phase[iphase],"pPP");
		iphase++;
		strcpy(traffic_phase[iphase],"Pdiff");
		iphase++;
		strcpy(traffic_phase[iphase],"sP");
		iphase++;
		strcpy(traffic_phase[iphase],"PP");
		iphase++;
		strcpy(traffic_phase[iphase],"sPP");
		iphase++;
		strcpy(traffic_phase[iphase],"pPcP");
		iphase++;
		strcpy(traffic_phase[iphase],"sPcP");
		iphase++;
		traffic_range_sec = 15;
	}
	else if(strcmp(my_input->PHASE,"PcP") == 0)
	{
		iphase = 0;
		strcpy(traffic_phase[iphase],"pPcP");
		iphase++;
		strcpy(traffic_phase[iphase],"P");
		iphase++;
		strcpy(traffic_phase[iphase],"Pdiff");
		iphase++;
		strcpy(traffic_phase[iphase],"PP");
		iphase++;
		strcpy(traffic_phase[iphase],"pPP");
		iphase++;
		strcpy(traffic_phase[iphase],"sPP");
		iphase++;
		traffic_range_sec = 15;
	}
	else if(strcmp(my_input->PHASE,"Pdiff") == 0)
	{
		iphase = 0;
		strcpy(traffic_phase[iphase],"pPdiff");
		iphase++;
		strcpy(traffic_phase[iphase],"pPcP");
		iphase++;
		strcpy(traffic_phase[iphase],"Sdiff");
		iphase++;
		strcpy(traffic_phase[iphase],"pPP");
		iphase++;
		strcpy(traffic_phase[iphase],"sPP");
		iphase++;
		traffic_range_sec = 15;
	}
	else if(strcmp(my_input->PHASE,"Sdiff") == 0)
	{
		iphase = 0;
		strcpy(traffic_phase[iphase],"sSdiff");
		iphase++;
		strcpy(traffic_phase[iphase],"SKS");
		iphase++;
		strcpy(traffic_phase[iphase],"sSKS");
		iphase++;
		strcpy(traffic_phase[iphase],"SKKS");
		iphase++;
		strcpy(traffic_phase[iphase],"sSKKS");
		iphase++;
		traffic_range_sec = 15;
	}
	else if(strcmp(my_input->PHASE,"SS") == 0)
	{
		iphase = 0;
		strcpy(traffic_phase[iphase],"ScS");
		iphase++;
		strcpy(traffic_phase[iphase],"sScS");
		iphase++;
		strcpy(traffic_phase[iphase],"sSS");
		iphase++;
		strcpy(traffic_phase[iphase],"sS");
		iphase++;
		traffic_range_sec = 15;
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
		strcpy(traffic_phase[iphase],"sSSS");
		iphase++;

		traffic_range_sec = 15;
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
		strcpy(traffic_phase[iphase],"sScSScS");
		iphase++;
		traffic_range_sec = 15;
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
		strcpy(traffic_phase[iphase],"sSSSS");
		iphase++;
		traffic_range_sec = 20;
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
		strcpy(traffic_phase[iphase],"sSSSSS");
		iphase++;
		traffic_range_sec = 30;
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
		strcpy(traffic_phase[iphase],"sScSScSScSm");
		iphase++;
		traffic_range_sec = 30;
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
		traffic_range_sec = 30;
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
		strcpy(traffic_phase[iphase],"sScSScSScSScS");
		iphase++;
		traffic_range_sec = 30;
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
		strcpy(traffic_phase[iphase],"sSSSm");
		iphase++;
		traffic_range_sec = 30;
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
		strcpy(traffic_phase[iphase],"sSSSSm");
		iphase++;
		traffic_range_sec = 30;
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
		strcpy(traffic_phase[iphase],"sSSSSSm");
		iphase++;
		traffic_range_sec = 30;
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
		strcpy(traffic_phase[iphase],"sSSSSSS");
		iphase++;
		traffic_range_sec = 30;
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
		strcpy(traffic_phase[iphase],"sSSSSSSm");
		iphase++;
		traffic_range_sec = 30;
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
		traffic_range_sec = 30;
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
		traffic_range_sec = 30;
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
	//char traffic_file[500];
	//FILE* out;
	int count;
	//sprintf(traffic_file, "traffic_file.%s.%s.%s", my_record->EQ, my_record->name, my_record->PHASE);
	//out = fopen(traffic_file,"w");



	// construct traffic phase system
	my_record->num_traffic = iphase;
	my_record->traffic_phase = (char**)malloc(sizeof(char*)*iphase);
	int kkk;
	for(kkk = 0; kkk< iphase ; kkk++)
	{
		my_record->traffic_phase[kkk] = (char*)malloc(sizeof(char)*20);
	}
	my_record->traffic_time = (double*)malloc(sizeof(double)*iphase);


	// now compute and find all the traffic phase PREM time and decide if there is overlap
	for(count = 0 ; count < iphase;  count++)
	{
		sprintf(taup_command,"csh %s/code_dir/c104.get_taup_time_for_phase.sh  %s %s %s |awk 'NR==1 {print $1}' ", my_input->PWD, my_record->EQ, my_record->name,traffic_phase[count]);

		shell_pipe_double(taup_command, &prem_tmp);
		
		// get traffic name
		strcpy(my_record->traffic_phase[count], traffic_phase[count]);
		// get relative traffic time
		my_record->traffic_time[count] = prem_tmp - my_record->prem;
		my_record->traffic_range_sec = traffic_range_sec;

		// if is current phase`s depth phase, we dont deal with it
		if( strcmp(my_record->depth_phase,traffic_phase[count]) == 0)
		{
				fprintf(my_input->traffic_phase_file,"%10s %15s %10.2lf %10.2lf %10s\n",
						my_record->name, traffic_phase[count], prem_tmp, 
						prem_tmp - my_record->prem , "good");
			continue;
		}


		// for SKS/SKKS/sSKS/sSKKS, we dont deal with it
		if( strcmp(traffic_phase[count],"SKS") ==0 ||
				strcmp(traffic_phase[count],"SKKS") ==0 ||
				strcmp(traffic_phase[count],"sSKS") ==0 ||
				strcmp(traffic_phase[count],"sSKKS") ==0 )
		{
				fprintf(my_input->traffic_phase_file,"%10s %15s %10.2lf %10.2lf %10s\n",
						my_record->name, traffic_phase[count], prem_tmp, 
						prem_tmp - my_record->prem , "good");
			continue;
		}
		// if S is affected by ScS, we ignore
		// we have some expection here
		// for S, we dont wanna to throw any traffic record since S is usually good
		if(strcmp(my_record->PHASE, "S") == 0 && strcmp( traffic_phase[count], "ScS") == 0)
		{
			my_record->quality = 0;
			fprintf(my_input->traffic_phase_file,"%10s %15s %10.2lf %10.2lf %10s\n",
					my_record->name, traffic_phase[count], prem_tmp, 
					prem_tmp - my_record->prem , "good");
			continue;
		}
		if(strcmp(my_record->PHASE, "P") == 0 && strcmp( traffic_phase[count], "PcP") == 0)
		{
			my_record->quality = 0;
			fprintf(my_input->traffic_phase_file,"%10s %15s %10.2lf %10.2lf %10s\n",
					my_record->name, traffic_phase[count], prem_tmp, 
					prem_tmp - my_record->prem , "good");
			continue;
		}


		// compare the traffic_phase time with record PREM time, if close enough, we clasify it as bad
		if( fabs(my_record->prem - prem_tmp) <  traffic_range_sec) 
		{
			// for uppder going phase, we just use it for visual reference 
			//sprintf(upper_flag,"%c",my_record->PHASE[0]);
			//if(strcmp(upper_flag,"s")==0 || strcmp(upper_flag, "p") ==0)
				//my_record->quality = 0;
			//else 
			my_record->quality = -1;
			my_record->traffic_phase_nearby = 1;
			//printf("sta %s flag %d \n", my_record->name, my_record->traffic_phase_nearby);

			// phase_name, absolute_PREM, relative_PREM, flag
			// flag  too_close / good
			fprintf(my_input->traffic_phase_file,"%10s %15s %10.2lf %10.2lf %10s\n",
					my_record->name, traffic_phase[count], prem_tmp, 
					prem_tmp - my_record->prem , "too_close");
		}
		else
		{
			fprintf(my_input->traffic_phase_file,"%10s %15s %10.2lf %10.2lf %10s\n",
					my_record->name, traffic_phase[count], prem_tmp, 
					prem_tmp - my_record->prem , "good");
		}

	}

	


	return 0;
}






