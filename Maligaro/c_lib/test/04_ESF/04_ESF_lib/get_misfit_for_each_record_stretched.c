#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<hongyulib.h>
#include<ESF.h>

int get_misfit_for_each_record_stretched(new_RECORD* my_record, new_INPUT* my_input)
{
puts("---> Get Misfit for each recrod and get the SNR for stretched");
	int ista;
	int count;

	for(ista =0; ista < my_input->sta_num ; ista ++)
	{
		int npts_phase = (int)(my_input->phase_len / my_input->delta);

		if(my_record[ista].quality == -1)
			continue;

//printf("--->processing for sta %s \n",my_record[ista].name);
		//calculate the misfit from ONSET to ENDSET with an additional 6sec extension
		
		double npts_ONSET,npts_ENDSET;
		double extra_time = 3;	//sec
		npts_ONSET =(int) ( ( my_record[ista].ONSET - 3 - my_record[ista].phase_beg ) / my_input->delta );
		npts_ENDSET =(int) ( ( my_record[ista].ENDSET + 3 - my_record[ista].phase_beg ) / my_input->delta );

		if(npts_ONSET < 0)
			npts_ONSET = 0;
		if(my_record[ista].ENDSET + 3 - my_record[ista].phase_beg > my_record[ista].phase_len )
			npts_ENDSET = (int) ( my_record[ista].phase_len / my_input->delta);


		//for empirical wavelet and for records
		double misfit_ES=0;
		double misfit_record=0;
		double misfit = 0;
		double AMP_ES = amplitude(my_record[ista].ES_win, npts_phase);
		double AMP_record = amplitude(my_record[ista].phase_win, npts_phase);
//printf("sta %s AMP ES %15.10lf record %15.10lf \n", my_record[ista].name, AMP_ES, AMP_record);
		if(AMP_ES ==0 || AMP_record == 0)
		{
			my_record[ista].quality = -1;
			continue;
		}
		for(count = npts_ONSET; count < npts_ENDSET; count++)
		{
			//misfit_ES +=  fabs( my_record[ista].ES_win[count] );
			//misfit_record +=  fabs( my_record[ista].phase_win[count] );
			misfit += fabs( my_record[ista].ES_win[count]/AMP_ES -  my_record[ista].phase_win[count] / AMP_record);
		}

		//average it with the number of points
		//misfit_ES = misfit_ES / (npts_ENDSET - npts_ONSET);
		//misfit_record = misfit_record / (npts_ENDSET - npts_ONSET);
		misfit = misfit / (npts_ENDSET - npts_ONSET);

		my_record[ista].misfit = misfit;

		// using misfit window to set SNR_new
		double phase_signal=0;
		double noise_signal=0;
		int i;
		for(i=npts_ONSET;i<npts_ENDSET;i++)
		{
			phase_signal += fabs(my_record[ista].phase_win[i]);
		}

		for(i=0;i<my_record[ista].npts_noise;i++)
		{
			noise_signal += fabs(my_record[ista].noise_win[i]);
		}
		//printf("noise signal is %lf phase signa; is %lf npts noise phase %d %d\n", noise_signal, phase_signal, my_record[ista].npts_noise,my_record[ista].npts_phase);
		if( noise_signal == 0 )
		{
			puts("ERROR: noise_signal is 0 SNR problem!");
			my_record[ista].quality = -1;
			//getchar();
			continue;
		}
		else
		{
			double SNR_sig = phase_signal / (npts_ENDSET - npts_ONSET);
			double SNR_noi = noise_signal / (my_record[ista].noise_len/my_input->delta);
			//printf("SNR sig is %lf SNR nos is %lf \n", SNR_sig, SNR_noi);
			double SNR = SNR_sig/SNR_noi;
			my_record[ista].SNR_sig = SNR_sig;
			my_record[ista].SNR_noi = SNR_noi;
			my_record[ista].SNR = SNR;

			double SNR_noise_for_S = 0;
			char command[300];
			sprintf(command,"grep -w %s noise_output_for_S_Sdiff.data | awk 'NR==1 {print $2}'", my_record[ista].name);
			SNR_noise_for_S = shell_pipe(command);
			//printf("SNR_sig is %lf SNR_noise for S is %lf \n", SNR_sig, SNR_noise_for_S);
			my_record[ista].SNR2 = SNR_sig / SNR_noise_for_S;

		}

		//printf("---> In getting misfit SNR for %s is %lf \n",my_record[ista].name, my_record[ista].SNR);
	}


	// if phase is S /Sdiff output noise
	char noise_output[200];
	sprintf(noise_output,"noise_output_for_S_Sdiff.data");
	FILE* out2;
	if(strcmp(my_record[ista].PHASE,"S") ==0 )
	{
		out2 = fopen(noise_output,"w");
		for(ista =0; ista < my_input->sta_num ; ista ++)
		{
			if(my_record[ista].quality == -1)
				continue;
			fprintf(out2,"%10s %15.10lf\n", my_record[ista].name, my_record[ista].SNR_noi);
			my_record[ista].SNR2 = my_record[ista].SNR;
		}
	}
	else if(strcmp(my_record[ista].PHASE,"Sdiff") ==0 )
	{
		out2 = fopen(noise_output,"a");
		for(ista =0; ista < my_input->sta_num ; ista ++)
		{
			if(my_record[ista].quality == -1)
				continue;
			fprintf(out2,"%10s %15.10lf\n", my_record[ista].name, my_record[ista].SNR_noi);
			my_record[ista].SNR2 = my_record[ista].SNR;
		}
	}
	fclose(out2);



	// output ONSET ENDSET for plotting
	FILE* out;
	char out_name[200];
	sprintf(out_name,"%s_ONSET_ENDSET.out",my_input->PHASE);
	out = fopen(out_name,"w");
	for(ista =0; ista < my_input->sta_num; ista ++)
	{
		if(my_record[ista].SNR != my_record[ista].SNR )
			continue;
		fprintf(out,"%7s %7.3lf %7.3lf\n", my_record[ista].name,  my_record[ista].ONSET, my_record[ista].ENDSET);
	}
	fclose(out);

	return 0;
}
