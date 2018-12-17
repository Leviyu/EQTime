#include "hongyulibcpp.h"
/*************************************************************
* This C function returns point B(lat ,lon)
*	
*	Input: A_lat, A_lon
*		   AZ, dist
*	
*	Hongyu DATE: 
*	Key words: 
*************************************************************/

double point_AZ_dist_point(double A_lat, double A_lon, double AZ, double dist, double* B_lat, double* B_lon)
{
	
	double dist_A_B(); 
	double R = 6371.0;   //km
	double pi = 3.1415926;
	//read in INPUT:

	double ang_dist;
	ang_dist = dist/R;	

	//turn AZ into radans
	AZ = AZ * pi/180.0;
	A_lat = A_lat * pi/180.0;
	A_lon = A_lon * pi/180.0;
	*B_lat = asin( sin(A_lat)*cos(ang_dist) + cos(A_lat)*sin(ang_dist)*cos(AZ) );
	*B_lon = A_lon + atan2( sin(AZ) * sin(ang_dist)*cos(A_lat), cos(ang_dist)- sin(A_lat)*sin(*B_lat) );

//	printf("The B point is lat: %lf lon: %lf", *B_lat, *B_lon);
	*B_lat = *B_lat / pi * 180.0;
	*B_lon = *B_lon / pi * 180.0;
	
	if( *B_lon > 180)
		*B_lon = *B_lon - 360;
	else if(*B_lon < -180 )
		*B_lon = *B_lon + 360; 

	return 0;
}
