#include<hongyulib.h>


/*************************************************************
* This C function returns flag whether if file exist
*  1 means exist
*  0 means not exist
*	
*	Hongyu DATE: June 20 2014
*	Key words: line
*************************************************************/

int file_exist(char *filename)
{

    FILE *file;
	file = fopen(filename,"r");
	if ( file != NULL )
	   {
			fclose(file);
      		return 1;
 		}
   return 0;
}

