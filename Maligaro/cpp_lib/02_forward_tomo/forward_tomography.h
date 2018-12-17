#ifndef __FORWARD_TOMOGRAPHY_H
#define __FORWARD_TOMOGRAPHY_H
// forward tomography data structure declaration here
#include<cstdlib>
#include<string>
#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>
#include<iomanip>
#include "../01_cpp_lib/hongyulibcpp.h"
#include<cmath>
#include<complex>
#include<vector>
//extern "C"{
//#include "../../c_lib/c01_tools/hongyulib.h"
//}
using namespace std;


class big_new_record;
class new_record;
class new_tomo;
class CRUST_MODEL;
class virtual_station;


// strucutre of cell
class new_cell
{
	public:
	double dvs;
	double STD;							// standard deviation 
	int idep, ilon, ilat;

	// sumation of number of ray going through cell
	int 	sum_num_in_cell;
	double 	sum_dvs_in_cell;
	double 	delta_dvs;					// delta of updated_tomo - before_updated_tomo
	double 	gradient;					// the 3D gradient structure model
	double 	BAZ;
	int		num_BAZ;
	int 	vtk_index;
	vector<int>	vtk_index_array;

	new_cell();
	~new_cell();
};
// Model Structure
class new_tomo
{
	public:
		string INFILE;
		string MODEL_INFILE;
		string MODEL_model;
		string MODEL;
		string MODEL_DIR;
		string dv_type;

		string Iteration_Scheme;
		int Iteration_M;
		int Iteration_N;
		int Iteration_MN;



		double weight_ratio_path_length_RMS_tomo;
		int		num_dep,num_lat,num_lon;
		vector<int> num_lon2;
		double dep_min, dep_max, dep_delta;
		double lat_min, lat_max, lat_delta;
		double lon_min, lon_max, lon_delta;


		int dep_profile_num;
		vector<double> depth_profile;

		// plotting parameter
		int plot_profile_num;
		vector<double> plot_profile_dep;

		// iteration info
		int current_iteration;
		int current_layer_num;
		int tomo_iterations;
		int tomo_layer_number;
		vector<double> tomo_layer_array;

		double current_layer_dep_min;		// depth of current layer min value
		double current_layer_dep_max;		// depth of current layer max value

		int CMB_lat_cell_number;

		string taup_path_dir;
		string cross_point_dir;

		vector<double> dep;
		vector<double> lat;
		vector<double> lon;
		vector< vector<double> >  lon2;

		vector< vector< vector<new_cell>  >  > my_cell;



		double 	  layer_thickness;			// the layer_thickness to look for when averaging for dvs

		string tomo_model_file;				// netcdf tomography file

		void initiate_tomo();				// allocate space for model
		int read_INFILE();
		int read_MODEL_INFILE();
		int read_tomography();


		string output_tomo_file;
		int output_tomography_info();
		int output_tomography_info2();
		int output_tomography_info3();
		int output_starting_tomography();


		int convert_to_new_tomo(new_tomo* old_tomo);
		int define_vertical_profile();
		int define_horizontal_profile();


		double find_dvs_with_dep_lat_lon(double dep, double lat, double lon);
		int    find_index_lat(double lat);
		int    find_index_lon(double lon, int index_lat);
		int    find_index_dep(double dep,int* index_min, int* index_max);
		int    find_index_dep2(double dep);


		string flag_is_new_tomo;
		bool	is_new_tomo;

		int getDep(int idep);

		int print();

		int RMS_layer_num;
		vector<double> RMS_dep;
		vector<double> RMS_weight;
		double get_delta_lat_lon_distance_with_depth(int index_dep);


		void construct_RMS_profile();


		void pointA_pointB_find_cross_pointC(double dep1, double lat1, double lon1,
				double dep2, double lat2, double lon2,
				int index_dep1,int index_lat1, int index_lon1,
				int index_dep2,int index_lat2, int index_lon2,
				double* mid_dep, double* mid_lat, double* mid_lon,
				double* delta_angle_d1_d2, double* radius);

		void forward_tomography_func(big_new_record* my_big_record);
		void forward_tomo_for_station(big_new_record* my_big_record);
		int forward_tomo_for_one_station(new_record* my_record);
		int distribute_dt_residual_and_convert_to_dvs(new_record* record);
		void update_tomo_for_current_iteration();
		void output_vtk_format_model();
		void output_time_info(big_new_record* my_big_record);
		void output_record_path_crosssection(big_new_record* my_big_record);
		void output_record_path_line(big_new_record* my_big_record);
		void output_record_path_line_in_one_file(big_new_record* my_big_record);
		void output_coverage_hit_count();
		void output_delta_tomography(new_tomo* my_tomo);
		void output_gradient_tomography();
		void output_vertical_gradient_tomograph();
		void output_STD_tomography();
		void output_smoothed_tomography();
		void S1_update(new_record* my_record);
		//void S2_update(big_new_record* my_big_record);
		//void S3_update(big_new_record* my_big_record);
		//void S4_update(big_new_record* my_big_record);
		//void S5_update(big_new_record* my_big_record);
		void calculate_turning_depth(new_record* my_record);
		void output_cross_section(big_new_record* my_big_record);
		void output_cross_section_for_one_record(new_record* my_record);
		//void output_cross_section_hit_count(big_new_record* my_big_record);
		//void output_cross_section_for_one_record_hit_count(new_record* my_record);
		double find_hit_with_dep_lat_lon(double dep, double lat , double lon);

		int CONVERT_write_to_netcdf(char* netcdf_file);
		int construct_1D_reference();
		void output_RMS_profile();
		void output_tomo_for_media();

		new_tomo();
		~new_tomo();
};
// Record Structure
class new_record
{
	public:
		string EQ;
		string STA;
		string NET;
		string PHASE;
		string COMP;


		double eq_lat;
		double eq_lon;
		double eq_dep;
		double sta_lat;
		double sta_lon;
		double eq_mag;
		double DIST;
		double AZ;
		double BAZ;
		int polarity_flag;	
		int  quality_flag;			// 1 is good -1 is bad 0 is not known
		double ccc;
		double SNR;
		double stretch_ccc;
		double stretch_coeff;
		double misfit;
		double phase_amplitude;

		double dt_obs_prem;				// travel time anomaly relative to PREM from Empirical Waveform
		double PREM;
		double code_PREM;
		double PREM_tomo;
		double dt_tomo_PREM;
		//double dt_residual_before;		// dt(T_obs - T_tomo)
		double dt_residual_after;		// dt(T_obs - T_new_tomo)
		double dt_residual_for_current_iteration;	// dt(T_obs - T_tomo)

		double turning_depth;
		double dt_tomo_correction;
		string record_file;


		double phase_peak_time_rel_PREM;
		int 	npts_phase_peak_rel_start;
		double 	noise_beg;
		double 	noise_len;
		double phase_beg;
		double phase_len;
		double  long_beg;
		double  long_len;
		vector<double> long_win;
		vector<double> phase_win;
		vector<double> long_win_orig;
		string read_sac_flag;		
		double delta;
		double phase_beg_rel_PREM;		// phase begin time relative to PREM
		double record_weight;
		double SNR2;
		double misfit2;
		double ONSET;
		double ENDSET;
		double tstar_factor;
		double tstar_ccc;
		double ccc3;
		double misfit_pre;
		double misfit_bak;
		double record_gau_factor;
		double EW_gau_factor;
		double gau_misfit;
		double polarity;
		double polarity_prediction;
		double traffic_phase_nearby;




		double ellip_corr;			// ellipticity correction for current record
		double incident;			//incident angle

		double current_iteration_coeff;

		// store info for taup_path
		vector<double> angle;
		vector<double> radius;
		int taup_path_max_num;

		// here is the array to store the cross-points for each record
		int CP_num;
		
		vector<double> CP_lat;
		vector<double> CP_lon;
		vector<double> CP_dep;
		vector<int> CP_ilat;
		vector<int> CP_ilon;
		vector<int> CP_idep;
		vector<double> CP_dl;
		vector<double>  CP_v_PREM;
		vector<double> current_CP_weight;
		vector<double> CP_weight;




		void initiate_CP();
		void free_CP();

		double crust_correction;

		// extra info read in
		// taup_path and cross-point path
		string taup_path_file;
		string cross_point_file;

		void read_cross_point_info( new_tomo* my_tomo);
		void read_taup_path_info(string taup_path_dir);
		void find_cross_points(new_tomo* my_tomo);
		void convert_long_win_to_velocity();

		void calculate_tomo_correction(new_tomo* my_tomo);
		void get_ellip_corr();
		void read_sac_file_relative_to_PREM();
		void read_sac_file();
		void calculate_SNR();
		void get_crustal_correction();
		void get_incident();
		string sac_file;
		void download_sac_file();
		void get_polar_flag();

		vector<CRUST_MODEL> my_crust;

		// record related EQ bin and STA bin
		int EQ_BIN_NUM;
		vector<int> EQ_BIN_LAT;
		vector<int> EQ_BIN_LON;

		int STA_BIN_NUM;
		vector<int> STA_BIN_LAT;
		vector<int> STA_BIN_LON;

		new_record();
		~new_record();
};
// declare big_new_record
class big_new_record
{
	public:
		big_new_record();
		~big_new_record();
//
		double VS_LATITUDE_INC;
		double VS_EQ_RADIUS_DEGREE;
		double VS_STA_RADIUS_DEGREE;
		double VS_RECORD_NUM_THRESHOLD;
		double VS_EXISTING_RECORD_NUM_THRESHOLD;
		string S_ES_DIR;

		int eventinfo_max_threshold;
		int eventStation_min_threshold;
		string EXISTING_EVENTINFO;
		int existing_sta_num;
		string PHASE;
		double phase_dist_min;
		double phase_dist_max;
		string SRCDIR;

		double MASK_MIN;
		double MASK_MAX;


		double SNR_CUT;
		double CCC_CUT;

		// unique EQ and STA storage
		int unique_EQ_num;
		vector<string> EQ_LIST;
		vector<double>  EQ_LAT;
		vector<double>  EQ_LON;
		int unique_STA_num;
		vector<string> STA_LIST;
		vector<double>  STA_LAT;
		vector<double>  STA_LON;

		int sta_num;
		string record_file;
		string eventStation;
		vector<new_record> my_record;
		vector<new_record> existing_record;
		double delta;
		double long_len;
		double long_beg;
		double noise_beg;
		double noise_len;
		double phase_beg;
		double phase_len;


		string timeinfo_outfile;
		
		void read_record_file(new_record* my_record);
		void initiate_big_record();
		void read_INFILE();


		void big_record_read_cross_point_file(new_tomo* my_tomo);
		void get_ellip_corr();
		void get_incident();
		void read_sac_file_and_store_relative_to_phase();
		void read_eventStation();
		void get_crustal_correction();

		void read_sac_file();
		void calculate_SNR();
		void read_in_polarity_file();
		vector<vector<virtual_station> > my_grid;
		vector<virtual_station> my_vs; 
		int my_vs_index;
		int my_vs_index_old;


		// grid related parameter
		int grid_lat_num;
		vector<int> grid_lon_num;


		void virtual_station_grid_initiate();
		void virtual_station_main();
		void catagorize_existing_eventinfo_to_VS();
		void catagorize_eventstation_to_VS();
		void count_record_existance_for_grid_pair();
		int loop_EX_EQ_STA_count_record(virtual_station* eq_grid, virtual_station* sta_grid);
		void output_VS_info_for_one_VS(int vs_index);
		void catagorize_records_into_VS_based_on_each_record( new_record* my_record, int sta_num);
		void for_each_EQ_check_num_records_for_stacking();
		int find_EQ_STA_bin_record_existance_num(new_record* my_record, int sta_num, int ilat_eq, 
			int ilon_eq, int ilat_sta, int ilon_sta)  ;
		void get_PHASE_min_max_dist();
		void for_each_EQ_check_num_records_for_stacking(new_record* my_record, int sta_num, int ilat_eq, 
			int ilon_eq, int ilat_sta, int ilon_sta  );
		// void count_record_existance_for_sing_grid_pair(int ilat_eq, int ilon_eq, int ilat_sta, int ilon_sta, 
		// 	int* exist_eventinfo, int* exist_eventStation);
		void count_eventStation_existance_for_sing_grid_pair(int ilat_eq, int ilon_eq, int ilat_sta, int ilon_sta, int* exist_eventinfo);
		void count_eventinfo_existance_for_sing_grid_pair(int ilat_eq, int ilon_eq, int ilat_sta, int ilon_sta, int* exist_eventStation);
		void get_unique_EQ_latlon(string EQ_NAME, double* lat, double* lon);
		void get_unique_STA_latlon(string STA_NAME, double* lat, double* lon);
		int find_EQ_STA_PHASE_number_in_eventinfo(string EQ_NAME, string STA, string PHASE);
		int find_EQ_STA_PHASE_number_in_eventStation(string EQ_NAME, string STA);
		void make_virtual_station(int ilat_eq, int ilon_eq, int ilat_sta, int ilon_sta);
		void make_virtual_station_for_EQ(int ilat_eq, int ilon_eq, int ilat_sta, int ilon_sta, int ieq_index);
		int individual_VS_processing();
		void plot_current_vs(int vs_index);
};
class virtual_station : public new_record
{
	public:
		int ivs;
		int ilat_eq;
		int ilon_eq;
		int ilat_sta;
		int ilon_sta;

		string exist;


		// vector<string> EQ_NAME_array;
		int EQ_index;
		vector<int> eventinfo_index_array;
		int eventinfo_index;
		vector<int> eventStation_index_array;
		int eventStation_index;
		void initiate(big_new_record* my_big_record);
		void destruct();

		double MASK_MIN;
		double MASK_MAX;
		double one_period;
		void calculate_misfit(double* tstar_ES, double* phase_win, int npts);


		double amp_scale;


		// grid basic information
		double grid_lat;
		double grid_lon;
		int ilat;
		int ilon;
		double grid_radius;
		double grid_height;
		double grid_time;					// centra grid travel time for one EQ, as a reference time

		double VS_LATITUDE_INC;

		// for records within grid range, store record tag info
		int npts_record_sum;
		vector<int> record_tag;				// store the record line number in eventinfo to make sure that we can find each record

		// for given EQ, when we do stacking, we need az baz slowness info
		double grid_dist;					// distance from EQ to grid center
		double AZ;							//az from EQ to grid center
		double BAZ;
		double incident_angle;
		double PREM;						//PREM time from EQ to grid center
		// the stacking window
		double delta;
		double WIN_BEG;
		double WIN_LEN;
		double LONG_BEG;
		double LONG_LEN;
		double noise_beg;
		double noise_len;
		double phase_beg;
		double phase_len;
		int     long_npts;
		vector<double> long_win;			// the stacked record for current grid
		int station_num_threshold;			//  if station number in range is not greater then this value, grid is skipped
		double ave_SNR;						// average SNR of records within range
		double stack_SNR;
		double stack_SNR_peak;
		string S_ES_file;
		int find_stack_ONSET_time();
		double virtual_stack_ONSET;
		double tstar_ccc;
		double tstar_factor;
		double gau_ccc;
		double gau_factor;

		double quality;

		// when we do stacking, arrays to store time-slowness-amplitude info
		
		double dt_ave;
		double dt_STD;
		double crustal_correction_ave;
		double crustal_correction_STD;



		//double* fix_BAZ_time;
		//double* fix_BAZ_slowness;
		//double* fix_BAZ_amp;
		//double* fix_slow_time;
		//double* fix_slow_BAZ;
		//double* fix_slow_amp;
		//double fix_BAZ_delay_time;
		//double fix_slow_delay_time;

		big_new_record* my_big_record;
		
		void find_records_within_range();
		void stack_records_from_one_EQ();
		void output_stacked_record();
		void get_dt_ave_STD();
		void get_crust_correction_ave_STD();

		void relocate_grid_center();
		void get_SNR_before_and_after_stack();
		//void get_grid_dist(virtual_station EQ_grid, virtual_station STA_grid);
		void get_grid_dist(const virtual_station& EQ_grid, const virtual_station& STA_grid);
		void get_grid_dist_final();
		void get_traffic_time();
		void update_VS_info();
		void mask_window_and_store_into_long_orig();
		void make_quality_decision();
		void calculate_VS_misfit(double* tstar_ES, double* phase_win, int npts);
		void define_ONSET_ENDSET();
		void define_period_for_record();
		void update_SNR();


		// virtual_station();
		// ~virtual_station();

		void initiate_grid();

		// initiate traffic phase storage
		vector<string> traffic_phase_list;
		vector<double> traffic_phase_time;



		// station stacking usage
		string 	out_stacked_record_rel_PREM;
		string 	out_stacked_record_raw;
		//double* record_win_rel_PREM;
		//double* record_win_rel_PREM_shifted;
		//double* record_win_raw;
		//double* record_win_raw_shifted;



		// existing  EQ and STA storage
		int EX_EQ_NUM;
		vector<string> EX_EQ_NAME;
		vector<double> EX_EQ_LAT;
		vector<double> EX_EQ_LON;

		int EX_STA_NUM;
		vector<string> EX_STA_NAME;
		vector<double> EX_STA_LAT;
		vector<double> EX_STA_LON;
		// current  EQ and STA storage
		int CU_EQ_NUM;
		vector<string> CU_EQ_NAME;
		vector<double> CU_EQ_LAT;
		vector<double> CU_EQ_LON;

		int CU_STA_NUM;
		vector<string> CU_STA_NAME;
		vector<double> CU_STA_LAT;
		vector<double> CU_STA_LON;

		// skip flag
		// if eventStation does not give enough 
		// 		EQ < 1
		// 		STA < threshold
		// 	then skip it, skip flag = 1
		double eq_skip_flag;
		double sta_skip_flag;


		// grid eventinfo/eventStation number
		// int exist_eventStation;
		// int exist_eventinfo;
};
class CRUST_MODEL
{
	public:
		vector< vector<double> > boundary;
		vector< vector<double> > vs;
		vector< vector<double> > vp;
		vector< vector<double> > rho;
		



		void read_crust_model();
		void get_single_station_correction(double lat, double lon, double* thickness, double* CRUST_TIME, double* PREM_TIME );
		void get_single_station_correction_vp(double lat, double lon, double* thickness, double* CRUST_TIME, double* PREM_TIME );


		CRUST_MODEL();
		~CRUST_MODEL();
};


#endif
