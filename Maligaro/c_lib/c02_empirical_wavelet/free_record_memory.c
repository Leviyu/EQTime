
#include "ESF.h"
int free_record_memory(new_RECORD* my_record, new_INPUT* my_input)
{
	printf("---> Free record memory\n");
	int i;

	/*
	// read in each phase for current phase
	for(i=0; i<my_input->sta_num;i++)
	{
		//free memory for each record
		printf(" %d / %d \n", i, my_input->sta_num);
		free(my_record[i].long_win);
		free(my_record[i].long_orig);
		free(my_record[i].phase_win);
		free(my_record[i].noise_win);
		free(my_record[i].ES_win);
		free(my_record[i].stretched_ES_win);
	}
	*/
	return 0;
}
