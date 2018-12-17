#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>
#define DEBUG 0

int SC_initiate_station(new_STATION* my_station, new_INPUT* my_input)
{
	puts(" initiate station here !!!!!" );
fflush(stdout);
	int MAX = 10000;
	int count;
	int ista;

	for(ista = 0; ista < my_input->MAX_STA; ista++)
	{
	//printf("initiate station for %d \n", ista);
		my_station[ista].name = (char*)malloc(sizeof(char)*100);
		my_station[ista].dt_array = (double*)malloc(sizeof(double)*MAX);
		my_station[ista].neighbour_sta_dt_array = (double*)malloc(sizeof(double)*MAX);
		my_station[ista].BAZ = (double*)malloc(sizeof(double)*MAX);
		my_station[ista].incident = (double*)malloc(sizeof(double)*MAX);

		my_station[ista].neighbour_sta_array = (char**)malloc(sizeof(char*)*MAX);
		for(count = 0; count < MAX; count++)
			my_station[ista].neighbour_sta_array[count] = (char*)malloc(sizeof(char)*10);
	}

	return 0;
}
