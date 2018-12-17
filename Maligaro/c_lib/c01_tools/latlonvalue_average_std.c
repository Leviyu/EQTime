#include "hongyulib.h"
/******************************************************************
 *  This code read in lat lon value and convert them into averaged cell format
 *  for each cell we give its STD
 *  INPUT:
 *		file: lat_lon_value file
 *		double: grid_inc
 *		
 *	OUTPUT:
 *	file: lat lon value_avg value_std
 *
******************************************************************/




void latlonvalue_average_std(char* input, double grid_inc, char* output)
{
	//puts("Begin latlonvalue_avg_std now!");
	FILE* in;
	FILE* out;

	in = fopen(input,"r");
	out = fopen(output,"w");


	//printf("Get file num  %s \n", input);
	int file_num = count_file_num(input);
//printf("---> max input is %d \n", file_num);
	int MAX = 500000;
	double* lat;
	double* lon;
	double* value;
	lat = (double*)malloc(sizeof(double)*file_num);
	lon = (double*)malloc(sizeof(double)*file_num);
	value = (double*)malloc(sizeof(double)*file_num);

	int count;
	for(count = 0; count < file_num; count ++)
	{
		fscanf(in,"%lf %lf %lf",&lat[count], &lon[count], &value[count] );
		//printf("%lf %lf %lf\n",lat[count], lon[count], value[count] );
	}


	double lat_tmp;
	double lon_tmp;
	double latmin,latmax,lonmin,lonmax;

	int NUM;
	double STD;
	double avg;
	double SUM;
	double STD_SUM;


	for(lat_tmp = -90+grid_inc/2; lat_tmp <= 90 - grid_inc/2  ; lat_tmp = lat_tmp + grid_inc)
	{
		latmin = lat_tmp - grid_inc/2;
		latmax = lat_tmp + grid_inc/2;
		for(lon_tmp = -180 + grid_inc/2; lon_tmp <= 180 - grid_inc/2  ; lon_tmp = lon_tmp + grid_inc)
		{
			lonmin = lon_tmp - grid_inc/2;
			lonmax = lon_tmp + grid_inc/2;

			NUM = 0;
			STD = 0;
			STD_SUM = 0;
			avg = 0;
			SUM = 0;


			for(count = 0; count < file_num; count++)
			{

				if(lat[count] >= latmin && lat[count] <latmax && lon[count]>= lonmin && lon[count] < lonmax)
				{
					NUM ++;
					SUM += value[count];
				}
			}

			if(NUM ==0 )
			{
				avg = 0;
				STD = 0;
			}
			else 
			{
			avg = SUM/NUM;
			//printf("working on %lf %lf %lf \n", lat_tmp, lon_tmp, avg);

			for(count = 0; count < file_num; count++)
			{

				if(lat[count] >= latmin && lat[count] <latmax && lon[count]>= lonmin && lon[count] < lonmax)
				{
					STD_SUM += ( value[count] - avg ) * (value[count] - avg);
				}
			}
			STD = sqrt( STD_SUM / NUM);
			}


			fprintf(out, "%10.3lf %10.3lf %10.3lf %10.3lf\n", lat_tmp, lon_tmp, avg, STD);

		}
	}

	

	fclose(in);
	fclose(out);

}
