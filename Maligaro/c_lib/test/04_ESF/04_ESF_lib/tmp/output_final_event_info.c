#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>
int output_final_event_info(new_RECORD* my_record, new_INPUT* my_input)
{
	int output_final_event_info_for_record(new_RECORD* my_record, new_PHASE* my_phase, FILE* out, new_INPUT* my_input);
	int output_final_pick_onset_info_for_record(new_RECORD* my_record, new_PHASE* my_phase, FILE* out);
	int ista;	
	int phase_flag;
	char phase[20];
	strcpy(phase,my_input->phase);
	phase_flag = 0;

	FILE* out;
	char out_name[200];
	sprintf(out_name,"eventinfo.%s.%s.%s",my_input->EQ, phase, my_input->comp);
	out=fopen(out_name,"w");
	

	for(ista = 0; ista< my_input->sta_num;ista++)
	{
		// output record info for each record
		output_final_event_info_for_record(&my_record[ista], &my_record[ista].T[phase_flag],out, my_input);
	}
	fclose(out);



	// output pick ONSET info
	FILE* pick_out;
	char pick_name[200];
	sprintf(pick_name,"eventpick.%s.%s.%s",my_input->EQ, phase, my_input->comp);
	pick_out=fopen(pick_name,"w");

	for(ista = 0; ista< my_input->sta_num;ista++)
	{
		// output record info for each record
		output_final_pick_onset_info_for_record(&my_record[ista], &my_record[ista].T[phase_flag],pick_out);
	}
	fclose(pick_out);

	return 0;
}

int output_final_pick_onset_info_for_record(new_RECORD* my_record, new_PHASE* my_phase, FILE* out)
{
	// output format
	// 1		2					3					4	
	// sta		time_phase_peak		npts_phase_peak		stretch_factor
	
	fprintf(out,"%7s %10.5lf %10d %5.2lf\n",my_record->name, my_phase->time_phase_peak, my_phase->npts_phase_peak, my_phase->best_stretch_coefficient);

	return 0;
}

int output_final_event_info_for_record(new_RECORD* my_record, new_PHASE* my_phase, FILE* out, new_INPUT* my_input)
{

// output format
// 1 	2	3	4	5	6		7		8		9		10		11	12		13			14			   15   	16  			17  18 	 	19			   20 		21
// sta NET dist AZ BAZ STA_lat STA_lon  EQ_lat EQ_lon EQ_dep EQ_mag EQ_name polar_flag quality_flag   prem  phase_amplitude    ccc  SNR  dt_obs_PREM     phase_name best_stretch_ccc
// 22
// best_stretch_coefficient


	if(my_phase->SNR == my_phase->SNR && fabs(my_phase->SNR) > my_input->SNR_CUT && fabs(my_phase->best_stretch_ccc) > my_input->CCC_CUT )
		my_phase->quality = 1;
	//printf("STA %s SNR %lf CCC %lf SNR_CUT %lf CCC_Cut %lf quality %d \n", my_record->name, my_phase->SNR, my_phase->best_stretch_ccc, my_input->SNR_CUT, my_input->CCC_CUT, my_phase->quality);

	char sta[20];
	strcpy(sta,my_record->name);
	char NET[20];
	strcpy(NET,my_record->NET);
	double 	DIST = my_record->DIST;
	double AZ = my_record->AZ;
	double BAZ = my_record->BAZ;
	double STA_lat = my_record->sta_lat;
	double STA_lon = my_record->sta_lon;
	double EQ_lat = my_record->eq_lat;
	double EQ_lon = my_record->eq_lon;
	double EQ_dep = my_record->eq_dep;
	double EQ_mag = my_record->eq_mag;
	char EQ_name[20];
	strcpy(EQ_name,my_record->eq_name);

	char phase_name[20];
	strcpy(phase_name,my_phase->phase_name);
	int polar_flag = my_phase->polar_flag;
	int quality_flag = my_phase->quality;
	double prem = my_phase->prem;
	double amp = my_phase->amplitude;
	double ccc = my_phase->ccc;
	double SNR = my_phase->SNR;
	double misfit = my_phase->misfit;



	// for Lp Sdiff SS SSS data, we add delay time to the dt_residual
	//double dt_obs_prem = my_phase->dt_obs_prem - my_input->bp_delay_time;
	double dt_obs_prem = my_phase->dt_obs_prem;

	double best_ccc = my_phase->best_stretch_ccc;
	double best_coefficient = my_phase->best_stretch_coefficient;

	fprintf(out,"%6s %6s %8.3lf %8.3lf %8.3lf %8.3lf %8.3lf %8.3lf %8.3lf %8.3lf %8.3lf %13s %2d %2d %8.3lf %10.9lf %5.2lf %5.2lf %6.3lf %6s %5.2lf %5.2lf %5.2lf \n",
			sta, NET, DIST, AZ, BAZ, STA_lat, STA_lon, EQ_lat, EQ_lon, EQ_dep, EQ_mag, EQ_name, polar_flag, quality_flag, prem, amp, ccc ,SNR, dt_obs_prem, phase_name,
			best_ccc, best_coefficient, misfit);


	return 0;
}
