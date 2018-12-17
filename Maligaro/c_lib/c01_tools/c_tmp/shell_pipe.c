#include "hongyulib.h"
/******************************************************************
 * This is a c script for using shell command in C using PIPE
 *
 *	Input: command 
 *
 *
 *
 *	DATE:Sep 26	 2014			Keywords:
******************************************************************/

double shell_pipe(char* command)
{
	FILE* pipe;
	double result;

	pipe = popen(command,"r");

	fscanf(pipe,"%lf",&result);
	pclose(pipe);

	return result;
}



