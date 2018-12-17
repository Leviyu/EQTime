#include "hongyulibcpp.h"

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

double SEMUCB(double DEPTH)
{
	//printf("input depth is %lf \n",DEPTH);
	if(DEPTH <0 || DEPTH > 6370)
		puts("ERROR in TX2011 ref, depth is negative or is greater then 6370km");

	double result;

	int line_num = 31;
	//read in reference 
	double depth[500] = {
0,
100,
221,
300,
411,
500,
600,
661,
700,
800,
900,
1000,
1100,
1200,
1300,
1400,
1500,
1600,
1700,
1800,
1900,
2000,
2100,
2200,
2300,
2400,
2500,
2600,
2700,
2800,
2890
	};

	double vs[500] = {
3.2,
4.5309,
4.58438,
4.65853,
5.03347,
5.20782,
5.41421,
5.93628,
6.03853,
6.25009,
6.3158,
6.37507,
6.43954,
6.50623,
6.56919,
6.62789,
6.68347,
6.73796,
6.79161,
6.84396,
6.89447,
6.94199,
6.98504,
7.02322,
7.05947,
7.09783,
7.14278,
7.19947,
7.26907,
7.31028,
7.32138
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
