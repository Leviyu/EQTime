
#include "ESF.h"
// stretch ES to find the best waveform to match each record
int SC_LocalAverage(new_RECORD* my_record, new_INPUT* my_input, new_STATION* my_station)
{
	puts("---> SC_LocalAverage");

	double dist_A_B(double A_lat, double A_lon, double B_lat, double B_lon);

	int ista, count;
	int ista_tmp = 0;
	double lat1,lat2,lon1,lon2;
	double DIST;
	int count2;
	double weight_sum = 0;
	int current_neighbour;


	int out_put_inc = 100;
	int out_iterations = 1;

	for(count = 0 ; count < my_input->MAX_STA; count ++)
	{
		my_station[count].neighbour_sta_num = 0;
		my_station[count].neighbour_dt_ave = 0;
		weight_sum = 0;
		if(count == out_put_inc*out_iterations)
		{
			out_iterations ++;
printf("--> LocalAverage working on uniq station %d \n", count);
		}

		if(my_station[count].count == 0)
			continue;


		for(count2 = 0 ; count2 < my_input->MAX_STA; count2 ++)
		{
			if(count == count2 )
				continue;
		if(my_station[count2].count == 0)
			continue;


			// define the distance between 2 different station
			lat1 = my_station[count].lat;
			lon1 = my_station[count].lon;
			lat2 = my_station[count2].lat;
			lon2 = my_station[count2].lon;
			DIST = dist_A_B(lat1, lon1, lat2, lon2);
			DIST = DIST /111;
			if(DIST  <= my_input->local_average_radius )
			{
				// store info
				current_neighbour = my_station[count].neighbour_sta_num ;

				my_station[count].neighbour_dt_ave += my_station[count2].dt_ave;

				 my_station[count].neighbour_sta_num ++;
				 // here we use 1 as weight for each station, can apply gaussian weight later
				 weight_sum = weight_sum +1;


			}

		}

		my_station[count].neighbour_dt_ave /= weight_sum;

	}


	// output neighbour info for each uniq station
	FILE* out2;
	char out_name[200];
	int kkk;
	for(count = 0 ; count < my_input->MAX_STA; count ++)
	{
		if(my_station[count].count == 0)
			continue;

		sprintf(out_name, "out.neighbours_of_%s", my_station[count].name);
		out2 = fopen(out_name,"w");
		for(kkk = 0 ; kkk < my_station[count].neighbour_sta_num ; kkk ++)
		{
			 // sta_name, lat lon, dt , std 
			 fprintf(out2, "%10s %10.3lf %10.3lf %10.3lf %10.3lf \n",
					 my_station[count2].name,
					 my_station[count2].lat,
					 my_station[count2].lon, 
					 my_station[count2].dt_ave,
					 my_station[count2].dt_STD);
		}
		fclose(out2);

	}

	// output big info for mystation
	FILE* out;
	out = fopen("out.my_station.info","w");
	for(count = 0; count < my_input->MAX_STA; count++)
	{
		if(my_station[count].count == 0)
			continue;
		// name
		// lat 
		// lon
		// dt_ave
		// dt_STD
		// record count in current sta
		// neighbour station num
		// neighbour station dt average
		fprintf(out, "%10s %10.3lf %10.3lf %10.3lf %10.3lf %5d %5d %10.3lf \n", my_station[count].name, my_station[count].lat, my_station[count].lon, my_station[count].dt_ave, my_station[count].dt_STD, my_station[count].count, my_station[count].neighbour_sta_num ,my_station[count].neighbour_dt_ave);

	}

	fclose(out);




	return 0;
}

