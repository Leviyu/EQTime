#include "hongyulib.h"
/******************************************************************
 * This is a c script to convert the cartisian to spherical 
 *
 *	Input: lat, lon, dep
 *		lat lon is in degree, depth is in km
 *
 *	Output: x,y,z
 *     
 *     x: positive zero defined as X_origin_dg deg 
 *     y: positive zero defined as 90 deg perpendicular to x in equator plane, counter-clockwise
 *     z: positive zero defined as north pole
 *
 *	DATE:Feb 08 2015		Keywords: sph2cart
******************************************************************/
#define PI 3.14159265
#define X_origin_deg 0

void cart2sph( double x, double y, double z, double* lat, double* lon, double* dep)
{


	double radius = sqrt(x*x + y*y + z*z);
	*dep = 6371 - radius;


	if(z == 0)
	{
		*lat = 0;
	}
	else if(z > 0)
	{
		double phi = atan( sqrt(x*x + y*y) / z  );
		phi = phi / PI * 180;
		*lat = 90 - phi;
	}
	else 
	{
		double phi = atan( sqrt(x*x + y*y) / fabs(z)  );
		phi = phi / PI * 180;
		*lat =  phi - 90;
	}




	if(x == 0) 
	{
		if(y > 0 )
			*lon = 90;
		if(y < 0)
			*lon = -90;
	}
	else
	{
		double theta = atan2( y , x);
		theta = theta / PI * 180;
		if(theta > 180)
			theta = theta - 360;
		*lon = theta;
	}
}


