
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>

int get_first_ES(new_RECORD* my_record, new_INPUT* my_input, double* ES1, int phase_flag,double phase_len)
{
puts("---> get first ES");
	int normalize_array(double* array, int len);
	int output_array1();
	double amplitude();
	int npts_phase = (int)(phase_len / my_input->delta);
	int i,j;
//printf(" ntps phase is %d \n", npts_phase);
//output_array1("check.out",my_record[i].T[phase_flag].phase_win, 800);

	for(i=0;i<npts_phase;i++)
		ES1[i] = 0;


	for(i=0;i<my_input->sta_num;i++)
	{
		// skip those with nan SNR
		if( my_record[i].T[phase_flag].SNR != my_record[i].T[phase_flag].SNR ||my_record[i].T[phase_flag].amplitude == 0 || my_record[i].T[phase_flag].SNR < 1.5 )
			continue;
	
		//define a weight to stack the first ES
		double weight = 1;
		if( my_record[i].T[phase_flag].SNR > 3)
			weight = 1;
		else if (my_record[i].T[phase_flag].SNR > 2)
			weight = 0.7;
		else if (my_record[i].T[phase_flag].SNR > 1.5)
			weight = 0.5;

		// stack the rest to ES1
		for(j=0;j<npts_phase;j++ )
		{
			if( my_record[i].T[phase_flag].phase_win[j] != my_record[i].T[phase_flag].phase_win[j] )
				printf("ERROR problem with record %d j is %d\n", i , j);
			ES1[j] += my_record[i].T[phase_flag].phase_win[j] / my_record[i].T[phase_flag].amplitude*weight;
			if(ES1[j] != ES1[j])
				ES1[j] = 0;
		}
	}
	
	


	// normalize ES1
	normalize_array(ES1,npts_phase);
	

	// output first ES1
	char FIRST_ES[200] = "First_ES.out";
	output_array1(FIRST_ES,ES1, npts_phase);

	return 0;
}

