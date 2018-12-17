#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>
#define DEBUG 0

int initiate_phase(new_RECORD* my_record,double prem, char* sac_file, new_INPUT* my_input)
{
	// read in the parameter in my_input

	strcpy(my_record->PHASE,my_input->PHASE);
	strcpy(my_record->COMP,my_input->COMP);
	my_record->long_beg = my_input->long_beg;
	my_record->long_len = my_input->long_len;
	my_record->phase_beg = my_input->phase_beg;
	my_record->phase_len = my_input->phase_len;
	my_record->noise_beg = my_input->noise_beg;
	my_record->noise_len = my_input->noise_len;
	my_record->npts_phase = (int)(my_input->phase_len/my_input->delta);
	my_record->npts_long = (int)(my_input->long_len/my_input->delta);
	my_record->npts_noise = (int)(my_input->noise_len/my_input->delta);
	// get prem from taup 
	my_record->prem = prem;	
	my_record->long_win=(double*)malloc(sizeof(double)*(int)(my_record->long_len/my_input->delta));
	my_record->phase_win=(double*)malloc(sizeof(double)*(int)(my_record->phase_len/my_input->delta));
	my_record->stretched_phase_win=(double*)malloc(sizeof(double)*(int)(my_record->phase_len/my_input->delta));
	my_record->noise_win=(double*)malloc(sizeof(double)*(int)(my_record->noise_len/my_input->delta));
	my_record->stretched_ES_win = (double*)malloc(sizeof(double)*(int)(my_record->phase_len/my_input->delta));
	my_record->stretched_gaussian_win = (double*)malloc(sizeof(double)*(int)(my_record->phase_len/my_input->delta));
	my_record->ES_win = (double*)malloc(sizeof(double)*(int)(my_record->phase_len/my_input->delta));


	read_long_window(sac_file,my_record,my_input);

	read_phase_window(my_record, my_input);

	read_noise_window(my_record, my_input);

	get_phase_amplitude(my_record);




	return 0;
}


	//output for checking
	//if(DEBUG == 1)
	//{
		//char out_file[200];
		//sprintf(out_file,"long.%s.%s.%s",my_record->name,my_record->PHASE,my_record->COMP);
		//output_array1(out_file,my_record->long_win,my_record->npts_long);
		//sprintf(out_file,"phase.%s.%s.%s",my_record->name,my_record->PHASE,my_record->COMP);
		//output_array1(out_file,my_record->phase_win,my_record->npts_phase);
		//sprintf(out_file,"noise.%s.%s.%s",my_record->name,my_record->PHASE,my_record->COMP);
		//output_array1(out_file,my_record->noise_win,my_record->npts_noise);
//
	//}



	// shift ScS phase window according to the time anomaly of S
	//if(strcmp(my_record->PHASE,"ScS") == 0  )
	//{
		//double ScS_window_shift_time = 0;
		//double S_prem = 0;
		//char ScS_window_shift_command[400];
		//sprintf(ScS_window_shift_command,"grep %s eventinfo.%s.S.T | awk 'NR==1 {print $19}' > s_shift_time.ESF", 
				//my_record->name, my_input->EQ);
		////printf("eventinfo is %s \n", ScS_window_shift_command);
		//FILE* pipe;
		//pipe = popen(ScS_window_shift_command,"r");
		//pclose(pipe);
		//FILE* in;
		//double prem_tmp;
		//in=fopen("s_shift_time.ESF","r");
		//fscanf(in,"%lf",&ScS_window_shift_time );
		//fclose(in);
//
////printf("sta is %s scs shift itme is %lf S_prem is %lf \n", station_name, ScS_window_shift_time, S_prem);
		//if( fabs(ScS_window_shift_time) > 30)
			//ScS_window_shift_time = 0;
		////printf("sta is %s ScS window shift time is %lf \n", my_record->name,ScS_window_shift_time);
//
		//my_record->phase_beg = my_record->phase_beg + ScS_window_shift_time;
//
		//// shift noise window by (PREM_S - PREM_ScS) in order to get the same SNR as S
		////double dt_PREM_ScS_S = prem - S_prem;
		//T->noise_beg = T->noise_beg - dt_PREM_ScS_S;
	//}
