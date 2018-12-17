#include<stdlib.h>
#include<stdio.h>
#include<hongyulib.h>
#include<math.h>
#include<string.h>
#include<ESF.h>
/******************************************************************
 * This is a c script to acquire the first EW for each phase of current EQ
 *
 *	Input: 
 *			my_record[]
 *			my_input
 *
 *
 *	Output:
 *			ES[] 			EW of current phase
 *
 *
 *	DATE:				Keywords:
 *	Reference:
******************************************************************/

int get_first_EW_for_phase(new_RECORD* my_record, new_INPUT* my_input, double* ES)
{
		// for ScS/ SS/ SSS/ ScSScS/ Sdiff, construct E.W. using S E.W.
		if(strcmp(my_input->PHASE,"S")==0 || strcmp(my_input->PHASE,"P")==0)
		{
			// for S phase, construct the E.W. by stacking records.
			get_first_ES(my_record,my_input,ES);
		}
		else 
		{
			read_in_modified_S_ES(my_input,ES);
		}

	return 0;
}


