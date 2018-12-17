#ifndef __HONGYULIBCPP_H
#define __HONGYULIBCPP_H

#include<cstdlib>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<cmath>
#include<math.h>
//#include "sacio.h"
extern "C" {
#include "/usr/local/sac/include/sacio.h"
//##include "/Users/Leviyu/bin/sac/include/sacio.h"
}

using namespace std;




int normalize_array_with_flag(double* array, int len, int flag);

// stretch ES by coeff and get output to tmp_ES
//int stretch_ES_function(double* current_ES, int npts_phase, double coeff,double*  tmp_ES);

// construct main lobe from array_in and output to array_out
//int construct_array_with_main_lobe(double* array_in, int* npts_in, double* array_out);


void cart2sph(double x, double y, double z, double* lat, double* lon, double* dep );

double dist_A_B(double A_lat, double A_lon, double B_lat, double B_lon);
double point_AZ_dist_point(double A_lat, double A_lon, double AZ, double dist, double* B_lat, double* B_lon);

double az_A_B(double A_lat, double A_lon, double B_lat, double B_lon);

double reference_TNA_SNA(double depth);
double reference_LLNL(double depth);
int reference_STW105(double depth, double* radius_out, double* density_out, double* Vpv_out,double* Vph_out, double* Vsv_out, double* Vsh_out, double* Qu_out,double* Qk_out, int flag);
double SEMUCB(double DEPTH);

double taup_incident(double EQ_lat, double EQ_lon, double EQ_dep, double STA_lat, double STA_lon, 	string phase);

int prem_500(double depth, double* radius_out, double* density_out, double* Vpv_out,double* Vph_out,
		 double* Vsv_out, double* Vsh_out, double* Qu_out,double* Qk_out, int flag);
int prem_aniso(double depth, double* radius_out, double* density_out, double* Vpv_out,double* Vph_out,
		 double* Vsv_out, double* Vsh_out, double* Qu_out,double* Qk_out, int flag);


int count_file_num(string name);


int  shell_pipe_text(char* command, char* text_result);
int  shell_pipe_double(char* command, double* text_result);
int CMT_transition( double strike, double dip, double rake, double* Mxx, double* Myy, double* Mzz, double* Mxy, double* Mxz, double* Myz);
//
int CMT_transition_rse( double strike, double dip, double rake, double* Mrr, double* Mss, double* Mee, double* Mrs, double* Mre, double* Mse);
//int count_file_num(char *filename);
bool is_file_exist(std::string name);
void wiginterpd(double *x, double *y, int npts, double *xx, double *yy, int Newnpts, int flag);


int output_array2( string output_name, double* array1,double* array2,int file_num, int normalize_flag);

int output_array1( std::string output_name, double* array1,int file_num);

int prem(double depth, double* radius_out, double* density_out, double* Vpv_out,double* Vphout,double* Vsv_out, double* Vsh_out, double* Qu_out,double* Qk_out, int flag);
double amplitude(double* array, int len);


int normalize_array(double* array, int len);

int amplitudeloc(double* array, int len, int* max_amp_loc, double* amplitude, int flag);

double gaussian_func(double a, double b, double c, double d, double x);

int t_star_func_mine(double t_star, int npts , double delta, double* tstar_array);
int BINFD(int LX, int* LX1, int* N);

double shell_pipe(std::string command);
//std::string exec(const char* cmd);
std::string exec(std::string command);

//#int shell_pipe_command(char* command);

//int sac2xy(char* sac_file, double begin_time, double length, double* out_array);
//
double taup_time(double EQ_lat, double EQ_lon, double EQ_dep, double STA_lat, double STA_lon, std::string  phase);

//////int sac2xy_relative_to_phase( double EQ_lat, double EQ_lon, double EQ_dep, double STA_lat, double STA_lon, string phase, string sac_file, double beg_to_phase, double length, double* out_array);
int sac2xy_relative_to_phase( double delta ,double EQ_lat, double EQ_lon, double EQ_dep, double STA_lat, double STA_lon, std::string phase, std::string sac_file, double beg_to_phase, double length, double* out_array);


//double interp3_linear(double *p,double dx,double dy,double dz);

void waypoint(double EVLO,double EVLA,double stlo, double stla,double dist,double *plo,double *pla);


//int hilbert(double* array_in, int npts, double* array_out);

void DFT(double* inreal, double* inimag, double* outreal, double* outimag, int n);

void IDFT(double* inreal, double* inimag, double* outreal, double* outimag, int n);

//int hilbert2(double* array_in, int npts, double* array_out);
void sph2cart(double lat, double lon, double dep, double* x, double*y, double* z);
double interp2_linear(double A, double B, double C, double D,double dx,double dy);
int prem_p(double depth, double* vp, int flag);
int prem_s(double depth, double* vs, int flag);
double tx2011(double DEPTH);
int prem_pac06(double depth, double* radius_out, double* density_out, double* Vpv_out,double* Vph_out, double* Vsv_out, double* Vsh_out, double* Qu_out,double* Qk_out, int flag);


//int latlon_statics(char* file_name,double lat_delta, double lon_delta, char* out_name);
int convolution2(double* array1, int npts1, double* array2, double* array_out);
int t_star_find_uniq(double t_star,double* tstar_array, int npts, double delta);
int convolution(double* array1, int npts1, double* array2,int npts2, double* array_out);
void cart2sph( double x, double y, double z, double* lat, double* lon, double* dep);



int sac2xy_with_delta(std::string sac_file, double begin_time, double length, double* out_array, double DELTA);




int ellipcor_kennett(char *Phase, double depth, double colat, double gcp, double az, double *cor);

char* string_to_char(std::string mystring);


int ak135(double depth, double* radius_out, double* density_out, double* Vpv_out,double* Vsv_out, double* Qu_out,double* Qk_out, int flag);
int reference_iasp91(double depth, double* vs, double* vp);









int tstar_ES_function(double* current_ES, int npts_phase, double coeff,double*  tmp_ES);
int t_star_find_uniq(double t_star , double* tstar_array, int npts, double delta);



std::string double_to_string(double mydouble);
std::string int_to_string(int mydouble);
double string_to_double(std::string mystring);





int stretch_gaussian_find_best_match_for_given_interval(double* phase_win, int npts_phase, double coeff_min, double coeff_max, double coeff_delta, double* best_ccc, double* best_coeff,
		int* best_time_shift, double* best_ES);

int construct_array_with_main_lobe(double* array_in, int* npts_in, double* array_out);

int CCC_posi(double* x, int npts_x, double* y, int npts_y, int* shift, double* ccc , int flag);
int CCC( double* x, int npts_x, double* y, int npts_y, int* shift, double* ccc, int flag);


int stretch_record_find_best_match_for_given_interval(double* record,
		double* phase_win, int npts_phase, double coeff_min, double coeff_max, 
		double coeff_delta, double* best_ccc, double* best_coeff,
		int* best_time_shift, double* best_ES,int flag);

int stretch_ES_function(double* current_ES, int npts_phase, double coeff,double*  tmp_ES);









#endif
