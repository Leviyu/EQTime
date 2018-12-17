#include "ESF.h"



// Store iterative constructed ES into every record
void store_ES_into_record(new_RECORD* my_record, new_INPUT* my_input, double* current_ES)
{


	int count;
	int ista;
	for(ista = 0; ista < my_input->sta_num; ista++)
	{
		for(count = 0; count < (int)(my_input->phase_len/ my_input->delta); count++ )
			my_record[ista].ES_win[count] = current_ES[count];

	}
}
