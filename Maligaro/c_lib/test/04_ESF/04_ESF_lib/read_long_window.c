#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>
int read_long_window(char* sac_file, new_RECORD* my_record, new_INPUT* my_input)
{

	double long_beg = my_record->long_beg;
	double long_len = my_record->long_len;
	char velocity_or_displacement[200];
	strcpy(velocity_or_displacement, my_input->velocity_or_displacement);
	int polar_flag = my_record->polar_flag;
	double prem = my_record->prem;

	int i;
	float yarray[1000000];
	double x_tmp[100000];
	double y_tmp[100000];
	double new_x[100000];
	double* new_y;
	new_y = (double*)malloc(sizeof(double)*100000);
	// initiate new_y
	for(i=0;i<100000;i++)
		new_y[i]=0.0000000001;


// ====================================================================
// read in sac file to yarray
// ====================================================================
	int len = 0;
	int max = 1000000;
	int nerr = 0;
	float del = 0;
	float beg = 0;
	rsac1(sac_file,yarray,&len,&beg,&del,&max,&nerr,strlen(sac_file));


	// checking if sac problem
	if(nerr != 0)
	{
		printf("--->read sac file error sac_file is %s\n",sac_file);
		my_record->quality = -1;
		beg = 0;
		for(i=0;i<100000;i++)
			yarray[i]= 0.000000001;
		return 0;
	}

// ====================================================================
// for record with beg < -500 length < 700sec set it as bad qualit = -1
// if its bad, we change it to constant value to make sure we have something output
// for plotting
// ====================================================================
	// check for bad quality record , and set them to 1e-8
	if( beg < -500 || (int)(len*del) < 700 )
	{
		my_record->quality = -1;
		beg = 0;
		for(i=0;i<100000;i++)
			yarray[i]= 0.000000001;
		printf("bad quality sac file error sac_file is %s\n",sac_file);
		return 0;
	}
	//store long data
	int npts_long_beg = (int)((prem +long_beg - beg)/del);
	int npts_long_len = (int)(long_len / del);
	int npts_long_end = npts_long_beg + npts_long_len;
//printf(" npts long end len %d %d beg %d  \n", npts_long_end, len, npts_long_beg);
// ====================================================================
// make sure that our long window does not go beyond origional record
//
// ====================================================================
	if(npts_long_end >= len || npts_long_beg <= 0 )
	{
		my_record->quality = -1;
		for(i=0;i<100000;i++)
			yarray[i]= 0.000000001;
		printf("---> long window reached the end of trace %s  \n",sac_file);
		return 0;
	}

// ====================================================================
//  define whether its in displacement or velocity
// ====================================================================
	for(i=0;i<npts_long_len;i++)
	{
		// make sure non nan value
		if(yarray[npts_long_beg+i] != yarray[npts_long_beg+i] )
			yarray[npts_long_beg+i] = 0.000000001;
			
		if( yarray[npts_long_beg+1+i] != yarray[npts_long_beg+1+i])
			yarray[npts_long_beg+1+i] = 0.000000001;

		x_tmp[i] = 0+i*del;
		if(strcmp(velocity_or_displacement, "velocity")==0)
			y_tmp[i] = (yarray[npts_long_beg+i+1] - yarray[npts_long_beg+i] )/del;
		else 
			y_tmp[i] = yarray[npts_long_beg+i];
		//printf("y tmp is %15.10lf\n", y_tmp[i]);
	}


	int new_npts = (int)(long_len/my_input->delta);
	// if quality is -1 , skip these step 
	if(my_record->quality ==  -1)
	{
		for(i=0;i<new_npts;i++)
		{	
			my_record->long_win[i]=0.0000000001;;
		}
		return 0;
	}


// ====================================================================
//  intepolate with wig method 
//  see sac for detail
// ====================================================================
	for(i=0;i<new_npts;i++)
		new_x[i]=0+i*my_input->delta;

	wiginterpd(x_tmp,  y_tmp, npts_long_len,  new_x, new_y,new_npts , 0);

	// flip the recrod according to the polarity
	if(polar_flag !=0 )
	{
	for(i=0;i< new_npts;i++)
		new_y[i] = new_y[i] * polar_flag;
	}

	for(i=0;i<new_npts;i++)
	{	
		my_record->long_win[i]=new_y[i];
	}

	free(new_y);
	return 0;
}

