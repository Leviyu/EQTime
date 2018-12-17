#include "hongyulib.h"
/******************************************************************
 * This is a c script to generage gaussian function
 *
 *	the gaussian function is expressed as 
 *						(x-b)^2
 *	f(x) = a * exp(-1* ----------)+ d
 *						2*c^2	
 *	Input: 
 *	double a,b,c,d and x
 *	
 *	for a normalized standard Gaussian
 *	a = 1
 *	b = 0
 *	c determines the shape
 *	d = 0
 *
 *	Output:
 *
 *
 *	DATE:				Keywords:
 *	Reference:
******************************************************************/

double gaussian_func(double a, double b, double c, double d, double x)
{
	double fx;

	fx = a* exp(-1* (x-b)*(x-b) / (2*c*c) ) + d;


	return fx;
}
