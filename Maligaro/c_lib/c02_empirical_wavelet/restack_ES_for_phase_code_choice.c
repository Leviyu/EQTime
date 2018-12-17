
#include "hongyulib.h"
#include "ESF.h"
/******************************************************************
 * This is a c script to restack EW using records that 
 * with good SNR and ccc
 *
 *	Input:
 *
 *
 *	Output:
 *
 *
 *	DATE:				Keywords:
 *	Reference:
******************************************************************/

int restack_ES_for_phase_code_choice(new_RECORD* my_record, new_INPUT* my_input, double* ES)
{
printf("---> Restack E.W. with code choice\n ");
	int npts_phase = (int)( my_input->phase_len / my_input->delta  );
	int ista,i;
	int num_records_make_new_ES=0;

	// empty ES
	for(i=0;i<npts_phase;i++)
		ES[i] = 0;

	for(ista=0;ista< my_input->sta_num; ista ++)
	{
		if(my_record[ista].quality <= 0)
			continue;


		my_record[ista].weight = get_weight_from_SNR_CCC( my_record[ista].SNR, my_record[ista].ccc);
		if(strstr(my_record[ista].name, "PPP") != NULL )
			continue;

		if(my_record[ista].weight == 0)
			continue;

		// hongyu change
		// here we grab records from eventinfo.ed_checked
		// and we only use records that we picked already to construct the S E.W.
		if(my_input->ED_CHECKED_FLAG == 1 )
		{
			if(strcmp( my_record[ista].PHASE, "S" )  == 0 && my_record[ista].checked_to_be_good == 1)
			{
				double kkk = 1;
			}
			else 
			{
				continue;
			}
		}


		for(i=0;i<npts_phase;i++)
		{
			if(my_record[ista].phase_win[i] != my_record[ista].phase_win[i]) 
				my_record[ista].phase_win[i] = 0;
			ES[i] += my_record[ista].phase_win[i]*my_record[ista].weight;
		}
		num_records_make_new_ES++;
	}
	printf("---> Restack E.W. Done %d records is included to remake the ES \n",num_records_make_new_ES);

	if(num_records_make_new_ES == 0)
	{
		printf("--> if 0 record is needed to restack E.W. then we use everytrace with the same weight to stack for one \n");

		for(ista=0;ista< my_input->sta_num; ista ++)
		{
			//if(my_record[ista].quality == -1)
				//continue;
			//if(strstr(my_record[ista].name, "PPP") != NULL)
				//continue;
				//my_record[ista].weight = get_weight_from_SNR_CCC( my_record[ista].SNR, my_record[ista].ccc);
				//if(my_record[ista].weight == 0)
					//continue;
			//printf(" %d th record weight is %lf \n", ista, my_record[ista].weight);
			for(i=0;i<npts_phase;i++)
			{
				if(my_record[ista].phase_win[i] != my_record[ista].phase_win[i]) 
				{
					my_record[ista].phase_win[i] = 0;
					printf(" ERROR  NULL  value found !\n");
				}
				ES[i] += my_record[ista].phase_win[i];
			}
			num_records_make_new_ES++;
		}
		//output_array1("ttt.ttt",ES,npts_phase);
		printf("---> Restack E.W. Done %d records is included to remake the ES \n",num_records_make_new_ES);
		normalize_array(ES, npts_phase);
		return 1;

	}

	return 0;
}
