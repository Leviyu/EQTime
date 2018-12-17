#include "hongyulib.h"

double variance(char* infile)
{
	double var;

	FILE* in;
	int file_num = count_file_num(infile);
	int count;
	double sum;

	in = fopen(infile,"r");


	if(file_num == 0)
	{
		puts("ERROR read in file has 0 points!");
		return 0;
	}

	double data[file_num];
	for(count = 0; count < file_num; count++)
	{
		fscanf(in,"%lf", &data[count]);
	}


	// get the average
	sum = 0;
	for(count = 0; count < file_num; count++)
	{
		if(data[count] != data[count])
		{
			puts("ERROR read in data has nan!");
			data[count] = 0;
		}
		sum +=data[count];
	}


	double avg = sum/file_num;

	printf("variance func: file num is %d avf is %lf \n", file_num, avg);
	

	for(count = 0; count < file_num; count++)
	{
		var += (data[count] - avg ) * ( data[count] - avg );
	}


	var = var / file_num;

	return var;
}

