#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>
#include<hongyulib.h>

int empirical_source_function(new_RECORD* my_record, new_INPUT* my_input)
{
fprintf(my_input->out_logfile,"--->  empirical_source_function Begin \n");
puts("         ---> emp begin");
//printf("len delta %lf %lf \n", my_input->phase_len, my_input->delta );
	//define a 2D  ES array 
	double ccc,shift_time;
	int npts_phase, i,j,ista, npts_shift;
	npts_phase = (int) (my_input->phase_len / my_input->delta );
	double old_ES[npts_phase];
	double new_ES[npts_phase];
	double current_ES[npts_phase];
	double first_EW[npts_phase];
	int restack_flag;


puts("         ---> get first E.W.");
	// store all the E.W. produced while stacking
	double** ES;
	ES = (double**)malloc(sizeof(double*)*30);
	for(i=0;i<30;i++)
		ES[i] = (double*)malloc(sizeof(double)*npts_phase);
	
	fprintf(my_input->out_logfile, "============================================\n");
	fprintf(my_input->out_logfile, "==============Get E.W. for current phase==============\n");
	fprintf(my_input->out_logfile, "============================================\n");
	get_first_EW_for_phase(my_record,my_input,ES[1]);
	get_first_EW_for_phase(my_record,my_input,first_EW);


	fprintf(my_input->out_logfile, "============================================\n");
	fprintf(my_input->out_logfile, "==============First CCC loop Begin ==============\n");
	fprintf(my_input->out_logfile, "============================================\n");
puts("         ---> starting ccc looping");
	// ===========================================================
	//	2. start loop ccc to get a stable E.W.
	// ===========================================================
	int loop_num;
	int loop_num_max = 5;
	for(loop_num = 1; loop_num <= loop_num_max ; loop_num++)
	{
		fprintf(my_input->out_logfile,"---> Working on ccc loop %d \n\n",loop_num);
		// get current E.W. and store into current_ES and normalize it
		for(i=0;i<npts_phase;i++)
			current_ES[i]=ES[loop_num][i];
		normalize_array(current_ES, npts_phase);

		//int empirical_source_for_each_record(new_RECORD* my_record, new_INPUT* my_input, double* current_ES);
		empirical_source_for_each_record(my_record,my_input,current_ES, loop_num);

		
		get_ONSET_ENDSET_for_each_record_origional_phase(my_record,my_input);
		//stack again to get new ES with weight (SNR, ccc)
		restack_flag = restack_ES_for_phase(my_record,my_input,ES[loop_num +1]);

		// normalize ES
		normalize_array(ES[loop_num + 1], npts_phase);

		// for S in case 0 record is included to make E.W. we use sum all as its E.W.
		for(i=0;i<npts_phase;i++)
			current_ES[i]=ES[loop_num+1][i];

		if(restack_flag == 1)
		{
			break;
			//return 1;
		}

		// if ccc new_ES & old_ES > 0.99 exit
		for(i = 0; i< npts_phase; i++)
		{
			old_ES[i] = ES[loop_num][i];
			new_ES[i] = ES[loop_num+1][i];
		}
		CCC(old_ES,npts_phase,new_ES, npts_phase, &npts_shift, &ccc, 0);
		fprintf(my_input->out_logfile," old and new ES ccc is %lf\n",ccc);

		// output first iteration EW and STD
		if(loop_num ==1 )
		{
			output_STD_of_first_iteration_ES(my_record,my_input, new_ES);
			output_current_ES_for_phase_first_iteration(my_input, new_ES);
		}

		// for Phase ScS,since using S ES for ccc therefore we dont stack again 
		if(strcmp(my_input->PHASE,"S")!=0 && strcmp(my_input->PHASE,"P")!=0)
		{
			for(i=0;i<npts_phase;i++)
				current_ES[i]=new_ES[i];
			break;
		}

		if(fabs(ccc) > 0.95 )
			break;
	}
	


	//output long and phase whindow
puts("         ---> output_long_phase_win and first ES & STD");
	output_long_phase_window(my_record, my_input);
	output_STD_of_first_ES(my_record,my_input, new_ES);
	output_current_ES_for_phase(my_input, new_ES);

	
puts("         ---> stretching ");
	// stretch ES to find the best waveform to match each record
	strcpy(my_input->stretch_flag,"stretch");
	// first stretching here ***
	stretch_ES_and_CCC(my_record, my_input, old_ES);

puts("         ---> restack ");
	//update ES by stretching records (using stretch factor) and stack again
	double stretch_record_stack[npts_phase];
	stretch_record_restack_ES(my_record, my_input, stretch_record_stack); 
	normalize_array(stretch_record_stack, npts_phase);
	output_STD_of_second_ES(my_record,my_input, stretch_record_stack);
	output_current_ES_for_phase_second(my_input, stretch_record_stack);


	 //define E.W. to use here
	double EW_new[npts_phase];
	int count;
	if(strcmp(my_input->PHASE,"S") == 0 || strcmp(my_input->PHASE,"P") == 0 )
	{
		for(count = 0; count < npts_phase; count++)
			EW_new[count] = stretch_record_stack[count];
	}
	else 
	{
		for(count = 0; count < npts_phase; count++)
			EW_new[count] = first_EW[count];
	}


puts("         ---> t* ");
	// stretch updated E.W. with tstar and best fit each record 
	strcpy(my_input->stretch_flag,"tstar");
	// second  stretching here ***
	stretch_ES_and_CCC_tstar(my_record, my_input, EW_new);

	//find ONSET (arrival time)	 and ENDSET time
puts("         ---> get_ONSET_ENDSET_for_each_record_stretched ");
	get_ONSET_ENDSET_for_each_record_stretched(my_record,my_input);

	//define if record is good
puts("         ---> define goodness ");
	define_goodness_of_record(my_record, my_input);

	//restack_ES_for_phase_after_tstar(my_record, my_input, EW_new);
puts("         ---> output ES STD 3rd ");
	output_STD_of_third_ES(my_record,my_input, EW_new);
	output_current_ES_for_phase_third(my_input, EW_new);
	//output_STD_of_third_ES(my_record,my_input, stretch_record_stack);
	//output_current_ES_for_phase_third(my_input, stretch_record_stack);


	// use gaussian to best fit each t* record and define arrival time
	// third stretching here ***
puts("         ---> define stretch ONSET with gaussian ");
	define_stretch_EW_ONSET( my_record, my_input);




	// output ES for each record 
puts("         ---> output_ES_for_each_record ");
	output_ES_for_each_record(my_record, my_input);



fprintf(my_input->out_logfile,"--->  empirical_source_function End \n");
puts("         ---> emp end");

	return 0;
}

			// ===========================================================
			//	for SS, we use max amplitude within phase_win to define polarity
			//	normalize if flipped the record
			// ===========================================================
			//if(loop_num == 1 && my_input->POLAR_SOLUTION == 2 && strcmp(my_input->PHASE,"SS") ==0 )
			//{
				//double AMP_posi = 0;
				//double AMP_nega = 0;
				//int max_amp_loc;
				//int idd;
				//// find negative amp and normalize with it
				//amplitudeloc(my_record[ista].phase_win, (int)(my_record[ista].phase_len/my_input->delta),&max_amp_loc, &AMP_nega, -1);
				//amplitudeloc(my_record[ista].phase_win, (int)(my_record[ista].phase_len/my_input->delta),&max_amp_loc, &AMP_posi, 1);
				//if(fabs(AMP_posi) < fabs(AMP_nega) )
				//{
					//my_record[ista].polar_flag *= -1;
					//// flip the record
					//if(AMP_posi == 0) AMP_posi = 1;
					//if(AMP_nega == 0) AMP_nega = 1;
					//for(idd = 0; idd< (int)(my_record[ista].phase_len/my_input->delta); idd++)
						//my_record[ista].phase_win[idd] *= 1/AMP_nega;
					//for(idd = 0; idd< (int)(my_record[ista].long_len/my_input->delta); idd++)
						//my_record[ista].long_win[idd] *= 1/AMP_nega;
					//for(idd = 0; idd< (int)(my_record[ista].noise_len/my_input->delta); idd++)
						//my_record[ista].noise_win[idd] *= 1/AMP_nega;
				//}
				//else 
				//{
					//my_record[ista].polar_flag *= 1;
			//
				//}
			//}
