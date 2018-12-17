
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
		//out2 = fopen(noise_output,"w");
		for(ista =0; ista < my_input->sta_num ; ista ++)
		{
			//if(my_record[ista].quality == -1)
				//continue;

			fprintf(out2,"%10s %15.10lf\n", my_record[ista].name, my_record[ista].SNR_noi*my_record[ista].amplitude);
			my_record[ista].SNR2 = my_record[ista].SNR;
		}
		fclose(out2);
	}
	//else if(strcmp(my_input->PHASE,"Sdiff") ==0 )
	//{
		//char noise_output[200];
		//sprintf(noise_output,"noise_output_for_S_Sdiff.data");
		//FILE* out2;
		//out2 = fopen(noise_output,"a");
		//for(ista =0; ista < my_input->sta_num ; ista ++)
		//{
			//if(my_record[ista].quality == -1)
				//continue;
			//fprintf(out2,"%10s %15.10lf\n", my_record[ista].name, my_record[ista].SNR_noi*my_record[ista].amplitude);
			//my_record[ista].SNR2 = my_record[ista].SNR;
		//}
		//fclose(out2);
	//}


	//for(ista =0; ista < my_input->sta_num ; ista ++)
	//{
//
		//if(my_record[ista].quality == -1)
			//continue;
			//if(strcmp(my_record[ista].PHASE,"ScS")==0 || strcmp(my_record[ista].PHASE,"SS")==0 || strcmp(my_record[ista].PHASE,"SSS")==0 ||
					//strcmp(my_record[ista].PHASE,"ScSScS")==0)
			//{
				//double SNR_noise_for_S = 0;
				//char command[500];
				//sprintf(command,"grep -w %s noise_output_for_S_Sdiff.data | awk 'NR==1 {print $2}'", my_record[ista].name);
				//SNR_noise_for_S = shell_pipe(command);
				//my_record[ista].SNR2 = my_record[ista].SNR_sig*my_record[ista].amplitude / SNR_noise_for_S;
			//}
//
	//}
		//printf("---> In getting misfit SNR for %s is %lf \n",my_record[ista].name, my_record[ista].SNR);


//puts("---> Update SNR and get  SNR2 for each recrod  for stretched DONE");

	return 0;
}
