

#include "hongyulib.h"
/* Calculate the distance between two poitns
 * InPUT: lat_A, lon_A, lat_B, lon_B
 *
 * OUTPUT: distance in km
 */

double dist_A_B(double A_lat, double A_lon, double B_lat, double B_lon) 
{
	double dlong,dlat;
	double pi = 3.1415926;
	//read in INPUT:
	// go into radians
	A_lat = A_lat * pi/180.0; 	
	B_lat = B_lat * pi/180.0; 	
	A_lon = A_lon * pi/180.0; 	
	B_lon = B_lon * pi/180.0; 	
	
	dlat= B_lat - A_lat;
	dlong = B_lon - A_lon;
	double a,c,d;


	a = sin(dlat/2)* sin(dlat/2)+ cos(A_lat)*cos(B_lat)*sin(dlong/2)*sin(dlong/2);	
	c=2*atan2(sqrt(a),sqrt(1-a));
	d=6371*c;
	return d;
}


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

