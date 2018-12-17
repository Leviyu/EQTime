#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<hongyulib.h>
#include<sacio.h>
#include<ESF.h>


/******************************************************************
// This function does the cross-correlation between E.W, with each record and update the 
// phase window for each phase
 *
 *	Input:
 *	my_record
 *	my_input
 *	current_ES
 *
 *
 *	Output:
 *
 *
 *	DATE:				Keywords:
 *	Reference:
******************************************************************/

int empirical_source_for_each_record(new_RECORD* my_record, new_INPUT* my_input, double* current_ES, int loop_num)
{
	int ista;
	int npts_phase;
	int ccc_flag; 
	double ccc_tmp;
	int npts_shift;
	npts_phase = (int) (my_input->phase_len / my_input->delta );
	int idd;
	double AMP_phase=0;
	int max_amp_loc;
	double tmp_new_ES[npts_phase];
	double tmp_new_PHASE[npts_phase];
	double ccc;
	int i;
	double shift_time;


	// loop ccc ES with each record
	for(ista=0;ista<my_input->sta_num;ista++)
	{
		// skip those with nan SNR
		if( my_record[ista].quality == -1)
			continue;
		if(my_record[ista].polar_flag == 0)
			ccc_flag = 0;
		else 
			ccc_flag = 1;

			// ===========================================================
			//	for low polarity prediction record, we use long phase window to ccc
			//	and using ccc sign (+ or -) to define polarity
			//	normalize if flipped
			// ===========================================================
			if( loop_num == 1 && my_input->POLAR_SOLUTION == 2 && my_record[ista].polar_flag == 0 )
			{
				CCC(current_ES,npts_phase,my_record[ista].phase_win, npts_phase, &npts_shift, &ccc_tmp, ccc_flag);
				if(ccc_tmp > 0)
					my_record[ista].polar_flag = 1;
				else 
				{
					my_record[ista].polar_flag = -1;
					// find negative amp and normalize with it
					amplitudeloc(my_record[ista].phase_win, (int)(my_record[ista].phase_len/my_input->delta),&max_amp_loc, &AMP_phase, -1);
					if(AMP_phase == 0)
						AMP_phase =1;
					// normalize 
					for(idd = 0; idd< (int)(my_record[ista].phase_len/my_input->delta); idd++)
						my_record[ista].phase_win[idd] *= 1/AMP_phase ;
					for(idd = 0; idd< (int)(my_record[ista].long_len/my_input->delta); idd++)
						my_record[ista].long_win[idd] *= 1/AMP_phase;
					for(idd = 0; idd< (int)(my_record[ista].noise_len/my_input->delta); idd++)
						my_record[ista].noise_win[idd] *= 1/AMP_phase;
				}
			}


		// ===========================================================
		//	cut both record and E.W. to small window around the peak to ccc
		// convert the ES and phase_win to 0.1 ~ 1 part and normalized to 0 ~ 1 
		//int construct_array_with_main_lobe(double* array_in, int* npts_in, double* array_out)
		// ===========================================================
		construct_array_with_main_lobe(current_ES, &npts_phase, tmp_new_ES);
		construct_array_with_main_lobe(my_record[ista].phase_win, &npts_phase, tmp_new_PHASE);
		CCC(tmp_new_ES,npts_phase,tmp_new_PHASE, npts_phase, &npts_shift, &ccc, ccc_flag);

//printf("--->sta %s loop %d ccc %lf \n", my_record[ista].name, loop_num, ccc);
		// ===========================================================
		// use shift from ESF to shift record and update phase win
		// if shift too big, we shift it back
		// ===========================================================
		shift_time = npts_shift*my_input->delta;
		if( fabs(shift_time) > 35.0)
		{
			my_record[ista].quality = -1;
			shift_time = 0;
		}
		my_record[ista].phase_beg -= shift_time;

		if(my_record[ista].phase_beg < my_record[ista].long_beg) 
			my_record[ista].phase_beg += shift_time;


		// ===========================================================
		//	update phase window
		// ===========================================================
		read_phase_window(&my_record[ista],my_input);


		my_record[ista].ccc = ccc;

		for(i=0;i<npts_phase;i++)
			my_record[ista].ES_win[i] = current_ES[i];
	}

	return 0;
}
		//// update ccc store es
		//if(my_record[ista].polar_flag == 1)
			//my_record[ista].ccc = ccc;
		//else if(my_record[ista].polar_flag == -1)
			//my_record[ista].ccc = -1*ccc;
		//else if(my_record[ista].polar_flag == 0)
