#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>
// stretch ES to find the best waveform to match each record
int SC_read_uniq_sta_list(new_RECORD* my_record, new_INPUT* my_input, new_STATION* my_station)
{
	puts("---> SC_read_uniq_sta_list");


	int ista, count;

	FILE* in; 
	in = fopen(my_input->uniq_sta_list,"r");


	for(count = 0 ; count < my_input->MAX_STA; count ++)
	{
		fscanf(in, "%s ", my_station[count].name);
	}




	return 0;
}

