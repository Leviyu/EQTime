#ifndef EQ_data_H
#define EQ_data_H
#include<stdlib.h>
#include<stdio.h>

struct new_INPUT
{
	char* EQ;
	char* event_file;
	char* polarity_file;
	int sta_num;
	char* phase;
	char* comp;
	char* velocity_or_displacement;
	double long_beg,long_len;
	double phase_beg, phase_len;
	double noise_beg, noise_len;
	double distmin, distmax;


	double S_filter_delay;					// filter delay of 10sec filter for S Empirical Wavelet


	double SNR_CUT;
	double CCC_CUT;
	double bp_delay_time;					//bp filter caused delay time acquired using the delay time between S ES bp and S ES

	double delta;
	int npts_peak_ONSET;					// npts between peak and ONSET of S E.W.

};
typedef struct new_INPUT new_INPUT;


struct new_PHASE
{
	char* phase_name;		// phase name
	double polarity;		// polarity coefficient given by radiation pattern
	int polar_flag;			// watch for the polarity of current phase 
								// if 1 == positive
								// -1 == negative
								// 0 == not set
	int npts_signal_beg;
	int npts_signal_end;	// npts where signal beg and end



	int quality;			// the quality of current phase
							// 1 == good
							// 0 == not set 
							// -1 == bad

	// long window
	double* long_win;		// stored long window 
	int npts_long;
	double long_beg;
	double long_len;

	// phase window
	double* phase_win;
	int npts_phase;
	double phase_beg;			// window begin time for current phase eg. -10s
	double phase_len;

	double* noise_win;			// noise window
	int npts_noise;
	double noise_beg;
	double noise_len;

	double* ES_win;
	double* stretched_ES_win;
	double best_stretch_ccc;
	double best_stretch_coefficient;


	int npts_ES;
	double ES_beg;
	double ES_len;


	double DEPMIN;			// minimum value of phase window
	double DEPMAX;			//maximum value of phase window
	double amplitude;		// amplitude for current phase

	double prem;			// prem arrival time for current phase


	double ccc;				// cross-correlation coefficient between Empirical source and the record
	double SNR;				
	double SNR_new;			// new SNR using misfit window
	double dt_obs_prem;
	double ONSET;
	double ENDSET;
	double misfit;


	int npts_SS_peak_zero;	// the npts difference between SS /ScSScS peak and the zero point in front of peak




	double time_phase_peak;		// phase_peak time relative to phase PREM
	int npts_phase_peak;		// phase peak npts 


};
typedef struct new_PHASE  new_PHASE;

struct new_RECORD
{
	char* COMP;				//component  T / R/ Z

	char* name;				// record station name
	char* NET;				// network of this sta
	double DIST;			//distance of this sta
	double AZ;				//AZ of this sta
	double BAZ;				//BAZ of this sta
	double sta_lat;
	double sta_lon;
	double eq_lat;
	double eq_lon;
	double eq_dep;
	double eq_mag;
	char* eq_name;

	int NPTS;				// total points of data
	double B;					// begin time 
	double E;					// end time
	double DELTA;			// delta time

	new_PHASE* T;
};
typedef struct new_RECORD new_RECORD;

int initiate_input(new_INPUT* my_input);
int read_input_info(new_INPUT* my_input);
int read_eventstation_list(new_RECORD* my_record, new_INPUT* my_input);
int initiate_record_name(new_RECORD* my_record);
int read_record_for_phase(new_RECORD* my_record, new_INPUT* my_input);
int read_record_for_phase_for_one_sta(new_RECORD* my_record, new_INPUT* my_input);
int initiate_phase(new_PHASE* T,double prem, char* sac_file, new_INPUT* my_input, char* station_name);
int find_instrument_type(new_RECORD* my_record, char* instru);
double* read_long_window(char* sac_file, double long_beg, double long_len, double prem,char* velocity_or_displacement, new_INPUT* my_input, int polar_flag);
int read_phase_window(double* long_win,double long_beg, double long_len, double phase_beg, double phase_len,double* phase_win, new_INPUT* my_input);
int output_long_phase_window(new_RECORD* my_record, int phase_flag, new_INPUT* my_input);
int get_phase_amplitude(new_PHASE* T);
int get_SNR(new_PHASE* T);
int empirical_source_function(new_RECORD* my_record, new_INPUT* my_input);
int get_first_ES(new_RECORD* my_record, new_INPUT* my_input, double* ES1, int phase_flag, double phase_len);
double get_weight_from_SNR_CCC(double SNR, double ccc_orig);
int output_final_event_info(new_RECORD* my_record, new_INPUT* my_input);
int output_final_event_info_for_record(new_RECORD* my_record, new_PHASE* my_phase, FILE* out, new_INPUT* my_input);
int output_ES_for_each_record(new_RECORD* my_record, new_INPUT* my_input, int phase_flag, double phase_len);
int get_ONSET_ENDSET_for_each_record(new_RECORD* my_record, new_INPUT* my_input, int phase_flag,double phase_len);
int stretch_ES_and_CCC(new_RECORD* my_record, new_INPUT* my_input, double* current_ES);
int stretch_ES_function(double* current_ES, int npts_phase, double coeff,double*  tmp_ES);
int read_record_for_phase(new_RECORD* my_record, new_INPUT* my_input);
int free_record_memory(new_RECORD* my_record, new_INPUT* my_input);
int CCC(double* x, int npts_x, double* y, int npts_y, int* shift, double* ccc, int flag);
int output_STD_of_ES(new_RECORD* my_record,new_INPUT*  my_input,double* current_ES);
int output_final_pick_onset_info_for_record(new_RECORD* my_record, new_PHASE* my_phase, FILE* out);
int BEST_GAUSSIAN_read_ES(char* path, double* ES, int* len);
int BEST_GAUSSIAN_construct_new_ES(double* old_ES, int* npts_old_ES, double* new_ES, int* npts_new_ES);
int construct_array_with_main_lobe(double* array_in, int* npts_in, double* array_out);
int read_in_modified_S_ES(new_INPUT* my_input, double* array);
double get_weight_from_SNR(double SNR);
int remake_empirical_source(new_RECORD* my_record,new_INPUT* my_input, double* current_ES);
int output_current_ES_for_phase(new_INPUT* my_input, double* new_ES);
int get_misfit_for_each_record(new_RECORD* my_record, new_INPUT* my_input);
#endif


