#include "hongyulib.h"


/*************************************************************
* This C function returns the lines of input file
*	
*	Hongyu DATE: June 20 2014
*	Key words: line
*************************************************************/

int count_file_num(char *filename)
{
		char buffer[1024];
		 FILE *fp;
		 unsigned long lines = 0;

		 fp = fopen(filename, "r");
		 if (NULL == fp)
		 {
		  printf("fopen error!\n");
		 }
		 while(NULL != fgets(buffer, sizeof(buffer), fp))
		 {
		  lines++;
		 }

		 fclose(fp);

		 return lines;
}

