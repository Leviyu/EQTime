#include "hongyulib.h"
#include "ESF.h"

int get_first_ES(new_RECORD* my_record, new_INPUT* my_input, double* ES)
{
	printf( "---> Getting first E.W. now \n");
	int npts_phase = (int)(my_input->phase_len / my_input->delta);
	int i,j, num_of_record_in_ES;
	num_of_record_in_ES = 0;
	for(i=0;i<npts_phase;i++)
		ES[i] = 0.001;

	for(i=0;i<my_input->sta_num;i++)
	{
		// skip those with bad quality
		if( my_record[i].quality == -1)
			continue;

		//for S we define the stacking window
		// compute the signal to noise ratio of each record 
		// stack the rest to ES
		for(j=0;j<npts_phase;j++ )
		{
			ES[j] += my_record[i].phase_win[j];
		}
		num_of_record_in_ES ++;
	}

	// normalize ES
	normalize_array(ES,npts_phase);
	


	// output first ES
	char FIRST_ES[200] = "First_ES.out";
	output_array1(FIRST_ES,ES, npts_phase);

	printf("--->  Getting first E.W. DONE  %d records are included to make the first EW \n", num_of_record_in_ES);
	return 0;
}

