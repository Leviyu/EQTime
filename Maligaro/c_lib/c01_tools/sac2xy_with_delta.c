#include "hongyulib.h"
/******************************************************************
 * This is a c script to read in sac file and return a xy array
 * we manually convert the sampling rate  
 *
 *	Input: 
 *	sac_file
 *	double 					begin_time			the begin time of we want 
 *	double 					length				the length we want in sec
 *	
 *
 *	Output:
 *	double*					out_array				the y array we want
 *
 *
 *
 *	DATE:				Keywords:
 *	Reference:
******************************************************************/
int sac2xy_with_delta(char* sac_file, double begin_time, double length, double* out_array, double DELTA)
{
	float yarray[1000000];
	int len;
	float beg,del;
	int nerr;
	int max = 1000000;


	rsac1(sac_file,yarray,&len,&beg,&del,&max,&nerr,strlen(sac_file));

	//printf("--> reading sac file %s yarray 1 is %lf \n", sac_file, yarray[1]);
	//printf(" beg time %lf len %lf \n", begin_time, length);
	if(nerr != 0)
	{
		printf("ERROR in sax2xy, read in sac file : %s \n",sac_file);
	}

	double end_time = begin_time + length;			//
	int npts_sac = (int)(length / del)+1;
	double XTMP[npts_sac];
	double YTMP[npts_sac];

	int npts_out = (int)(length/DELTA) +1;
	double new_x[npts_out];		// new x to store time
	//double new_y[npts_out];		// new y to store amplitude
	int icount;
	double itime;

	// get x_array
	for(icount = 0; icount < npts_sac; icount++)
		XTMP[icount] = 0 + icount * del;

	// get the selected window from yarray 
	int new_count = 0;
	for(icount = 0; icount < npts_sac; icount ++)
	{
		itime = begin_time + icount * del;
		if(itime >= begin_time)
		{
			int yarray_index = (int)((itime -beg)/del);
			YTMP[new_count] = yarray[yarray_index];
			new_count ++;
		}
		if(itime > end_time)
			break;
	}



	// construct new_x
	for(icount = 0; icount < npts_out; icount++)
		new_x[icount] = 0+ icount * DELTA;

	// using wiginterpd to interpolate
	wiginterpd(XTMP,YTMP,npts_sac,new_x,out_array,npts_out,0);

//output_array2("lololololo",new_x, out_array, npts_out,1);
//output_array2("XYlololololo",XTMP, YTMP, npts_sac,1);

	return 0;
}

