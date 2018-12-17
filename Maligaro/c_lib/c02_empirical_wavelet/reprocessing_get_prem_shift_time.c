#include "ESF.h"
// if reprocessing flag is 1, we read in and store prem_shift time 
// from previously picked records



void reprocessing_get_prem_shift_time(new_RECORD* my_record, new_INPUT* my_input)
{
	int count = 0;
	int file_num = 0;
	char buff[400];
	char read_tmp[400];
	int N = 1000;

	my_input->Reprocessing_shift = 13;
	my_input->npts_Reprocessing_shift = (int)(my_input->Reprocessing_shift / my_record->delta);


	int ista = 0;
	for(ista = 0; ista < my_input->sta_num ; ista++)
	{
		
		my_record[ista].dt_picked_shift = 0;
		if(my_input->Reprocessing_Flag != 1)
			continue;
		// read INFILE information
		FILE* in2;
		char INFILE_name2[200];
		sprintf(INFILE_name2,"list.Reprocessing_Flag.%s.%s",my_input->EQ,my_input->PHASE);
		in2=fopen(INFILE_name2,"r");
		file_num = count_file_num(INFILE_name2);
		//printf(" file name is %s file num is %d \n", INFILE_name2, file_num);
		for(count = 0;count < file_num; count ++)
		{
			fgets(read_tmp,N, in2);              // line
			//printf(" read tmp %s \n",read_tmp);

			sscanf(read_tmp,"%s" ,buff);
			if (strcmp(buff,my_record[ista].name) == 0)
			{
				sscanf(read_tmp,"%s %lf %d %lf " ,buff, &my_record[ista].dt_picked_shift,
						&my_record[ista].polar_flag, &my_record[ista].polarity);
				//printf("reprocessing sta %s is shifted %lf \n", my_record[ista].name,
						//my_record[ista].dt_picked_shift);
				break;
			}
		}
		//printf("sta %s dt shift %lf \n", my_record[ista].name, my_record[ista].dt_picked_shift);
		fclose(in2);
	}

	return;
}

