
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>

double get_weight_from_SNR_CCC(double SNR, double ccc_orig)
{
	double weight;
	double ccc;

	ccc = ccc_orig;

	if( SNR > 4 )
		weight = fabs(ccc)*1.0;
	else if ( SNR > 3)
		weight = fabs(ccc)*0.8;
	else if ( SNR > 2)
		weight = fabs(ccc)*0.6;
	else if ( SNR > 1.5)
		weight = fabs(ccc)*0.3;
	else
		weight = 0;
	return weight;
}
