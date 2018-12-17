#include<hongyulib.h>

/*************************************************************
* This C function is a 1D reference earth model for TX2011 tomography
*	INPUT
*		double depth;
*
*	OUTPUT
*		double*	vs
*
*	Hongyu DATE: June 20 2014
*	Key words: prem vs vp out 
*************************************************************/

double reference_LLNL(double DEPTH)
{
	//printf("input depth is %lf \n",DEPTH);
	if(DEPTH <0 || DEPTH > 6370)
		puts("ERROR in TX2011 ref, depth is negative or is greater then 6370km");

	double result;

	int line_num = 41;
	//read in reference 
	double depth[500] = {
0,
80,
115,
150,
185,
220,
265,
310,
355,
410,
450,
500,
550,
600,
635,
660,
721,
771,
871,
971,
1071,
1171,
1271,
1371,
1471,
1571,
1671,
1771,
1871,
1971,
2071,
2171,
2271,
2371,
2471,
2571,
2671,
2741,
2771,
2871,
2891
	};

	double vs[500] = {
3.2,
4.47822,
4.46909,
4.4545,
4.45468,
4.5816,
4.68261,
4.71424,
4.74045,
4.93808,
5.07616,
5.22083,
5.363,
5.50537,
5.53107,
5.82747,
6.08097,
6.22689,
6.29768,
6.36628,
6.43242,
6.49394,
6.55343,
6.61127,
6.66472,
6.717,
6.76617,
6.81436,
6.86212,
6.91058,
6.95355,
6.99987,
7.04128,
7.08397,
7.13106,
7.17435,
7.21547,
7.24654,
7.24634,
7.24546,
7.24526
	};


	int index_min = 0;
	int index_max = 0;

	for(index_max = 0; index_max < line_num; index_max ++)
	{
		if(depth[index_max] > DEPTH)
			break;
	}

	index_min = index_max -1;



	double vs_min = vs[index_min];
	double vs_max = vs[index_max];

	result = (DEPTH - depth[index_min])*(vs_max- vs_min)/(depth[index_max]-depth[index_min]) + vs[index_min];

	//printf("depth is %lf  dep minmax is %lf %lf vs is %lf \n", DEPTH ,depth[index_max], depth[index_min], result);

	return result;
}
