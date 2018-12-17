#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sacio.h>
#include<ESF.h>
int read_input_info(new_INPUT* my_input)
{
	// read in input file
	FILE* in;
	in=fopen("input","r");
	char buff[200];
	char tmp[200];
	int N=200;

	// get event_file
	fgets(buff,N,in);	
	sscanf(buff,"%s %s",tmp, my_input->EQ); 			//line1
	fgets(buff,N,in);	
	sscanf(buff,"%s %s",tmp, my_input->event_file); 			//line2
	fgets(buff,N,in);	
	sscanf(buff,"%s %s",tmp, my_input->phase); 			//line3
	fgets(buff,N,in);	
	sscanf(buff,"%s %s",tmp, my_input->comp); 			//line4
	fgets(buff,N,in);	
	sscanf(buff,"%s %lf %lf",tmp, &my_input->long_beg, &my_input->long_len); 			//line5
	fgets(buff,N,in);	
	sscanf(buff,"%s %lf %lf",tmp, &my_input->phase_beg, &my_input->phase_len); 			//line6
	fgets(buff,N,in);	
	sscanf(buff,"%s %lf %lf",tmp, &my_input->noise_beg, &my_input->noise_len); 			//line7
	fgets(buff,N,in);	
	sscanf(buff,"%s %s",tmp, my_input->velocity_or_displacement); 			//line8
	fgets(buff,N,in);	
	sscanf(buff,"%s %lf %lf",tmp, &my_input->distmin, &my_input->distmax); 			//line9
	fgets(buff,N,in);	
	sscanf(buff,"%s %lf",tmp, &my_input->SNR_CUT); 			//line10
	fgets(buff,N,in);	
	sscanf(buff,"%s %lf",tmp, &my_input->CCC_CUT); 			//line11
	fgets(buff,N,in);	
	sscanf(buff,"%s %s",tmp, my_input->polarity_file); 			//line12
	fgets(buff,N,in);	
	sscanf(buff,"%s %lf",tmp, &my_input->delta); 			//line13



	fclose(in);

	// output input
	FILE* out;
	out=fopen("input.out","w");
	fprintf(out,"EQ is %s\n",my_input->EQ);
	fprintf(out,"eventfile %s\n",my_input->event_file);
	fprintf(out,"phase is  %s\n",my_input->phase);
	fprintf(out,"comp is %s\n",my_input->comp);
	fprintf(out,"ve_or_di is %s\n",my_input->velocity_or_displacement);
	fprintf(out,"long_beg_len is %lf %lf \n",my_input->long_beg, my_input->long_len);
	fprintf(out,"phase_beg_len is %lf %lf \n",my_input->phase_beg, my_input->phase_len);
	fprintf(out,"noise_beg_len is %lf %lf \n",my_input->noise_beg, my_input->noise_len);
	fprintf(out,"dist_min_max is %lf %lf \n",my_input->distmin, my_input->distmax);
	fclose(out);	
	




	return 0;
}
