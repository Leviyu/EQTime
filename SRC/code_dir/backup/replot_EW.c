#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<hongyulib.h>
#include<ESF.h>

int main(int argc, char* argv[])
{
	char EQ_name[200];
	char PHASE_name[200];
	double delta;
	// read in EQ and PHASE
	scanf("%s %s %lf ",EQ_name, PHASE_name, &delta);

	// read in input info
	new_INPUT my_input;
	initiate_input(&my_input);
	strcpy(my_input.EQ,EQ_name);
	strcpy(my_input.PHASE,PHASE_name);
	my_input.delta = delta;

	read_input_info(&my_input);
	my_input.sta_num =  count_file_num(my_input.eventinfo_file);

	new_RECORD my_record[my_input.sta_num];
	//read_eventstation_list(my_record, &my_input);
	read_eventinfo_list(my_record, &my_input);

printf("Define stretching factor window");
		// define the stretching factor window
		// How: we use the average as the reference and select 70% of total records to restack the new E.W.
	int count;
	int NUM_good_record = 0;
	double stretch_ave = 0;
	double stretch_sum = 0;
	for(count = 0; count < my_input.sta_num; count++)
	{
		//printf("sta %s quality %d \n", my_record[count].name,my_record[count].quality);
		if(my_record[count].quality != 1 )
			continue;
		NUM_good_record++;
		stretch_sum += my_record[count].best_stretch_coefficient;
	}
	stretch_ave = stretch_sum / NUM_good_record;
//printf("num of good is %d stretch ave is %lf \n", NUM_good_record, stretch_ave);

	// find the stretching window
	double interval = 0.1;
	double stretch_beg = stretch_ave;
	double stretch_end = stretch_ave;
	for(interval = 0.1; interval < 2; interval = interval + 0.1)
	{
		stretch_beg = stretch_ave - interval;
		stretch_end = stretch_ave + interval;
		
		// find the num of recors that is within this window
		int num_in_window = 0;
		for(count = 0; count < my_input.sta_num; count++)
		{
			if(my_record[count].quality != 1 )
				continue;
			if(my_record[count].best_stretch_coefficient > stretch_beg &&
					my_record[count].best_stretch_coefficient < stretch_end )
				num_in_window ++;
		}
//printf("interval %lf num in window %d \n",interval, num_in_window);
		double percentage = num_in_window / NUM_good_record;
		if(percentage > 0.7)
			break;
	}

//printf("Stretching window for %s is %lf %lf \n", my_input.PHASE, stretch_beg, stretch_end);	
	printf("phase len %lf delta %lf sta num %d \n", my_input.phase_len, my_input.delta, my_input.sta_num);
	int sta_num = my_input.sta_num;
	int npts_phase = (int)(my_input.phase_len/ my_input.delta);
	double array[sta_num][npts_phase];

	double tmp;


//puts("	 read in phase window for each record " );
	for(count = 0; count < my_input.sta_num; count++)
	{
		if(my_record[count].quality != 1 )
			continue;
		FILE* in;
		char file_name[200];
		sprintf(file_name,"%s.%s.%s.%s.phase",my_input.EQ, my_record[count].name,my_input.PHASE,my_input.COMP);
//printf("read in phase window for sta %s \n", my_record[count].name);
		in = fopen(file_name,"r");
		int file_num = count_file_num(file_name);
		int iline;
		char read_tmp[500];
		char buff[500];
		int N = 500;
		for(iline = 0; iline < file_num; iline++)
		{
			fscanf(in,"%lf %lf", &tmp, &array[count][iline]);
		}

		fclose(in);
	}

	int icount,pcount;
	double STD[npts_phase];
	double new_ES[npts_phase];
	for(pcount = 0; pcount < npts_phase; pcount++)
	{
		new_ES[pcount] = 0;
		for(count = 0; count < my_input.sta_num; count++)
		{
	//printf("sta %s coeff %lf \n", my_record[count].name, my_record[count].best_stretch_coefficient);
			if(my_record[count].quality != 1 )
				continue;


			if(my_record[count].best_stretch_coefficient > stretch_beg &&
					my_record[count].best_stretch_coefficient < stretch_end )
			{
				// stack and recompute the STD 
				new_ES[pcount] +=array[count][pcount];

			}
		}
	}
	// normalize new_ES
	normalize_array(new_ES, npts_phase);
//puts("getting STD");

	for(pcount = 0; pcount < npts_phase; pcount++)
	{
		STD[pcount] = 0;
		for(count = 0; count < my_input.sta_num; count++)
		{
			if(my_record[count].quality != 1 )
				continue;


			if(my_record[count].best_stretch_coefficient > stretch_beg &&
					my_record[count].best_stretch_coefficient < stretch_end )
			{
				// stack and recompute the STD 
				//printf("new es %lf array %lf \n",new_ES[pcount], array[count][pcount]);
				STD[pcount]+= pow( new_ES[pcount] - array[count][pcount] ,2 );
			}
		}
		STD[pcount] = sqrt(STD[pcount]/NUM_good_record);

	}

	//output newES and STD
	FILE* out;
	char out_name[200];
	sprintf(out_name,"%s.%s.%s.new_ES",my_input.EQ, my_input.PHASE,my_input.COMP);
	output_array1(out_name, new_ES,npts_phase);
	// out put STD
	sprintf(out_name,"%s.%s.%s.new_ES_STD",my_input.EQ, my_input.PHASE,my_input.COMP);
	out = fopen(out_name,"w");
	for(count = 0; count < npts_phase; count++)
		fprintf(out,"%d %lf\n",count+1, new_ES[count]+STD[count]);
	for(count = npts_phase-1; count >=0 ; count--)
		fprintf(out,"%d %lf\n",count+1, new_ES[count]-STD[count]);
	fclose(out);


	return 0;
}


