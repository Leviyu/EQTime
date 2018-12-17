#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>

int initiate_phase(new_PHASE* T,double prem, char* sac_file, new_INPUT* my_input, char* station_name)
{
	



	// read in the parameter in my_input
	T->phase_name = (char*)malloc(sizeof(char)*10);
	strcpy(T->phase_name,my_input->phase);

	T->long_beg = my_input->long_beg;
	T->long_len = my_input->long_len;
	T->phase_beg = my_input->phase_beg;
	T->phase_len = my_input->phase_len;
	T->noise_beg = my_input->noise_beg;
	T->noise_len = my_input->noise_len;
	T->npts_phase = (int)(my_input->phase_len/my_input->delta);
	T->npts_long = (int)(my_input->long_len/my_input->delta);
	T->npts_noise = (int)(my_input->noise_len/my_input->delta);
	// get prem from taup 
	T->prem = prem;	
	T->long_win=(double*)malloc(sizeof(double)*(int)(T->long_len/my_input->delta));
	T->phase_win=(double*)malloc(sizeof(double)*(int)(T->phase_len/my_input->delta));
	T->noise_win=(double*)malloc(sizeof(double)*(int)(T->noise_len/my_input->delta));

	// shift ScS phase window according to the (obs - orem ) of S
	if(strcmp(T->phase_name,"ScS") == 0  )
	{
		double ScS_window_shift_time = 0;
		double S_prem = 0;
		char ScS_window_shift_command[400];
		sprintf(ScS_window_shift_command,"grep %s eventinfo.%s.S.T | awk 'NR==1 {print $19,$15}' > s_shift_time.ESF", 
				station_name, my_input->EQ);
		FILE* pipe;
		pipe = popen(ScS_window_shift_command,"r");
		pclose(pipe);
		FILE* in;
		double prem_tmp;
		in=fopen("s_shift_time.ESF","r");
		fscanf(in,"%lf %lf",&ScS_window_shift_time, &S_prem);
		fclose(in);

//printf("sta is %s scs shift itme is %lf S_prem is %lf \n", station_name, ScS_window_shift_time, S_prem);

		if( fabs(ScS_window_shift_time) > 15)
			ScS_window_shift_time = 0;
		//printf("ScS window shift time is %lf S_prem is %lf \n", ScS_window_shift_time, S_prem);

		T->phase_beg = T->phase_beg + ScS_window_shift_time;

		// shift noise window by (PREM_S - PREM_ScS) in order to get the same SNR as S
		double dt_PREM_ScS_S = prem - S_prem;
		T->noise_beg = T->noise_beg - dt_PREM_ScS_S;
	}
	
	
	int polar_flag = T->polar_flag;
//printf("sta is %s polar flag is %d \n", station_name, T->polar_flag);

//puts("read in long window data	interp to my_input->delta ");
	T->long_win=read_long_window(sac_file,T->long_beg,T->long_len,prem, my_input->velocity_or_displacement, my_input, polar_flag);
	
//puts(" read in phase window data");
	read_phase_window(T->long_win,T->long_beg,T->long_len, T->phase_beg,T->phase_len,T->phase_win, my_input);

//puts(" read in noise window data");
	read_phase_window(T->long_win,T->long_beg,T->long_len, T->noise_beg,T->noise_len,T->noise_win, my_input);

//puts(" store amplitude of phase window");
	get_phase_amplitude(T);

	// store SNR info
	get_SNR(T);

	return 0;
}
