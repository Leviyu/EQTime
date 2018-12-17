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
#define X_origin_deg 0

void sph2cart(double lat, double lon, double dep, double* x, double* y, double* z)
{
	double radius = 6371 - dep;
	double theta = lon - X_origin_deg;
	double gama = (90 - lat);


	theta = theta/180*PI;
	gama = gama/180*PI;

	*x = radius*sin(gama)*cos(theta);
	*y = radius*sin(gama)*sin(theta);
	*z = radius*cos(gama);


}


