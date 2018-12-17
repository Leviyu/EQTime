//extern "C"
//{
//#include<sacio.h>
//}
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

int sac2xy_with_delta(string sac_file2, double begin_time, double length, double* out_array, double DELTA)
{
	//printf("process sac2xy_with_delta \n");
	int NUM = 1000000;
	float* yarray;
	yarray = (float*)malloc(sizeof(float)*NUM);
	int len;
	float beg,del;
	int nerr;
	int max = 1000000;

	int i;
	char* sac_file;
	sac_file = strdup(sac_file2.c_str());
	//printf("sac file is %s \n", sac_file);

	rsac1(sac_file,yarray,&len,&beg,&del,&max,&nerr,strlen(sac_file));
	//printf("--> reading sac file %s yarray 1 is %lf \n", sac_file, yarray[1]);
	//printf(" beg time %lf len %lf \n", begin_time, length);
	//printf("nerr is %d   0 is good \n", nerr);
	//cout << "del ois " << del << " beg is "<< beg<< " begin_time is "<< begin_time << endl;
	if(nerr != 0)
	{
		printf("ERROR in sax2xy, read in sac file : %s \n",sac_file);
		return 1;
	}

	if( beg < -500 || (int)(len*del) < 700 )
	{
		beg = 0;
		for(i = 0; i< max ; i++)
			yarray[i] = 0.00000000001;
		printf(" ---> bad quality sac, file %s \n", sac_file);
	}
	int npts_long_beg = (int)((begin_time - beg)/del); 
	int npts_long_len = (int) ( length / del);
	int npts_long_end = npts_long_beg + npts_long_len;
cout << "  npts_long_end " << npts_long_end << " len "<< len << endl;
	if( npts_long_end >= len || npts_long_beg <= 0)
	{
		for(i = 0; i< max ; i++)
			yarray[i] = 0.00000000001;
		printf(" ---> bad quality sac, reached end of file %s \n", sac_file);
	}




	double end_time = begin_time + length;			//
	int npts_sac = (int)(length / del)+1;
	double XTMP[npts_sac];
	double YTMP[npts_sac];

	int npts_out = (int)(length/DELTA) ;
	double new_x[npts_out];		// new x to store time
	double new_y[npts_out];		// new y to store amplitude
	int icount;
	double itime;

//printf(" %lf %lf %lf %d %d \n",  begin_time, length, del, npts_sac, npts_out);
	// get x_array
	for(icount = 0; icount < npts_sac; icount++)
	{
		XTMP[icount] = 0 + icount * del;
		//printf("%lf\n", XTMP[icount]);
	}

	// get the selected window from yarray 
	int new_count = 0;
	for(icount = 0; icount < npts_sac; icount ++)
	{
		itime = begin_time + icount * del;
		if(itime >= begin_time)
		{
			int yarray_index = (int)((itime -beg)/del);
			if( yarray[ yarray_index] != yarray[ yarray_index])
				yarray[ yarray_index] = 0;
			YTMP[new_count] = yarray[yarray_index];
			//YTMP[new_count] = 1;
			//printf(" YTMP  %lf \n", YTMP[new_count] );
			new_count ++;
		}
		if(itime > end_time)
			break;
	}


	// construct new_x
	for(icount = 0; icount < npts_out; icount++)
		new_x[icount] = 0+ icount * DELTA;



//output_array2("XYlololololo",XTMP, YTMP, npts_sac,0);
//printf("use wiginterpd  npts sac %d npts out %d \n", npts_sac ,npts_out);
	// using wiginterpd to interpolate
	wiginterpd(XTMP,YTMP,npts_sac,new_x,out_array,npts_out,0);
//printf("use wiginterpd DONE\n");

	free(yarray);

//output_array2("lololololo",new_x, out_array, npts_out,1);
//f

	return 0;
}

