#include<hongyulib.h>

/***********************************************************
 * This C function use trilinear interpolation algorithm to
 * interpolate at points within a cubic. The cubic is set at:
 *
 * upper pannel
 * ( 0 , 1 , 1 ) ( 1 , 1 , 1 )
 * ( 0 , 0 , 1 ) ( 1 , 0 , 1 ) 
 *lower pannel
 * ( 0 , 1 , 0 ) ( 1 , 1 , 0 )
 * ( 0 , 0 , 0 ) ( 1 , 0 , 0 ) 
 *
 * double *p    ----  Input cubic value, length is 8.
 * double *dx   ----  New position dx.
 * double *dy   ----  New position dy.
 * double *dz   ----  New position dz.
 *
 * Shule Yu
 * May 24 2014
 *
 * Key words: interpolate, 3D, 3d, linear.
 *
 * Reference:
 *     http://en.wikipedia.org/wiki/Trilinear_interpolation
***********************************************************/

double interp3_linear(double *p,double dx,double dy,double dz){
	double result;
    double c00,c10,c01,c11;
    double c0,c1;

    if ( dx<0 || dx>1 || dy<0 || dy>1 || dz<0 || dz>1 ){
        printf("In interp3_linear: Point outside of cubic !\n");
        exit(1);
    }

    // Step1. Calculate value at ( dx , 0 , 0 ) ( dx , 1 , 0 ) ( dx , 0 , 1 ) ( dx , 1 , 1 ).
    c00=p[0]*(1-dx)+p[1]*dx;
    c10=p[2]*(1-dx)+p[3]*dx;
    c01=p[4]*(1-dx)+p[5]*dx;
    c11=p[6]*(1-dx)+p[7]*dx;

    // Step2. Calculate value at ( dx , dy , 0 ) ( dx , dy , 1 ).
    c0=c00*(1-dy)+c10*dy;
    c1=c01*(1-dy)+c11*dy;
    // Step3. Calculate value at ( dx, dy, dz ).
	result = c0*(1-dz)+c1*dz;

	return result;

}
