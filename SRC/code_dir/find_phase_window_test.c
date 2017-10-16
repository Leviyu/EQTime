#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<hongyulib.h>

#define DELTA 0.1

int main()
{

	double EQ_lat;
	double EQ_lon;
	double EQ_dep;
	double STA_lat;
	double STA_lon ;
	char PHASE[5] ;
	char sac_file[50];
	double beg_to_phase;
	double length;
	double out_array[200000];
	char out_file[200] = "sac2xy.out";


	//1. read station info and sac file name
	scanf(" %lf %lf %lf %lf %lf %s %s %lf %lf ", &EQ_lat, &EQ_lon, &EQ_dep,&STA_lat, &STA_lon, PHASE, sac_file, &beg_to_phase, &length);


	// get the taup_time 
	double taup_time(double EQ_lat, double EQ_lon, double EQ_dep, double STA_lat, double STA_lon, char* phase);
	double PREM = taup_time(EQ_lat,EQ_lon, EQ_dep, STA_lat, STA_lon,PHASE);
	printf("PREM time for phase %s is %lf \n", PHASE, PREM);

	// find the wanted window relative the phase and output it to sac2xy.out
	int sac2xy_relative_to_phase( double EQ_lat, double EQ_lon, double EQ_dep, double STA_lat, double STA_lon, char* phase, char* sac_file, double beg_to_phase, double length, double* out_array);
	sac2xy_relative_to_phase(EQ_lat, EQ_lon, EQ_dep, STA_lat, STA_lon, PHASE, sac_file, beg_to_phase, length, out_array);


	int npts_out = (int)(length / DELTA);
	double x_out[npts_out];
	int count;
	for(count = 0; count< npts_out; count++)
	{
		x_out[count] = beg_to_phase + count * DELTA;
	}

	output_array2(out_file, x_out,out_array,npts_out,0);





	return 0;
}

