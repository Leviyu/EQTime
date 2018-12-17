
#include "ESF.h"
#define DEBUG 0

int initiate_phase(new_RECORD* my_record,double prem, char* sac_file, new_INPUT* my_input)
{
	// read in the parameter in my_input

	strcpy(my_record->PHASE,my_input->PHASE);
	strcpy(my_record->COMP,my_input->COMP);
	my_record->long_beg = my_input->long_beg;
	my_record->long_len = my_input->long_len;
	// change phase beg
	//my_record->phase_beg = my_input->phase_beg;
	my_record->phase_beg = my_input->phase_beg + my_record->dt_picked_shift;;
	my_record->phase_len = my_input->phase_len;
	my_record->noise_beg = my_input->noise_beg;
	my_record->noise_len = my_input->noise_len;
	my_record->npts_phase = (int)(my_input->phase_len/my_input->delta);
	my_record->npts_long = (int)(my_input->long_len/my_input->delta);
	my_record->npts_noise = (int)(my_input->noise_len/my_input->delta);
	// get prem from taup 
	
	

	//my_input->delta = 0.1;
	//printf("  delta is %lf \n", my_input->delta );
	//printf(" long len is %lf delta is %lf \n", my_record->long_len, my_input->delta );
	//printf(" HHH1 \n");
	//printf("SIZE1 is %d \n", (int)(my_record->long_len/my_input->delta)) ;

	//my_record->prem = prem + my_record->dt_picked_shift; 	
	int X=(int)(my_record->long_len/my_input->delta);
	my_record->prem = prem;
	my_record->long_win=(double*)malloc(sizeof(double)*X);
	my_record->long_orig=(double*)malloc(sizeof(double)*X);
	my_record->phase_win=(double*)malloc(sizeof(double)*(int)(my_record->phase_len/my_input->delta));
	my_record->stretched_phase_win=(double*)malloc(sizeof(double)*(int)(my_record->phase_len/my_input->delta));
	my_record->noise_win=(double*)malloc(sizeof(double)*(int)(my_record->noise_len/my_input->delta));
	my_record->stretched_ES_win = (double*)malloc(sizeof(double)*(int)(my_record->phase_len/my_input->delta));
	my_record->stretched_gaussian_win = 
		(double*)malloc(sizeof(double)*(int)(my_record->phase_len/my_input->delta));
	my_record->ES_win = (double*)malloc(sizeof(double)*(int)(my_record->phase_len/my_input->delta));
	my_record->stretched_phase_win_flag = 0;
	my_record->shift_time_recorder = 0;
	my_input->iteration_flag = 0;

	//my_record->traffic_time = (double*)malloc(sizeof(double)*iphase);
	//printf(" read long \n");
	read_long_window(sac_file,my_record,my_input);

	//printf(" read phase \n");
	read_phase_window(my_record, my_input);

	//printf(" read noise \n");
	read_noise_window(my_record, my_input);

	//printf(" get amp \n\n");
	get_phase_amplitude(my_record);


	return 0;
}

