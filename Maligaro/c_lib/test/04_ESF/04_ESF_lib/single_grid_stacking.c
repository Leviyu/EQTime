#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>
#define DEBUG 0

int single_grid_stacking(new_GRID** my_grid, new_INPUT* my_input, new_RECORD* my_record)
{
puts("--> Working on grid Stacking");
	double az_A_B(double A_lat, double A_lon, double B_lat, double B_lon);
	double dist_A_B(double A_lat, double A_lon, double B_lat, double B_lon) ;
	int ilat;
	int ilon;
	double lat_tmp;
	double lon_tmp;
	int ista;
	double dist_deg_lat;
	double dist_deg_lon;
	double dist_deg_tmp_lat;
	double dist_deg_tmp_lon;


puts("--> searching for station within each grid");
printf("HHHHHH my input num lat lon %d %d \n", my_input->num_lat, my_input->num_lon);
	for(ilat = 0; ilat < my_input->num_lat; ilat++)
		for(ilon = 0; ilon < my_input->num_lon; ilon++)
		{
//printf(" lat beg is %lf \n", my_grid[ilat][ilon].lat_beg);
			//current grid is my_gird[ilat][ilon]
			lat_tmp = my_grid[ilat][ilon].lat;
			lon_tmp = my_grid[ilat][ilon].lon;

			my_grid[ilat][ilon].sum_num = 0;

//printf("--> working on ialt ilon %d %d %lf %lf \n", ilat , ilon, lat_tmp, lon_tmp);
			// iterative through all record and find every station within range of this grid
			for(ista = 0 ; ista < my_input->sta_num; ista++)
			{
				dist_deg_tmp_lat = fabs( my_grid[ilat][ilon].lat - my_record[ista].sta_lat );
				dist_deg_tmp_lon = fabs( my_grid[ilat][ilon].lon - my_record[ista].sta_lon );

//printf("dist lat lon is %lf %lf station latlon %lf %lf \n", dist_deg_tmp_lat, dist_deg_tmp_lon, my_record[ista].sta_lat, my_record[ista].sta_lon );
				// if within range
				if( dist_deg_tmp_lat < my_input->AREA_RADIUS &&
						dist_deg_tmp_lon < my_input->AREA_RADIUS )
				{
					my_grid[ilat][ilon].sum_list[my_grid[ilat][ilon].sum_num] = ista;
					my_grid[ilat][ilon].sum_num ++;
				}
			}

printf("for grid lat lon %lf %lf , there is %d sta withiin %lf deg \n", lat_tmp, lon_tmp, my_grid[ilat][ilon].sum_num , my_input->AREA_RADIUS)	;		
		}


puts("--> Fix BAZ and Stack for varying slowness");
	int count;
	double clat;
	double clon;
	double dangle = 0;
	double az_sta = 0;
	double az_event = 0;
	double dist_along_ray = 0;
	double dist_tmp=0;
	double slowness = 0;
	int j,k,itime;
	int long_npts = (int) (my_input->long_len / my_input->delta );
printf("long len is %lf delta %lf npts %d \n", my_input->long_len, my_input->delta, long_npts);
// iterative through each grid and stack

	FILE* out;


	for(ilat = 0; ilat < my_input->num_lat; ilat++)
		for(ilon = 0; ilon < my_input->num_lon; ilon++)
		{
			if(my_grid[ilat][ilon].sum_num < 2 )
			{
				//printf("ERROR too few station for grid ilat ilon %d %d, skip this one\n", ilat, ilon);
				continue;
			}
			else
			{
				printf("we got something here\n");
			}

			clat = my_grid[ilat][ilon].lat;
			clon = my_grid[ilat][ilon].lon;

	char fix_BAZ_out[200];
	sprintf(fix_BAZ_out,"fix_baz.%d.%d", ilat,ilon);
	out = fopen(fix_BAZ_out,"w");
	fprintf(out, "clat %lf clon %lf num_of_station %d \n", clat, clon, my_grid[ilat][ilon].sum_num );

	// output station name 
	for(count = 0; count < my_grid[ilat][ilon].sum_num; count ++)
	{
		ista = my_grid[ilat][ilon].sum_list[count];
		fprintf(out,"%6s ", my_record[ista].name);
	}



			//varying slowness
			j = 0;	
			for(slowness = 2; slowness < 35; slowness+= 1)
			{
				//slowness here is in s/deg, convert it to s/km 
				// for a given slowness, the stacking signal is 
				my_grid[ilat][ilon].fix_BAZ_slowness[j] = slowness;
				my_grid[ilat][ilon].fix_BAZ_amp[j] = 0;

			
				// we fix BAZ and vary slowness to calculate the time
				// for eac station within the grid, calculate the delay time
				//iterative through each time step for stacking
				for(itime = 0; itime < long_npts; itime++ )
				{
					my_grid[ilat][ilon].fix_BAZ_time[j] = itime * my_input->delta;
				for(count = 0; count < my_grid[ilat][ilon].sum_num; count ++)
				{
					ista = my_grid[ilat][ilon].sum_list[count];
					lat_tmp = my_record[ista].sta_lat;
					lon_tmp = my_record[ista].sta_lon;
					//distance between sta and grid point in km
					dist_tmp = dist_A_B(lat_tmp, lon_tmp, clat, clon);

					//angle between raypath and grid_sta geometry
//printf("%lf %lf %lf %lf \n", clat , clon, lat_tmp, lon_tmp);
					az_sta = az_A_B(clat, clon, lat_tmp, lon_tmp);
					az_event = my_record[ista].BAZ;
					dangle = az_sta - az_event;
//printf("az sta is %lf az event is %lf dist is %lf \n", az_sta, az_event, dist_tmp);
					//distance along ray
					dist_along_ray =  cos( dangle / 180 * 3.1415926) * dist_tmp  ;
					//slowness in s/deg convert to s/km
					my_grid[ilat][ilon].fix_BAZ_delay_time = dist_along_ray * slowness /111;
					//convert it to npts
					int tmp_npts = (int) (my_grid[ilat][ilon].fix_BAZ_delay_time / my_input->delta);

//printf("dangle is %lf baz %lf slowness %lf distance along ray %lf , time delay %lf \n", dangle ,az_event, slowness,dist_along_ray,  my_grid[ilat][ilon].fix_BAZ_delay_time);
					//stacking to sum
					int new_npts = itime - tmp_npts;
					if(new_npts < 0)
						new_npts = 0;
					else if(new_npts > long_npts)
						new_npts = long_npts;
					my_grid[ilat][ilon].fix_BAZ_amp[j] += my_record[ista].long_win[new_npts];

				}
				//write out it to file
				double real_time = my_grid[ilat][ilon].fix_BAZ_time[j]+my_input->TIME_REF + my_input->long_beg;
		fprintf(out, " %lf %lf %lf \n", real_time, my_grid[ilat][ilon].fix_BAZ_slowness[j], my_grid[ilat][ilon].fix_BAZ_amp[j]);
				}

				j++;

			}

		}

	return 0;
}

