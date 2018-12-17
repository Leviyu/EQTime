#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>
/******************************************************************
 * This is a c script to write message to logfile 
 *
 *	Input:
 *
 *
 *	Output:
 *
 *
 *	DATE:				Keywords:
 *	Reference:
******************************************************************/

int EWM_logfile_write(new_INPUT* my_input, char* message)
{

	fprintf(my_input->out_logfile,"%s\n",message);
	
	return 0;
}
