#include "forward_tomography.h"



void virtual_station::initiate(big_new_record* my_big_record)
{
	//cout << " --> VS is destructed"  << endl;

	// specify the storage array for eventinfo and eventStation
	int station_max = 1000;
	this->eventinfo_index = 0;
	this->eventinfo_index_array.resize(station_max);
	this->eventStation_index = 0;
	this->eventStation_index_array.resize(station_max);

	//cout << " new virtual_station is declared! " << endl;
	// distribute space for record tag arrat
	
	int record_max = 1000;
	this->record_tag.resize(record_max);
	this->npts_record_sum = 0;
	//this->long_win = new double[MAX];
	//this->long_win.resize(MAX);
	this->phase_win.resize(500);
	this->eq_skip_flag = 0;
	this->sta_skip_flag = 0;

	this->eventStation_index = 0;
	this->LONG_BEG = my_big_record->long_beg;
	this->LONG_LEN = my_big_record->long_len;
	this->long_npts = (int)(my_big_record->long_len / my_big_record->delta);
	this->long_win.resize(this->long_npts);
	this->phase_beg = my_big_record->phase_beg;
	this->phase_len = my_big_record->phase_len;
	this->noise_beg = my_big_record->noise_beg;
	this->noise_len = my_big_record->noise_len;
	this->delta = my_big_record->delta;
	this->MASK_MIN = my_big_record->MASK_MIN;
	this->MASK_MAX = my_big_record->MASK_MAX;

}


void virtual_station::destruct()
{

	// erase eventinfo
	this->eventinfo_index_array.erase(this->eventinfo_index_array.begin(),
			this->eventinfo_index_array.end());
	// erase eventStation
	this->eventStation_index_array.erase(this->eventStation_index_array.begin(),
			this->eventStation_index_array.end());

	// erase long win
	this->long_win.erase(this->long_win.begin(),this->long_win.end());
	// erase phase_win
	this->phase_win.erase(this->phase_win.begin(), this->phase_win.end());

	// erase record tag
	this->record_tag.erase(this->record_tag.begin(),this->record_tag.end());

}


void virtual_station::initiate_grid()
{
	cout << " --> Build Virtual Station grid network" << endl;

	double ilat, ilon;
	double current_lat;

	// grid radius in km
	this->grid_radius = this->VS_LATITUDE_INC * 110;


	for(ilat = -89; ilat < 89; ilat+= this->VS_LATITUDE_INC)
	{
		current_lat = ilat;


	}
}


void virtual_station::get_grid_dist(const virtual_station& EQ_grid, const virtual_station& STA_grid)
{
	// use ilat_eq/ilon_eq/ilat_sta/ilon_sta to get the
	// eq_lat eq_lon and sta_lat sta_lon
	double eq_lat = EQ_grid.grid_lat;
	double eq_lon = EQ_grid.grid_lon;
	
	double sta_lat = STA_grid.grid_lat;
	double sta_lon = STA_grid.grid_lon;

	this->eq_lat = eq_lat;
	this->eq_lon = eq_lon;
	this->sta_lat = sta_lat;
	this->sta_lon = sta_lon;


	double grid_dist = dist_A_B(eq_lat ,eq_lon, sta_lat , sta_lon);
	grid_dist = grid_dist/111;
	this->grid_dist = grid_dist;

	//cout << " --> get grid distance " << eq_lat << " "<< eq_lon << " "<<sta_lat << " "
		//<< sta_lon << " distance is " << this->grid_dist << endl;
}
void virtual_station::get_grid_dist_final()
{
	cout << "--> get_grid_dist_final "<< endl;


	double grid_dist = dist_A_B(this->eq_lat ,this->eq_lon, this->sta_lat , this->sta_lon);
	grid_dist = grid_dist/111;
	this->grid_dist = grid_dist;

	//cout << " --> get grid distance " << eq_lat << " "<< eq_lon << " "<<sta_lat << " "
		//<< sta_lon << " distance is " << this->grid_dist << endl;
}


// For each VS, we check all possible traffic for it and store 
// phases that are within current phase into a single file
// the file would be used to make traffic flag in catalog plots
void virtual_station::get_traffic_time()
{
	
	//double traffic_time_range = 100;



	//vector<string> this->traffic_phase_list;
	//this->traffic_phase_list.resize(20);

	if( this->PHASE.compare("S") == 0)
	{
		this->traffic_phase_list.push_back("ScS");
		this->traffic_phase_list.push_back("sS");
		this->traffic_phase_list.push_back("SKS");
		this->traffic_phase_list.push_back("SKS");
		this->traffic_phase_list.push_back("sSKS");
	}
	else if( this->PHASE.compare("ScS") ==0 )
	{
		this->traffic_phase_list.push_back("S");
		this->traffic_phase_list.push_back("sS");
		this->traffic_phase_list.push_back("SS");
		this->traffic_phase_list.push_back("sSS");
		this->traffic_phase_list.push_back("sScS");
	}
	else if( this->PHASE.compare("Sdiff")  ==0)
	{
		this->traffic_phase_list.push_back("sSdiff");
		this->traffic_phase_list.push_back("SKS");
		this->traffic_phase_list.push_back("sSKS");
		this->traffic_phase_list.push_back("SKKS");
		this->traffic_phase_list.push_back("sSKKS");
	}
	else if( this->PHASE.compare("SS") ==0 )
	{
		this->traffic_phase_list.push_back("ScS");
		this->traffic_phase_list.push_back("sScS");
		this->traffic_phase_list.push_back("sS");
	}
	else if( this->PHASE.compare("SSS") ==0 )
	{
		this->traffic_phase_list.push_back("ScS");
		this->traffic_phase_list.push_back("sScS");
		this->traffic_phase_list.push_back("ScSScS");
		this->traffic_phase_list.push_back("sScSScS");
		this->traffic_phase_list.push_back("sSSS");
		this->traffic_phase_list.push_back("sSS");
	}
	else if( this->PHASE.compare("ScSScS") ==0 )
	{
		this->traffic_phase_list.push_back("SS");
		this->traffic_phase_list.push_back("sSS");
		this->traffic_phase_list.push_back("SSS");
		this->traffic_phase_list.push_back("sSSS");
		this->traffic_phase_list.push_back("S4");
		this->traffic_phase_list.push_back("sS4");
		this->traffic_phase_list.push_back("S5");
		this->traffic_phase_list.push_back("sScSScS");
	}
	else if( this->PHASE.compare("S3m") ==0 )
	{
		this->traffic_phase_list.push_back("S4");
		this->traffic_phase_list.push_back("sS4");
		this->traffic_phase_list.push_back("S5");
		this->traffic_phase_list.push_back("sS5");
		this->traffic_phase_list.push_back("S6");
		this->traffic_phase_list.push_back("sS6");
		this->traffic_phase_list.push_back("ScS3");
		this->traffic_phase_list.push_back("sS3m");
	}
	else if( this->PHASE.compare("S4") ==0 )
	{
		this->traffic_phase_list.push_back("ScSScS");
		this->traffic_phase_list.push_back("sScSScS");
		this->traffic_phase_list.push_back("ScS3");
		this->traffic_phase_list.push_back("sScS3");
	}
	else if( this->PHASE.compare("S4m") ==0 )
	{
		this->traffic_phase_list.push_back("S5");
		this->traffic_phase_list.push_back("S6");
		this->traffic_phase_list.push_back("ScS4");
		this->traffic_phase_list.push_back("sScS4");
		this->traffic_phase_list.push_back("ScS5");
		this->traffic_phase_list.push_back("sScS5");
	}
	else if( this->PHASE.compare("S5") ==0 )
	{
		this->traffic_phase_list.push_back("S3m");
		this->traffic_phase_list.push_back("S4m");
		this->traffic_phase_list.push_back("ScSScS");
		this->traffic_phase_list.push_back("sScSScS");
		this->traffic_phase_list.push_back("ScS3");
		this->traffic_phase_list.push_back("sScS3");
		this->traffic_phase_list.push_back("sS5");
	}
	else if( this->PHASE.compare("S5m") ==0 )
	{
		this->traffic_phase_list.push_back("ScS4");
		this->traffic_phase_list.push_back("sScS4");
		this->traffic_phase_list.push_back("ScS5");
		this->traffic_phase_list.push_back("sScS5");
		this->traffic_phase_list.push_back("S6");
		this->traffic_phase_list.push_back("sS5m");
	}
	else if( this->PHASE.compare("S6") ==0 )
	{
		this->traffic_phase_list.push_back("S3m");
		this->traffic_phase_list.push_back("S4m");
		this->traffic_phase_list.push_back("S5m");
		this->traffic_phase_list.push_back("ScSScS");
		this->traffic_phase_list.push_back("sScSScS");
		this->traffic_phase_list.push_back("ScS3");
		this->traffic_phase_list.push_back("sScS3");
	}
	else if( this->PHASE.compare("ScS3") ==0 )
	{
		this->traffic_phase_list.push_back("SSS");
		this->traffic_phase_list.push_back("sSSS");
		this->traffic_phase_list.push_back("S4");
		this->traffic_phase_list.push_back("sS4");
		this->traffic_phase_list.push_back("S5");
		this->traffic_phase_list.push_back("sS5");
		this->traffic_phase_list.push_back("S6");
		this->traffic_phase_list.push_back("sScS3");
	}
	else if( this->PHASE.compare("ScS3m") ==0 )
	{
		this->traffic_phase_list.push_back("ScS4");
		this->traffic_phase_list.push_back("sScS4");
		this->traffic_phase_list.push_back("S4m");
		this->traffic_phase_list.push_back("S5m");
		this->traffic_phase_list.push_back("S6m");
		this->traffic_phase_list.push_back("sScS5");
		this->traffic_phase_list.push_back("sScS3m");
	}
	else if( this->PHASE.compare("ScS4") ==0 )
	{
		this->traffic_phase_list.push_back("ScS3m");
		this->traffic_phase_list.push_back("sScS3m");
		this->traffic_phase_list.push_back("S3m");
		this->traffic_phase_list.push_back("S4m");
		this->traffic_phase_list.push_back("S5m");
		this->traffic_phase_list.push_back("sScS4");
	}
	else if( this->PHASE.compare("ScS4m") ==0 )
	{
		this->traffic_phase_list.push_back("ScS5");
		this->traffic_phase_list.push_back("sScS5");
		this->traffic_phase_list.push_back("ScS6");
		this->traffic_phase_list.push_back("sScS6");
		this->traffic_phase_list.push_back("S5m");
		this->traffic_phase_list.push_back("S6m");
		this->traffic_phase_list.push_back("sScS4m");
	}
	else if( this->PHASE.compare("ScS5") ==0 )
	{
		this->traffic_phase_list.push_back("ScS3m");
		this->traffic_phase_list.push_back("sScS3m");
		this->traffic_phase_list.push_back("ScS4m");
		this->traffic_phase_list.push_back("sScS4m");
		this->traffic_phase_list.push_back("S4");
		this->traffic_phase_list.push_back("S5");
		this->traffic_phase_list.push_back("S6");
		this->traffic_phase_list.push_back("sScS5");
	}
	else if( this->PHASE.compare("ScS5m") ==0 )
	{
		this->traffic_phase_list.push_back("sScS5");
		this->traffic_phase_list.push_back("ScS6");
		this->traffic_phase_list.push_back("sScS6");
		this->traffic_phase_list.push_back("sScS5m");
	}
	else if( this->PHASE.compare("P") ==0 )
	{
		this->traffic_phase_list.push_back("PcP");
		this->traffic_phase_list.push_back("pP");
	}
	else if( this->PHASE.compare("Pdiff") ==0 )
	{
		this->traffic_phase_list.push_back("pPdiff");
	}

	//cout << " current traffic vector size is " << this->traffic_phase_list.size() << endl;
	//cout << this->traffic_phase_list.at(0)<< endl;

	// 2. loop through each phase and calculate the travel time for each phase
	//cout << " current VS traffic is / EQ name for vs is "<< this->EQ << endl;
	// and write to file
	ofstream myfile;
	string traffic_name = "traffic."+std::to_string(this->ivs);
	myfile.open(traffic_name);
	cout << " lets get the fucking traffic travel time " << endl;
	for(auto i:this->traffic_phase_list)
	{
		cout <<" ==> Working on "<<  i << endl;
		string command = "get_taup_time_2 " + this->my_big_record->PHASE + " "
			+ std::to_string(this->eq_lat) + " "
			+ std::to_string(this->eq_lon) + " " 
			+ std::to_string(this->eq_dep) + " "
			+ std::to_string(this->sta_lat) + " "
			+ std::to_string(this->sta_lon) + " "
			+ i;
		cout << "command is "<< command << endl;

		double prem_traffic = atof(exec(command).c_str());
		this->traffic_phase_time.push_back(prem_traffic);


		// manage condition when we dont show certain traffic
		// For S, if traffic is ScS and distance 
		int pass = 0;
		if( this->my_big_record->PHASE.compare("S") == 0
				&& i.compare("ScS") == 0 && this->grid_dist > 83 )
			pass = 1;
		// when traffic is SKS/SKKS, make sure distance is within 80-125
		else if(  ( i.compare("SKS") == 0 || 
					i.compare("SKKS") == 0 ||
					i.compare("sSKS") == 0 ||
					i.compare("sSKKS") == 0 ) && ( this->grid_dist < 80 || this->grid_dist > 125 ) )
			pass = 1;

		if(pass != 1)
			myfile<<" PHASE "+ i << " dt "<< prem_traffic << endl;


		// judge quality based on traffic 
		// make sure ScS is not S`s traffic phase
		if( this->my_big_record->PHASE.compare("S") == 0
				&& i.compare("ScS") == 0)
			continue;
		// make sure SKS is not traffic phase
		if ( i.compare("SKS") == 0 || 
					i.compare("SKKS") == 0 ||
					i.compare("sSKS") == 0 ||
					i.compare("sSKKS") == 0 )
			continue;
		// make sure depth phase is not traffic
		string depth_phase = "s"+this->my_big_record->PHASE;
		if( depth_phase.compare(i) == 0)
			continue;

		// now lets check each traffic and if traffic is too close 
		// then we make the quality of it -1
		if( prem_traffic > this->MASK_MIN and prem_traffic < this->MASK_MAX ) 
		{
			string shallow_phase = "s"+this->PHASE;
			if( shallow_phase.compare(i) != 0)
			{
					this->quality_flag = -1;
			}
		}

	}
	cout << "traffic time size "<< this->traffic_phase_time.size() << endl;

	





}


// This function first read in S_ES file 
// and then t*(S_ES) to find the best fit to virtual stack
// then find the best fit gaussian to t*(S_ES) and set ONSET on gaussian
int virtual_station::find_stack_ONSET_time()
{
	cout << " find_stack_ONSET_time "<< endl;

	// define traffic file for current virtual station
	this->get_traffic_time();
	



	string S_ES_DIR = this->my_big_record->S_ES_DIR;
	this->S_ES_file = S_ES_DIR+"/S_ES."+this->EQ;
	//cout << " S_ES FILE is " << this->S_ES_file << endl;

	// check if S_ES exist
	if(! is_file_exist(this->S_ES_file)  )
	{
		cout  << " ERROR S_ES file for event does not exist" << endl;
		return 1;
	}
		


	// 1 . read in S_ES
	ifstream myfile;
	myfile.open(this->S_ES_file.c_str());
	int LINE = count_file_num(this->S_ES_file);
	int count;
	double X_TMP[LINE];
	double S_ES[LINE];

	//cout << " S_ES is "<< this->S_ES_file<< " file count is "<< LINE << endl;
	//cout << " Stack SNR is " << this->stack_SNR << endl;

	for(count = 0; count < LINE; count++)
		myfile >> X_TMP[count] >> S_ES[count];
	myfile.close();

	// find the maximum location of long win
	int max_loc = 0;
	double amp = 0;
	
	//int npts_beg = (int) ( -15 / this->delta );
	//int npts_end = (int) ( 23 / this->delta );
	int npts_beg = 0;
	int npts_end = (int)(this->LONG_LEN / this->delta);
	
	
	for(count = npts_beg ; count < npts_end ; count++  )
	{
		double time_rel_PREM = this->LONG_BEG + count * this->delta;
		if(time_rel_PREM < -25 || time_rel_PREM > 25)
			continue;

		if( this->long_win[count] > amp )
		{
			amp = this->long_win[count];
			max_loc = count;
		}
	}
	cout << " maxloc is  "<< max_loc << " amp is "<< amp << endl;

	/*
	// we use a smaller window to find the phase max loc
	int long_zoom_beg_npts = (int) ( (fabs(this->LONG_BEG) - 20 ) / this->delta);
	double long_zoom_len = 40;
	int npts_long_zoom = long_zoom_len/this->delta;
	double long_zoom[npts_long_zoom];
	for(count = 0; count < npts_long_zoom ; count++ )
	{
		long_zoom[count] = this->long_win[ long_zoom_beg_npts + count];
	}

	amplitudeloc( long_zoom  , npts_long_zoom, & max_loc, &amp,1 );
	if( max_loc > 50000 || max_loc < -50000 )
		max_loc = 0;
	if ( long_zoom_beg_npts  > 50000 || long_zoom_beg_npts < -50000 )
		long_zoom_beg_npts = 0;
	max_loc = max_loc + long_zoom_beg_npts;
	*/

	// cross-correlation phase_window with S_ES to find the proper location of
	// phase window

	// our phase window is defined around the peak value in un-masked area
	double phase_start_time = this->LONG_BEG + (max_loc - LINE/2) * this->delta;
	this->WIN_BEG = phase_start_time;
	this->WIN_LEN = LINE * this->delta;
	//double phase_start_time = 0;
	cout << "phase start time is "<< phase_start_time << endl;
	double phase_win_max = 0;
	for(count = 0 ; count < LINE ; count++)
	{
		int npts_tmp = (int) (max_loc - LINE/2) + count;
		if(npts_tmp >= this->long_npts || npts_tmp < 0 )
			this->phase_win[count] = 0;
		else
			this->phase_win[count] = this->long_win[npts_tmp];
		if(this->phase_win[count] > phase_win_max)
			phase_win_max = this->phase_win[count];
	}



	if(phase_win_max == 0)
	{
		cout << " ERROR phase_win_max is 0 "<< endl;
		return 1;
	}

	// use phase_win_max to normalize long_win and long_win_orig
	int long_npts = (int)(this->LONG_LEN / this->delta);
	for(count = 0; count < long_npts; count ++)
	{
		this->long_win[count] /= phase_win_max;
		this->long_win_orig[count] /= phase_win_max;
	}

	//int normalize_array_with_flag(double* array, int len, int flag)
	// normalize phase_win
	normalize_array_with_flag(&this->phase_win[0], LINE, 1);

	int max_loc_orig;
	double amp_orig;
	amplitudeloc(&this->long_win_orig[0],this->long_npts,&max_loc_orig, &amp_orig,1);
	// if long_amp > 2* phase_amp
	if( amp_orig > 2)
		this->amp_scale = 0.5;
	else
		this->amp_scale = 1;



	
	// cross-correlation phase_wind with S_ES to find best CCC match
	// 1. construct main lobe with phase window
	//int construct_array_with_main_lobe(double* array_in, int* npts_in, double* array_out)
	////double tmp_phase_win[LINE];
	//double tmp_phase_win[2000];
	//construct_array_with_main_lobe(phase_win,&LINE,tmp_phase_win);
	////double tmp_S_ES[LINE];
	//double tmp_S_ES[2000];
	//construct_array_with_main_lobe(S_ES,&LINE,tmp_S_ES);
	//int CCC( double* x, int npts_x, double* y, int npts_y, int*shift, double* ccc, int flag)
	// ccc between S_ES and phase_win to find best match
	//int npts_shift = 0;
	//double tmp_ccc = 0;
	//CCC(tmp_S_ES,LINE,tmp_phase_win,LINE,&npts_shift , &tmp_ccc,1);
	////CCC(tmp_S_ES,LINE,tmp_phase_win,LINE, &npts_shift, &tmp_ccc,1);
	//CCC(S_ES,LINE,phase_win,LINE,&npts_shift,&tmp_ccc,1);
	//cout << " sta index "<< this->my_big_record->my_vs_index 
		//<<" npts shift is "<< npts_shift<<endl;

	// shift phase window
	//for(count = 0 ; count < LINE ; count++)
	//{
		//int npts_tmp = (int) (max_loc - LINE/2) + count + npts_shift;
		//if(npts_tmp >= this->long_npts || npts_tmp < 0 )
			//phase_win[count] = 0;
		//else
			//phase_win[count] = this->long_win[npts_tmp];
	//}



	double xx[LINE];
	int vs_index = this->my_big_record->my_vs_index;
	//string phase_win_file = "phase_win.vs."+std::to_string(vs_index);
	//for(count = 0 ; count < LINE; count++)
		//xx[count] = phase_start_time+  count  *this->delta;
	//output_array2(phase_win_file,xx,&this->phase_win[0],LINE,0 );


	// 2. t* S_ES to fit virtual stack
	double coeff_min = 0.5;
	double coeff_max = 30;
	double coeff_delta = 3;
	double best_ccc;
	double best_coeff;
	int best_time_shift;
	double best_ES[LINE];
	double best_ES_gau[LINE];
	stretch_record_find_best_match_for_given_interval( S_ES,&this->phase_win[0], LINE,  coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff, & best_time_shift, best_ES ,1);

	coeff_min = best_coeff - 1.5;
	coeff_max = best_coeff + 1.5;
	coeff_delta = 1;
	stretch_record_find_best_match_for_given_interval( S_ES,&this->phase_win[0], LINE,  coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff, & best_time_shift, best_ES ,1);
	//cout << " best ccc "<< best_ccc << " best coeff "<< best_coeff << " best time shift "<< best_time_shift << endl;

	coeff_min = best_coeff - 0.5;
	coeff_max = best_coeff + 0.5;
	coeff_delta = 0.25;
	stretch_record_find_best_match_for_given_interval( S_ES,&this->phase_win[0], LINE,  coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff, & best_time_shift, best_ES ,1);
	//cout << " best ccc "<< best_ccc << " best coeff "<< best_coeff << " best time shift "<< best_time_shift << endl;
	coeff_min = best_coeff - 0.15;
	coeff_max = best_coeff + 0.15;
	coeff_delta = 0.1;
	stretch_record_find_best_match_for_given_interval( S_ES,&this->phase_win[0], LINE,  coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff, & best_time_shift, best_ES ,1);

	coeff_min = best_coeff - 0.05;
	coeff_max = best_coeff + 0.05;
	coeff_delta = 0.025;
	stretch_record_find_best_match_for_given_interval( S_ES,&this->phase_win[0], LINE,  coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff, & best_time_shift, best_ES ,1);
	coeff_min = best_coeff - 0.02;
	coeff_max = best_coeff + 0.02;
	coeff_delta = 0.01;
	stretch_record_find_best_match_for_given_interval( S_ES,&this->phase_win[0], LINE,  coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff, & best_time_shift, best_ES ,1);
	cout << " best ccc "<< best_ccc << " best coeff "<< best_coeff << " best time shift "<< best_time_shift << endl;

	this->tstar_ccc = best_ccc;
	this->tstar_factor = best_coeff;

	// for situations where t* is very small, which suggest that 
	// the SEW should be shrinked to match record
	// we apply stretching algorithm to it
	if( best_coeff < 100) 
	{
		coeff_min = 0.3;
		coeff_max = 8;
		coeff_delta = 1;
		stretch_record_find_best_match_for_given_interval( S_ES,&this->phase_win[0], LINE, 
				coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff, & best_time_shift, best_ES ,2);

		coeff_min = best_coeff - 0.5;
		coeff_max = best_coeff + 0.5;
		coeff_delta = 0.25;
		stretch_record_find_best_match_for_given_interval( S_ES,&this->phase_win[0], LINE,  
				coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff, & best_time_shift, best_ES ,2);

		coeff_min = best_coeff - 0.125;
		coeff_max = best_coeff + 0.125;
		coeff_delta = 0.05;
		stretch_record_find_best_match_for_given_interval( S_ES,&this->phase_win[0], LINE,  
				coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff, & best_time_shift, best_ES ,2);

		coeff_min = best_coeff - 0.025;
		coeff_max = best_coeff + 0.025;
		coeff_delta = 0.01;
		stretch_record_find_best_match_for_given_interval( S_ES,&this->phase_win[0], LINE,  
				coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff, & best_time_shift, best_ES ,2);

		this->stretch_ccc = best_ccc;
		this->stretch_coeff = best_coeff;
	}
	else
	{
		this->stretch_ccc = 0;
		this->stretch_coeff = 0;
	}
	//cout << " --> best coff "<< this->stretch_coeff << " ccc "<< this->stretch_ccc << endl;

	
	// outout t* E_ES
	//best_time_shift = 0;
	string current_tstar_ES = "tstar_ES."+std::to_string(vs_index);
	double yy[LINE];
	for(count = 0 ; count < LINE; count++)
	{
		xx[count] = phase_start_time+ ( count - best_time_shift)  *this->delta;
		yy[count] = best_ES[count] * this->amp_scale;
	}

	//output_array2(current_tstar_ES,xx,best_ES,LINE,0 );
	output_array2(current_tstar_ES,xx,yy,LINE,1);

	// shift phase win 
	for(count = 0 ; count < LINE ; count++)
	{
		int npts_tmp = (int) (max_loc - LINE/2) + count - best_time_shift;
		if(npts_tmp >= this->long_npts || npts_tmp < 0 )
			this->phase_win[count] = 0;
		else
			this->phase_win[count] = this->long_win[npts_tmp];
	}
	// create a phase_win_orig from long_win_orig
	// this is to make sure in phase_win_orig, there is no masked 
	// window
	double phase_win_orig[LINE];
	for(count = 0 ; count < LINE ; count++)
	{
		int npts_tmp = (int) (max_loc - LINE/2) + count - best_time_shift;
		if(npts_tmp >= this->long_npts || npts_tmp < 0 )
			phase_win_orig[count] = 0;
		else
			phase_win_orig[count] = this->long_win_orig[npts_tmp];
	}

	int shift_time_tmp = best_time_shift;


	// 3. find best fit gaussian 
	coeff_min = 0.1;
	coeff_max = 60;
	coeff_delta = 5;
	stretch_gaussian_find_best_match_for_given_interval( best_ES, LINE,  coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff, & best_time_shift, best_ES_gau );
	coeff_min = best_coeff - 3;
	coeff_max = best_coeff + 3;
	coeff_delta = 1;
	stretch_gaussian_find_best_match_for_given_interval( best_ES, LINE,  coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff, & best_time_shift, best_ES_gau );
	coeff_min = best_coeff-0.5;
	coeff_max = best_coeff + 0.5;
	coeff_delta = 0.3;
	stretch_gaussian_find_best_match_for_given_interval( best_ES, LINE,  coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff, & best_time_shift, best_ES_gau );
	coeff_min = best_coeff-0.15;
	coeff_max = best_coeff + 0.15;
	coeff_delta = 0.1;
	stretch_gaussian_find_best_match_for_given_interval( best_ES, LINE,  coeff_min, coeff_max, coeff_delta, &best_ccc, &best_coeff, & best_time_shift, best_ES_gau );
	cout << "gau best ccc "<< best_ccc << " best coeff "<< best_coeff << " best time shift "<< best_time_shift << endl;

	string current_gau = "gau_ES."+std::to_string(vs_index);
	//best_time_shift = 0;
	cout << " shift time "<< best_time_shift << endl;
	for(count = 0 ; count < LINE; count++)
	{
		xx[count] = phase_start_time+ ( count - best_time_shift - shift_time_tmp)   *this->delta;
		yy[count] = best_ES_gau[count] * this->amp_scale;
	}

	//output_array2(current_gau,xx,best_ES_gau,LINE, 0 );
	output_array2(current_gau,xx,yy,LINE, 1);

	this->gau_ccc = best_ccc;
	this->gau_factor = best_coeff;

	// store infor

	//find the gaussian function ONSET
	//cout << " find the ONSET time" << endl;
	amplitudeloc( best_ES_gau ,LINE, &max_loc, &amp,1 );
	int ONSET;
	double ONSET_time;
	double threshold = 0.02;
	for(count = max_loc ; count > 0; count --)
	{
		if( best_ES_gau[count] < amp*threshold )
		{
			ONSET = count;
			ONSET_time = xx[count];
			this->virtual_stack_ONSET = ONSET_time;
			break;
		}
	}
	cout << " stack ONSET is " << this->virtual_stack_ONSET << endl;

	// calculate misfit
	this->define_ONSET_ENDSET();
	

	//this->calculate_VS_misfit(best_ES,&this->phase_win[0], LINE);
	this->calculate_VS_misfit(best_ES,phase_win_orig, LINE);
	this->update_SNR();

	this->make_quality_decision();



	return 0;
}

void virtual_station::update_SNR()
{
	// 1. update SNR with ONSET-ENDSET window as signal window
	// and noise window 
	

	// 1.1 calculate the signal window 
	// we use long win orig to calculate
	double signal = 0;
	double signal_peak = 0;
	int signal_beg = (int) ((this->ONSET - this->LONG_BEG) / this->delta);
	int signal_end = (int) ((this->ENDSET - this->LONG_BEG) / this->delta);
	int i;
	int signal_count = 0;
	for(i = signal_beg; i < signal_end ; i++)
	{
		signal += fabs(this->long_win_orig[i]);
		signal_count ++;
		if( this->long_win_orig[i] > signal_peak )
			signal_peak = this->long_win_orig[i];
	}
	if( signal_count == 0)
		signal = 1;
	else
		signal = signal / signal_count;


	// 1.2 calculate noise win
	double noise = 0;
	double noise_peak = 0;
	int noise_count = 0;
	int noise_beg = (int) ( ( this->noise_beg - this->LONG_BEG) / this->delta);
	int noise_end = (int) ( ( this->noise_beg + this->noise_len - this->LONG_BEG) / this->delta);
	for(i = noise_beg ; i < noise_end ; i++)
	{
		double time = this->LONG_BEG + i * this->delta;
		// if time is within traffic range, we mask it out
		int j = 0;
		int too_close = 0;
		for( j = 0; j < (int) (this->traffic_phase_time.size()) ; j++)
		{
			double dt = time - this->traffic_phase_time.at(j);
			if( dt > this->MASK_MIN and dt < this->MASK_MAX + this->one_period )
			{
				too_close = 1;
				break;
			}
		}
		if( too_close == 1)
			break;

		noise += fabs(this->long_win_orig[i]);
		noise_count ++;
		if( this->long_win_orig[i] > noise_peak )
			noise_peak = this->long_win_orig[i];
	}
	if(noise_count == 0)
		noise = 0;
	else
		noise = noise / noise_count;

	this->stack_SNR = signal / noise;
	if( noise_peak == 0)
		this->stack_SNR_peak = 1;
	else
		this->stack_SNR_peak = signal_peak / noise_peak;
	cout << " updated stack SNR is "<< this->stack_SNR << " SNR peak is "<< 
		this->stack_SNR_peak<<endl;

	return;
}



void virtual_station::define_ONSET_ENDSET()
{
	cout << " Define ONSET ENDSET" << endl;
	// onset is defined in phase window 
	int npts_phase_win = (int)(this->WIN_LEN / this->delta);
	int max_loc;
	double amp;
	//int amplitudeloc(double* array, int len, int* max_amp_loc, double* amplitude, int flag)
	// normalize phase_win
	//int normalize_array_with_flag(double* array, int len, int flag) 
	amplitudeloc(&this->phase_win[0],npts_phase_win,&max_loc,&amp,1);
	if( amp == amp and amp != 0 )
		normalize_array_with_flag(&this->phase_win[0],npts_phase_win,1);
	else if( amp == 0)
	{
		// make phase win all 1

	}

	cout << " phase win max loc is "<< max_loc << " WIN_BEG is "<< this->WIN_BEG << " phase_win amp is "
		<<amp<< endl;
	double threshold = 0.2;
	int i = 0;
	// Define ONSET
	for(i = max_loc; i> 0 ; i--)
	{
		if( this->phase_win[i] < amp * threshold )
		{
			this->ONSET = this->WIN_BEG + i * this->delta;
			break;
		}
	}
	// Define ENDSET
	for(i = max_loc; i<  npts_phase_win ; i++)
	{
		if( this->phase_win[i] < amp * threshold )
		{
			this->ENDSET = this->WIN_BEG + i * this->delta;
			break;
		}
	}
	
	// check how long is ONSET-ENDSET window
	// if onset or endse is more then 7seconds away from peak
	// then we make onset 7 seconds 
	double peak_time = this->WIN_BEG + max_loc * this->delta;
	if( fabs(this->ONSET - peak_time ) > 7 )
		this->ONSET = peak_time - 7;
	if( fabs(this->ENDSET - peak_time ) > 7 )
		this->ENDSET = peak_time + 7;

	if(this->ONSET < this->WIN_BEG )
		this->ONSET = this->WIN_BEG;
	if(this->ENDSET >= this->WIN_BEG + this->WIN_LEN)
		this->ENDSET = this->WIN_BEG + this->WIN_LEN;

	cout << "--> ONSET ENDSET "<< this->ONSET << " "<< this->ENDSET << endl; 

	this->define_period_for_record();

	return;

}

void virtual_station::define_period_for_record()
{
	// defined the period of current record,
	// if period is greater then 15 seconds, then we make it 15seconds
	this->one_period = fabs(this->ONSET - this->ENDSET);
	cout << " one period is "<< this->one_period << endl;

	return;
}

void virtual_station::calculate_VS_misfit(double* tstar_ES, double* phase_win, int npts)
{

	// get amplitude of tstar_ES and phase_win
	//int amplitudeloc(double* array, int len, int* max_amp_loc, double* amplitude, int flag)
	int max_loc_ES, max_loc_phase;
	double amp_ES, amp_phase;
	amplitudeloc(tstar_ES,npts,&max_loc_ES,&amp_ES,1);
	amplitudeloc(phase_win,npts,&max_loc_phase,&amp_phase,1);
	//cout << " max ES loc/amp "<< max_loc_ES << " " << amp_ES<< endl;
	//cout << " max phase loc/amp "<< max_loc_phase << " " << amp_phase<< endl;
	


	// misfit is defined as point average between tstar_ES and record
	//this->misfit = 
	double sum = 0;
	int count;
	int npts_count = 0;
	for(count = 0; count < npts; count ++)
	{
		double time = this->WIN_BEG + count * this->delta;
		if( time > this->ONSET && time < this->ENDSET )
		{
			sum += fabs( tstar_ES[count] - phase_win[count]);
			npts_count ++;
		}
	}
	if(npts_count == 0)
	{
		npts_count = 1;
		cout << "ERROR calculate_misfit npts is zero!"<< endl;
	}
	sum = sum / npts_count;
	this->misfit = sum;

	// calculate misfit pre and back
	//
	// 1. work on misfit_pre
	this->misfit_pre = 0;
	npts_count = 0;
	sum = 0;
	for(count = 0; count < npts; count ++)
	{
		double time = this->WIN_BEG + count * this->delta;
		if( time > this->ONSET - this->one_period && time < this->ONSET )
		{
			sum += fabs( tstar_ES[count] - phase_win[count]);
			npts_count ++;
		}
	}
	if(npts_count == 0)
	{
		npts_count = 1;
		cout << "ERROR calculate_misfit_pre npts is zero!"<< endl;
	}
	this->misfit_pre = sum / npts_count;
	// 2. work on misfit_bak
	this->misfit_bak = 0;
	npts_count = 0;
	sum = 0;
	for(count = 0; count < npts; count ++)
	{
		double time = this->WIN_BEG + count * this->delta;
		if( time > this->ENDSET && time < this->ENDSET + this->one_period	 )
		{
			sum += fabs( tstar_ES[count] - phase_win[count]);
			npts_count ++;
		}
	}
	if(npts_count == 0)
	{
		npts_count = 1;
		cout << "ERROR calculate_misfit_pre npts is zero!"<< endl;
	}
	this->misfit_bak = sum / npts_count;
	
	cout << "--> misfit is "<< this->misfit<< " pre "<< this->misfit_pre
		<< " back "<< this->misfit_bak<< endl;



	return;
}



void virtual_station::make_quality_decision()
{

	// use SNR and CCC to deside if current record is good
	double SNR_CUT = this->my_big_record->SNR_CUT;
	double CCC_CUT = this->my_big_record->CCC_CUT;
	//cout << " SNR is "<< this->stack_SNR << " ccc is "<< this->tstar_ccc << endl;
	//cout << "CUT SNR is "<< SNR_CUT << " ccc is "<< CCC_CUT << endl;
	if( this->stack_SNR > SNR_CUT &&
			this->tstar_ccc > CCC_CUT )
		this->quality_flag = 1;
	else
		this->quality_flag = 0;

	if(this->misfit_pre > 0.3 )
		this->quality_flag = 0;

	// if averageSNR < 1 bad
	//if( this->ave_SNR < 1 )
		//this->quality_flag = 0;


	// if misfit pre too big, quality -1
	if( this->misfit_pre > 0.3)
		this->quality_flag = -1;

	// if tstar ccc too small, -1
	//if( this->tstar_ccc < 0.92)
		//this->quality_flag = -1;


	// check for possible traffic, 
	// loop through traffic, if virtual_stack_ONSET is too close to traffic, 
	// make quality -1
	for( int i = 0; i < this->traffic_phase_list.size();i++)
	{
		string phase_name = this->traffic_phase_list[i];
		if( phase_name.compare("SKS") == 0 ||
		phase_name.compare("SKKS") == 0 ||
		phase_name.compare("sSKS") == 0 ||
		phase_name.compare("sSKKS") == 0)
			continue;

		string depth_phase;
		// if is depth phase, continue
		if( this->my_big_record->PHASE.find("S") != std::string::npos)
			depth_phase = "s"+this->my_big_record->PHASE;
		else if( this->my_big_record->PHASE.find("P") != std::string::npos)
			depth_phase = "p"+this->my_big_record->PHASE;
		if( depth_phase.compare(phase_name) == 0)
			continue;

	
		
		double prem_traffic = this->traffic_phase_time[i];
		if( fabs(prem_traffic - this->virtual_stack_ONSET) < 15 )
			this->quality_flag = -1;
	}


	return;
}





void virtual_station::find_records_within_range()
{
	
	int ista;



	for(ista = 0; ista < this->my_big_record->sta_num; ista ++)
	{
		double grid_lat = this->grid_lat;
		double grid_lon = this->grid_lon;
		double record_lat = this->my_big_record->my_record[ista].sta_lat;
		double record_lon = this->my_big_record->my_record[ista].sta_lon;
		//double eq_lat = this->my_big_record->my_record[ista].eq_lat;
		//double eq_lon = this->my_big_record->my_record[ista].eq_lon;

		// calculate the distance bewteen grid and record
		double distance;
		distance = dist_A_B( grid_lat, grid_lon, record_lat, record_lon);
		distance = distance/111;

		if(distance < this->grid_radius )
		{
			// add this record into grid
			int num = this->npts_record_sum;
			this->record_tag[ num ] = ista;
			this->npts_record_sum ++;
		}

	}


}


// inherite the info of event and station 
// into virtual station
void virtual_station::update_VS_info()
{
	cout << "--> update VS info" << endl;
	int tag = this->record_tag[0];
	this->eq_dep = this->my_big_record->my_record[tag].eq_dep;
	this->eq_lat = this->my_big_record->my_record[tag].eq_lat;
	this->eq_lon = this->my_big_record->my_record[tag].eq_lon;
	this->PHASE = this->my_big_record->PHASE;

}



void virtual_station::stack_records_from_one_EQ()
{
	cout << "stack_records_from_one_EQ "<< endl;
	int ista;
	int npts;
	double weight;
	double dist;
	double sta_lat;
	double sta_lon;
	int tag;
	
	// initiate long_win
	for(npts = 0; npts < this->long_npts; npts++)
		this->long_win[npts] = 0;
	int stacked_record_num = 0;


	for(ista = 0 ; ista < this->npts_record_sum ; ista++)
	{


		tag = this->record_tag[ista];
		if(tag == 0) 
			continue;
		cout << "working on ista "<< ista << " eventStation index is "<< tag<< endl;
		//cout << "tag "<< tag << endl;
		//cout << " stacking for vs " << this->my_big_record->my_vs_index << " station index "<< tag << endl; 
		sta_lon = this->my_big_record->my_record[tag].sta_lon;
		sta_lat = this->my_big_record->my_record[tag].sta_lat;
		// cout << "sta lon lat "<< sta_lon << endl;
		dist = dist_A_B( this->grid_lat, this->grid_lon,  sta_lat , sta_lon );
		dist = dist / 111;

		// decide what gaussian STD to use,
		// if npts_record_sum > 10 then make it 1
		// else make it 5
		int gau_factor = 5;
		if( this->npts_record_sum > 10 )
			gau_factor = 1;
		else
			gau_factor = 5;


		//gaussian_func(double a, double b, double c, double d, double x)
		// dist is in degree
		// checkout the link below for detailed info
		// https://github.com/Leviyu/project_documentation/blob/master/01_gaussian/gaussian_shape.ipynb
		weight = gaussian_func(1, 0, gau_factor , 0, dist);
		if (weight == 0)
			continue;
		int current_record_polar = this->my_big_record->my_record[tag].polarity_flag;

		//cout << "working on ista "<< ista <<" weight :"<< weight<< " current_record_polar is "<<current_record_polar << endl;
		for(npts = 0 ; npts < this->long_npts ; npts ++)
		{
			if(  this->my_big_record->my_record[tag].long_win[npts] !=  
					this->my_big_record->my_record[tag].long_win[npts] )
				continue;
			//this->long_win[npts] += 1;
			this->long_win[npts] += this->my_big_record->my_record[tag].long_win[npts] 
				* weight * current_record_polar;
		}
		stacked_record_num ++;
	}

	if(stacked_record_num == 0)
		return;





	normalize_array_with_flag( &this->long_win[0], this->long_npts ,1);
	// output stacked record
	int count = 0;
	ofstream myfile;
	myfile.open("out.stack."+std::to_string(this->my_big_record->my_vs_index));
	for(count = 0; count < this->long_npts ; count ++)
		myfile << this->long_win[count] << endl;
	myfile.close();
	
}

void virtual_station::mask_window_and_store_into_long_orig()
{
	cout << " --> mask_window_and_store_into_long_orig " << endl;
	int count = 0;
	int MAX = 2000;

	int npts_long = (int)( this->LONG_LEN / this->delta);
	cout << "my delta is "<< this->delta<< " npts long is "<< npts_long << endl;
	this->long_win_orig.resize(MAX);
	//return ;

	// 1. store long win into long win orig
	if( this->long_win[0] == this->long_win[0] )
	{
		for(count = 0; count < npts_long ; count++)
		{
			this->long_win_orig[count] = this->long_win[count];
		}
	}

	//2. Mask long win
	double mask_min = this->MASK_MIN;
	double mask_max = this->MASK_MAX;
	for(count = 0; count < npts_long ; count++)
	{
		double time = count * this->delta + this->LONG_BEG;
		if( time < mask_min || time > mask_max )
			this->long_win[count] = 0;
	}

}


void virtual_station::get_dt_ave_STD()
{

	double dt_SUM;
	double dt_ave;
	double dt_STD;
	int ista;
	dt_SUM = 0;
	for(ista = 0 ; ista < this->npts_record_sum ; ista++)
	{		
		int tag = this->record_tag[ista];
		double dt = this->my_big_record->my_record[tag].dt_obs_prem;
		dt_SUM += dt;
		cout << "+++ dt "<< dt << endl; 
	}


	dt_ave = dt_SUM / this->npts_record_sum;

	dt_STD = 0;
	for(ista = 0 ; ista < this->npts_record_sum ; ista++)
	{		
		int tag = this->record_tag[ista];
		double dt = this->my_big_record->my_record[tag].dt_obs_prem;

		dt_STD = (dt - dt_ave ) * (dt - dt_ave );
	}

	dt_STD = sqrt(dt_STD);

	this->dt_ave = dt_ave;
	this->dt_STD = dt_STD;

}

void virtual_station::get_crust_correction_ave_STD()
{

	double dt_SUM;
	double dt_ave;
	double dt_STD;
	int ista;
	dt_SUM = 0;
	for(ista = 0 ; ista < this->npts_record_sum ; ista++)
	{		
		int tag = this->record_tag[ista];
		double dt = this->my_big_record->my_record[tag].crust_correction;
		dt_SUM += dt;
		//cout << "+++ dt "<< dt << endl; 
	}


	dt_ave = dt_SUM / this->npts_record_sum;

	dt_STD = 0;
	for(ista = 0 ; ista < this->npts_record_sum ; ista++)
	{		
		int tag = this->record_tag[ista];
		double dt = this->my_big_record->my_record[tag].crust_correction;

		dt_STD = (dt - dt_ave ) * (dt - dt_ave );
	}

	dt_STD = sqrt(dt_STD);

	this->crustal_correction_ave = dt_ave;
	this->crustal_correction_STD = dt_STD;

}

void virtual_station::output_stacked_record()
{
	cout << " --> output_stacked_record" << endl;


	// 1.get long_win_orig amplitude
	int max_loc;
	double amp;
	amplitudeloc(&this->long_win_orig[0],this->long_npts,&max_loc, &amp,1);


	ofstream myfile;
	int vs_index = this->my_big_record->my_vs_index;
	string phase = this->my_big_record->PHASE;
	//cout << "====================== > in VS vsindex is "<< vs_index << " PHASE is " <<phase  <<endl;
	this->out_stacked_record_rel_PREM = "long_win.vs."+phase+"."+to_string(vs_index);
	myfile.open(this->out_stacked_record_rel_PREM.c_str());

	//normalize_array_with_flag(&this->long_win[0], this->long_npts,1);
	
	
	// ===========================
	// ----> output long_win
	double X[this->long_npts];
	int npts;
	for(npts = 0; npts < this->long_npts; npts++)
	{
		X[npts] = this->LONG_BEG + npts * this->delta;
		myfile << X[npts] << "  " << this->long_win_orig[npts] * this->amp_scale << endl;
	}
	myfile.close();

}


// 
// This code relocated the center of grid based on stations within range
// if stations within range is less or equal then 3, we skip
//
void virtual_station::relocate_grid_center()
{
	
	if ( this->npts_record_sum <= this->station_num_threshold)
	{
		double skip;
		skip = 0;
	}
	else 
	{
		double new_lat;
		double new_lon;
		int count;
		int tag;
		new_lat = 0;
		for(count = 0; count < this->npts_record_sum ; count++)
		{
			// to avoid the 0-180 transition
			// when lon > 150 || < -150, we use 0-360 range and change it back to -180 ~180 range
			tag = this->record_tag[count];
			new_lat += this->my_big_record->my_record[tag].sta_lat;
		}
		new_lat = new_lat / this->npts_record_sum;


		// we use one record to decide whick lon range to use
		tag = 0;
		double lon_tmp;
		lon_tmp = this->my_big_record->my_record[tag].sta_lon;
		new_lon = 0;
		if(   lon_tmp > 150 || lon_tmp < -150  )
		{
			for(count = 0; count < this->npts_record_sum ; count++)
			{
				// to avoid the 0-180 transition
				// when lon > 150 || < -150, we use 0-360 range and change it back to -180 ~180 range
				tag = this->record_tag[count];
				new_lon += this->my_big_record->my_record[tag].sta_lon + 180;
			}
			new_lon = new_lon / this->npts_record_sum;
			new_lon = new_lon - 180;
		}
		else
		{
			for(count = 0; count < this->npts_record_sum ; count++)
			{
				// to avoid the 0-180 transition
				// when lon > 150 || < -150, we use 0-360 range and change it back to -180 ~180 range
				tag = this->record_tag[count];
				new_lon += this->my_big_record->my_record[tag].sta_lon;
			}
			new_lon = new_lon / this->npts_record_sum;

		}


		this->grid_lat = new_lat;
		this->grid_lon = new_lon;

	}

}


/******************************************************************
 * This is a c script to calculate the SNR 
 * 1. the average SNR for all stations winthin range
 * 2. the SNR of stacked record 
 *
 *	Input:
 *
 *
 *	Output:
 *
 *
 *	DATE:				Keywords:
 *	Reference:
******************************************************************/
void virtual_station::get_SNR_before_and_after_stack()
{

	double ave_SNR;
	double stack_SNR;
	ave_SNR = 0;
	stack_SNR = 0;
	int tag;

	int count;
	int total_count = 0;
	for(count = 0; count < this->npts_record_sum; count++     )
	{

		tag = this->record_tag[count];
		double tmp_SNR;
		tmp_SNR = this->my_big_record->my_record[tag].SNR;
		if(tmp_SNR != tmp_SNR || tmp_SNR == 0)
		{
			//cout << "--> EORROR tmp_SNR problem in get_SNR_before_and_after_stack "<< endl;
			continue;
		}
		ave_SNR += tmp_SNR;
		total_count ++;
	}

	if(total_count == 0)
		ave_SNR = 1;
	else
		ave_SNR = ave_SNR / total_count;
	this->ave_SNR = ave_SNR;


	// get SNR afeter 
	// make sure the long window exist
	if( this->long_win_orig[0] != this->long_win_orig[0])
		cout << "->> ERROR long win is not read in yet, SNR cant be calcualted" << endl;


	double noise_signal;
	double phase_signal;
	noise_signal = 0;
	phase_signal = 0;

	int npts_noise_beg;
	int npts_noise_len;
	int npts_phase_beg;
	int npts_phase_len;

	npts_noise_beg = (int) ( (this->noise_beg - this->LONG_BEG) / this->delta );
	npts_phase_beg = (int) ( (this->phase_beg - this->LONG_BEG ) / this->delta );
	npts_noise_len = (int) (this->noise_len  / this->delta);
	npts_phase_len = (int) (this->phase_len  / this->delta);

	cout << " npts_noise_beg is "<< npts_noise_beg << " npts_noise_len is "<< npts_noise_len << endl;
	for(count = 0; count < npts_noise_len ; count++)
	{
		noise_signal += fabs (  this->long_win_orig[npts_noise_beg + count] );
	}

	for(count = 0; count < npts_phase_len ; count++)
	{
		phase_signal += fabs ( this->long_win_orig[npts_phase_beg + count] );
	}

	if( noise_signal == 0)
	{
		cout << "ERROR when calculateing SNR, noise signal is zero!" << endl;
		this->stack_SNR = 1;
	}
	else
	{
		phase_signal /= npts_phase_len;
		noise_signal /= npts_noise_len;
		this->stack_SNR= phase_signal / noise_signal;
	}


	return ;

}



