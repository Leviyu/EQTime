#include "hongyulib.h"
#include "ESF.h"


/******************************************************************
// This function does the cross-correlation between E.W, with each record and update the 
// phase window for each phase
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

	// construct main lobe for ES
	construct_array_with_main_lobe(current_ES, &npts_phase, tmp_new_ES);

	// loop ccc ES with each record
	for(ista=0;ista<my_input->sta_num;ista++)
	{
		if( my_record[ista].beyong_window_flag == -1)
			continue;

		if(my_record[ista].polar_flag == 0)
			ccc_flag = 1;
		else 
			ccc_flag = 1;

			// ===========================================================
			//	for low polarity prediction record, we use long phase window to ccc
			//	and using ccc sign (+ or -) to define polarity
			//	normalize if flipped
			// ===========================================================
			if( loop_num == 1 && my_input->POLAR_SOLUTION == 2 && my_record[ista].polar_flag == 0 )
			{
				my_record[ista].polar_flag = 1;
				if(strstr(my_record[ista].name, "PPP") != NULL)
				{
					my_record[ista].polar_flag = -1;
					// find negative amp and normalize with it
					amplitudeloc(my_record[ista].phase_win, (int)(my_record[ista].phase_len/my_input->delta),&max_amp_loc, &AMP_phase, -1);
					if(AMP_phase == 0)
						AMP_phase = -1;
					// normalize 
					for(idd = 0; idd< (int)(my_record[ista].phase_len/my_input->delta); idd++)
						my_record[ista].phase_win[idd] *= 1/AMP_phase ;
					for(idd = 0; idd< (int)(my_record[ista].long_len/my_input->delta); idd++)
					{
						my_record[ista].long_win[idd] *= 1/AMP_phase;
						my_record[ista].long_orig[idd] *= 1/AMP_phase;
					}
					for(idd = 0; idd< (int)(my_record[ista].noise_len/my_input->delta); idd++)
						my_record[ista].noise_win[idd] *= 1/AMP_phase;
				}
			}

		// ===========================================================
		//	cut both record and E.W. to small window around the peak to ccc
		// convert the ES and phase_win to 0.1 ~ 1 part and normalized to 0 ~ 1 
		//int construct_array_with_main_lobe(double* array_in, int* npts_in, double* array_out)
		// ===========================================================
		construct_array_with_main_lobe(my_record[ista].phase_win, &npts_phase, tmp_new_PHASE);
		if( my_input->Reprocessing_Flag == 1)
			CCC2(tmp_new_ES,npts_phase,tmp_new_PHASE, npts_phase, &npts_shift, &ccc, ccc_flag, my_input->npts_Reprocessing_shift);
		else
			CCC(tmp_new_ES,npts_phase,tmp_new_PHASE, npts_phase, &npts_shift, &ccc, ccc_flag);


		// ===========================================================
		// use shift from ESF to shift record and update phase win
		// if shift too big, we shift it back
		// ===========================================================
		shift_time = npts_shift*my_input->delta;

		my_record[ista].phase_beg -= shift_time;

		// ===========================================================
		//	update phase window
		// ===========================================================
		read_phase_window(&my_record[ista],my_input);

		fprintf(my_input->out_shift,"loop_num %d sta %s shift %lf ccc %lf \n",
				loop_num, my_record[ista].name,
				shift_time,ccc);

		my_record[ista].ccc = ccc;

	}

	return 0;
}
