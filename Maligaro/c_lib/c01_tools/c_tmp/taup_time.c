#include "hongyulib.h"
/******************************************************************
 * This is a c script to acquire the taup_time using c
 *
 *	Input: 
 *	EQ_lat
 *	EQ_lon
 *	EQ_dep
 *	STA_lat
 *	STA_lon
 *	
 *	char* phase
 *	
 *
 *	Output:
 *	PREM_time  			which is the PREM time for this phase
 *
 * NOTE: model is prem
 * 		 we only return the first travel_time
 * 		 for multi-path using taup_time in csh to acquire more info
 *
 *
 *	DATE:Oct 21 2014				Keywords: taup_time
******************************************************************/

double taup_time(double EQ_lat, double EQ_lon, double EQ_dep, double STA_lat, double STA_lon, char* phase)
{

	double PREM_time;
	char command[300];
	double shell_pipe(char* command);

	sprintf(command,"taup_time -mod prem -ph %s -evt %lf %lf -sta %lf %lf -h %lf | awk 'NR==6 {print $4}'", phase, EQ_lat, EQ_lon, STA_lat, STA_lon, EQ_dep);


	PREM_time = shell_pipe(command);

	return PREM_time;
}


