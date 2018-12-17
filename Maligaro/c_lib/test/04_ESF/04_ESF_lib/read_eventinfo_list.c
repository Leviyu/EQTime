#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>
int read_eventinfo_list(new_RECORD* my_record, new_INPUT* my_input)
{
puts("---> Read eventinfo ");
	FILE* in;
	in=fopen(my_input->eventinfo_file,"r");
	char buff[2000];
	char buff3[2000];
	char buff1[2000];
	char sta_tmp[2000];
	double polarity;
	int N = 2000;
	int i,j,k;

	for(i=0;i<my_input->sta_num;i++)
	{

		initiate_record_name(&my_record[i]);
		fgets(buff,N,in);
//printf("buff is %s \n",buff);
		//sscanf(buff,"%s %s %lf %lf %lf	%lf %lf %lf %lf %lf %lf %s 	%d 	%d  %lf %lf %lf %lf %lf %s 	%lf %lf %lf  %s 	%lf %d %lf	%lf %lf %lf %lf %lf  ",
		sscanf(buff,"%s %s %lf %lf %lf	%lf %lf %lf %lf %lf %lf %s 	%d 	%d  %lf %lf %lf %lf %lf %s 	%lf %lf %lf  %s 	%lf %d %lf	%lf %lf %lf %lf %lf %lf %lf %lf %lf ",
				my_record[i].name,
				my_record[i].NET,
				&my_record[i].DIST,
				&my_record[i].AZ,
				&my_record[i].BAZ,
				&my_record[i].sta_lat,
				&my_record[i].sta_lon,
				&my_record[i].eq_lat,
				&my_record[i].eq_lon,
				&my_record[i].eq_dep,
				&my_record[i].eq_mag,
				my_record[i].EQ,
				&my_record[i].polar_flag,
				&my_record[i].quality,
				&my_record[i].prem,
				&my_record[i].amplitude,
				&my_record[i].ccc,
				&my_record[i].SNR,
				&my_record[i].dt_obs_prem,
				my_record[i].PHASE,
				&my_record[i].best_stretch_ccc,
				&my_record[i].best_stretch_coefficient,
				&my_record[i].misfit,
				my_record[i].COMP,
				&my_record[i].time_phase_peak,
				&my_record[i].npts_phase_peak,
				&my_record[i].noise_beg,
				&my_record[i].noise_len,
				&my_record[i].phase_beg,
				&my_record[i].weight,
				&my_record[i].SNR2,
				&my_record[i].misfit2,
				&my_record[i].ONSET,
				&my_record[i].ENDSET,
				&my_record[i].best_tstar,
				&my_record[i].best_tstar_ccc);


		
	}

	fclose(in);


printf("read eventinfo done\n");
	return 0;
}
