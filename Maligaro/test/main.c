#include<stdlib.h>
#include<stdio.h>
#include "rad.h"
#include<string.h>



int main(int argc, char** argv)
{
	//Tprintf(" %s \n", argv[1]);



	double strike = 107;
	double dip = 38;
	double rake = -141;
	double az = 314.338;
	double take_off = 33.47;
	double prediction = 0;

	take_off = atoi(argv[1]);
	az = atoi(argv[2]);
	char sta[200];
	strcpy(sta,argv[3]);

//double rad_pat(double strike,double dip,double rake,double az,double take_off,int P)	
	prediction = rad_pat(strike,dip,rake,az,take_off,2);
	printf(" sta %s polarity prediction is %10.2lf \n", sta, prediction);




	return 0;
}
