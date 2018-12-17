#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>
// stretch ES to find the best waveform to match each record
int SC_single_station(new_RECORD* my_record, new_INPUT* my_input, new_STATION* my_station)
{
	puts("---> SC_single_station");


	int ista, count;
	int ista_tmp = 0;

	double BAZ_tmp;
	double sin_incident;
	int MAX = 1000000;
	double* BAZ;
	double* incident;
	char command[1000];
	BAZ = (double*)malloc(sizeof(double)*MAX);
	incident = (double*)malloc(sizeof(double)*MAX);

	int out_put_inc = 100;
	int out_iterations = 1;


	for(count = 0 ; count < my_input->MAX_STA; count ++)
	{

		my_station[count].dt_ave = 0;
		my_station[count].dt_STD = 0;
		my_station[count].count = 0;

		if(count == out_put_inc*out_iterations)
		{
			out_iterations ++;
printf("--> working on uniq station %d \n", count);
		}
		// loop through my record to find match
		// find dt_ave here
		for(ista = 0; ista < my_input->sta_num; ista++)
		{
			if(strcmp( my_record[ista].name, my_station[count].name) == 0)
			{
				my_station[count].dt_ave += my_record[ista].dt_obs_prem;


				ista_tmp = ista;
				my_station[count].lat = my_record[ista_tmp].sta_lat;
				my_station[count].lon = my_record[ista_tmp].sta_lon;

				//store BAZ
				BAZ_tmp = my_record[ista].BAZ;
				//BAZ[my_station[count].count ] = my_record[ista].BAZ;
				my_station[count].BAZ[my_station[count].count ] = BAZ_tmp;
				//
				// store incident angle
				sprintf(command,"cat /DATA2/taup_time_dir/taup_time.%s.%s.%s.T|awk 'NR==6 {print $7}'",
						my_record[ista].EQ,
						my_record[ista].name,
						my_record[ista].PHASE);

				//sprintf(command,"taup_time -mod prem -evt %lf %lf -sta %lf %lf -h %lf -ph %s |awk 'NR==6 {print $7}'", 
						//my_record[ista].eq_lat, my_record[ista].eq_lon, my_record[ista].sta_lat,
						//my_record[ista].sta_lon, my_record[ista].eq_dep, my_record[ista].PHASE);
				sin_incident = shell_pipe(command);
				// convert the rad
				sin_incident = sin( sin_incident / 180 * 3.1415926);
				my_station[count].incident[ my_station[count].count ] = sin_incident;


				// store dt into dt_array for current uniq station
				my_station[count].dt_array[ my_station[count].count ] = my_record[ista].dt_obs_prem;
				

				my_station[count].count++;

			}

		}


		if(my_station[count].count == 0)
			continue;

		my_station[count].dt_ave = my_station[count].dt_ave / my_station[count].count;

		// loop through my record to find match
		// find dt_STD here
		for(ista = 0; ista < my_input->sta_num; ista++)
		{
			if(strcmp( my_record[ista].name, my_station[count].name) == 0)
			{
				my_station[count].dt_STD += pow(my_record[ista].dt_obs_prem- my_station[count].dt_ave, 2);
			}

		}
		my_station[count].dt_STD = sqrt(my_station[count].dt_STD/my_station[count].count);

//printf("ave STD %lf %lf \n", my_station[count].dt_ave, my_station[count].dt_STD);
	}



	// output all records at each uniq station
	FILE* out_current;
	char out_current_file[200];
	int kkk;
	double dt_tmp;

	for(count = 0 ; count < my_input->MAX_STA; count ++)
	{
		if(my_station[count].count == 0)
			continue;

		sprintf(out_current_file,"out.records_of_%s",my_station[count].name);
		out_current = fopen(out_current_file,"w");
		for(kkk = 0; kkk< my_station[count].count; kkk++)
		{
			BAZ_tmp = my_station[count].BAZ[kkk];
			sin_incident = my_station[count].incident[kkk];
			dt_tmp = my_station[count].dt_array[kkk];
			fprintf(out_current,"%10.3lf %10.3lf %10.3lf\n", BAZ_tmp,sin_incident, dt_tmp);
		}
		fclose(out_current);
	}






	// output info for mystation
	//FILE* out;
	//out = fopen("out.my_station.info","w");
	//for(count = 0; count < my_input->MAX_STA; count++)
	//{
		//(my_station[count].count == 0)
			//continue;
		////output name lat lon dt+ave dt_STD count 
		//fprintf(out, "%10s %10.3lf %10.3lf %10.3lf %10.3lf %5d \n", my_station[count].name, my_station[count].lat, my_station[count].lon, my_station[count].dt_ave, my_station[count].dt_STD, my_station[count].count);
	//}

	//fclose(out);




	return 0;
}

