
#include "ESF.h"
#define DEBUG 0

int initiate_grid(new_GRID** my_grid, new_INPUT* my_input)
{
puts("--> Initigate grid ");
	double lon_beg = my_input->lon_beg;
	double lon_end = my_input->lon_end;
	double lat_beg = my_input->lat_beg;
	double lat_end = my_input->lat_end;
	double lon_delta = my_input->lon_delta;
	double lat_delta = my_input->lat_delta;
	double radius_deg = 5;
	
	// define dimension of grid 
	int num_lat = (int)( fabs(lat_end - lat_beg) / lat_delta );
	int num_lon = (int)( fabs(lon_end - lon_beg) / lon_delta );
	my_input->num_lat = num_lat;
	my_input->num_lon = num_lon;
	int count;
    printf("--> number of lat %d  and lon %d \n ", num_lat, num_lon);

	int i,j;
	for(i = 0; i< num_lat ; i++)
		for(j = 0; j<num_lon ; j++)
		{
			my_grid[i][j].lat_beg  =  lat_beg;
			my_grid[i][j].lon_beg  =  lon_beg;
			my_grid[i][j].lat_end  =  lat_end;
			my_grid[i][j].lon_end  =  lon_end;
			my_grid[i][j].lat_delta  =  lat_delta;
			my_grid[i][j].lon_delta  =  lon_delta;

			my_grid[i][j].lat = lat_beg + i*lat_delta;
			my_grid[i][j].lon = lon_beg + j*lon_delta;
			my_grid[i][j].radius_deg = radius_deg;

			my_grid[i][j].sum_num = 0;
			my_grid[i][j].prem = 0;
			my_grid[i][j].ray_parameter = 0;
			my_grid[i][j].AZ = 0;
			my_grid[i][j].BAZ = 0;


			my_grid[i][j].sum_list =(int*)malloc(sizeof(int)*10000);
			my_grid[i][j].fix_BAZ_time =(double*)malloc(sizeof(double)*10000);
			my_grid[i][j].fix_BAZ_slowness =(double*)malloc(sizeof(double)*10000);
			my_grid[i][j].fix_BAZ_amp =(double*)malloc(sizeof(double)*10000);

			my_grid[i][j].fix_slow_amp =(double*)malloc(sizeof(double)*10000);
			my_grid[i][j].fix_slow_BAZ =(double*)malloc(sizeof(double)*10000);
			my_grid[i][j].fix_slow_time =(double*)malloc(sizeof(double)*10000);

		}



	return 0;
}
