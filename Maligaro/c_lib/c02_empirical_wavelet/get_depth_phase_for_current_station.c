#include "hongyulib.h"
#include "ESF.h"

void get_depth_phase_for_current_station( new_RECORD* my_record)
{

	if(strstr( my_record->PHASE, "S") != NULL  )
		sprintf(my_record->depth_phase,"s%s", my_record->PHASE);
	else if(strstr( my_record->PHASE, "P") != NULL  )
		sprintf(my_record->depth_phase,"p%s", my_record->PHASE);

}
