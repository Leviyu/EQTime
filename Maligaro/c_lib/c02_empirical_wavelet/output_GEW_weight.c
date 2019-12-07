
#include "ESF.h"
/******************************************************************
 * This is a c script for output current empirical source
******************************************************************/

int output_GEW_weight(new_INPUT* my_input, new_RECORD * my_record)
{
	printf("---> output GEW weights \n");

	// output the weight of each individual record
	int i;
	char command[300] = "GEW_stack_weight.data";
	FILE *out2;
	out2 = fopen(command,"w");
	for( i = 0 ; i < my_input->sta_num ; i++)
	{
		fprintf(out2,"%s %f", my_record[i].name, my_record[i].weight);	
	}

	fclose(out2);
	return 0;
}

