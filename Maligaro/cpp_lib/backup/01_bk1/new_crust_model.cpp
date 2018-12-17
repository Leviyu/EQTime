#include "hongyulib.h"

// initiate
CRUST_MODEL::CRUST_MODEL()
{

	int LINE_MAX = 9;
	int global_npts = 70000;

	this->boundary = new double*[LINE_MAX];
	this->vs = new double*[LINE_MAX];
	this->vp = new double*[LINE_MAX];
	this->rho = new double*[LINE_MAX];
	int count;
	for(count = 0; count < LINE_MAX;count++)
	{

		this->boundary[count] = new double[global_npts];
		this->vs[count] = new double[global_npts];
		this->vp[count] = new double[global_npts];
		this->rho[count] = new double[global_npts];
	}
}

// delete 
CRUST_MODEL::~CRUST_MODEL()
{

	int count ;
	int LINE_MAX = 9;
	for(count =0; count < LINE_MAX ; count++)
	{
		delete [] this->boundary[count];
		delete [] this->vs[count];
		delete [] this->vp[count];
		delete [] this->rho[count];
	}
	delete [] this->boundary;
	delete [] this->vs;
	delete [] this->vp;
	delete [] this->rho;

}



void CRUST_MODEL::read_crust_model()
{
cout << "---> Read in CRUST 1.0 model" << endl;

	int LINE_MAX = 9;

	int layer_num = 1;
	string current_file_name;
	ifstream myfile;
	double tmp1, tmp2, tmp3;
	int i;
	for( layer_num= 1; layer_num <= LINE_MAX;  layer_num++)
	{
		current_file_name = "xyz-bd"+to_string(layer_num);
		myfile.open(current_file_name.c_str());
		for(i = 0 ; i < 180*360 ; i++ )
		{
			myfile >> tmp1>>tmp2>>this->boundary[layer_num-1][i];
			//cout << tmp1 << " "<< tmp2<< endl;
		}
		myfile.close();


		current_file_name = "xyz-vs"+to_string(layer_num);
		myfile.open(current_file_name.c_str());
		for(i = 0 ; i < 180*360 ; i++ )
			myfile >> tmp1>>tmp2>>this->vs[layer_num-1][i];
		myfile.close();

		current_file_name = "xyz-vp"+to_string(layer_num);
		myfile.open(current_file_name.c_str());
		for(i = 0 ; i < 180*360 ; i++ )
			myfile >> tmp1>>tmp2>>this->vp[layer_num-1][i];
		myfile.close();

		current_file_name = "xyz-ro"+to_string(layer_num);
		myfile.open(current_file_name.c_str());
		for(i = 0 ; i < 180*360 ; i++ )
			myfile >> tmp1>>tmp2>>this->rho[layer_num -1][i];
		myfile.close();
	}



}


void CRUST_MODEL::get_single_station_correction(double lat, double lon, double* thickness, double* CRUST_TIME, double* PREM_TIME)
{
	//cout << "--> get_single_station_correction "<< endl;

	// 1. find the lat lon index
	// lat from -89.5 ~ 89.5 180 points
	// lon -179.5 ~ 179.5 360 points

	int index_lat, index_lon;
	index_lat = 0;
	index_lon = 0;

	int count;
	// find lat index
	// lat ranges from 89.5 -> -89.5
	for(count = 0; count < 180; count++)
	{
		double current_lat = 89.5 - 1*count;

		if( fabs(current_lat - lat) <= 0.5  )
		{
			index_lat = count;
			break;
		}
	}

	// find lon index
	for(count = 0; count < 360; count++)
	{
		double current_lon = -179.5 + 1*count;

		if( fabs(current_lon - lon) <= 0.5  )
		{
			index_lon = count;
			break;
		}
	}

	int current_index = index_lon  + index_lat *360;
cout << "lat lon "<< lat <<  " "<< lon << endl;
cout << "index lat lon is current index"<< index_lat << " " << index_lon << " "<< current_index << endl;

	int LINE_MAX = 9;
	double vs[LINE_MAX];
	double vp[LINE_MAX];
	double rho[LINE_MAX];
	double bd[LINE_MAX];
	double thickness_tmp[LINE_MAX];

	for(count = 0; count < LINE_MAX ;count ++)
	{
		//cout << "--> work in line "<< count<<endl;
		vs[count] = this->vs[count][current_index];
		//cout<<"vs is "<< vs[count] << endl;
		vp[count] = this->vp[count][current_index];
		rho[count] = this->rho[count][current_index];
		bd[count] = this->boundary[count][current_index];
			//thickness_tmp[count] = fabs( bd[count]);
		if(count == 0)
			thickness_tmp[count] = 0;
		else
			thickness_tmp[count] = fabs(bd[count] - bd[count-1]);
			//thickness_tmp[count] = fabs(bd[count+1] - bd[count]);
	}


	// 2. decide the maximum depthrange 
	double max_dep = 0 ;
	double dep_PREM = 24.4;
	double dep_CRUST = bd[LINE_MAX - 2]*-1;

	if( dep_PREM > dep_CRUST )
		max_dep = dep_PREM;
	else
		max_dep = dep_CRUST;
//cout << "max dep is "<< max_dep << endl;

	// 3. integrate PREM time
	// here we integrate over every 1km
	double dep_tmp = 0;
	*PREM_TIME = 0;
	for(dep_tmp = 0; dep_tmp<max_dep; dep_tmp ++)
	{
		double tmp_vs;
		prem_s(dep_tmp , &tmp_vs , 0);
		if(tmp_vs == 0)
			cout << "ERROR get_single_station_correction tmp_vs is 0"<<endl;
		*PREM_TIME += 1 / tmp_vs;
	}

//cout << " PREM crust time is "<< *PREM_TIME << endl;

	// 4 integrate CRUST TIME
	*CRUST_TIME = 0;
	if (dep_PREM <= dep_CRUST )
	{
		for(count = 0; count < LINE_MAX -1 ; count++)
		{
			double current_vs = vs[count];
			double current_thickness = thickness_tmp[count];
			double current_dt;
			if(current_vs == 0 || current_thickness == 0)
				current_dt = 0;
			else
				current_dt = current_thickness/ current_vs;
			*CRUST_TIME += current_dt;
		}
	}
	else
	{
		for(count = 0; count < LINE_MAX -1 ; count++)
		{
			double current_vs = vs[count];
			double current_thickness = thickness_tmp[count];
			double current_dt;
			if(current_vs == 0 || current_thickness == 0)
				current_dt = 0;
			else
				current_dt = current_thickness/ current_vs;
			*CRUST_TIME += current_dt;
		}
		// for layer in the mantle
		double mantle_time = ( fabs (dep_PREM - dep_CRUST)  ) / vs[LINE_MAX-1];
		*CRUST_TIME += mantle_time;
	}


//cout << " CRUST TIME IS "<< *CRUST_TIME << endl;

	*thickness = fabs(bd[LINE_MAX -2] - bd[0]);





}



void CRUST_MODEL::get_single_station_correction_vp(double lat, double lon, double* thickness, double* CRUST_TIME, double* PREM_TIME)
{
	//cout << "--> get_single_station_correction "<< endl;

	// 1. find the lat lon index
	// lat from -89.5 ~ 89.5 180 points
	// lon -179.5 ~ 179.5 360 points

	int index_lat, index_lon;
	index_lat = 0;
	index_lon = 0;

	int count;
	// find lat index
	// lat ranges from 89.5 -> -89.5
	for(count = 0; count < 180; count++)
	{
		double current_lat = 89.5 - 1*count;

		if( fabs(current_lat - lat) <= 0.5  )
		{
			index_lat = count;
			break;
		}
	}

	// find lon index
	for(count = 0; count < 360; count++)
	{
		double current_lon = -179.5 + 1*count;

		if( fabs(current_lon - lon) <= 0.5  )
		{
			index_lon = count;
			break;
		}
	}

	int current_index = index_lon  + index_lat *360;
cout << "lat lon "<< lat <<  " "<< lon << endl;
cout << "index lat lon is current index"<< index_lat << " " << index_lon << " "<< current_index << endl;

	int LINE_MAX = 9;
	double vs[LINE_MAX];
	double vp[LINE_MAX];
	double rho[LINE_MAX];
	double bd[LINE_MAX];
	double thickness_tmp[LINE_MAX];

	for(count = 0; count < LINE_MAX ;count ++)
	{
		//cout << "--> work in line "<< count<<endl;
		vs[count] = this->vs[count][current_index];
		//cout<<"vs is "<< vs[count] << endl;
		vp[count] = this->vp[count][current_index];
		rho[count] = this->rho[count][current_index];
		bd[count] = this->boundary[count][current_index];
		if(count == 0)
			thickness_tmp[count] = 0;
		else
			thickness_tmp[count] = fabs(bd[count] - bd[count-1]);
	}


	// 2. decide the maximum depthrange 
	double max_dep = 0 ;
	double dep_PREM = 24.4;
	double dep_CRUST = bd[LINE_MAX - 2]*-1;

	if( dep_PREM > dep_CRUST )
		max_dep = dep_PREM;
	else
		max_dep = dep_CRUST;
//cout << "max dep is "<< max_dep << endl;

	// 3. integrate PREM time
	// here we integrate over every 1km
	double dep_tmp = 0;
	*PREM_TIME = 0;
	for(dep_tmp = 0; dep_tmp<max_dep; dep_tmp ++)
	{
		double tmp_vs;
		prem_p(dep_tmp , &tmp_vs , 0);
		if(tmp_vs == 0)
			cout << "ERROR get_single_station_correction tmp_vs is 0"<<endl;
		*PREM_TIME += 1 / tmp_vs;
	}

//cout << " PREM crust time is "<< *PREM_TIME << endl;

	// 4 integrate CRUST TIME
	*CRUST_TIME = 0;
	if (dep_PREM <= dep_CRUST )
	{
		for(count = 0; count < LINE_MAX -1 ; count++)
		{
			double current_vs = vp[count];
			double current_thickness = thickness_tmp[count];
			double current_dt;
			if(current_vs == 0 || current_thickness ==0)
				current_dt = 0;
			else
				current_dt = current_thickness/ current_vs;
			*CRUST_TIME += current_dt;
		}
	}
	else
	{
		for(count = 0; count < LINE_MAX -1 ; count++)
		{
			double current_vs = vp[count];
			double current_thickness = thickness_tmp[count];
			double current_dt;
			if(current_vs == 0 || current_thickness == 0)
				current_dt = 0;
			else
				current_dt = current_thickness/ current_vs;
			*CRUST_TIME += current_dt;
		}
		// for layer in the mantle
		double mantle_time = ( fabs (dep_PREM - dep_CRUST)  ) / vp[LINE_MAX-1];
		*CRUST_TIME += mantle_time;
	}


//cout << " CRUST TIME IS "<< *CRUST_TIME << endl;

	*thickness =  fabs( bd[LINE_MAX -2] - bd[0]);


}
