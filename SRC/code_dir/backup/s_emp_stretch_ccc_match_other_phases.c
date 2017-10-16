#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<hongyulib.h>
#include<math.h>



int main()
{
	int CCC( double* x, int npts_x, double* y, int npts_y, int*shift, double* ccc, int flag);
	int construct_array_with_main_lobe(double* array_in, int* npts_in, double* array_out);
	double y1[10000];
	double y1_tmp[10000];
	double y2[10000];
	double y2_tmp[10000];
	double y[30][10000];
	double y_tmp[30][10000];
	int i,j;
	char file1[100];
	char file2[100];

	scanf("%s %s ", file1, file2);

	int N = 10000;
	FILE* in;
	FILE* in2;
	in = fopen(file1,"r");
	in2 = fopen(file2,"r");

	int NUM1 = count_file_num(file1);
	int NUM2 = count_file_num(file2);

	for(i=0;i<NUM1;i++)
		fscanf(in,"%lf",&y1[i]);

	for(j=0;j<NUM2;j++)
		fscanf(in2,"%lf",&y2[j]);

	construct_array_with_main_lobe(y1,&NUM1,y1_tmp);
	construct_array_with_main_lobe(y2,&NUM2,y2_tmp);

	int shift[30];
	double ccc[100];
	double coeff;

	for(i = 0; i<30; i++ )
	{
		coeff = 0.2+i*0.05;
		stretch_ES_function(y2_tmp,NUM2, coeff, y[i]);
		stretch_ES_function(y2,NUM2, coeff, y_tmp[i]);

		CCC(y1_tmp,NUM1, y[i], NUM2, &shift[i], &ccc[i],0);
		//printf("index %d CCC is %lf coeff is %lf\n",i,ccc[i], coeff);

	}

	int max_index=0;
	int max_shift=0;
	double max_ccc= 0;
	for(i = 0; i<30; i++)
	{
		if(ccc[i] > max_ccc)
		{
			max_index = i;
			max_shift = shift[i];
			max_ccc = ccc[i];
		}

	}

	printf("max ccc %lf shift %d index %d \n", max_ccc, max_shift, max_index);
	double x1[NUM1];
	double x2[NUM2];

	for(i = 0;i<NUM1; i++)
		x1[i] = i;

	for(i = 0;i<NUM2; i++)
		x2[i] = i+shift[max_index];

	output_array2("ES_best_match.out",x2,y_tmp[max_index],NUM2,0);

	return 0;
}

