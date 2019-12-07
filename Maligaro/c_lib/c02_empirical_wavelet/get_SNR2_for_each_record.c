
#include "hongyulib.h"
#include "ESF.h"
int get_SNR2_for_each_record(new_RECORD* my_record, new_INPUT* my_input)
{
puts("---> Update SNR and get  SNR2 for each recrod  for stretched");
	int ista;
	int count;
	double shell_pipe(char* command);

	// out put S/Sdiff noise 
	// if phase is S /Sdiff output noise
	if(strcmp(my_input->PHASE,"S") ==0 || strcmp(my_input->PHASE,"Sdiff") ==0 )
	{
		char noise_output[200];
		sprintf(noise_output,"noise_output_for_S_Sdiff.data");
		FILE* out2;
		out2 = fopen(noise_output,"a");
		for(ista =0; ista < my_input->sta_num ; ista ++)
		{
			fprintf(out2,"%10s %15.10lf\n", my_record[ista].name, my_record[ista].SNR_noi*my_record[ista].amplitude);
			my_record[ista].SNR2 = my_record[ista].SNR;
		}
		fclose(out2);
	}

	return 0;
}
