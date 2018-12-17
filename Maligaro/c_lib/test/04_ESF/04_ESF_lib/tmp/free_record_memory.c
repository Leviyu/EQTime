
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>
int free_record_memory(new_RECORD* my_record, new_INPUT* my_input)
{
	puts("---> Free record memory");
	int i,j,k;

	// read in each phase for current phase
	for(i=0; i<my_input->sta_num;i++)
	{
		//free memory for each record
		free(my_record[i].T[0].long_win);
		free(my_record[i].T[0].phase_win);
		free(my_record[i].T[0].noise_win);
		free(my_record[i].T[0].ES_win);
		free(my_record[i].T[0].stretched_ES_win);
		free(my_record[i].T[0].phase_name);
	}
	return 0;
}
