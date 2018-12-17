#include "hongyulib.h"
/******************************************************************
 * This is a c script to convert the spherical coordinate to 
 * cartesian coordinate
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

void cart2sph(double x, double y, double z, double* lat, double* lon, double* dep )
{

	*lon = atan2(y,x) / PI * 180;
	*lat = atan2(z,sqrt(x*x + y*y)) / PI * 180;
	*dep = 6371 - sqrt(x * x + y * y + z * z);

}


