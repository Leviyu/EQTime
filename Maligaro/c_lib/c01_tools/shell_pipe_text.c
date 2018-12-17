
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

int  shell_pipe_text(char* command, char* text_result)
{
	FILE* pipe;

	pipe = popen(command,"r");

	fscanf(pipe,"%s",text_result);
	pclose(pipe);

	return 0;
}



