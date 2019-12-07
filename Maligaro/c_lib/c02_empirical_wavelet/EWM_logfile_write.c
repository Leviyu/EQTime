
#include "ESF.h"
/******************************************************************
 * This is a c script to write message to logfile 
******************************************************************/

int EWM_logfile_write(new_INPUT* my_input, char* message)
{

	fprintf(my_input->out_logfile,"%s\n",message);
	
	return 0;
}
