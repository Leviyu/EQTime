#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>

int empirical_source_function(new_RECORD* my_record, new_INPUT* my_input)
{
puts("--->  empirical_source_function Begin ");
	double get_weight_from_SNR_CCC();
	int normalize_array(double* array, int len);

	//define a 2D  ES array 
	int npts_phase, i,j,ista, npts_shift, phase_flag;
	npts_phase = (int) (my_input->phase_len / my_input->delta );
	double** ES;
	double* ES1;
	double ccc,shift_time;
	double old_ES[npts_phase];
	double new_ES[npts_phase];
	phase_flag = 0;

	// Initiate 2D ES array
	double current_ES[npts_phase];
	ES = (double**)malloc(sizeof(double*)*30);
	for(i=0;i<30;i++)
		ES[i] = (double*)malloc(sizeof(double)*npts_phase);
	ES1 = (double*)malloc(sizeof(double)*npts_phase);

//printf("npts flag is %d npts phase is %d phase len is %lf \n", phase_flag , npts_phase, my_record[2].T[phase_flag].phase_len);

		// if Phase is ScS then Read in S ES as its ES
		if(strcmp(my_input->phase,"ScS")==0 || strcmp(my_input->phase, "SS")==0 || strcmp(my_input->phase, "SSS")==0 || strcmp(my_input->phase, "ScSScS")==0)
		{
			read_in_modified_S_ES(my_input,ES[1]);
		}
		else 
		{
			// 1. get first ES
			get_first_ES(my_record,my_input, ES1, phase_flag, my_input->phase_len);

			for(i=0;i<npts_phase;i++)
				ES[1][i]= ES1[i];
		}

	// ============================== 2 .start loop ccc to a stable ES================================
	//
	int loop_num;
	for(loop_num = 1; loop_num <= 5 ; loop_num++)
	{
		//get_current_ES
		for(i=0;i<npts_phase;i++)
			current_ES[i]=ES[loop_num][i];

		//ccc ES with each record
		for(ista=0;ista<my_input->sta_num;ista++)
		{
			// skip those with nan SNR
			if( my_record[ista].T[phase_flag].SNR != my_record[ista].T[phase_flag].SNR  )
				continue;

			int ccc_flag;
			if(my_record[ista].T[phase_flag].polar_flag == 0)
				ccc_flag = 0;
			else 
				ccc_flag = 1;
//printf("--------------> ccc for %d th sta %s \n", loop_num, my_record[ista].name);
			CCC(current_ES,npts_phase,my_record[ista].T[phase_flag].phase_win, npts_phase, &npts_shift, &ccc, ccc_flag);

			// use shift from ESF to shift record and update phase win
			shift_time = npts_shift*my_input->delta;
			if( fabs(shift_time) > 30.0)
				shift_time = 0;
			my_record[ista].T[phase_flag].phase_beg -= shift_time;

			if(my_record[ista].T[phase_flag].phase_beg < my_record[ista].T[phase_flag].long_beg) 
				my_record[ista].T[phase_flag].phase_beg += shift_time;

				// update phase win
			read_phase_window(my_record[ista].T[phase_flag].long_win,my_record[ista].T[phase_flag].long_beg, my_record[ista].T[phase_flag].long_len,
					my_record[ista].T[phase_flag].phase_beg, my_record[ista].T[phase_flag].phase_len,my_record[ista].T[phase_flag].phase_win, my_input);

				// update ccc store es
			if(my_record[ista].T[phase_flag].polar_flag == 1)
				my_record[ista].T[phase_flag].ccc = ccc;
			else if(my_record[ista].T[phase_flag].polar_flag == -1)
				my_record[ista].T[phase_flag].ccc = -1*ccc;
			else if(my_record[ista].T[phase_flag].polar_flag == 0)
				my_record[ista].T[phase_flag].ccc = ccc;

			my_record[ista].T[phase_flag].ES_win = (double*)malloc(sizeof(double)*npts_phase);
			for(i=0;i<npts_phase;i++)
				my_record[ista].T[phase_flag].ES_win[i] = current_ES[i];



			// decide which distance record to stack the new ES
			double dist_min=0;
			double dist_max=180;;
			if(strcmp(my_record[ista].T[phase_flag].phase_name, "S") == 0)
			{
				dist_min = 30;
				dist_max = 100;
			}
			else if (strcmp(my_record[ista].T[phase_flag].phase_name, "ScS") == 0)
			{
				dist_min = 50;
				dist_max = 78;
			}
			else if (strcmp(my_record[ista].T[phase_flag].phase_name, "Sdiff") == 0)
			{
				dist_min = 104;
				dist_max = 150;
			}

			//stack again to get new ES with weight (SNR, ccc)
			double weight;
			weight = get_weight_from_SNR_CCC( my_record[ista].T[phase_flag].SNR, my_record[ista].T[phase_flag].ccc);
			if(my_record[ista].T[phase_flag].SNR > my_input->SNR_CUT &&
					fabs( my_record[ista].T[phase_flag].ccc) > 0.4  &&
					my_record[ista].T[phase_flag].amplitude != 0.0 &&
					my_record[ista].DIST <dist_max &&
					my_record[ista].DIST > dist_min )
			{
//printf("ista is %d name is %s \n", ista, my_record[ista].name);
				for(i=0;i<npts_phase;i++)
				{
					ES[loop_num +1 ][i] += my_record[ista].T[phase_flag].phase_win[i] /my_record[ista].T[phase_flag].amplitude *weight;
					//ES[loop_num +1 ][i] += my_record[ista].T[phase_flag].phase_win[i] /my_record[ista].T[phase_flag].amplitude *weight* my_record[ista].T[phase_flag].polar_flag;
					if( ES[loop_num +1 ][i] != ES[loop_num +1 ][i] )
					{
						puts("ERROR: ES[loop_num][i] is nan ");
						getchar();
						ES[loop_num +1 ][i] = 0.0000000001;
					}
				}
			}

		}

		// normalize ES
		normalize_array(ES[loop_num + 1], npts_phase);

		// if ccc new_ES & old_ES > 0.99 exit
		for(i = 0; i< npts_phase; i++)
		{
			old_ES[i] = ES[loop_num][i];
			new_ES[i] = ES[loop_num+1][i];
		}
		CCC(old_ES,npts_phase,new_ES, npts_phase, &npts_shift, &ccc, 0);
		printf("old and new ES ccc is %lf\n",ccc);

		// for Phase ScS,since using S ES for ccc therefore we dont stack again 
		if(strcmp(my_input->phase,"ScS")==0 || strcmp(my_input->phase,"SS")==0 || strcmp(my_input->phase,"SSS")==0 ||
				strcmp(my_input->phase,"ScSScS")==0)
			break;

		if(fabs(ccc) > 0.95 )
			break;
	}

	//puts("------> loop done!");
	
	// output current ES for current phase
	output_current_ES_for_phase(my_input, new_ES);
	

	// stretch ES to find the best waveform to match each record
	stretch_ES_and_CCC(my_record, my_input, current_ES);

	// remake Empirical Source
	if(strcmp(my_input->phase,"ScS")==0  || strcmp(my_input->phase,"SS")==0 || strcmp(my_input->phase,"SSS")==0 ||strcmp(my_input->phase,"Sdiff")==0  ||strcmp(my_input->phase,"ScSScS")==0)
		printf("We Do not remake Empirical Wavelet for %s cause we use tranformed and stretched  S as Empirical Wavelet", my_input->phase);	
	else
		remake_empirical_source(my_record,my_input, current_ES);

	// output ES for each record 
	output_ES_for_each_record(my_record, my_input, phase_flag, my_input->phase_len);

	//find ONSET (arrival time)	 and ENDSET time
	get_ONSET_ENDSET_for_each_record(my_record, my_input, phase_flag , my_input->phase_len);

	// calculate the misfit part
	get_misfit_for_each_record(my_record, my_input);

	
	// need quality flag  in output_ES_for_each record
	output_STD_of_ES(my_record, my_input, new_ES);
	return 0;
}
