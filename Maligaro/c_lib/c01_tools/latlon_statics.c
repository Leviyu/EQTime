#include "hongyulib.h"
/******************************************************************
 * This is a c script for 
 *
 *	Input:
 *
 *	char* filename with (lat,lon)
 *
 *
 *	Output:
 *
 *
 *	DATE:				Keywords:
 *	Reference:
******************************************************************/



int latlon_statics(char* file_name,double lat_delta, double lon_delta, char* out_name)
{

	FILE* in;
	in = fopen(file_name,"r");
	int count;

	int line = count_file_num(file_name);
	double* biglat;
	double* biglon;
	//int N = 1000000;
	biglat = (double*)malloc(sizeof(double)*line);
	biglon = (double*)malloc(sizeof(double)*line);
	assert(biglat);
	assert(biglon);

	puts("start read in lat lon file");
	printf("total %d lines \n", line);
	for(count = 0; count < line; count ++)
	{
		char tmp[200];
		fgets(tmp,200,in);
		sscanf(tmp,"%lf %lf", &biglat[count], &biglon[count]);
		//printf("reading %d line %lf %lf \n", count, biglat[count],biglon[count]);
		//fscanf(in,"%lf %lf", &biglat[count], &biglon[count]);
	}



	int ilat, ilon;
	double lat_beg = -90;
	double lon_beg = -180;
	double lat_end = 90;
	double lon_end = 180;
	double lat,lon;
	FILE* out;
	out = fopen(out_name,"w");


	puts("lat lon file already read in memory!");

	for(lat = lat_beg; lat < lat_end; lat = lat + lat_delta  )
	{
		for(lon = lon_beg; lon <= lon_end; lon = lon + lon_delta)
		{
			
			int current_count = 0;
			for(count = 0; count < line; count ++)
			{
				if( fabs(lat - biglat[count]) < lat_delta/2 && fabs(lon - biglon[count]) < lon_delta/2 )
				{

					current_count ++;	
				}

			}

			fprintf(out,"%10.3lf %10.3lf %10d\n", lat, lon, current_count);
			//printf("%10.3lf %10.3lf %10d\n", lat, lon, current_count);
		}
	}












	return 0;
}


