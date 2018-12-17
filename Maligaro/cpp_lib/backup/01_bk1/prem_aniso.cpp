#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "hongyulib.h"

/*************************************************************
* This C function calculate reference model PREM_500
*	
*	depth: depth read in
*	*radius_out: 
*	*density
*	*Vp
*	*Vs
*	flag: = 0 use isotropic result
*		  = 1 use anisotropic result
*	NOTE: ocean part is ignored 
*
*	Hongyu DATE: June 20 2014
*	Key words: prem vs vp out 
*************************************************************/

int prem_aniso(double depth, double* radius_out, double* density_out, double* Vpv_out,double* Vph_out,
double* Vsv_out, double* Vsh_out, double* Qu_out,double* Qk_out, int flag)
{
	double x = 0;			//normalized radius
	double radius = 0;
	double Vs = 0;			//velocity of S wave without anisotropy
	double Vp = 0;	 		//velocity of P wave without anisotropy

	double density = 0;
	double Qk = 0;
	double Qu = 0;

	double Vph,Vpv,Vsh,Vsv;
	double a = 6371;		// radius of earth
	double eta = 0;			// eta


	double ORIG_DATA[100000] = 
	{
0,13.0885,11.2182,3.59469,1328.0,85.0,11.2182,3.59469,1.0,
100,13.0863,11.2166,3.59362,1328.0,85.0,11.2166,3.59362,1.0,
200,13.0798,11.212,3.5904,1328.0,85.0,11.212,3.5904,1.0,
300,13.0689,11.2042,3.58503,1328.0,85.0,11.2042,3.58503,1.0,
400,13.0536,11.1933,3.57751,1328.0,85.0,11.1933,3.57751,1.0,
500,13.034,11.1794,3.56785,1328.0,85.0,11.1794,3.56785,1.0,
600,13.0101,11.1623,3.55603,1328.0,85.0,11.1623,3.55603,1.0,
700,12.9818,11.1421,3.54207,1328.0,85.0,11.1423,3.54207,1.0,
800,12.9491,11.1188,3.52597,1328.0,85.0,11.1188,3.52597,1.0,
900,12.9121,11.0924,3.50771,1328.0,85.0,11.0924,3.50771,1.0,
1000,12.8707,11.0629,3.48731,1328.0,85.0,11.0629,3.48731,1.0,
1100,12.825,11.0303,3.46475,1328.0,85.0,11.0303,3.46475,1.0,
1200,12.7749,10.9946,3.44005,1328.0,85.0,10.9946,3.44005,1.0,
1221.5,12.7636,10.9866,3.43447,1328.0,85.0,10.9866,3.43447,1.0,
1221.5,12.1663,10.3554,0,57822.0,0.0,10.3554,0,1.0,
1300,12.125,10.3094,0,57822.0,0.0,10.3094,0,1.0,
1400,12.0692,10.2493,0,57822.0,0.0,10.2493,0,1.0,
1500,12.0099,10.1871,0,57822.0,0.0,10.1871,0,1.0,
1600,11.9468,10.1226,0,57822.0,0.0,10.1226,0,1.0,
1700,11.8799,10.0554,0,57822.0,0.0,10.0554,0,1.0,
1800,11.809,9.98525,0,57822.0,0.0,9.98525,0,1.0,
1900,11.734,9.91177,0,57822.0,0.0,9.91177,0,1.0,
2000,11.6548,9.83467,0,57822.0,0.0,9.83467,0,1.0,
2100,11.5712,9.75364,0,57822.0,0.0,9.75364,0,1.0,
2200,11.4831,9.66837,0,57822.0,0.0,9.66837,0,1.0,
2300,11.3904,9.57853,0,57822.0,0.0,9.57853,0,1.0,
2400,11.293,9.48381,0,57822.0,0.0,9.48381,0,1.0,
2500,11.1907,9.3839,0,57822.0,0.0,9.3839,0,1.0,
2600,11.0834,9.27849,0,57822.0,0.0,9.27849,0,1.0,
2700,10.9709,9.16726,0,57822.0,0.0,9.16726,0,1.0,
2800,10.8532,9.04988,0,57822.0,0.0,9.04988,0,1.0,
2900,10.7301,8.92606,0,57822.0,0.0,8.92606,0,1.0,
3000,10.6015,8.79547,0,57822.0,0.0,8.79547,0,1.0,
3100,10.4673,8.6578,0,57822.0,0.0,8.6578,0,1.0,
3200,10.3273,8.51273,0,57822.0,0.0,8.51273,0,1.0,
3300,10.1813,8.35995,0,57822.0,0.0,8.35995,0,1.0,
3400,10.0294,8.19915,0,57822.0,0.0,8.19915,0,1.0,
3480,9.90349,8.06458,0,57822.0,0.0,8.06458,0,1.0,
3480,5.56645,13.6886,7.22538,57822.0,312.0,13.6886,7.22538,1.0,
3500,5.55641,13.6837,7.22559,57822.0,312.0,13.6837,7.22559,1.0,
3600,5.50642,13.6595,7.22647,57822.0,312.0,13.6595,7.22647,1.0,
3630,5.49145,13.6523,7.2267,57822.0,312.0,13.6523,7.2267,1.0,
3630,5.49145,13.6523,7.2267,57822.0,312.0,13.6523,7.2267,1.0,
3700,5.45657,13.568,7.19492,57822.0,312.0,13.568,7.19492,1.0,
3800,5.40681,13.4495,7.15006,57822.0,312.0,13.4495,7.15006,1.0,
3900,5.35706,13.333,7.10561,57822.0,312.0,13.333,7.10561,1.0,
4000,5.30724,13.2176,7.06136,57822.0,312.0,13.2176,7.06136,1.0,
4100,5.25729,13.103,7.01711,57822.0,312.0,13.103,7.01711,1.0,
4200,5.20713,12.9884,6.97264,57822.0,312.0,12.9884,6.97264,1.0,
4300,5.15669,12.8731,6.92772,57822.0,312.0,12.8731,6.92772,1.0,
4400,5.1059,12.7567,6.88216,57822.0,312.0,12.7567,6.88216,1.0,
4500,5.05469,12.6384,6.83574,57822.0,312.0,12.6384,6.83574,1.0,
4600,5.00299,12.5177,6.78823,57822.0,312.0,12.5177,6.78823,1.0,
4700,4.95073,12.3939,6.73943,57822.0,312.0,12.3939,6.73943,1.0,
4800,4.89783,12.2664,6.68912,57822.0,312.0,12.2664,6.68912,1.0,
4900,4.84422,12.1347,6.6371,57822.0,312.0,12.1347,6.6371,1.0,
5000,4.78983,11.998,6.58313,57822.0,312.0,11.998,6.58313,1.0,
5100,4.7346,11.8558,6.52702,57822.0,312.0,11.8558,6.52702,1.0,
5200,4.67844,11.7074,6.46855,57822.0,312.0,11.7074,6.46855,1.0,
5300,4.62129,11.5523,6.40749,57822.0,312.0,11.5523,6.40749,1.0,
5400,4.56307,11.3897,6.34365,57822.0,312.0,11.3897,6.34365,1.0,
5500,4.50372,11.2192,6.2768,57822.0,312.0,11.2192,6.2768,1.0,
5600,4.44317,11.04,6.20673,57822.0,312.0,11.04,6.20673,1.0,
5600,4.44316,11.04,6.20672,57822.0,312.0,11.04,6.20672,1.0,
5650,4.41241,10.8853,6.06124,57822.0,312.0,10.8853,6.06124,1.0,
5701,4.38071,10.7274,5.91294,57822.0,312.0,10.7274,5.91294,1.0,
5701,3.99214,10.2185,5.50452,57822.0,143.0,10.2185,5.50452,1.0,
5736,3.98399,10.1645,5.47775,57822.0,143.0,10.1645,5.47775,1.0,
5771,3.97584,10.1105,5.45097,57822.0,143.0,10.1105,5.45097,1.0,
5771,3.97584,10.1105,5.45097,57822.0,143.0,10.1105,5.45097,1.0,
5821,3.91282,9.85588,5.30682,57822.0,143.0,9.85588,5.30682,1.0,
5871,3.8498,9.60122,5.16268,57822.0,143.0,9.60122,5.16268,1.0,
5921,3.78678,9.34655,5.01854,57822.0,143.0,9.34655,5.01854,1.0,
5971,3.72378,9.09193,4.87442,57822.0,143.0,9.09193,4.87442,1.0,
5971,3.54325,8.86489,4.71364,57822.0,143.0,8.86489,4.71364,1.0,
6016,3.51639,8.77848,4.68252,57822.0,143.0,8.77848,4.68252,1.0,
6061,3.48951,8.69204,4.65139,57822.0,143.0,8.69204,4.65139,1.0,
6106,3.46264,8.60561,4.62027,57822.0,143.0,8.60561,4.62027,1.0,
6151,3.43578,8.5192,4.58914,57822.0,143.0,8.5192,4.58914,1.0,
6151,3.3595,7.7293,4.34748,57822.0,80.0,7.97975,4.34277,0.97654,
6171,3.36167,7.7523,4.34297,57822.0,80.0,7.9938,4.36033,0.96877,
6191,3.36384,7.77531,4.33846,57822.0,80.0,8.00786,4.3779,0.96099,
6211,3.36602,7.79831,4.33395,57822.0,80.0,8.02192,4.39547,0.95321,
6231,3.36819,7.82132,4.32943,57822.0,80.0,8.03598,4.41304,0.94543,
6251,3.37036,7.84432,4.32492,57822.0,80.0,8.05004,4.43061,0.93765,
6271,3.37254,7.86732,4.32041,57822.0,80.0,8.0641,4.44818,0.92987,
6291,3.37471,7.89031,4.3159,57822.0,80.0,8.07815,4.46574,0.92210,
6291,3.37471,7.94982,4.39645,57822.0,600.0,8.14037,4.54911,0.92209,
6311,3.37688,7.97251,4.39186,57822.0,600.0,8.1548,4.567,0.91432,
6331,3.37906,7.99522,4.38726,57822.0,600.0,8.16924,4.5849,0.90654,
6346.6,3.38076,8.01295,4.38368,57822.0,600.0,8.18051,4.59887,0.90047,
6346.6,2.9,6.79151,3.88904,57822.0,600.0,6.79151,3.88904,1.00000,
6356,2.9,6.79151,3.88904,57822.0,600.0,6.79151,3.88904,1.00000,
6356,2.6,5.79328,3.19101,57822.0,600.0,5.79328,3.19101,1.00000,
6368,2.6,5.79328,3.19101,57822.0,600.0,5.79328,3.19101,1.00000,
6368,1.02,1.44996,0,57822.0,3.19,1.44996,3.19,1.00000,
6371,1.02,1.44996,0,57822.0,3.19,1.44996,3.19,1.00000,
	};

//6368,1.02,1.44996,0,57822.0,0.,1.44996,0,1.00000,
//6371,1.02,1.44996,0,57822.0,0.,1.44996,0,1.00000,
	int LINE = 97;


	double old_radius,old_vsv, old_vsh, old_density, old_vpv, old_vph, old_Qk, old_Qm, old_eta;
	// 1. radius
	// 2.  density
	// 3. vpv
	// 4. vsv
	// 5. Qk
	// 6. Qm
	// 7. vph
	// 8. vsh
	// 9. eta

if(depth <0 || depth >= 2891 )
	cout << "ERROR prem_500 velocity error !"<< endl;
	int count;
	// loop through to find the 
	for(count = 0; count < LINE ;count ++)
	{
		if( count == 0 )
		{
			old_radius = ORIG_DATA[ 0+ 9*count];
			old_density= ORIG_DATA[ 1+ 9*count];
			old_vpv = ORIG_DATA[ 2+ 9*count];
			old_vsv = ORIG_DATA[ 3+ 9*count];
			old_Qk = ORIG_DATA[ 4+ 9*count];
			old_Qm = ORIG_DATA[ 5+ 9*count];
			old_vph = ORIG_DATA[ 6+ 9*count];
			old_vsh = ORIG_DATA[ 7+ 9*count];
			old_eta = ORIG_DATA[ 8+ 9*count];
		}


		double current_radius = 6371 - depth;
		if(   current_radius <= ORIG_DATA[0 + 9*count] )
		{
			density = old_density + ( ORIG_DATA[1+9*count] - old_density ) / (ORIG_DATA[0 + 9*count] - old_radius) *(current_radius - old_radius);
			Vpv = old_vpv+ ( ORIG_DATA[2+9*count] - old_vpv ) / (ORIG_DATA[0 + 9*count] - old_radius) *(current_radius - old_radius);
			Vsv = old_vsv+ ( ORIG_DATA[3+9*count] - old_vsv ) / (ORIG_DATA[0 + 9*count] - old_radius) *(current_radius - old_radius);
			Qk = old_Qk+ ( ORIG_DATA[4+9*count] - old_Qk ) / (ORIG_DATA[0 + 9*count] - old_radius) *(current_radius - old_radius);
			Qu = old_Qm+ ( ORIG_DATA[5+9*count] - old_Qm ) / (ORIG_DATA[0 + 9*count] - old_radius) *(current_radius - old_radius);
			Vph = old_vph+ ( ORIG_DATA[6+9*count] - old_vph ) / (ORIG_DATA[0 + 9*count] - old_radius) *(current_radius - old_radius);
			Vsh = old_vsh+ ( ORIG_DATA[7+9*count] - old_vsh ) / (ORIG_DATA[0 + 9*count] - old_radius) *(current_radius - old_radius);
			break;

		}
		// store information
			old_radius = ORIG_DATA[ 0+ 9*count];
			old_density= ORIG_DATA[ 1+ 9*count];
			old_vpv = ORIG_DATA[ 2+ 9*count];
			old_vsv = ORIG_DATA[ 3+ 9*count];
			old_Qk = ORIG_DATA[ 4+ 9*count];
			old_Qm = ORIG_DATA[ 5+ 9*count];
			old_vph = ORIG_DATA[ 6+ 9*count];
			old_vsh = ORIG_DATA[ 7+ 9*count];
			old_eta = ORIG_DATA[ 8+ 9*count];

	}


	//output
	*radius_out = radius;
	*density_out = density;
	*Vpv_out = Vpv;
	*Vsv_out = Vsv;
	*Vph_out = Vph;
	*Vsh_out = Vsh;
	*Qu_out = Qu;
	*Qk_out = Qk;

	return 0;
}






