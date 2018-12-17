#include "hongyulib.h"


/*************************************************************
*	This c function translate the 
*	strike dip rake 
*	into 
*	6 parameter CMT
*************************************************************/

#define PI 3.1415926
int CMT_transition( double strike, double dip, double rake, double* Mxx, double* Myy, double* Mzz, double* Mxy, double* Mxz, double* Myz)
{


	double phi 		= strike / 180 * PI;
	double lambda 	=  rake / 180 * PI;
	double delta = dip / 180 * PI;
	
	double M0 = 1;



	*Mxx = -M0 * (sin(delta)*cos(lambda)*sin(2*phi) + sin(2*delta)*sin(lambda)*sin(phi)*sin(phi)  );
	*Myy = M0  * (sin(delta)*cos(lambda)*sin(2*phi) - sin(2*delta)*sin(lambda)*cos(phi)*cos(phi)  );
	*Mzz = M0  * sin(2*delta)*sin(lambda);

	*Mxy = M0  * (sin(delta)*cos(lambda)*cos(2*phi) + 0.5*sin(2*delta)*sin(lambda)*sin(2*phi) );
	*Mxz = -M0 * (cos(delta)*cos(lambda)*cos(phi)   +  cos(2*delta)*sin(lambda)*sin(phi)  );
	*Myz = -M0 * (cos(delta)*cos(lambda)*sin(phi) - cos(2*delta)*sin(lambda)*cos(phi) );




		 return 0;
}
