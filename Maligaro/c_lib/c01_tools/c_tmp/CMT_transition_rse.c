#include<hongyulib.h>


/*************************************************************
*	This c function translate the 
*	strike dip rake 
*	into 
*
*	The xyz coornidate is 
*	x point to north
*	y point to east
*	z point to downward
*
*
* 	to translate the xyz coornate to rse coornidate
*
* 	r point tp up
* 	s point to south
* 	e point to east
*
*	6 parameter CMT
*************************************************************/

#define PI 3.1415926
int CMT_transition_rse( double strike, double dip, double rake, double* Mrr, double* Mss, double* Mee, double* Mrs, double* Mre, double* Mse)
{

	double phi 		= strike / 180 * PI;
	double lambda 	=  rake / 180 * PI;
	double delta = dip / 180 * PI;
	
	double M0 = 1;

	double Mxx,Myy, Mzz, Mxy, Mxz, Myz;


	Mxx = -M0 * (sin(delta)*cos(lambda)*sin(2*phi) + sin(2*delta)*sin(lambda)*sin(phi)*sin(phi)  );
	Myy = M0  * (sin(delta)*cos(lambda)*sin(2*phi) - sin(2*delta)*sin(lambda)*cos(phi)*cos(phi)  );
	Mzz = M0  * sin(2*delta)*sin(lambda);

	Mxy = M0  * (sin(delta)*cos(lambda)*cos(2*phi) + 0.5*sin(2*delta)*sin(lambda)*sin(2*phi) );
	Mxz = -M0 * (cos(delta)*cos(lambda)*cos(phi)   +  cos(2*delta)*sin(lambda)*sin(phi)  );
	Myz = -M0 * (cos(delta)*cos(lambda)*sin(phi) - cos(2*delta)*sin(lambda)*cos(phi) );

	*Mrr = Mzz;
	*Mss = Mxx;
	*Mee = Myy;
	*Mrs = Mxz;
	*Mre = Myz;
	*Mse = Mxy;



		 return 0;
}
