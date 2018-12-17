#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>
#include "hongyulib.h"
/******************************************************************
 * This is a c script for initiate the input struct
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

int initiate_input(new_INPUT* my_input)
{
	// initiate my_input
	my_input->EQ = (char*)malloc(sizeof(char)*200);
	my_input->filter_flag = (char*)malloc(sizeof(char)*200);
	my_input->event_file = (char*)malloc(sizeof(char)*200);
	my_input->eventinfo_file = (char*)malloc(sizeof(char)*200);
	my_input->polarity_file = (char*)malloc(sizeof(char)*200);
	my_input->PHASE = (char*)malloc(sizeof(char)*200);
	my_input->COMP = (char*)malloc(sizeof(char)*200);
	my_input->velocity_or_displacement = (char*)malloc(sizeof(char)*200);
	my_input->SNR_phase = (char*)malloc(sizeof(char)*200);
	my_input->logfile = (char*)malloc(sizeof(char)*200);
	my_input->PWD = (char*)malloc(sizeof(char)*500);



	// get DIR from INFILE file
	char command[2000];
	sprintf(command , "cat ./INFILE |grep CURRENT_DIR |awk '{print $2}'");
	shell_pipe_text(command, my_input->PWD);

	
	return 0;
}
