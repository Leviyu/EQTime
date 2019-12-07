
#include "hongyulib.h"
#include "ESF.h"
/******************************************************************
 * stretch each record using stretch_facotr 
 * and then restack for new ES
 *
******************************************************************/

int stretch_record_restack_ES_based_on_code_choice(new_RECORD* my_record, new_INPUT* my_input, double* ES)
{
	printf("---> stretch_record_restack_ES \n");
	int npts_phase = (int)( my_input->phase_len / my_input->delta  );
	double ES_tmp[npts_phase];
	int ista,i;
	int num_records_make_new_ES=0;

	double stretch_factor_record;

	// initiate ES
	for(i=0;i<npts_phase;i++)
		ES[i] = 0;
	


	for(ista=0;ista< my_input->sta_num; ista ++)
	{
		if( my_record[ista].quality <= 0)
			continue;

		if( my_record[ista].best_stretch_coefficient == 0)
			my_record[ista].best_stretch_coefficient = 1;
		stretch_factor_record = 1/my_record[ista].best_stretch_coefficient;
		//stretch phase win using stretch factor
		stretch_ES_function(my_record[ista].phase_win, npts_phase, stretch_factor_record, my_record[ista].stretched_phase_win);
		my_record[ista].stretched_phase_win_flag = 1;

		if(strstr(my_record[ista].name, "PPP") != NULL)
			continue;

		if(my_record[ista].weight == 0)
			continue;
		for(i=0;i<npts_phase;i++)
		{
			if(my_record[ista].stretched_phase_win[i] != my_record[ista].stretched_phase_win[i] )
				my_record[ista].stretched_phase_win[i] = 0;

			//ES_tmp[i] += stretch_tmp[i]*my_record[ista].weight;
			ES[i] += my_record[ista].stretched_phase_win[i]*my_record[ista].weight;
		}
		num_records_make_new_ES++;
	}
	printf( "---> Restack stretch_record_restack_ES  E.W. Done %d records is included to remake the ES \n",num_records_make_new_ES);


	if(num_records_make_new_ES == 0)
	{
		printf(" if 0 record is needed to restack E.W. then we use everytrace with the same weight to stack for one \n");

		for(ista=0;ista< my_input->sta_num; ista ++)
		{

			// if current record is not stretched yet
			if(my_record[ista].stretched_phase_win_flag != 1)
			{
				if(my_record[ista].best_stretch_coefficient == 0)
					my_record[ista].best_stretch_coefficient = 1;
				stretch_factor_record = 1/my_record[ista].best_stretch_coefficient;
				stretch_ES_function(my_record[ista].phase_win, npts_phase, stretch_factor_record, my_record[ista].stretched_phase_win);
			}

			for(i=0;i<npts_phase;i++)
			{
				if(my_record[ista].stretched_phase_win[i] != my_record[ista].stretched_phase_win[i] )
					my_record[ista].stretched_phase_win[i] = 0;

				ES[i] += my_record[ista].stretched_phase_win[i];
			}
			num_records_make_new_ES++;

		}
		printf(" ---> Restack E.W. Done %d records is included to remake the ES \n",num_records_make_new_ES);
	}

	return 0;
}
