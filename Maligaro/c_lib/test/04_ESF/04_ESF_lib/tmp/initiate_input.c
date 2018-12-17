#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>
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
	my_input->event_file = (char*)malloc(sizeof(char)*200);
	my_input->polarity_file = (char*)malloc(sizeof(char)*200);
	my_input->phase = (char*)malloc(sizeof(char)*200);
	my_input->comp = (char*)malloc(sizeof(char)*200);
	my_input->velocity_or_displacement = (char*)malloc(sizeof(char)*200);



	
	return 0;
}
