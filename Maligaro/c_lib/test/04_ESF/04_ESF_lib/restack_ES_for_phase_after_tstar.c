#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<ESF.h>
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

int restack_ES_for_phase_after_tstar(new_RECORD* my_record, new_INPUT* my_input, double* ES)
{
	puts("---> Restack E.W. ");
	int npts_phase = (int)( my_input->phase_len / my_input->delta  );
	int ista,i;
	int num_records_make_new_ES=0;
	double ccc_tmp;
	for(ista=0;ista< my_input->sta_num; ista ++)
	{
		ccc_tmp = my_record[ista].best_tstar_ccc;
		my_record[ista].weight = get_weight_from_SNR_CCC( my_record[ista].SNR, ccc_tmp);
		if(my_record[ista].quality == -1)
			continue;
		if(my_record[ista].weight == 0)
			continue;
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
		printf(" if 0 record is needed to restack E.W. then we use everytrace with the same weight to stack for one \n");


		for(ista=0;ista< my_input->sta_num; ista ++)
		{
			if(my_record[ista].quality == -1)
				continue;
			//printf(" %d th record weight is %lf \n", ista, my_record[ista].weight);
			for(i=0;i<npts_phase;i++)
			{
				if(my_record[ista].phase_win[i] != my_record[ista].phase_win[i]) 
					my_record[ista].phase_win[i] = 0;
				ES[i] += my_record[ista].phase_win[i];
			}
			num_records_make_new_ES++;
		}
		printf("\n ---> Restack E.W. Done %d records is included to remake the ES \n",num_records_make_new_ES);


		normalize_array(ES, npts_phase);


		//char output_name[200] = "restack.out";
		//output_array1(  output_name,  ES, npts_phase );
		return 1;
	}

	return 0;
}
