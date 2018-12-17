#include "hongyulib.h"
/*************************************************************
* This C function returns azimuth from A to B
*	
*	
*	Hongyu DATE: 
*	Key words: 
*************************************************************/
double az_A_B(double A_lat, double A_lon, double B_lat, double B_lon)
{
	double pi = 3.1415926;
	double R = 6371;
	A_lat = A_lat *pi/180;
	B_lat = B_lat *pi/180;
	A_lon = A_lon *pi/180;
	B_lon = B_lon *pi/180;


	double AZ = atan2(sin(B_lon - A_lon)*cos(B_lat), cos(A_lat)*sin(B_lat)-sin(A_lat)*cos(B_lat)*cos(B_lon-A_lon) );
	//double AZ = atan2(cos(A_lat)*sin(B_lat)-sin(A_lat)*cos(B_lat)*cos(B_lon-A_lon), sin(B_lon-A_lon)*cos(B_lat));
	AZ = AZ /pi *180;


	return AZ;
}

