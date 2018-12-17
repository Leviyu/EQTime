#include "hongyulib.h"
/******************************************************************
 * This is a c script to read in sac file and return a xy array
 * we manually convert the sampling rate to 0.025 here
 *
 *	Input: 
 *	sac_file
 *	double 					begin_time			relative to phase
 *	double 					length				the length we want in sec
 *	
 *
 *	Output:
 *	double*					out_array				the y array we want
 *
 *
 *
 *	DATE:				Keywords:
 *	Reference:
******************************************************************/
int sac2xy_relative_to_phase( double EQ_lat, double EQ_lon, double EQ_dep, double STA_lat, double STA_lon, char* phase, char* sac_file, double beg_to_phase, double length, double* out_array)
{

	int sac2xy(char* sac_file, double begin_time, double length, double* out_array);
	double taup_time(double EQ_lat, double EQ_lon, double EQ_dep, double STA_lat, double STA_lon, char* phase);



	double prem_time = taup_time(EQ_lat, EQ_lon,EQ_dep, STA_lat, STA_lon, phase);
	double abs_beg = beg_to_phase + prem_time;

	sac2xy(sac_file, abs_beg, length, out_array);


	return 0;
}

