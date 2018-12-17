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

int sac2xy_relative_to_phase(double delta, double EQ_lat, double EQ_lon, double EQ_dep, double STA_lat, double STA_lon, 
		string phase, string sac_file, double beg_to_phase, double length, double* out_array)
{

	double prem_time ;
	prem_time = taup_time(EQ_lat, EQ_lon,EQ_dep, STA_lat, STA_lon, phase);
	printf("prem time is %lf  begin to phase %lf \n", prem_time, beg_to_phase);
	double abs_beg = beg_to_phase + prem_time;



	//sac2xy(delta ,sac_file, abs_beg, length, out_array);

	//int sac2xy_with_delta(string sac_file2, double begin_time, double length, double* out_array, double DELTA)
	//string message;
	//message = "reading fucking sac file " +sac_file + "fuck you all " ;
	//cout <<message << endl;

	int flag = sac2xy_with_delta(sac_file,abs_beg, length, out_array, delta);
	if( flag !=  0 )
		printf(" sac2xy_with_delta problem!");
	else
		printf(" sac2xy_with_delta success! ");

	return 0;
}


