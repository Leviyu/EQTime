
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

int shell_pipe_command(char* command)
{
	FILE* pipe;

	pipe = popen(command,"r");
	pclose(pipe);

	return 0;
}



