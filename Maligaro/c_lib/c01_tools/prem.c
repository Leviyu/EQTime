#include "hongyulib.h"
/*************************************************************
* This C function read in the depth and output the prem parameter
*	
*	depth: depth read in
*	*radius_out: 
*	*density
*	*Vp
*	*Vs
*	flag: = 0 use isotropic result
*		  = 1 use anisotropic result
*	NOTE: ocean part is ignored in this prem model
*
*	Hongyu DATE: June 20 2014
*	Key words: prem vs vp out 
*************************************************************/

int prem(double depth, double* radius_out, double* density_out, double* Vpv_out,double* Vph_out,
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

	radius = 6371.0 - depth;	
	//normalized radius
	x = radius / a;

	//for inner core
	if(depth <= 6371 && depth > 5149.5)
	{
		density = 13.0885 - 8.8381*x*x;
		Vp = 11.2622 - 6.3640*x*x;
		Vs = 3.6678 - 4.4475*x*x;
		Vsv = Vs;
		Vsh = Vs;
		Vpv = Vp;
		Vph = Vp;
		Qu = 84.6;
		Qk = 1327.7;
		
	}
	
	//for outer core
	if(depth <= 5149.5 && depth > 2891 )
	{
		density = 12.5815-1.2638*x - 3.6426*x*x - 5.5281*x*x*x;
		Vp = 11.0487 - 4.0362*x + 4.8023*x*x - 13.5732*x*x*x;
		Vs = 0;
		Vsv = Vs;
		Vsh = Vs;
		Vpv = Vp;
		Vph = Vp;
		Qu = 10000;
		Qk = 57823;
	}

	//for lower mantle 
	if(depth <= 2891 && depth > 2741 )
	{
		density = 7.9565 - 6.4761*x + 5.5283*x*x - 3.0807*x*x*x;
		Vp = 15.3891 - 5.3181*x + 5.5242*x*x - 2.5514*x*x*x;
		Vs = 6.9254 + 1.4672*x - 2.0834*x*x + 0.9783*x*x*x;
		Vsv = Vs;
		Vsh = Vs;
		Vpv = Vp;
		Vph = Vp;
		Qu = 312;
		Qk = 57823;
	}

	if( depth <= 2741 && depth > 771 )
	{
		density = 7.9565 - 6.4761*x + 5.5283*x*x - 3.0807*x*x*x;
		Vp = 24.9520 - 40.4673*x + 51.4832*x*x - 26.6419*x*x*x;
		Vs = 11.1671 - 13.7818*x + 17.4575*x*x - 9.2777*x*x*x;
		Vsv = Vs;
		Vsh = Vs;
		Vpv = Vp;
		Vph = Vp;
		Qu = 312;
		Qk = 57823;
	}


	if( depth <= 771 && depth > 670 )
	{
		density = 7.9565 - 6.4761*x + 5.5283*x*x - 3.0807*x*x*x;
		Vp = 29.2766 - 23.6027*x + 5.5242*x*x - 2.5514*x*x*x;
		Vs = 22.3459 - 17.2473*x - 2.0834*x*x + 0.9783*x*x*x;
		Vsv = Vs;
		Vsh = Vs;
		Vpv = Vp;
		Vph = Vp;
		Qu = 312;
		Qk = 57823;
	}

	//for transition
	if( depth <= 670 && depth > 600 )
	{
		density = 5.3197 - 1.4836*x;
		Vp = 19.0957 - 9.8672*x;
		Vs = 9.9839 - 4.9324*x;
		Vsv = Vs;
		Vsh = Vs;
		Vpv = Vp;
		Vph = Vp;
		Qu = 143;
		Qk = 57823;
	}


	if( depth <= 600 && depth > 400 )
	{
		density = 11.2494 - 8.0298*x;
		Vp = 39.7027 - 32.6166*x;
		Vs = 22.3512 - 18.5856*x;
		Vsv = Vs;
		Vsh = Vs;
		Vpv = Vp;
		Vph = Vp;
		Qu = 143;
		Qk = 57823;
	}


	if( depth <= 400 && depth > 220 )
	{
		density = 7.1089 - 3.8045*x;
		Vp = 20.3926 - 12.2569*x;
		Vs = 8.9496 - 4.4597*x;
		Vsv = Vs;
		Vsh = Vs;
		Vpv = Vp;
		Vph = Vp;
		Qu = 143;
		Qk = 57823;
	}

	//printf("flag in code is %d \n", flag);
	// determine whether using isotropic parameter
	if( flag == 0 ) // isotropic
	{
		if(  depth > 24.4 && depth <= 220 )
		{
			Vpv = 4.1875 + 3.9382*x;
			Vph = Vpv;
			Vsv = 2.1519 + 2.3481*x;
			Vsh = Vsv; 
		}
		// for density and Qu Qk
		if( depth <=220 && depth > 80 )
		{
			density = 2.6910 + 0.6924*x;
			Qu = 80;
			Qk = 57823;
			eta = 3.3687 - 2.4778*x;
		}

		// for LID
		if( depth <= 80 && depth > 24.4 )
		{
			density = 2.6910 + 0.6924*x;
			Qu = 600 ;
			Qk = 57823;
			eta = 3.3687 - 2.4778*x;
		}
	}
	else if (flag == 1 ) //anisotropic
	{
		//for LVZ
		if( depth > 80 && depth <= 220 )
		{
			density = 2.6910 + 0.6924*x;

			Vpv = 0.8317 + 7.2180*x;
			Vph = 3.5908 + 4.6172*x;

			Vsv = 5.8582 - 1.4678*x;
			Vsh = -1.0839 + 5.7176*x;

			Qu = 80;
			Qk = 57823;
			eta = 3.3687 - 2.4778*x;
		}

		// for LID
		if( depth > 24.4 && depth <= 80 )
		{
			density = 2.6910 + 0.6924*x;
			Vpv = 0.8317 + 7.2180*x;
			Vph = 3.5908 + 4.6172*x;

			Vsv = 5.8582 - 1.4678*x;
			Vsh = -1.0839 + 5.7176*x;

			Qu = 600 ;
			Qk = 57823;
			eta = 3.3687 - 2.4778*x;
		}
	}


	//for crust
	if( depth > 15  && depth <= 24.4 )
	{
		density = 2.9;
		Vp = 6.8;
		Vs = 3.9;
		Vsv = Vs;
		Vsh = Vs;
		Vpv = Vp;
		Vph = Vp;
		Qu = 600;
		Qk = 57823;
	}

//ignore ocean
	if( depth >=0 && depth <= 15 )
	{
		density = 2.6;
		Vp = 5.8;
		Vs = 3.2;
		Vsv = Vs;
		Vsh = Vs;
		Vpv = Vp;
		Vph = Vp;
		Qu = 600;
		Qk = 57823;
	}


/*	// for ocean
	if(radius > 6368 && radius <= 6371)
	{
		density = 1.02;
		Vp = 1.45;
		Vs = 0;
		Qu = 100000;
		Qk = 57824;
	}
*/



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






int prem_s(double depth, double* vs, int flag)
{
	int prem(double depth, double* radius_out, double* density_out, double* Vpv_out,double* Vph_out, double* Vsv_out, double* Vsh_out, double* Qu_out,double* Qk_out, int flag);
	double radius, density, vpv,vph,vsv,vsh,Qu,Qk;

	//printf("flag is %d  should be 0\n", flag);
	prem(depth,&radius, &density, &vpv, &vph,&vsv,&vsh, &Qu, &Qk,flag);
	*vs = vsh;
	return 0;

}
int prem_p(double depth, double* vp, int flag)
{
	int prem(double depth, double* radius_out, double* density_out, double* Vpv_out,double* Vph_out, double* Vsv_out, double* Vsh_out, double* Qu_out,double* Qk_out, int flag);
	double radius, density, vpv,vph,vsv,vsh,Qu,Qk;

	prem(depth,&radius, &density, &vpv, &vph,&vsv,&vsh, &Qu, &Qk,flag);
	*vp = vph;
	return 0;
}





/*************************************************************
* This C function read in the depth and output the prem parameter
*	
*	depth: depth read in
*	*radius_out: 
*	*density
*	*Vp
*	*Vs
*	flag: = 0 use isotropic result
*		  = 1 use anisotropic result
*	NOTE: ocean part is ignored in this prem model
*
*	Hongyu DATE: June 20 2014
*	Key words: prem vs vp out 
*************************************************************/

int prem_pac06(double depth, double* radius_out, double* density_out, double* Vpv_out,double* Vph_out,
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

	radius = 6371.0 - depth;	
	//normalized radius
	x = radius / a;

	//for inner core
	if(depth <= 6371 && depth > 5149.5)
	{
		density = 13.0885 - 8.8381*x*x;
		Vp = 11.2622 - 6.3640*x*x;
		Vs = 3.6678 - 4.4475*x*x;
		Vsv = Vs;
		Vsh = Vs;
		Vpv = Vp;
		Vph = Vp;
		Qu = 84.6;
		Qk = 1327.7;
		
	}
	
	//for outer core
	if(depth <= 5149.5 && depth > 2891 )
	{
		density = 12.5815-1.2638*x - 3.6426*x*x - 5.5281*x*x*x;
		Vp = 11.0487 - 4.0362*x + 4.8023*x*x - 13.5732*x*x*x;
		Vs = 0;
		Vsv = Vs;
		Vsh = Vs;
		Vpv = Vp;
		Vph = Vp;
		Qu = 10000;
		Qk = 57823;
	}

	//for lower mantle 
	if(depth <= 2891 && depth > 2741 )
	{
		density = 7.9565 - 6.4761*x + 5.5283*x*x - 3.0807*x*x*x;
		Vp = 15.3891 - 5.3181*x + 5.5242*x*x - 2.5514*x*x*x;
		Vs = 6.9254 + 1.4672*x - 2.0834*x*x + 0.9783*x*x*x;
		Vsv = Vs;
		Vsh = Vs;
		Vpv = Vp;
		Vph = Vp;
		Qu = 312;
		Qk = 57823;
	}

	if( depth <= 2741 && depth > 771 )
	{
		density = 7.9565 - 6.4761*x + 5.5283*x*x - 3.0807*x*x*x;
		Vp = 24.9520 - 40.4673*x + 51.4832*x*x - 26.6419*x*x*x;
		Vs = 11.1671 - 13.7818*x + 17.4575*x*x - 9.2777*x*x*x;
		Vsv = Vs;
		Vsh = Vs;
		Vpv = Vp;
		Vph = Vp;
		Qu = 312;
		Qk = 57823;
	}


	if( depth <= 771 && depth > 670 )
	{
		density = 7.9565 - 6.4761*x + 5.5283*x*x - 3.0807*x*x*x;
		Vp = 29.2766 - 23.6027*x + 5.5242*x*x - 2.5514*x*x*x;
		Vs = 22.3459 - 17.2473*x - 2.0834*x*x + 0.9783*x*x*x;
		Vsv = Vs;
		Vsh = Vs;
		Vpv = Vp;
		Vph = Vp;
		Qu = 312;
		Qk = 57823;
	}

	//for transition
	if( depth <= 670 && depth > 600 )
	{
		density = 5.3197 - 1.4836*x;
		Vp = 19.0957 - 9.8672*x;
		Vs = 9.9839 - 4.9324*x;
		Vsv = Vs;
		Vsh = Vs;
		Vpv = Vp;
		Vph = Vp;
		Qu = 143;
		Qk = 57823;
	}


	if( depth <= 600 && depth > 400 )
	{
		density = 11.2494 - 8.0298*x;
		Vp = 39.7027 - 32.6166*x;
		Vs = 22.3512 - 18.5856*x;
		Vsv = Vs;
		Vsh = Vs;
		Vpv = Vp;
		Vph = Vp;
		Qu = 143;
		Qk = 57823;
	}


	if( depth <= 400 && depth > 220 )
	{
		density = 7.1089 - 3.8045*x;
		Vp = 20.3926 - 12.2569*x;
		Vs = 8.9496 - 4.4597*x;
		Vsv = Vs;
		Vsh = Vs;
		Vpv = Vp;
		Vph = Vp;
		Qu = 143;
		Qk = 57823;
	}

	//printf("flag in code is %d \n", flag);
	// determine whether using isotropic parameter
	if( flag == 0 ) // isotropic
	{
		if(  depth > 24.4 && depth <= 220 )
		{
			Vpv = 4.1875 + 3.9382*x;
			Vph = Vpv;
			Vsv = 2.1519 + 2.3481*x;
			Vsh = Vsv; 
		}
		// for density and Qu Qk
		if( depth <=220 && depth > 80 )
		{
			density = 2.6910 + 0.6924*x;
			Qu = 80;
			Qk = 57823;
			eta = 3.3687 - 2.4778*x;
		}

		// for LID
		if( depth <= 80 && depth > 24.4 )
		{
			density = 2.6910 + 0.6924*x;
			Qu = 600 ;
			Qk = 57823;
			eta = 3.3687 - 2.4778*x;
		}
	}
	else if (flag == 1 ) //anisotropic
	{
		//for LVZ
		if( depth > 80 && depth <= 220 )
		{
			density = 2.6910 + 0.6924*x;

			Vpv = 0.8317 + 7.2180*x;
			Vph = 3.5908 + 4.6172*x;

			Vsv = 5.8582 - 1.4678*x;
			Vsh = -1.0839 + 5.7176*x;

			Qu = 80;
			Qk = 57823;
			eta = 3.3687 - 2.4778*x;
		}

		// for LID
		if( depth > 24.4 && depth <= 80 )
		{
			density = 2.6910 + 0.6924*x;
			Vpv = 0.8317 + 7.2180*x;
			Vph = 3.5908 + 4.6172*x;

			Vsv = 5.8582 - 1.4678*x;
			Vsh = -1.0839 + 5.7176*x;

			Qu = 600 ;
			Qk = 57823;
			eta = 3.3687 - 2.4778*x;
		}
	}


	//for crust
	if( depth > 15  && depth <= 24.4 )
	{
		density = 2.9;
		Vp = 6.8;
		Vs = 3.9;
		Vsv = Vs;
		Vsh = Vs;
		Vpv = Vp;
		Vph = Vp;
		Qu = 600;
		Qk = 57823;
	}

//ignore ocean
	if( depth >=0 && depth <= 15 )
	{
		density = 2.6;
		Vp = 5.8;
		Vs = 3.2;
		Vsv = Vs;
		Vsh = Vs;
		Vpv = Vp;
		Vph = Vp;
		Qu = 600;
		Qk = 57823;
	}


/*	// for ocean
	if(radius > 6368 && radius <= 6371)
	{
		density = 1.02;
		Vp = 1.45;
		Vs = 0;
		Qu = 100000;
		Qk = 57824;
	}
*/


	// overlay for MODEL PAC06
	if(depth >=0 && depth <= 0.2)
	{
		Vsh = 1.0;
		Vsv = 1.0;
		Qu = 20;
	}
	else if(depth > 0.2 && depth <= 6)
	{
		Vsh = 3.2;
		Vsv = 3.2;
		Qu = 20;
	}
	else if(depth > 6 && depth <= 66)
	{
		Vsh = 4.78;
		Vsv = 4.58;
		Qu = 160;
	}
	else if(depth > 66 && depth <= 163)
	{
		double kh = (4.34 - 4.51)/(163-66);
		double kv = (4.22 - 4.31)/(163-66);
		Vsh = 4.51 + kh*(depth - 66 );
		Vsv = 4.31 + kv*(depth - 66 );
		Qu = 100;
	}
	else if(depth > 163 && depth <= 303)
	{
		double kh = (4.64 - 4.34)/(303-163);
		double kv = (4.64 - 4.22)/(303-163);
		Vsh = 4.34 + kh*(depth - 163 );
		Vsv = 4.22 + kv*(depth - 163 );
		Qu = 120;
	}
	else if(depth > 303 && depth <= 406)
	{
		double kh = (4.86 - 4.64)/(406 - 303);
		double kv = (4.86 - 4.64)/(406 - 303);
		Vsh = 4.64 + kh*(depth - 303 );
		Vsv = 4.64 + kv*(depth - 303 );
		Qu = 160;
	}
	else if(depth > 406 && depth <= 516)
	{
		double kh = (5.22 - 5.02)/(516 - 406);
		double kv = (5.22 - 5.02)/(516 - 406);
		Vsh = 5.02 + kh*(depth - 406 );
		Vsv = 5.02 + kv*(depth - 406 );
		Qu = 180;
	}
	else if(depth > 516 && depth <= 651)
	{
		double kh = (5.54 - 5.28)/(651 - 516);
		double kv = (5.54 - 5.28)/(651 - 516);
		Vsh = 5.28 + kh*(depth - 516 );
		Vsv = 5.28 + kv*(depth - 516 );
		Qu = 180;
	}
	else if(depth > 651 && depth <= 736)
	{
		double kh = (6.22 - 6.01)/(736 - 651);
		double kv = (6.22 - 6.01)/(736 - 651);
		Vsh = 6.01 + kh*(depth - 651 );
		Vsv = 6.01 + kv*(depth - 651 );
		Qu = 310;
	}
	else if(depth > 736 && depth <= 900)
	{
		double kh = (6.33 - 6.22)/(900 - 736);
		double kv = (6.33 - 6.22)/(900 - 736);
		Vsh = 6.22 + kh*(depth - 736 );
		Vsv = 6.22 + kv*(depth - 736 );
		Qu = 310;
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






