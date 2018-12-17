#include "hongyulibcpp.h"


/***********************************************************
 * This C function use trilinear interpolation algorithm to
 * interpolate at points within a square. 
 *
 * D		C
 * A		B
 *	double A, B, C, D ----- value at 4 points
 * double dx   ----  New position dx.
 * double dy   ----  New position dy.
 *
 * Hongyu
 * Mar 15 2015
 *
 * Key words: interpolate, 2D, 2d, linear.
 *
***********************************************************/

double interp2_linear(double A, double B, double C, double D,double dx,double dy)
{
	double result;

    if ( dx<0 || dx>1 || dy<0 || dy>1 )
	{
        printf("In interp2_linear: Point outside of square !\n");
        exit(1);
    }

	// step 1
	double AB = A + (B-A)/1 * dx;
	double CD = D + (C-D)/1 * dx;

	//step 2
	result = AB + (CD - AB)/1 * dy;
	return result;

}
