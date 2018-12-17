
#include "ESF.h"
int read_long_window(char* sac_file, new_RECORD* my_record, new_INPUT* my_input)
{
	//void wiginterpd();
	int traffic_zero_out(new_RECORD* my_record, new_INPUT* my_input);

	// manually shifted time is added to PREM time
	//double long_beg = my_record->long_beg + my_record->dt_picked_shift;
	double long_beg = my_record->long_beg;
	double long_len = my_record->long_len;
	char velocity_or_displacement[200];
	strcpy(velocity_or_displacement, my_input->velocity_or_displacement);
	int polar_flag = my_record->polar_flag;
	//printf( "polarflag is %d \n", polar_flag);
	double prem = my_record->prem;

	//printf(" gebin \n");
	int i;
	int ymax =   150000;
	float yarray[ymax];
	double x_tmp[ymax];
	double y_tmp[ymax];
	double new_x[ymax];
	double new_y[ymax];
	//float yarray[500000];
	//double x_tmp[500000];
	//double y_tmp[500000];
	//double new_x[500000];
	//double new_y[500000];

	//printf(" sac is %s \n", sac_file);

// ====================================================================
// read in sac file to yarray
// ====================================================================
	int len = ymax;
	int max = ymax;
	int nerr = 0;
	float del = 0;
	float beg = 0;
	rsac1(sac_file,yarray,&len,&beg,&del,&max,&nerr,strlen(sac_file));
	//printf(" sac %s len %d beg %f del %f max %d nerr %d len %d \n", sac_file,len
			//,beg,del,max,nerr,strlen(sac_file));
	if(del != 0.1) 
	{
		del = 0.1;
	}

	if( len > 76000) 
		len = 75000;

	// checking if sac problem
	if(nerr != 0)
	{
		printf("---> nerr read sac file error sac_file is %s \n",sac_file);
		my_record->quality = -1;
		beg = 0;
		for(i=0;i<ymax;i++)
			yarray[i]= 0.000000001;
	}

	//printf(" HERE ! \n");

// ====================================================================
// for record with beg < -500 length < 700sec set it as bad qualit = -1
// if its bad, we change it to constant value to make sure we have something output
// for plotting
// ====================================================================
	// check for bad quality record , and set them to 1e-8
	//if( beg < -500 || (int)(len*del) < 700 )
	if( beg < -500 || (int)(len*del) < 700 )
	{
		//my_record->quality = -1;
		len = 50000;
		max = 50000;
		beg = 0;
		prem = 100;
		del = 0.1;
		for(i=0;i<ymax;i++)
			yarray[i]= 0.000000001;
		//printf("len bad quality sac file error sac_file is %s length is not right\n",sac_file);
	}
	//store long data
	int npts_long_beg = (int)((prem +long_beg - beg)/del);
	if(npts_long_beg < 0 )
		npts_long_beg = 100;
	int npts_long_len = (int)(long_len / del) +1;
	int npts_long_end = npts_long_beg + npts_long_len;
//printf(" npts long end len %d %d beg %d  \n", npts_long_end, len, npts_long_beg);
//printf(" nots long len is %d \n", npts_long_len);
// ====================================================================
// make sure that our long window does not go beyond origional record
//
// ====================================================================
	//if(npts_long_end >= len || npts_long_beg <= 0 )
	if(npts_long_end >= len )
	{
		my_record->quality = -1;
		for(i=0;i<ymax;i++)
			yarray[i]= 0.000000001;
		//printf("---> long window reached the end of trace %s  \n", sac_file);
	}

// ====================================================================
//  define whether its in displacement or velocity
// ====================================================================
	for(i=0;i<npts_long_len;i++)
	{
		// make sure non nan value
		if(yarray[npts_long_beg+i] != yarray[npts_long_beg+i] )
			yarray[npts_long_beg+i] = 0.000000001;

		x_tmp[i] = 0+i*del;
		if(strcmp(velocity_or_displacement, "velocity")==0)
		{
			y_tmp[i] = (yarray[npts_long_beg+i+1] - yarray[npts_long_beg+i] )/del;
			if( y_tmp[i] == 0)
				y_tmp[i] = 0.000000001;
		}
		else 
			y_tmp[i] = yarray[npts_long_beg+i];
		//printf("y tmp is %15.10lf\n", y_tmp[i]);
	}

//printf("-- > converted to velocity? \n");

	int new_npts = npts_long_len;
	//int new_npts = (int)(long_len/my_input->delta);
	// if quality is -1 , skip these step 
	/*
	if(my_record->quality ==  -1)
	{
		for(i=0;i<npts_long_len;i++)
		{	
			my_record->long_win[i]=0.0000000001;;
		}
		return 0;
	}
	*/


// ====================================================================
//  intepolate with wig method 
//  see sac for detail
// ====================================================================
	//for(i=0;i<new_npts;i++)
	for(i=0;i<npts_long_len;i++)
	{
		new_x[i]=0+i*my_input->delta;
		//printf(" y tmp %20.10lf \n",y_tmp[i]);
		new_y[i] = y_tmp[i];
	}
	//wiginterpd(x_tmp,  y_tmp, npts_long_len,  new_x, new_y,new_npts , 0);

	// flip the recrod according to the polarity
	if(polar_flag == -1 )
	{
		//printf(" sta %s is flipped cause of polarity %d \n", my_record->name , polar_flag);
	for(i=0;i< new_npts;i++)
		new_y[i] = new_y[i] * polar_flag;
	}

	//printf("long and orig \n");

// ====================================================================
//   this is getting interesting 
//   we store the origional long win into anyother trace called long_orig
//   and we store the traffic-phase zeroed out phase to be long_win
// ====================================================================
	for(i=0;i<new_npts;i++)
	{	
		//printf(" i is %d \n",i);
		//fflush(stdout);
		//printf(" %20.10lf \n", new_y[i]);
		my_record->long_win[i]=new_y[i];
		my_record->long_orig[i]=new_y[i];
	}

	// output long_orig
	//char long_ggg[200];
	//sprintf(long_ggg,"%s.long_data.new_y",my_record->name );
	//output_array2(long_ggg, new_x, my_record->long_orig, npts_long_len, 1);
	//output_array2(long_ggg, new_x, new_y, npts_long_len, 1);
		//output_array2(long_win_name,x_long,my_record->long_orig, new_npts_long, 1);


	// now construct long_win
	//printf("zero out \n");
	traffic_zero_out(my_record,  my_input);

	// output long_orig
	//sprintf(long_ggg,"%s.long_data.long_orig",my_record->name );
	//output_array2(long_ggg, new_x, my_record->long_orig, npts_long_len, 1);
		//output_array2(long_win_name,x_long,my_record->long_orig, new_npts_long, 1);


	return 0;
}


// this is a function to construct long window with traffic phase padded with zero
int traffic_zero_out( new_RECORD* my_record, new_INPUT* my_input)
{
	//printf("zero out traffic %s \n", my_record->name);

	int npts_long = (int)( my_record->long_len / my_input->delta) +1;
	int i;


	int count;
	my_record->traffic_in_noise_window_flag = 0;
	
	for(count = 0; count < my_record->num_traffic ; count++)
	{

		// if traffic exist in [-100,-20] window, we make 
		// traffic_in_noise_window_flag = 1
		if( my_record->traffic_time[count] > -100 &&
				my_record->traffic_time[count] < -20 )
			my_record->traffic_in_noise_window_flag = 1;



		// if is depth_phase and is close to main phase then 15 second
		// we use 15sec cause we dont have any estimation of one Period here
		// yet
		if( strcmp( my_record->traffic_phase[count], my_record->depth_phase) == 0)
			if( fabs( my_record->traffic_time[count]) < 30  )
				continue;

		// if phase contrains SKS or SKKS, we skip
		if( strcmp( my_record->traffic_phase[count], "SKS") == 0 ||
			strcmp( my_record->traffic_phase[count], "SKKS") == 0 ||
			strcmp( my_record->traffic_phase[count], "sSKS") == 0 || 
			strcmp( my_record->traffic_phase[count], "sSKKS") == 0 )
				continue;

		// for Phase S, if dist > 80 then we dont use traffic anymore
		if( strcmp(my_record->PHASE,"S") == 0 && my_record->DIST > 80)
			continue;
		if( strcmp(my_record->PHASE,"P") == 0 && my_record->DIST > 78)
			continue;



		double what_is_too_far_huawei = 200;
		if( strstr(my_record->PHASE,"P") != NULL )
			what_is_too_far_huawei = 50;
		if(my_record->traffic_time[count] < -1*what_is_too_far_huawei  ||
				my_record->traffic_time[count] > what_is_too_far_huawei )
			continue;

//printf(" zero out %d / %d phase %s \n", count , my_record->num_traffic, my_record->traffic_phase[count]);
		int pad_length = 8;
		int npts_pad_each_side = (int)( pad_length / my_input->delta);
		//int npts_min = (int)(-15 / my_input->delta);
		//int npts_max_with_1T = (int) (20 / my_input->delta) + 

		int npts_beg_pad;
		int npts_end_pad;
		npts_beg_pad  = (int)(( my_record->traffic_time[count] - my_record->long_beg )/my_input->delta - npts_pad_each_side );
		npts_end_pad  = (int)(( my_record->traffic_time[count] -my_record->long_beg ) /my_input->delta + npts_pad_each_side );

		if( npts_beg_pad <= 0 || npts_end_pad > (int)(my_input->phase_len / my_input->delta))
		{
			//printf(" npts problem \n");
			npts_beg_pad = 0;
			npts_end_pad = 0;
		}

		//printf("sta %s , traffic zoom out is %s \n", my_record->name, my_record->traffic_phase[count]);
		//printf("npts beg end is %d %d  \n", npts_beg_pad, npts_end_pad );

		int kkk;
		for(kkk = npts_beg_pad ; kkk< npts_end_pad ; kkk++)
		{
			my_record->long_win[kkk] = 0;
		}
	}



	return 0;
}













