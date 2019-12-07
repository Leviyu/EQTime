#include "ESF.h"

int find_instrument_type(new_RECORD* my_record, char* instru)
{
	char sac_file[200];
	int len,nerr,max=1000000,NPTS;
	float del,beg;
	float yarray[1000000];

	sprintf(sac_file,"%s.%s.%s.BH%s.sac",my_record->EQ, my_record->NET,my_record->name,my_record->COMP);

	rsac1(sac_file,yarray,&len, &beg, &del, &max, &nerr, strlen(sac_file));
	if( nerr == 0 )
	{
		strcpy(instru,"BH");
		return 0;
	}
	else
	{
	sprintf(sac_file,"%s.%s.%s.HH%s.sac",my_record->EQ, my_record->NET,my_record->name,my_record->COMP);

	rsac1(sac_file,yarray,&len, &beg, &del, &max, &nerr, strlen(sac_file));
	strcpy(instru,"HH");
	return 0;
	}
	// clean 
	return 0;
}

