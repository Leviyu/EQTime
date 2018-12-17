#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>
double* read_long_window(char* sac_file, double long_beg, double long_len,double prem, char* velocity_or_displacement, new_INPUT* my_input, int polar_flag)
{
	void wiginterpd();
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



	int len,max = 1000000,nerr;
	float beg,del;
	rsac1(sac_file,yarray,&len,&beg,&del,&max,&nerr,strlen(sac_file));
//printf("sac errlr flag is %d \n", nerr);
	if(nerr != 0)
	{
		printf("read sac file error sac_file is %s\n",sac_file);
		return new_y;
	}
	// check for bad quality record , and set them to 1e-8
	if( beg < -500 || (int)(len*my_input->delta) < 700 )
	{
		beg = 0;
		for(i=0;i<100000;i++)
			yarray[i]= 0.000000001;
	}
	//store long data
	int npts_long_beg = (int)((prem +long_beg - beg)/del);
	int npts_long_len = (int)(long_len / del);
	//printf("npts_long_beg is %d len is %d \n",npts_long_beg, npts_long_len);
	//printf("sac file is %s\n",sac_file);
//printf("DEBUG yarray 1 is %lf \n", yarray[npts_long_beg]);

	for(i=0;i<npts_long_len;i++)
	{
		if(yarray[npts_long_beg] != yarray[npts_long_beg] || yarray[npts_long_beg+1] != yarray[npts_long_beg+1])
		{
			yarray[npts_long_beg] = 0.0000000001;
			yarray[npts_long_beg+1] = 0.0000000001;
		}
		x_tmp[i] = 0+i*del;
		if(strcmp(velocity_or_displacement, "velocity")!=0)
			y_tmp[i] = yarray[npts_long_beg+i];
		else 
			y_tmp[i] = (yarray[npts_long_beg+i+1] - yarray[npts_long_beg+i] )/del;
		///printf("y tmp is %15.10lf\n", y_tmp[i]);
	}



	// using wiggins method to interpolate the data into delta= my_input->delta
	//double new_x[100000];
	//double new_y[100000];
	int new_npts = (int)(long_len/my_input->delta);
	for(i=0;i<new_npts;i++)
		new_x[i]=0+i*my_input->delta;
	wiginterpd(x_tmp,  y_tmp, npts_long_len,  new_x, new_y,new_npts , 0);

	// flip the recrod according to the polarity
	if(polar_flag !=0 )
	for(i=0;i< new_npts;i++)
		new_y[i] = new_y[i] * polar_flag;




	return new_y;
}

