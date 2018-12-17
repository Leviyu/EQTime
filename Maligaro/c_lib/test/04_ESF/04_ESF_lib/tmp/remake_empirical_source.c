#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>


/******************************************************************
 * This is a c script remake the empirical source from station
 * with good stretch_ccc and SNR
 * 
 * and then stretch again to acquire the suitable EMP for each record
 *
 * basically, all the records that goes into the good record is used to stack the new Empirical source
 *
 *
 *
 *	DATE:				Keywords:
 *	Reference:
******************************************************************/

int remake_empirical_source(new_RECORD* my_record,new_INPUT* my_input, double* current_ES)
{
	puts("---> remake_empirical_source Begin ==");
	int ista,i;
	int npts_phase;
	int phase_flag = 0;
	int normalize_array();
	npts_phase = (int) (my_input->phase_len / my_input->delta ); 
	double new_ES[npts_phase];
	for(i = 0; i< npts_phase; i++)
		new_ES[i] = 0;

	for(ista = 0; ista<my_input->sta_num; ista++)
	{
		if(my_record[ista].T[phase_flag].SNR != my_record[ista].T[phase_flag].SNR || my_record[ista].T[phase_flag].amplitude == 0.0 || my_record[ista].T[phase_flag].amplitude != my_record[ista].T[phase_flag].amplitude)
			continue;
		//stack again to get new ES with weight (SNR, ccc)
		double weight;
		weight = get_weight_from_SNR( my_record[ista].T[phase_flag].SNR);
		if( fabs(my_record[ista].T[phase_flag].SNR) > my_input->SNR_CUT && fabs(my_record[ista].T[phase_flag].best_stretch_ccc) > my_input->CCC_CUT )
		{
//printf(" ista %d SNR %lf best_ccc %lf weight %lf \n", ista, my_record[ista].T[phase_flag].SNR, my_record[ista].T[phase_flag].best_stretch_ccc, weight);
			for(i=0;i<npts_phase;i++)
			{
				new_ES[i] += my_record[ista].T[phase_flag].phase_win[i] /my_record[ista].T[phase_flag].amplitude *weight;
				//new_ES[i] += my_record[ista].T[phase_flag].phase_win[i] /my_record[ista].T[phase_flag].amplitude *weight* my_record[ista].T[phase_flag].polar_flag;
				if( new_ES[i] != new_ES[i] )
				{
					puts("ERROR: new_ESi] is nan ");
					new_ES[i] = 0.0000000001;
				}
			}
		}

	}

	//output_array1("horoable.out",new_ES, npts_phase);


	// normalize ES
	normalize_array(new_ES, npts_phase);
	
	for(ista = 0; ista<my_input->sta_num; ista++)
	{
		my_record[ista].T[phase_flag].ES_win = (double*)malloc(sizeof(double)*npts_phase);
		for(i=0;i<npts_phase;i++)
			my_record[ista].T[phase_flag].ES_win[i] = new_ES[i];
	}

	// skip for phase below
	//if(strcmp(my_input->phase, "ScS") ==0 ||  strcmp(my_input->phase, "SS") ==0 || strcmp(my_input->phase, "SSS") ==0 )
		//return 0;




	//redo the stretch and shrink game for new_ES to fit every record and record ccc and stretch ratio
	stretch_ES_and_CCC(my_record, my_input, new_ES); 


	return 0;
}
