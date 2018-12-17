#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "hongyulibcpp.h"


/*************************************************************
* This C function returns the lines of input file
*	
*	Hongyu DATE: June 20 2014
*	Key words: line
*************************************************************/

int count_file_num(string name)
{
	int lines;
	ifstream myfile;
	string tmp;

	myfile.open(name.c_str());

	lines = 0;
	while(getline(myfile, tmp))
	{
		lines++;

	}

	myfile.close();

	return lines;
}

