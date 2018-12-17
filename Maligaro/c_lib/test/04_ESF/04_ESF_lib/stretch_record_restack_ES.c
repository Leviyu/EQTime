#include<stdlib.h>
#include<hongyulib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<ESF.h>
/******************************************************************
 * stretch each record using stretch_facotr 
 * and then restack for new ES
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

int stretch_record_restack_ES(new_RECORD* my_record, new_INPUT* my_input, double* ES)
{
	fprintf(my_input->out_logfile, "---> stretch_record_restack_ES \n");
	int stretch_ES_function(double* current_ES, int npts_phase, double coeff,double*  tmp_ES);
	int npts_phase = (int)( my_input->phase_len / my_input->delta  );
	double ES_tmp[npts_phase];
	int ista,i;
	int num_records_make_new_ES=0;
	double new_ES[npts_phase];
	double ccc_use = 1;

	double stretch_factor_record;
	char stretch_record_out_name[1000];
	double x_phase[npts_phase];

	// initiate ES
	for(i=0;i<npts_phase;i++)
		ES[i] = 0;
	


	for(ista=0;ista< my_input->sta_num; ista ++)
	{
		//my_record[ista].weight = get_weight_from_SNR_CCC( my_record[ista].SNR, my_record[ista].ccc);
		my_record[ista].weight = get_weight_from_SNR_CCC( my_record[ista].SNR, ccc_use);
		if(my_record[ista].quality == -1)
			continue;
		//printf(" %d th record weight is %lf \n", ista, my_record[ista].weight);
		stretch_factor_record = 1/my_record[ista].best_stretch_coefficient;
		//stretch phase win using stretch factor
//printf("stretch factor is %lf weight is %lf  \n", stretch_factor_record, my_record[ista].weight );
		stretch_ES_function(my_record[ista].phase_win, npts_phase, stretch_factor_record, my_record[ista].stretched_phase_win);


		// output the stretched record
		for(i=0;i<npts_phase;i++)
			x_phase[i] = my_record[ista].phase_beg + i*my_input->delta;
		sprintf(stretch_record_out_name,"%s.%s.%s.%s.phase_stretch", my_record[ista].EQ,my_record[ista].name,my_record[ista].PHASE,my_record[ista].COMP);
		output_array2(stretch_record_out_name,x_phase,my_record[ista].stretched_phase_win, npts_phase, 0);

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
	fprintf(my_input->out_logfile, "---> Restack E.W. Done %d records is included to remake the ES \n",num_records_make_new_ES);



	output_array1("stretch_record_stacked_ES.tmp",ES, npts_phase);

	// we shrink the E.W. by 0.5 to make sure that we can match the skinny record
	//
	// int stretch_ES_function(double* current_ES, int npts_phase, double coeff,double*  tmp_ES)
	//double stretch_factor = 1.0;
	//stretch_ES_function(ES_tmp, npts_phase, stretch_factor,ES);


	return 0;
}
/*
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
		//char output_name[200] = "restack.out";
		//output_array1(  output_name,  ES, npts_phase );
		return 1;
	}
*/
