#ifndef HONGYULIB_H
#define HONGYULIB_H
/*
 * This is a header file that contains all the useful funcition I wrote
 *
 * June 8 2014 Hongyu
 */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include "sacio.h"
#include<complex.h>
#include<assert.h>





void hello();		// this is a test function

void hello2();		// this is a test function

double dist_A_B(double A_lat, double A_lon, double B_lat, double B_lon) ;

double variance(char* infile);

void latlonvalue_average_std(char* input, double grid_inc, char* output);

double point_AZ_dist_point(double A_lat, double A_lon, double AZ, double dist, 
		double* B_lat, double* B_lon);

double az_A_B(double A_lat, double A_lon, double B_lat, double B_lon);

int file_exist(char *filename);

int  shell_pipe_text(char* command, char* text_result);

int  shell_pipe_double(char* command, double* text_result);

int CMT_transition( double strike, double dip, double rake, double* Mxx,
		double*	Myy, double* Mzz, double* Mxy, double* Mxz, double* Myz);

int CMT_transition_rse( double strike, double dip, double rake, double* Mrr, 
		double* Mss, double* Mee, double* Mrs, double* Mre, double* Mse);

int count_file_num(char *filename);

int output_array2( char* output_name, double* array1,double* array2,int file_num,
		int normalize_flag);

int output_array1( char* output_name, double* array1,int file_num);

int prem(double depth, double* radius_out, double* density_out, double* Vpv_out,
		double* Vphout,double* Vsv_out, double* Vsh_out, double* Qu_out,double* Qk_out, int flag);

double amplitude(double* array, int len);

int normalize_array(double* array, int len);
int normalize_array_posi(double* array, int len);

int amplitudeloc(double* array, int len, int* max_amp_loc, double* amplitude, int flag);

double gaussian_func(double a, double b, double c, double d, double x);

int t_star_func_mine(double t_star, int npts , double delta, double* tstar_array);

int BINFD(int LX, int* LX1, int* N);

double shell_pipe(char* command);

int shell_pipe_command(char* command);

int sac2xy(char* sac_file, double begin_time, double length, double* out_array);

double taup_time(double EQ_lat, double EQ_lon, double EQ_dep, double STA_lat, double STA_lon, char* phase);

int sac2xy_relative_to_phase( double EQ_lat, double EQ_lon, double EQ_dep, double STA_lat,
		double STA_lon, char* phase, char* sac_file, double beg_to_phase, double length, double* out_array);

double interp3_linear(double *p,double dx,double dy,double dz);

void waypoint(double EVLO,double EVLA,double stlo, double stla,double dist,double *plo,double *pla);

void wiginterpd(double *x, double *y, int npts, double *xx, double *yy, int Newnpts, int flag);

int hilbert(double* array_in, int npts, double* array_out);

void DFT(double* inreal, double* inimag, double* outreal, double* outimag, int n);

void IDFT(double* inreal, double* inimag, double* outreal, double* outimag, int n);

int hilbert2(double* array_in, int npts, double* array_out);

void sph2cart(double lat, double lon, double dep, double* x, double*y, double* z);

double interp2_linear(double A, double B, double C, double D,double dx,double dy);

int prem_p(double depth, double* vp, int flag);

int prem_s(double depth, double* vs, int flag);

double tx2011(double DEPTH);

int sac2xy_with_delta(char* sac_file, double begin_time, double length, double* out_array, double DELTA);

int prem_pac06(double depth, double* radius_out, double* density_out, double* Vpv_out,double* Vph_out, double* Vsv_out, double* Vsh_out, double* Qu_out,double* Qk_out, int flag);

int latlon_statics(char* file_name,double lat_delta, double lon_delta, char* out_name);

int convolution2(double* array1, int npts1, double* array2, double* array_out);

int t_star_find_uniq(double t_star,double* tstar_array, int npts, double delta,char* tstar_lib);

int convolution(double* array1, int npts1, double* array2,int npts2, double* array_out);

void cart2sph( double x, double y, double z, double* lat, double* lon, double* dep);



#endif
