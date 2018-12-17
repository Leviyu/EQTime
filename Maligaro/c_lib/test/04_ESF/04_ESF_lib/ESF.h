#ifndef EQ_data_H
#define EQ_data_H
#include<stdlib.h>
#include<stdio.h>
#include "hongyulib.h"

struct new_INPUT
{

	char* PWD;
	char* logfile;
	FILE* out_logfile;
	char* EQ;
	char* event_file;
	char* eventinfo_file;
	char* polarity_file;
	int sta_num;
	char* PHASE;
	char* COMP;
	char* filter_flag;
	char* velocity_or_displacement;
	double filter_min, filter_max;
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


	int SNR_phase_flag;						// SNR is relative to specified phase
											// == 1 , relative to specified phase
											// == 0 , relative to current phase
	
	int POLAR_SOLUTION;

	char* SNR_phase;
	int S_npts_peak_onset;
	int S_npts_peak_beg;

	double DIST_DELTA;		// distance sum delta

	char stretch_flag[200];		// tstar is using tstar
							// stretch is using stretching


	int STACK_FLAG;

	// stacking info
	double lat_beg;
	double lat_end;
	double lat_delta;
	double lon_beg;
	double lon_end;
	double lon_delta;
	double AREA_RADIUS;	// the radius to search within that grid
	int num_lat;
	int num_lon;


	double TIME_REF; // the reference time for stacking 
	// this is specified to zoom in that area
	//
	//
	

	// store the path of uniq station list
	char uniq_sta_list[1000];
	double local_average_radius;
	int MAX_STA;


};
typedef struct new_INPUT new_INPUT;



struct new_RECORD
{
	char* EQ;
	char* COMP;				//component  T / R/ Z
	char* PHASE;
	char* name;				// record station name
	char* NET;				// network of this sta
	double DIST;			//distance of this sta
	double DIST_DELTA;		// distance sum delta
	double AZ;				//AZ of this sta
	double BAZ;				//BAZ of this sta
	double sta_lat;
	double sta_lon;
	double eq_lat;
	double eq_lon;
	double eq_dep;
	double eq_mag;

	double weight;			// weight of current record that goes into the E.W.

	int NPTS;				// total points of data
	double B;					// begin time 
	double E;					// end time
	double delta;			// delta time

	double polarity;		// polarity coefficient given by radiation pattern
	int polar_flag;			// watch for the polarity of current phase 
								// if 1 == positive
								// -1 == negative
								// 0 == not set
	int npts_signal_beg;
	int npts_signal_end;	// npts where signal beg and end


	double ray_parameter;

	int quality;			// the quality of current phase
							// 1 == good
							// 0 == not set 
							// -1 == problem record

	// long window
	double* long_win;		// stored long window 
	int npts_long;
	double long_beg;
	double long_len;


	// phase window
	double* phase_win;
	double* stretched_phase_win;	// stores the stretched phase win to best match E.W.
	int npts_phase;
	double phase_beg;			// window begin time for current phase eg. -10s
	double phase_len;

	double* noise_win;			// noise window
	int npts_noise;
	double noise_beg;
	double noise_len;

	double* ES_win;
	int npts_ES;
	double ES_beg;
	double ES_len;
	double* stretched_ES_win;
	double* stretched_gaussian_win;
	double best_stretch_ccc;
	double best_stretch_coefficient;

	double best_tstar;
	double best_tstar_ccc;


	double amplitude;		// amplitude for current phase
	double prem;			// prem arrival time for current phase
	double prem_SNR_phase;	// SNR_phase_prem time
	double dt_obs_prem;		// travel time anomaly relative to PREM

	double ccc;				// cross-correlation coefficient between Empirical source and the record
	double SNR;				
	double SNR2;
	double SNR_sig;
	double SNR_noi;
	double ONSET;
	double ENDSET;
	double misfit;
	double misfit2;


	int npts_SS_peak_zero;	// the npts difference between SS /ScSScS peak and the zero point in front of peak
	int npts_phase_peak;		// phase peak npts 
	double time_phase_peak;		// phase_peak time relative to phase PREM

	int SS_npts_phase_peak;
	double SS_time_phase_peak;
	double SS_onset_hardwire_from_S;



	// stacking info
	double ref_lat;
	double ref_lon;

	double stacking_delay_time;		// the delay time of each record relative to the reference point
	double fix_BAZ_delay_time;
	double fix_slow_delay_time;



	// station correction
	double dt_average;
	double dt_STD;



};
typedef struct new_RECORD new_RECORD;

struct new_STATION
{
	char* name;
	double lat;
	double lon;
	double dt_ave;
	double dt_STD;
	

	// num of records at this station
	int count;
	double* dt_array;
	double* incident;
	double* BAZ;
	double height;
	double ray_parameter;
	char** neighbour_sta_array;
	int neighbour_sta_num;
	double* neighbour_sta_dt_array;
	double neighbour_dt_ave;



};
typedef struct new_STATION new_STATION;

int SC_single_station(new_RECORD* my_record, new_INPUT* my_input, new_STATION* my_station);
int SC_LocalAverage(new_RECORD* my_record, new_INPUT* my_input, new_STATION* my_station);

struct new_GRID
{

	double lat_beg;
	double lat_end;
	double lat_delta;
	double lon_beg;
	double lon_end;
	double lon_delta;



	double lat;
	double lon;
	double dt;
	double height;		// not used yet

	double AZ;
	double BAZ;
	//double slowness;
	double ray_parameter;



	int sum_num;		// station number within this cell
	int* sum_list;	// list of station num that is within the range of current grid

	double radius_deg;	// search radius within that grid point

	double prem;		// prem_time for  the grid point for that phase


	// arrays to store time-slowness-amplitude info
	// arrays to store time-BAZ-amplitude info for plotting
	double* fix_BAZ_time;
	double* fix_BAZ_slowness;
	double* fix_BAZ_amp;

	double* fix_slow_time;
	double* fix_slow_BAZ;
	double* fix_slow_amp;

	double fix_BAZ_delay_time;
	double fix_slow_delay_time;

};
typedef struct new_GRID new_GRID;

int initiate_grid(new_GRID** my_grid, new_INPUT* my_input);
int single_grid_stacking(new_GRID** my_grid, new_INPUT* my_input, new_RECORD* my_record);

int initiate_input(new_INPUT* my_input);
int read_input_info(new_INPUT* my_input);
int read_eventstation_list(new_RECORD* my_record, new_INPUT* my_input);


#endif

int empirical_source_for_each_record(new_RECORD* my_record, new_INPUT* my_input, double* current_ES, int loop_num);

int initiate_record_name(new_RECORD* my_record);
int read_record_for_phase_for_one_sta(new_RECORD* my_record, new_INPUT* my_input);
int read_record_for_phase_for_one_sta_for_stacking(new_RECORD* my_record, new_INPUT* my_input);
int read_record_for_phase(new_RECORD* my_record, new_INPUT* my_input);
int read_record_for_phase_for_stacking(new_RECORD* my_record, new_INPUT* my_input);
int find_instrument_type(new_RECORD* my_record, char* instru);
int initiate_phase(new_RECORD* my_record,double prem, char* sac_file, new_INPUT* my_input);

int define_goodness_of_record(new_RECORD* my_record, new_INPUT* my_input);
int read_long_window(char* sac_file, new_RECORD* my_record, new_INPUT* my_input);
int read_phase_window(new_RECORD* my_record, new_INPUT* my_input);
int read_noise_window(new_RECORD* my_record, new_INPUT* my_input);
int get_phase_amplitude(new_RECORD* my_record);
int get_first_ES(new_RECORD* my_record, new_INPUT* my_input, double* ES);
int get_first_EW_for_phase(new_RECORD* my_record, new_INPUT* my_input, double* ES);
int empirical_source_function(new_RECORD* my_record, new_INPUT* my_input);
int get_ONSET_ENDSET_for_each_record_origional_phase(new_RECORD* my_record, new_INPUT* my_input);
int CCC( double* x, int npts_x, double* y, int npts_y, int*shift, double* ccc, int flag);
int get_misfit_for_each_record_origional_record(new_RECORD* my_record, new_INPUT* my_input);
double get_weight_from_SNR_CCC(double SNR, double ccc_orig);
int tstar_ES_function(double* current_ES, int npts_phase, double coeff,double*  tmp_ES);
int stretch_ES_and_CCC(new_RECORD* my_record, new_INPUT* my_input, double* current_ES);
int construct_array_with_main_lobe(double* array_in, int* npts_in, double* array_out);
int stretch_ES_function(double* current_ES, int npts_phase, double coeff,double*  tmp_ES);
int restack_ES_for_phase(new_RECORD* my_record, new_INPUT* my_input, double* ES);
int output_ES_for_each_record(new_RECORD* my_record, new_INPUT* my_input);
int get_ONSET_ENDSET_for_each_record_stretched(new_RECORD* my_record, new_INPUT* my_input);
int output_final_event_info(new_RECORD* my_record, new_INPUT* my_input);
int free_record_memory(new_RECORD* my_record, new_INPUT* my_input);
int output_long_phase_window(new_RECORD* my_record,new_INPUT* my_input);
int output_long_phase_window_distsum(new_RECORD* my_record,new_INPUT* my_input);
int output_final_pick_onset_info_for_record(new_RECORD* my_record,  FILE* out);
int output_long_phase_window_for_each(new_RECORD* my_record, new_INPUT* my_input);
int output_long_phase_window_for_each_distsum(new_RECORD* my_record, new_INPUT* my_input);
int define_new_phase_window_len(new_RECORD* my_record, new_INPUT* my_input);
int narrow_down_phase_window(new_RECORD* my_record, new_INPUT* my_input, int old_npts);
int output_STD_of_third_ES(new_RECORD* my_record,new_INPUT*  my_input,double* current_ES);
int output_current_ES_for_phase_third(new_INPUT* my_input, double* new_ES);
int get_misfit_for_each_record_stretched(new_RECORD* my_record, new_INPUT* my_input);
int get_SNR2_for_each_record(new_RECORD* my_record, new_INPUT* my_input);
int stretch_ES_find_best_match_for_given_interval(new_RECORD* my_record, double* ES_win, double* phase_win, int npts_phase, double coeff_min, double coeff_max, double coeff_delta, double* best_ccc, double* best_coeff,  int* best_time_shift, double* best_ES, new_INPUT* my_input);
int stretch_gaussian_find_best_match_for_given_interval(double* phase_win, int npts_phase, double coeff_min, double coeff_max, double coeff_delta, double* best_ccc, double* best_coeff,  int* best_time_shift, double* best_ES);
int stretch_record_find_best_match_for_given_interval(double* record,double* phase_win, int npts_phase, double coeff_min, double coeff_max, double coeff_delta, double* best_ccc, double* best_coeff,  int* best_time_shift, double* best_ES);
int define_stretch_EW_ONSET(new_RECORD* my_record, new_INPUT* my_input);
int read_in_modified_S_ES(new_INPUT* my_input, double* array);
int read_eventinfo_list(new_RECORD* my_record, new_INPUT* my_input);
int stretch_record_restack_ES(new_RECORD* my_record, new_INPUT* my_input, double* ES);
int output_STD_of_ES_with_stretch_record(new_RECORD* my_record,new_INPUT*  my_input,double* current_ES);
int output_STD_of_ES_with_first_ES(new_RECORD* my_record,new_INPUT*  my_input,double* current_ES);
int output_STD_of_ES_with_second_ES(new_RECORD* my_record,new_INPUT*  my_input,double* current_ES);
int output_current_ES_for_phase(new_INPUT* my_input, double* new_ES);
int output_current_ES_for_phase_second(new_INPUT* my_input, double* new_ES);
int output_STD_of_first_ES(new_RECORD* my_record,new_INPUT*  my_input,double* current_ES);
int output_STD_of_second_ES(new_RECORD* my_record,new_INPUT*  my_input,double* current_ES);
int SC_initiate_station(new_STATION* my_station, new_INPUT* my_input);
int SC_read_uniq_sta_list(new_RECORD* my_record, new_INPUT* my_input, new_STATION* my_station);
int output_current_ES_for_phase_first_iteration(new_INPUT* my_input, double* new_ES);
int output_STD_of_first_iteration_ES(new_RECORD* my_record,new_INPUT*  my_input,double* current_ES);
int stretch_ES_and_CCC_tstar(new_RECORD* my_record, new_INPUT* my_input, double* current_ES);
int restack_ES_for_phase_after_tstar(new_RECORD* my_record, new_INPUT* my_input, double* ES);
int EWM_logfile_write(new_INPUT* my_input, char* message);
int EW_travel_time_traffic_checking(new_RECORD* my_record, new_INPUT* my_input);
