#include "hongyulibcpp.h"
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
