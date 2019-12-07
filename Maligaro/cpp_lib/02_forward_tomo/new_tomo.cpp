
#include "forward_tomography.h"
/******************************************************************
 * This is a c script to initiate tomography 
******************************************************************/
void new_tomo::initiate_tomo()
{
	cout << "---> Tomography is initiated " << endl;
	int count;

	this->dep.resize(this->num_dep);
	this->lat.resize(this->num_lat);
	this->lon.resize(this->num_lon);
	this->num_lon2.resize(this->num_lat);

	//this->lon2 = new double*[this->num_lat];
	this->lon2.resize(this->num_lat);
	for(count = 0; count < this->num_lat; count ++)
		//this->lon2[count] = new double[500];
		this->lon2[count].resize(500);

	//this->my_cell = new new_cell**[this->num_dep];
	this->my_cell.resize(this->num_dep);
	int i;
	for(count = 0; count < this->num_dep; count++)
	{
		//this->my_cell[count] = new new_cell*[this->num_lat];
		this->my_cell[count].resize(this->num_lat);
		for(i=0; i < this->num_lat; i++)
		{
			//this->my_cell[count][i] = new new_cell[this->num_lon];
			this->my_cell[count][i].resize(this->num_lon);
		}

	}

}



/******************************************************************
 * This is a c script to free the memory space
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


new_tomo::~new_tomo()
{

	cout<< "new_tomo is killed flag is  "<<this->flag_is_new_tomo <<endl;
	int count;
	// free allocated memory
	//delete[] this->dep;
	//delete[] this->lat;
	//if( this->flag_is_new_tomo.compare("old") == 0 )
		//delete[] this->lon;
	//else
	//{
		//for(count = 0; count < this->num_lat; count++)
			//delete[] this->lon2[count];
		//delete[] this->lon2;
	//}

	// delete 2D array
	for(count = 0; count< this->num_dep; count++)
	{
			//delete[] this->my_cell[count];
			//delete[] this->dvs[count];
			//delete[] this->sum_dvs_in_cell[count];
			//delete[] this->sum_num_in_cell[count];
			//delete[] this->delta_dvs[count];
			//delete[] this->gradient[count];
	}
	//delete 3D array
	//delete[] this->my_cell;
	//delete[] this->dvs;
	//delete[] this->sum_dvs_in_cell;
	//delete[] this->sum_num_in_cell;
	//delete[] this->delta_dvs;
	//delete[] this->gradient;
}



void new_tomo::construct_RMS_profile()
{
	this->RMS_layer_num = this->num_dep;

	int count;
	int idep,ilat,ilon;
	int SUM_grid;
	for(count = 0; count < this->RMS_layer_num ; count++)
	{
		this->RMS_dep[count] = this->dep[count];
		this->RMS_weight[count] = 0;

		SUM_grid = 0;
		for(ilat = 0; ilat<this->num_lat;ilat++)
			for(ilon = 0 ; ilon < this->num_lon2[ilat] ; ilon++)
			{
				double tmp = this->my_cell[count][ilat][ilon].dvs;
				this->RMS_weight[count] += tmp * tmp;
				SUM_grid ++;
			}
		if(SUM_grid == 0 || SUM_grid != SUM_grid )
			cout << "ERROR SUM_grid problem " << endl;
		this->RMS_weight[count] = sqrt ( this->RMS_weight[count] / SUM_grid ); 
	}
}

void new_tomo::output_RMS_profile()
{
	ofstream myfile;
	string file = "RMS_profile."+this->MODEL;
	myfile.open(file.c_str());

	int count;
	for(count = 0; count < this->num_dep; count++)
	{
		myfile << this->dep[count] << this->RMS_weight[count]<< endl;

	}
	myfile.close();

}




double new_tomo::get_delta_lat_lon_distance_with_depth(int index_dep)
{


	double dep;
	double PI;
	double radius;
	double big_l;
	double delta_distance;
	dep = this->dep[index_dep];
	PI = 3.1415926;
	radius = 6371 - dep;
	big_l = 2*PI*radius;
	delta_distance = big_l / this->CMB_lat_cell_number;
	return delta_distance;
}







int new_tomo::print()
{
	string flag;
	flag = "============================================";
	cout << flag  << endl;
	flag = " Tomography General Info Print";
	cout << flag  << endl;

	flag = "Model Name:  " + this->MODEL;
	cout << flag  << endl;

	//if(this->CMB_lat_cell_number == this->CMB_lat_cell_number) 
	//{
	//flag = "CMB cell number:   " +to_string ( this->CMB_lat_cell_number );
	//cout << flag  << endl;
	//}

	//if( this->lat_min == this->lat_min )
	//{
	//flag = "Lat minmax  " + to_string ( this->lat_min ) + "  " + to_string(this->lat_max);
	//cout << flag  << endl;
	//}
	//if( this->num_dep == this->num_dep )
	//{
		//flag = "Dimension: Dep  " + to_string(this->num_dep) + "lat : " + to_string(this->num_lat) + "lon:  " + to_string(this->num_lon);
		//cout << flag  << endl;

		//flag = "depth profile: ";
		//cout << flag  << endl;
		
		//int count;
		//for(count = 0 ; count< this->num_dep; count++ )
			//cout << this->dep[count] << endl;

	//}


	flag = "============================================";
	cout << flag  << endl;


	return 0;
}



/******************************************************************
 * This is a c code to initiate new_tomo
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
new_tomo::new_tomo()
{

	//initiate the value in tomo
	this->num_dep = 50;
	this->num_lat = 180;
	this->num_lon = 360;

	this->CMB_lat_cell_number = 10;
	int MAX = 500;

	this->depth_profile.resize(MAX);
	this->RMS_dep.resize(MAX);
	this->RMS_weight.resize(MAX);
	this->plot_profile_dep.resize(MAX);
	this->tomo_layer_array.resize(MAX);


}



int new_tomo::getDep(int idep)
{


	return 0;
}



/******************************************************************
 * This is a c code to convert old tomo into new tomo
 *	in this function, we need to include necessary info for new_tomo
 *	Input
 *
 *
 *	Output:
 *
 *
 *	DATE:				Keywords:
 *	Reference:
******************************************************************/

int new_tomo::convert_to_new_tomo(new_tomo* old_tomo)
{
	
	cout << "---> Convert tomo into new tomo format " << endl;

	//cout << "haha " << my_tomo->num_dep<< endl;;

	this->INFILE = old_tomo->INFILE;
	this->read_INFILE();
	this->initiate_tomo();
	this->CMB_lat_cell_number = this->num_lat ;

	// define new tomo depth profile
	this->define_vertical_profile();
	this->print();
	this->define_horizontal_profile();

	cout << "Defination of vertical and horizontal profile of new tomo is done" << endl;
	cout << "dimension dep "<< this->num_dep << " lat " << this->num_lat << endl;



	int idep, ilat,ilon;
	// interpolate old tomo into new tomo
	for(idep = 0; idep < this->num_dep;idep++)
	for(ilat = 0; ilat < this->num_lat;ilat++)
	for(ilon = 0; ilon < this->num_lon2[ilat];ilon++)
	{
		double new_dep ;
		double new_lat ;
		double new_lon ;
		//new_lon = this->lon[ilat][ilon];
		new_lat = this->lat[ilat];
		new_dep = this->dep[idep];
		new_lon = this->lon2[ilat][ilon];

		//cout << "lon" << new_lon << endl;


		this->my_cell[idep][ilat][ilon].dvs = old_tomo->find_dvs_with_dep_lat_lon(new_dep,new_lat,new_lon);
		//if(new_dep > 400)
		//cout << " dep lat lon dvs " << new_dep << " " << new_lat << " " << new_lon << " " << this->my_cell[idep][ilat][ilon].dvs << endl;
		if(  this->my_cell[idep][ilat][ilon].dvs != this->my_cell[idep][ilat][ilon].dvs)
			cout << "ERROR find_dvs_with_dep_lat_lon non value problem !" << endl;
		if( this->my_cell[idep][ilat][ilon].dvs > 30
				|| this->my_cell[idep][ilat][ilon].dvs < -30)
		{
			cout << "ERROR find_dvs_with_dep_lat_lon  too big problem !" << endl;
			//cout << "idep ilat ilon "<< idep << " " << ilat << "  "  << ilon <<" "<< this->my_cell[idep][ilat][ilon].dvs << endl;
		}


	}

	cout << "-- > Convert to new tomo is done !"<< endl;
	return 0;
}











int new_tomo::define_horizontal_profile()
{
	cout << " define_horizontal_profile " << endl;
	
	// define longitude parameterization
	double PI;
	PI = 3.1415926;

	// define the length of grid on CMB
	int idep;
	idep = this->num_dep - 1;
	double CMB_equator_length;
	double CMB_latitude_delta_length;
	double CMB_latitude_delta_length_in_degree;
		//double latitude_angle_in_degree;
		//double latitude_angle_in_radi;
		//int lat_num_cells;
		//double lat_radius;
		//double current_lat_delta_length;
		//double current_lat_delta_length_in_degree;
	CMB_equator_length = 6371-this->dep[idep];
	CMB_latitude_delta_length = 3.1415926*CMB_equator_length / this->CMB_lat_cell_number;
	CMB_latitude_delta_length_in_degree = 180/this->CMB_lat_cell_number;

		double current_radius;
		double current_delta_length;
		double current_lon_circular_dl;
		double current_lat_value;
		double current_lon_delta_length_in_degree;
		double current_lon_number;
	// just to reminder, the CMB_lat_cell_number + 1 = num_lat


		//cout <<" CMB_latitude_delta_length_in_degree" << CMB_latitude_delta_length_in_degree << endl;
	int ilat,ilon;
	for(ilat = 0; ilat < this->num_lat; ilat++ )
	{
		//cout << "working on ilat " << ilat << endl;
		//cout << " lat " << this->lat[ilat] << endl;
		//cout << " min " << this->lat_min << endl;
		//cout << " CMB degree " << CMB_latitude_delta_length_in_degree << endl;

		this->lat[ilat] = this->lat_min + ilat*CMB_latitude_delta_length_in_degree;
		if( this->lat[ilat] == 90 )
			this->lat[ilat] = 89.9;
		else if( this->lat[ilat] == -90 )
			this->lat[ilat] = -89.9;

	 
		// for different latitude, we use an algorithm to calculate the longitude space
		// cauculate current radius
		current_radius = 6371 - this->dep[idep];
		current_delta_length = 3.1415926 * current_radius /  this->CMB_lat_cell_number;

		current_lat_value = abs( this->lat[ilat]);
		current_lon_circular_dl = 2*3.1415926*cos( current_lat_value / 180*3.1415926 ) *
			(6371 - current_radius);
		current_lon_number = ceil( current_lon_circular_dl / current_delta_length );
		current_lon_delta_length_in_degree = 360 / current_lon_number;
		
		
		//latitude_angle_in_degree = abs(this->lat[ilat]);
		//latitude_angle_in_radi = latitude_angle_in_degree / 180 * 3.1415926;
		//lat_radius = CMB_equator_length * cos( latitude_angle_in_radi );
		////cout << "lat radius " << lat_radius << endl;
		//lat_num_cells	= ceil (lat_radius  / CMB_latitude_delta_length);
		//current_lat_delta_length = lat_radius / lat_num_cells;
		//current_lat_delta_length_in_degree = 360 / lat_num_cells;

		//for(ilon = 0; ilon < this->)

		//cout << "current lat "<< this->lat[ilat] << " number of cell at current lat" << current_lon_number << endl;;
		//cout << "CMB lat cell number " << this->CMB_lat_cell_number << endl;
		//cout <<" current lon circular length is "<< current_lon_circular_dl << endl;
		//cout << "current_radius is " << current_radius << endl;
		//cout << "delta length " << current_delta_length<< endl;
		//cout << "current_lon_delta_length in degree  " << current_lon_delta_length_in_degree << endl;

		// store this lat dependent lon_num and value 

		this->num_lon2[ilat] = current_lon_number ;
		
		//cout << "ilat  " << ilat << endl;
		for(ilon = 0; ilon < this->num_lon2[ilat]; ilon++)
		{
			this->lon2[ilat][ilon] = -180 + ilon * current_lon_delta_length_in_degree;
			//cout << "         ilon " << this->lon2[ilat][ilon] << endl;
		}



	}
	//cout << "working on dep " << idep << endl;

	//cout << "tomo num lat is "<< this->num_lat<<endl;
	//for(ilat = 0; ilat < this->num_lat; ilat++ )
		//for(ilon = 0; ilon < this->num_lon2[ilat]; ilon++)
			//cout << "ilat is " << ilat <<"current lon number is  "<< this->num_lon2[ilat] << endl;


	return 0;
}






//// ========================================================
////
////
//// This is a script to find dvs value of a model given
////	INPUT:
////		dep lat lon
////	OUTPUT:
////		dvs or whatever the model stored
////
////	given a point dep lat lon, the following algorithm is implemented to 
////	give back a dvs value
////
////	1.we find the lat lon range of this point
////	index_lat_min index_lat_max index_lon_min index_lon_max 
////	2. we define how far to look for vertically
////		layer_thickness is the value to look for above and below to find all points that is 
////		within range to average
////	3. we do a weighted average for all points that is within range
////	4. we calculated the average dvs and return it
////
////
//// =========================================================

double new_tomo::find_dvs_with_dep_lat_lon(double dep, double lat , double lon)
{

	// 1. find the lon lat range of this point
	int index_lat_min;
	int index_lat_max;
	int index_lon_min;
	int index_lon_max;
	int index_dep_min;
	int index_dep_max;
	int index_dep;
	index_dep = 0;
	double ave_return;





	index_lat_min = this->find_index_lat(lat);
	index_lat_max = index_lat_min +1;


	if(index_lat_max >= this->num_lat)
		index_lat_max = this->num_lat -1;
	index_lon_min = this->find_index_lon(lon, index_lat_min);
	index_lon_max = index_lon_min +1;
	if(index_lon_max >= this->num_lon )
		index_lon_max = this->num_lon -1;



	// 2. define how for to look for vertically
	// find dep index minmax
	this->find_index_dep(dep, &index_dep_min,&index_dep_max);
	index_dep = this->find_index_dep2(dep);

	ave_return = this->my_cell[index_dep][index_lat_min][index_lon_min].dvs;
	return ave_return;
///*
	//double min_value;
	//min_value = this->my_cell[index_dep_min][index_lat_min][index_lon_min].dvs;
	//double max_value;
	//max_value = this->my_cell[index_dep_max][index_lat_min][index_lon_min].dvs;
	//ave_return = ( min_value + max_value )/2;




	 //DEBUG index 
	//if(dep > 400)
	//{
	//cout << "dep lat lon "<< dep << "   " << lat<< "   " << lon << endl;
	//cout<<setw(10)<<"index dep minmax lat minmax lon minmax  " 
		//<< index_dep_min << "   "
		//<< index_dep_max << "   "
		//<< index_lat_min << "   "
		//<< index_lat_max << "   "
		//<< index_lon_min << "   "
		//<< index_lon_max << endl;
	//}


	//2. we do a weighted average for all points that is winthin range
	

	int ilat,ilon,idep;
	int index;
	double weight[1000];
	double layer_ave[1000];
	double weight_SUM;
	double weight_ave_SUM;
	double dx,dy,tmp;
	double delta_distance;

	weight_SUM = 0;
	weight_ave_SUM = 0;

	double ave_A, ave_B,ave_C, ave_D;
	for(index = index_dep_min; index<= index_dep_max;index ++)
	{

		// define the 4 points for each layer
		// 			min			max lon
		// max		D			C
		// min		A			B
		// lat
		ave_A = this->my_cell[index][index_lat_min][index_lon_min].dvs;
		ave_B = this->my_cell[index][index_lat_min][index_lon_max].dvs;
		ave_C = this->my_cell[index][index_lat_max][index_lon_max].dvs;
		ave_D = this->my_cell[index][index_lat_max][index_lon_min].dvs;

		// the strateget is to calculate the average for each depth layer
		// then sum the average of each layer with a weight
		// the weight is proportational with each layer`s distance to target point
		dy = lat - this->lat[index_lat_min];
		if(this->flag_is_new_tomo == "old" )
			dx = lon - this->lon[index_lon_min];
		else if ( this->flag_is_new_tomo == "new" )
			dx = lon - this->lon2[index_lat_min][index_lon_min];
		// normalize
		delta_distance = this->get_delta_lat_lon_distance_with_depth(index);
		dy = dy / delta_distance;
		dx = dx / delta_distance;
		if(dy > 1) dy = 1;
		if(dy < 0) dy = 0;
		if(dx > 1) dx = 1;
		if(dx < 0) dx = 0;

		// define the weight
		// within 1 layer, weight = 1
		// within 2 layer, weight = 0.6
		// within 3 layer, weight = 0.4
		// within 4 layer, weight = 0.2
		//if(abs(index_dep - index) <= 1 )
			//weight[index] = 1;
		//else if(abs(index_dep - index) > 1 && abs(index_dep - index) <=2  )
			//weight[index] = 0.6;
		//else if(abs(index_dep - index) >2 && abs(index_dep - index) <= 3 )
			//weight[index] = 0.4;
		//else if(abs(index_dep - index) > 3 && abs(index_dep - index) <= 4 )
			//weight[index] = 0.2;
		//else 
			//weight[index] = 0;
		weight[index] = 1;

		//cout << "index dep   " << index_dep << "index  "<< index << endl;
		//cout << "weight is " << weight[index] << endl;
		weight_SUM += weight[index];
		layer_ave[index] = interp2_linear(ave_A, ave_B,ave_C,ave_D, dx, dy);
		weight_ave_SUM += layer_ave[index];
	}


	if(weight_SUM == 0)
		ave_return = 0;
	else
		ave_return = weight_ave_SUM / weight_SUM;

//*/


	////if(abs(ave_return) > 25)
	////{
	//cout<< "find dvs weight ave sum / weight  sum" << weight_ave_SUM 
		//<< "    " << setw(10) << weight_SUM 
		//<< " ave return  :  " << setw(10)<< ave_return 
		//<< " A  :  " << setw(10)<< ave_A
		//<< " B  :  " << setw(10)<< ave_B
		//<< " C  :  " << setw(10)<< ave_C
		//<< " D  :  " << setw(10)<< ave_D
		//<<  endl;
	//}


	return ave_return;

}

double new_tomo::find_hit_with_dep_lat_lon(double dep, double lat , double lon)
{

	// 1. find the lon lat range of this point
	int index_lat_min;
	int index_lat_max;
	int index_lon_min;
	int index_lon_max;
	int index_dep_min;
	int index_dep_max;
	int index_dep;
	index_dep = 0;
	double ave_return;





	index_lat_min = this->find_index_lat(lat);
	index_lat_max = index_lat_min +1;


	if(index_lat_max >= this->num_lat)
		index_lat_max = this->num_lat -1;
	index_lon_min = this->find_index_lon(lon, index_lat_min);
	index_lon_max = index_lon_min +1;
	if(index_lon_max >= this->num_lon )
		index_lon_max = this->num_lon -1;



	// 2. define how for to look for vertically
	// find dep index minmax
	this->find_index_dep(dep, &index_dep_min,&index_dep_max);
	index_dep = this->find_index_dep2(dep);

	ave_return = this->my_cell[index_dep][index_lat_min][index_lon_min].sum_num_in_cell;
	return ave_return;
}

//// ==================================================
////
//// Find the index of lat given a lat value
////
//// Here we give back the index that is close to target point
////
////===================================================
int new_tomo::find_index_lat(double lat)
{
	int index_return;
	index_return = 0;

	int ilat;
	for(ilat = 0; ilat < this->num_lat; ilat++)
	{

		if(this->lat[ilat] == lat)
		{
			index_return = ilat;
			break;
		}
		else if( this->lat[ilat] > lat)
		{
			index_return = ilat -1;
			break;
		}
		if(ilat == this->num_lat -1 )
		{
			index_return = this->num_lat -1;
			break;
		}

	}

	if(index_return < 0)
		index_return = 0;
	if(index_return > this->num_lat -1)
		index_return = this->num_lat -1 ;



	return index_return;
}



//// ==================================================
////
//// Find the index of lon given a lon value
////
//// Here we give back the index that is close to target point
////
////===================================================
int new_tomo::find_index_lon(double lon, int index_lat)
{
	int index_return;
	index_return = 0;
	int ilon;


	if( this->flag_is_new_tomo == "new")
	{
		for(ilon = 0; ilon < this->num_lon2[index_lat]; ilon++)
		{

			if(this->lon2[index_lat][ilon] == lon)
			{
				index_return = ilon;
				break;
			}
			else if( this->lon2[index_lat][ilon] > lon)
			{
				index_return = ilon -1;
				break;
			}
			if(ilon == this->num_lon2[index_lat]  -1)
			{
				index_return = this->num_lon2[index_lat] -1;
				break;
			}

		}

	if(index_return < 0)
		index_return = 0;
	if(index_return > this->num_lon2[index_lat] -1)
		index_return = this->num_lon2[index_lat] -1 ;
	}
	else if (this->flag_is_new_tomo == "old" )
	{

		for(ilon = 0; ilon < this->num_lon; ilon++)
		{

			if(this->lon[ilon] == lon)
			{
				index_return = ilon;
				break;
			}
			else if( this->lon[ilon] > lon)
			{
				index_return = ilon -1;
				break;
			}
			if(ilon == this->num_lon  -1)
			{
				index_return = this->num_lon -1;
				break;
			}

		}

	if(index_return < 0)
		index_return = 0;
	if(index_return > this->num_lon -1)
		index_return = this->num_lon -1 ;
	}

	//cout << "flag is " << this->flag_is_new_tomo << " num_lon -1 is "<< this->num_lon -1 << endl;;
	return index_return;
}













//// ==================================================
////
//// Find the index of dep given a dep value
////
//// Here we give back the index that is close to target point
////
//// Given a dep, we find its top and bottom depth defined by layer_thickness
////
//// For both top and bottom depth , we find the depth range that each depth layer is
//// in, and we choose the smaller number to be the index.
////
////===================================================
int new_tomo::find_index_dep(double dep, int* index_min, int* index_max)
{
	double dep_min ;
	double dep_max ;
	dep_min = dep - 0.5*this->layer_thickness;
	dep_max = dep + 0.5*this->layer_thickness;


	if(dep_max >= this->dep_max)
		dep_max = this->dep_max;
	if(dep_max <= this->dep_min) 
		dep_max = this->dep_min;
	if(dep_max == this->dep_max && dep_min == this->dep_max)
		dep_min = dep_max - 0.5*this->layer_thickness;
	if(dep_max == this->dep_min && dep_min == this->dep_min)
		dep_max = dep_min + 0.5*this->layer_thickness;

	int idep;
	for(idep = 0; idep < this->num_dep; idep++)
	{
		if(this->dep[idep] == dep_min)
		{
			*index_min = idep;
			break;
		}
		else if(this->dep[idep] > dep_min)
		{
			*index_min = idep -1;
			break;
		}
	}

	if(*index_min < 0)
		*index_min = 0;
	if(*index_min >= this->num_dep)
		*index_min = this->num_dep -1;


	for(idep = 0; idep < this->num_dep; idep++)
	{
		if(this->dep[idep] == dep_max)
		{
			*index_max = idep;
			break;
		}
		else if(this->dep[idep] > dep_max)
		{
			*index_max = idep -1;
			break;
		}
		if( idep == this->num_dep -1)
			*index_max = this->num_dep -1;
	}

	if(*index_max < 0)
		*index_max = 0;
	if(*index_max >= this->num_dep)
		*index_max = this->num_dep -1;


	if(*index_max < *index_min)
		*index_max = *index_min;


	return 0;
}



/******************************************************************
 * This is a c script to find the index of depth 
 * given a single depth value
 * the returned index is the min value between the range of the depth
 * value
 *
 *	Input:
 *
 *
 *	Output:
 *
 *
 *	DATE:Aug 2016				Keywords:
 *	Reference:
******************************************************************/



int new_tomo::find_index_dep2(double dep)
{
	double dep_min ;
	int index;
	index = 0;
	dep_min = dep;



	int idep;
	for(idep = 0; idep < this->num_dep; idep++)
	{
		if(this->dep[idep] == dep_min)
		{
			index = idep;
			break;
		}
		else if(this->dep[idep] > dep_min)
		{
			index = idep -1;
			break;
		}
		if(  idep == this->num_dep -1)
			index = this->num_dep -1;
	}

	if(index < 0)
		index = 0;
	if(index >= this->num_dep)
		index = this->num_dep -1;

	//cout << "dep dep_index   " << dep << "    " << index  << endl;

	return index;
}
int new_tomo::define_vertical_profile()
{
	
	int idep;
	//cout << "profile num " << this->dep_profile_num << endl;
	this->num_dep = this->dep_profile_num;
	for(idep = 0; idep < this->num_dep; idep++)
	{
		//cout << idep << endl;
		this->dep[idep] = this->depth_profile[idep];
		//cout << this->dep[idep] << endl;
	}


	return 0;
}




/******************************************************************
 * This is a c script to read in $MODEL.INFILE file
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


int new_tomo::read_MODEL_INFILE()
{
	string tmp;
	cout<< "--> Read MODEL INFILE \n " << endl;
	ifstream myfile;

	myfile.open(this->MODEL_INFILE.c_str());
	while(getline(myfile,tmp))
	{
		istringstream ss(tmp);

		string sub1;
		string sub2;
		string sub3;
		string sub4;
		string flag;
		ss >> sub1 >> sub2 >> sub3 >> sub4;

		flag = "<MODEL_NAME>";
		if(sub1.compare(flag) == 0)
			this->MODEL = sub2;

		flag = "<dep_num_min_max_delta>";
		if(sub1.compare(flag) == 0)
		{
			this->num_dep = atoi(sub2.c_str());
			this->dep_min = atoi(sub3.c_str());
			this->dep_max = atoi(sub4.c_str());
		}

		flag = "<lat_num_min_max_delta>";
		if(sub1.compare(flag) == 0)
		{
			this->num_lat = atoi(sub2.c_str());
			this->lat_min = atoi(sub3.c_str());
			this->lat_max = atoi(sub4.c_str());
		}

		flag = "<lon_num_min_max_delta>";
		if(sub1.compare(flag) == 0)
		{
			this->num_lon = atoi(sub2.c_str());
			this->lon_min = atoi(sub3.c_str());
			this->lon_max = atoi(sub4.c_str());
		}

		//cout << tmp << endl;
	}
	

	myfile.close();

	return 0;
}


/******************************************************************
 * This is a c script to read in the actual tomography model file
 * NOTE:
 * 	the read in dvs is relative to PREM isotroic velocity in percentage
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


int new_tomo::read_tomography()
{
	string tmp;
	ifstream myfile;
	int count;

	myfile.open(this->MODEL_model.c_str());
	while(getline(myfile,tmp))
	{
		istringstream ss(tmp);

		string sub1;
		string sub2;
		string sub3;
		string sub4;
		string flag;
		ss >> sub1 >> sub2 >> sub3 >> sub4;


		flag = "data:";
		if(sub1.compare(flag) != 0)
			continue;

		cout << " =============== Read Origional Tomography Model into Memory ===============" << endl;

		// skip 2 character
		string tmp1;
		string tmp2;
		myfile >>  tmp1 >> tmp2 ;
		//cout << tmp1 << tmp2 << endl;

		for(count = 0; count < this->num_dep; count++)
		{
			myfile >> tmp1;
			this->dep[count] = atof(tmp1.c_str());
			//cout << this->dep[count] << endl;
		}

		// skip three more character
		myfile >>  tmp1 >> tmp2 ;
		//cout << tmp1 << tmp2 << endl;
		myfile >>  tmp1 ;
		//cout << tmp1  << endl;


		// read in latitude 
		for(count = 0; count < this->num_lat; count++)
		{
			myfile >> tmp1;
			this->lat[count] = atof(tmp1.c_str());
			//cout << this->dep[count] << endl;
		}
		// skip three more character
		myfile >>  tmp1 >> tmp2 ;
		//cout << tmp1 << tmp2 << endl;
		myfile >>  tmp1 ;
		//cout << tmp1  << endl;


		// read in longitude 
		for(count = 0; count < this->num_lon; count++)
		{
			myfile >> tmp1;
			this->lon[count] = atof(tmp1.c_str());
			//cout << this->lon[count] << endl;
		}

		// skip three more character
		myfile >>  tmp1 >> tmp2 ;
		//cout << tmp1 << tmp2 << endl;
		myfile >>  tmp1 ;
		//cout << tmp1  << endl;

		// read in vs
		int idep, ilat, ilon;
		for(idep = 0; idep < this->num_dep;idep++)
		for(ilat = 0; ilat < this->num_lat;ilat++)
		for(ilon = 0; ilon < this->num_lon;ilon++)
		{
			myfile >> tmp1;
			double V = atof(tmp1.c_str());
			//cout << "Velocity is " << V  << endl;

			double PREM_V = 1;


			int flag_rel_to_prem = 1;
			double dvs;
			// 0 relative to prem
			// 1 relative to each model`s own reference
			if( flag_rel_to_prem == 1 )
			{

				if(this->MODEL.compare("GYPSUM_S") == 0)
				{
					PREM_V = reference_TNA_SNA(this->dep[idep]);
					// V is abo value
					dvs = (V - PREM_V) / PREM_V * 100;
				}
				else if( this->MODEL.compare("HMSL_S06") == 0 || 
						this->MODEL.compare("HMSL_P06") == 0 )
					dvs = V;
				else if(this->MODEL.compare("SAW642ANB") == 0 || this->MODEL.compare("SAW642AN") == 0)
				{
					double ddep, radius, density, vpv,vph,vsv,vsh,Qu,Qk;
					int flag = 0;
					prem_500(this->dep[idep], &radius, &density, &vpv, &vph, &vsv, &vsh, &Qu, &Qk, flag);
					PREM_V = vsh;
					dvs = ( V - PREM_V) / PREM_V * 100;
				}
				else if(this->MODEL.compare("S362AMI") == 0 ||
						this->MODEL.compare("S362ANI+M") == 0 ||
						this->MODEL.compare("S362WMANI") == 0 ||
						this->MODEL.compare("S362WMANI+M") == 0 )
				{
					double ddep, radius, density, vpv,vph,vsv,vsh,Qu,Qk;
					int flag = 0;
					reference_STW105(this->dep[idep], &radius, &density, &vpv, &vph, &vsv, &PREM_V, &Qu, &Qk, flag);
					dvs = (V - PREM_V )/ PREM_V  * 100;
				}
				else if( this->MODEL.compare("TX2011") == 0 )
				{
					//// V is dvs in percentage
					dvs = V;
				}
				else if( this->MODEL.compare("LLNL") == 0 )
				{
					prem_p(this->dep[idep], &PREM_V, 0);
					//PREM_V = reference_LLNL(this->dep[idep]);
					dvs = (V - PREM_V) / PREM_V * 100;
				}
				else if( this->MODEL.compare("GYPSUM_P") == 0 )
				{
					prem_p(this->dep[idep], &PREM_V, 0);
					dvs = (V - PREM_V) / PREM_V * 100;
				}
				else if( this->MODEL.compare("SEMUCB_WM1") == 0 )
				{
					double v_ref = SEMUCB( this->dep[idep]);
					dvs = (V - v_ref) / v_ref * 100;
				}
				else if( this->MODEL.compare("SMEAN") == 0 ||
						 this->MODEL.compare("PMEAN") == 0 || 
						  this->MODEL.compare("SP12RTS_P") == 0 ||
						  this->MODEL.compare("SP12RTS_S") == 0 ||
						  this->MODEL.compare("BDP00") == 0 ||
						  this->MODEL.compare("KH00P") == 0 
						)
				{
					dvs = V;
				}
				else if( this->MODEL.compare("SAW24B16") == 0)
				{
				
					double ddep, radius, density, vpv,vph,vsv,vsh,Qu,Qk;
					int flag = 1;
					prem_aniso(this->dep[idep], &radius, &density, &vpv, &vph, &vsv, &vsh, &Qu, &Qk, flag);
					PREM_V = vsh;
					dvs = ( V - PREM_V) / PREM_V * 100;

				}
				else if( this->MODEL.compare("MITP08") == 0)
				{
					dvs = V;
				}
				else
				{
					prem_s(this->dep[idep], &PREM_V, 0);
					dvs = (V - PREM_V) / PREM_V * 100;

				}
				//else 
				//{
////#this is for media usage
					//dvs = V;
					////prem_s(this->dep[idep], &PREM_V, 0);
					////dvs = (V - PREM_V) / PREM_V * 100;
				//}

				// ########################################



			}
			else if(flag_rel_to_prem == 0)
			{
				// construct model relative to PREM
				if(this->MODEL.compare("GYPSUM_S") == 0)
				{
					prem_s(this->dep[idep], &PREM_V, 0);
					// V is abo value
					dvs = (V - PREM_V) / PREM_V * 100;

				}
				else if( this->MODEL.compare("LLNL") == 0 )
				{
					prem_p(this->dep[idep], &PREM_V, 0);
					//PREM_V = reference_LLNL(this->dep[idep]);
					dvs = (V - PREM_V) / PREM_V * 100;
				}
				else if(this->MODEL.compare("SAW642ANB") == 0 || this->MODEL.compare("SAW642AN") == 0)
				{
					double ddep, radius, density, vpv,vph,vsv,vsh,Qu,Qk;
					int flag = 0;
					prem_s(this->dep[idep], &PREM_V, 0);
					PREM_V = vsh;
					dvs = ( V - PREM_V) / PREM_V * 100;
				}
				else if(this->MODEL.compare("S362AMI") == 0 ||
						this->MODEL.compare("S362ANI+M") == 0 ||
						this->MODEL.compare("S362WMANI") == 0 ||
						this->MODEL.compare("S362WMANI+M") == 0 )
				{
					double ddep, radius, density, vpv,vph,vsv,vsh,Qu,Qk;
					int flag = 0;
					prem_s(this->dep[idep], &PREM_V, 0);
					dvs = (V - PREM_V )/ PREM_V  * 100;
				}
				else if(this->MODEL.compare("HMSL_P06") == 0 )
				{
					double ddep, radius, density, vpv,vph,vsv,vsh,Qu,Qk;
					int flag = 0;
					ak135(this->dep[idep], &radius, &density, &vpv, &vsv , &Qu, &Qk,flag);
					PREM_V = vpv;
					double true_dvs = PREM_V *  (1 + V / 100);
					double prem_tmp;
					prem_p(this->dep[idep], &prem_tmp, 0);
					dvs = (true_dvs - prem_tmp ) / prem_tmp * 100;
				}
				else if(this->MODEL.compare("MITP08") == 0 )
				{
					double ddep, radius, density, vpv,vph,vsv,vsh,Qu,Qk;
					int flag = 0;
					ak135(this->dep[idep], &radius, &density, &vpv, &vsv , &Qu, &Qk,flag);
					PREM_V = vpv;
					double true_dvs = PREM_V *  (1 + V / 100);
					double prem_tmp;
					prem_p(this->dep[idep], &prem_tmp, 0);
					dvs = (true_dvs - prem_tmp ) / prem_tmp * 100;
				}
				else if(this->MODEL.compare("HMSL_S06") == 0 )
				{
					double ddep, radius, density, vpv,vph,vsv,vsh,Qu,Qk;
					int flag = 0;
					ak135(this->dep[idep], &radius, &density, &vpv, &vsv , &Qu, &Qk,flag);
					PREM_V = vsv;
					double true_dvs = PREM_V *  (1 + V / 100);
					double prem_tmp;
					prem_s(this->dep[idep], &prem_tmp, 0);
					dvs = (true_dvs - prem_tmp ) / prem_tmp * 100;
				}
				else if( this->MODEL.compare("TX2011") == 0 )
				{
					//// V is dvs in percentage
					double v_ref = tx2011( this->dep[idep]);
					double vs_tmp = v_ref * (1+V /100);
					double prem_tmp;
					prem_s(this->dep[idep], &prem_tmp, 0);
					dvs = (vs_tmp - prem_tmp ) / prem_tmp * 100;
				}
				else if( this->MODEL.compare("SEMUCB_WM1") == 0 )
				{
					double v_ref = SEMUCB( this->dep[idep]);
					dvs = (V - v_ref) / v_ref * 100;
				}
				else if( this->MODEL.compare("GYPSUM_P") == 0 )
				{
					prem_p(this->dep[idep], &PREM_V, 0);
					dvs = (V - PREM_V) / PREM_V * 100;
				}
				else if( this->MODEL.compare("SMEAN") == 0 ||
						 this->MODEL.compare("PMEAN") == 0 ||
						  this->MODEL.compare("SP12RTS_P") == 0 ||
						  this->MODEL.compare("SP12RTS_S") == 0 ||
						  this->MODEL.compare("BDP00") == 0 ||
						  this->MODEL.compare("KH00P") == 0 
						)
				{
					dvs = V;
				}
				else
				{
					prem_s(this->dep[idep], &PREM_V, 0);
					dvs = (V - PREM_V) / PREM_V * 100;
				}

			}
			


			//cout <<"dv type is "<< this->dv_type << endl;
			//this->dvs[idep][ilat][ilon] = dvs;
			this->my_cell[idep][ilat][ilon].dvs = dvs;

			if(dvs != dvs )
			{
				cout << "ERROR Read tomography model, dvs is Null !" << endl;
				cout << " V " << V << "PREM_V " << PREM_V << "dvs" << dvs << endl;
			}

			//if( this->dep[idep] == 2900)
			//{
				//cout << "depth " << this->dep[idep] <<"prem s is " << PREM_V << endl;
				//cout << " V " << V << "PREM_V " << PREM_V << "dvs" << dvs << endl;
				//cout << " dvs is  " <<  dvs << "V is " << V << endl;
			//cout << " my cell dvs is " <<  this->my_cell[idep][ilat][ilon].dvs << endl;
			//}
		}

	}
	

	myfile.close();

	return 0;
}



// This function construct the 1D Vs profile of current model 
int new_tomo::construct_1D_reference()
{

	double Vs[1000];

	int idep, ilat, ilon;

	double SUM_VS;
	int npts_points;
	ofstream myfile;
	string reference_1D = "reference_1D.data";
	myfile.open(reference_1D.c_str());

	for(idep = 0; idep < this->num_dep ; idep++)
	{
		SUM_VS = 0;
		npts_points = 0;
		double dep_tmp = this->dep[idep];

		for(ilat = 0; ilat < this->num_lat;ilat ++)
			for(ilon = 0; ilon < this->num_lon; ilon ++)
			{
				double PREM_V;
				prem_s(dep_tmp,&PREM_V,0);
				SUM_VS +=  PREM_V * (1+this->my_cell[idep][ilat][ilon].dvs/100);
				npts_points++;
			}
		SUM_VS = SUM_VS / npts_points;
		myfile << dep_tmp << " "<< SUM_VS << endl;
	}

	myfile.close();
	return 0;
}






/******************************************************************
 * This is a c script to read in INFILE file
 * INFILE contains information about:
 * 	directories
 * 	model parameterization
 * 	forward tomography iterations
 * 	plotting information
 * 	and other detailed algorithm parameter
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


int new_tomo::read_INFILE()
{
	string tmp;

	ifstream myfile;
	myfile.open(this->INFILE.c_str());

	while(getline(myfile,tmp))
	{
		//cout << tmp<< endl;
		istringstream ss(tmp);
		string sub1;
		string sub2;
		string sub3;
		string sub4;
		string sub5;
		string flag;

		ss >> sub1 >> sub2 >> sub3 >> sub4 >> sub5;

		flag = "<MODEL_NAME>";
		if(sub1.compare(flag) == 0)
		{
			this->MODEL = sub2;
			//cout << "model name is " << this->MODEL << endl;
		}
		flag = "<dv_type>";
		if(sub1.compare(flag) == 0)
			this->dv_type = sub2; 

		flag = "<layer_thickness>";
		if(sub1.compare(flag) == 0)
			this->layer_thickness = atof(sub2.c_str()); 

		flag = "<dep_num_min_max>";
		if(sub1.compare(flag) == 0)
		{
			this->num_dep = atoi(sub2.c_str()); 
			this->dep_min = atof(sub3.c_str()); 
			this->dep_max = atof(sub4.c_str()); 
		}

		flag = "<lat_num_min_max>";
		if(sub1.compare(flag) == 0)
		{
			this->num_lat = atoi(sub2.c_str()); 
			this->lat_min = atof(sub3.c_str()); 
			this->lat_max = atof(sub4.c_str()); 
		}

		flag = "<lon_num_min_max>";
		if(sub1.compare(flag) == 0)
		{
			this->num_lon = atoi(sub2.c_str()); 
			this->lon_min = atof(sub3.c_str()); 
			this->lon_max = atof(sub4.c_str()); 
		}

		flag = "<weighting_ratio_path_length_RMS_tomo>";
		if(sub1.compare(flag) == 0)
			this->weight_ratio_path_length_RMS_tomo = atof(sub2.c_str()); 

		flag = "<LS_iterations>";
		if(sub1.compare(flag) == 0)
			this->tomo_iterations= atoi(sub2.c_str()); 

		flag = "<Iteration_M>";
		if(sub1.compare(flag) == 0)
			this->Iteration_M = atoi(sub2.c_str()); 
		flag = "<Iteration_N>";
		if(sub1.compare(flag) == 0)
			this->Iteration_N = atoi(sub2.c_str()); 
		flag = "<Iteration_MN>";
		if(sub1.compare(flag) == 0)
			this->Iteration_MN = atoi(sub2.c_str()); 

		flag = "<Iteration_Scheme>";
		if(sub1.compare(flag) == 0)
			this->Iteration_Scheme = sub2;

		flag = "<MODEL_DIR>";
		if(sub1.compare(flag) == 0)
			this->MODEL_DIR = sub2;


		flag = "<taup_path_dir>";
		if(sub1.compare(flag) == 0)
			this->taup_path_dir = sub2;


		flag = "<cross_point_dir>";
		if(sub1.compare(flag) == 0)
			this->cross_point_dir = sub2;


		flag = "<plot_profile>";
		if(sub1.compare(flag) == 0)
		{
			this->plot_profile_num = atoi(sub2.c_str()); 
			int kkk;
			string tmp2;
			for(kkk = 0 ; kkk < this->plot_profile_num;kkk++)
			{
				getline(myfile,tmp2);
				istringstream ss2(tmp2);
				string sub11;
				string sub12;

				ss2 >> sub11 >> sub12 ;
				this->plot_profile_dep[kkk] = atof(sub11.c_str());
			}
		}

		flag = "<LS_layer_num>";
		if(sub1.compare(flag) == 0)
		{
			this->tomo_layer_number = atoi(sub2.c_str()); 
			int kkk;
			string tmp2;
			for(kkk = 0 ; kkk < this->tomo_layer_number;kkk++)
			{
				getline(myfile,tmp2);
				istringstream ss2(tmp2);
				string sub11;
				string sub12;

				ss2 >> sub11 >> sub12 ;
				this->tomo_layer_array[kkk] = atof(sub11.c_str());
			}
		}

		flag = "<depth_profile>";
		if(sub1.compare(flag) == 0)
		{
			this->dep_profile_num = atoi(sub2.c_str()); 
			//this->num_dep = atoi(sub2.c_str()); 
			//cout <<"dep prfile num " << this->dep_profile_num << endl;
			int kkk;
			string tmp2;
			for(kkk = 0 ; kkk < this->dep_profile_num;kkk++)
			{
				getline(myfile,tmp2);
				istringstream ss2(tmp2);
				string sub11;
				string sub12;

				ss2 >> sub11 >> sub12 ;
				this->depth_profile[kkk] = atof(sub11.c_str());
				//cout << this->depth_profile[kkk] << endl;
				//this->dep[kkk] = atof(sub11.c_str());
			}

			// debug make sure depth_profile exist
			if( this->dep_profile_num < 25 ) 
				cout <<"ERROR : dep_profile_num < 25 " << endl;



		}


		//flag = "<tomo_layer_num>";
		//if(sub1.compare(flag) == 0)
			//this->tomo_layer_number = atoi(sub2.c_str()); 

	}


	myfile.close();

	return 0;
}



void new_tomo::output_tomo_for_media()
{
	ofstream myfile;
	string outname = "output_for_media."+this->MODEL;
	myfile.open(outname.c_str());



	int ix,iy,iz;
	double nx,ny,nz;
	double lat,lon,dep;

	// we find the maximum value
	double maxmax = 0;
	for(ix = -6400 ; ix <= 6400 ;ix+= 25 )
		for(iy = -6400 ; iy <= 6400; iy +=25)
		{
			for(iz = -6400 ;iz <= 6400; iz +=25)
			{
				// normalized x y z is 
				nx = ix / 6400.0;
				ny = iy / 6400.0;
				nz = iz / 6400.0;

				// convert x y z in km into lat lon dep
				cart2sph(ix,iy,iz, &lat, &lon, &dep);
				
				double dvs = 999;
				if(dep <  0 || dep > 3480 )
					dvs = 999;
				else
				{
					dvs = this->find_dvs_with_dep_lat_lon(dep,lat,lon);
					if(dvs > maxmax )
					{
						maxmax = dvs;
						//cout << "max dvs is "<< maxmax<< endl;
					}
				}

			}
		}
	



	for(ix = -6400 ; ix <= 6400 ;ix+= 25 )
		for(iy = -6400 ; iy <= 6400; iy +=25)
		{
			for(iz = -6400 ;iz <= 6400; iz +=25)
			{
				// normalized x y z is 
				nx = ix / 6400.0;
				ny = iy / 6400.0;
				nz = iz / 6400.0;

				// convert x y z in km into lat lon dep
				cart2sph(ix,iy,iz, &lat, &lon, &dep);
				
				double dvs = 999;
				if(dep <  0 || dep > 3480 )
					dvs = 999;
				else
				{
					dvs = this->find_dvs_with_dep_lat_lon(dep,lat,lon);
					// normalize
					dvs = dvs / maxmax;
				}


				if(iz != 6400)
					myfile<< setw(15)<< setprecision(5)<< fixed << dvs << "|" ;
				else 
					myfile<< setw(15)<< setprecision(5)<< fixed << dvs << "\\n" << endl;

			}
		}


}



//// Output tomography info
//// Including
//// normalized dep lat lon dvs dep lat lon

int new_tomo::output_tomography_info()
{

	cout << "======================== Output Tomography info =====================" << endl;
	this->output_tomo_file = "output_tomo." + this->MODEL;

	// define output file parameter
	ofstream myfile;
	myfile.open(this->output_tomo_file.c_str());


	// first output dimensions
	myfile << this->num_dep << endl;
	myfile << this->num_lat << endl;
	myfile << this->num_lon << endl;
	

	// output model file
	int idep,ilat,ilon;
	cout << "Dimensions" << this->num_dep << this->num_lat << this->num_lon << endl;
	for(idep = 0; idep < this->num_dep;idep++)
	{	
		//cout << "Output depth " << this->dep[idep] << endl;
	for(ilat = 0; ilat < this->num_lat;ilat++)
	for(ilon = 0; ilon < this->num_lon;ilon++)
	{
		//normalize
		double xx,yy,zz;

		//void sph2cart(double lat, double lon, double dep, double* x, double* y, double* z)
		double dep = this->dep[idep];
		double lat = this->lat[ilat];
		double lon = this->lon[ilon];
		//double dvs = this->dvs[idep][ilat][ilon];
		double dvs = this->my_cell[idep][ilat][ilon].dvs;
		sph2cart(lat, lon, dep, &xx, &yy, &zz);
		double RADIUS = 6371;
		xx = xx/RADIUS;
		yy = yy/RADIUS;
		zz = zz/RADIUS;

	//cout  << xx << yy << zz << dvs << dep << lat << lon << endl;
	myfile <<setw(10)<<setprecision(5) << xx << "   " << yy << "   " << zz << "   " << dvs <<"   " <<dep <<"   "<< lat <<"   " << lon << endl;
	//cout <<setw(10)<<setprecision(5) << xx << "   " << yy << "   " << zz << endl;


	}
	}



	myfile.close();

	return 0;
}



/******************************************************************
 * This is a c script to output tomography model for new tomography
 * Here the lon is dependent on lat, therefore we use num_lon2 and lon2 
 * to get required lon
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


int new_tomo::output_tomography_info2()
{

	cout << "======================== Output Tomography info =====================" << endl;
	this->output_tomo_file = "output_tomo." + this->MODEL;

	// define output file parameter
	ofstream myfile;
	myfile.open(this->output_tomo_file.c_str());


	// first output dimensions
	myfile << this->num_dep << endl;
	myfile << this->num_lat << endl;
	myfile << this->num_lon << endl;
	

	// output model file
	int idep,ilat,ilon;
	cout << "Dimensions" << this->num_dep << this->num_lat << this->num_lon << endl;

	//output lat lon parameterization
	//for(ilat = 0; ilat < this->num_lat;ilat++)
	//{
		//cout<< "ilat is " << ilat << "current ilon is "<< this->num_lon2[ilat] << endl;
	//for(ilon = 0; ilon < this->num_lon2[ilat];ilon++)
	//{
		//cout << "lat lon" << this->lat[ilat] << "  "<<  this->lon2[ilat][ilon] << endl;
	//}
	//}


	for(idep = 0; idep < this->num_dep;idep++)
	{	
		//cout << "Output depth " << this->dep[idep] << endl;
	for(ilat = 0; ilat < this->num_lat;ilat++)
	for(ilon = 0; ilon <= this->num_lon2[ilat];ilon++)
	//for(ilon = 0; ilon < this->num_lon;ilon++)
	{
		//normalize
		double xx,yy,zz;

		//void sph2cart(double lat, double lon, double dep, double* x, double* y, double* z)
		double dep = this->dep[idep];
		double lat = this->lat[ilat];
		double dvs;
		double lon;
			//lon = -180+ilon;
		//if( ilon < this->num_lon2[ilat])
		//{
			lon = this->lon2[ilat][ilon];
			dvs = this->my_cell[idep][ilat][ilon].dvs;

		if( ilon == this->num_lon2[ilat])
		{
			lon = 180;
			dvs = this->my_cell[idep][ilat][0].dvs;
		}
		//}
		//else 
		//{
			//lon = -180+ilon;
			//dvs = 0;
		//}

		//if( dep >= 100 && dep <=300 && lat >= 0 && lat <= 2 && lon >= 0  && lon <=2 )
		if( dep == 2800 )
			double do_it;
		else
			continue;




		//double dvs = this->dvs[idep][ilat][ilon];
		sph2cart(lat, lon, dep, &xx, &yy, &zz);
		double RADIUS = 6371;
		xx = xx/RADIUS;
		yy = yy/RADIUS;
		zz = zz/RADIUS;

	//cout  << xx << yy << zz << dvs << dep << lat << lon << endl;
	int size = 15;
	int prec = 6;
	myfile <<fixed
		<<setw(size)<<setprecision(prec) << xx 
		<<setw(size)<<setprecision(prec) << yy
		<<setw(size)<<setprecision(prec) << zz
		<<setw(size)<<setprecision(prec) << dvs 
		<<setw(size)<<setprecision(prec) << dep
		<<setw(size)<<setprecision(prec) << lat
		<<setw(size)<<setprecision(prec) << lon << endl;
		
		
		
		//<< "   " << yy << "   " << zz << "   " << dvs <<"   " <<dep <<"   "<< lat <<"   " << lon << endl;
	//cout <<setw(10)<<setprecision(5) << xx << "   " << yy << "   " << zz << endl;


	}
	}



	myfile.close();

	return 0;
}

int new_tomo::output_starting_tomography()
{

	cout << "======================== Output starting Tomography info =====================" << endl;
	this->output_tomo_file = "output_starting_tomo." + this->MODEL;

	// define output file parameter
	ofstream myfile;
	myfile.open(this->output_tomo_file.c_str());

	// first output dimensions
	myfile << this->num_dep << endl;
	myfile << 179 << endl;
	myfile << 361 << endl;
	
	double dep;
	double lat;
	double dvs;
	double lon;

	// output model file
	int idep,ilat,ilon;
	for(idep = 0; idep < this->num_dep;idep++)
	{	
	for(lat = -89; lat <= 89 ; lat++ )
	for(lon = -180 ; lon <=180 ; lon++)
	{
		//normalize
		double xx,yy,zz;
		//void sph2cart(double lat, double lon, double dep, double* x, double* y, double* z)
			//lon = -180+ilon;
		//if( ilon < this->num_lon2[ilat])
		//{
			dep = this->dep[idep];

//cout << " working on dep "<< dep << " lat " << lat << " lon "<< lon << endl; 
		dvs = this->find_dvs_with_dep_lat_lon(dep,lat,lon);
		//}
		//else 
		//{
			//lon = -180+ilon;
			//dvs = 0;
		//}

		//if( dep >= 100 && dep <=300 && lat >= 0 && lat <= 2 && lon >= 0  && lon <=2 )
		//if( dep == 2800 )
			//double do_it;
		//else
			//continue;



		//double dvs = this->dvs[idep][ilat][ilon];
		sph2cart(lat, lon, dep, &xx, &yy, &zz);
		double RADIUS = 6371;
		xx = xx/RADIUS;
		yy = yy/RADIUS;
		zz = zz/RADIUS;

	//cout  << xx << yy << zz << dvs << dep << lat << lon << endl;
	int size = 15;
	int prec = 6;
	myfile <<fixed
		<<setw(size)<<setprecision(prec) << xx 
		<<setw(size)<<setprecision(prec) << yy
		<<setw(size)<<setprecision(prec) << zz
		<<setw(size)<<setprecision(prec) << dvs 
		<<setw(size)<<setprecision(prec) << dep
		<<setw(size)<<setprecision(prec) << lat
		<<setw(size)<<setprecision(prec) << lon << endl;
		
		//<< "   " << yy << "   " << zz << "   " << dvs <<"   " <<dep <<"   "<< lat <<"   " << lon << endl;
	//cout <<setw(10)<<setprecision(5) << xx << "   " << yy << "   " << zz << endl;

	}
	}



	myfile.close();

	return 0;
}
/******************************************************************
 * This is a c script to output tomography model for new tomography
 *	since our model lat lon is parametrized in a uniq way, after exploring 
 *	into vtk unstructured format, I feel the best way to plot the model is 
 *	to interpolate the new tomo into 1degree*1degree 
 *	and this is what happend in this function
 *
 * 	Here we implement a smoothing method, where for each cell
 * 	we average all dvs that is within some distance both vertically and horizontally
 *
 *	Input:
 *
 *	Output:
 *
 *
 *	DATE:	Aug 2016			Keywords:
 *	Reference:
******************************************************************/
#define smooth_horizontal_degree 	5
#define smooth_vertical_layer		1
void new_tomo::output_smoothed_tomography()
{

	cout << "======================== Output smoothed Tomography info =====================" << endl;
	this->output_tomo_file = "output_smoothed_tomo." + this->MODEL;

	// define output file parameter
	ofstream myfile;
	myfile.open(this->output_tomo_file.c_str());

	// first output dimensions
	myfile << this->num_dep << endl;
	myfile << 179 << endl;
	myfile << 361 << endl;
	
	double dep;
	double lat;
	double dvs;
	double lon;
	int iidep_min, iidep_max, iidep;
	double max_gradient;

	// output model file
	int idep,ilat,ilon;
	for(idep = 0; idep < this->num_dep;idep++)
	{	
	for(lat = -89; lat <= 89 ; lat++ )
	for(lon = -180 ; lon <=180 ; lon++)
	{
		//normalize
		double xx,yy,zz;
		dep = this->dep[idep];
		dvs = this->find_dvs_with_dep_lat_lon(dep,lat,lon);
		// ===========================================
		// working on finding the gradient value of current point
		iidep_min = idep - smooth_horizontal_degree;
		iidep_max = idep + smooth_horizontal_degree;
		//iidep_max = idep;
		if(iidep_min < 0)
			iidep_min = 0;
		if(iidep_max > this->num_dep -1)
			iidep_max = this->num_dep -1;


		double dep2,lat2,lon2;
		double sum_dvs;
		sum_dvs = 0;
		max_gradient = 0;
		int num_dvs_total;
		num_dvs_total = 0;
		double sum_weight;
		sum_weight = 0;
		for(iidep = iidep_min; iidep <= iidep_max ; iidep ++)
			for(lat2 = lat - smooth_vertical_layer; lat2 <= lat + smooth_vertical_layer ; lat2 ++)
				for(lon2 = lon - smooth_vertical_layer; lon2 <= lon + smooth_vertical_layer; lon2 ++)
				{
					//if(lon2 == lon && lat2 == lat)
						//continue;
					
					// define the weight 
					double weight_v;
					double weight_h;
					if( iidep - iidep_min <= 1 ) 
						weight_v = 1;
					else if (iidep - iidep_min >=2 && iidep - iidep_min <3)
						weight_v = 0.7;
					else if (iidep - iidep_min >=3 && iidep - iidep_min <=4 )
						weight_v = 0.5;
					else 
						weight_v = 0.25;

					if( abs (lat2 - lat) <=1 && abs(lon2 - lon) <=1) 
						weight_h = 1;
					else if ( abs (lat2 - lat) <=2 && abs(lon2 - lon) <=2)
						weight_h = 0.8;
					else if ( abs (lat2 - lat) <=4 && abs(lon2 - lon) <=4)
						weight_h = 0.6;
					else if ( abs (lat2 - lat) <=6 && abs(lon2 - lon) <=6)
						weight_h = 0.4;
					else 
						weight_h = 0.3;
					double weight;
					weight = weight_h * weight_v;



					dep2 = this->dep[iidep];
					double dvs_tmp;
					dvs_tmp = find_dvs_with_dep_lat_lon(dep2, lat2, lon2);
					sum_dvs += dvs_tmp * weight;
					sum_weight += weight;

					num_dvs_total ++;
				}

		double ave_dvs;
		ave_dvs = sum_dvs / num_dvs_total;


		//double dvs = this->dvs[idep][ilat][ilon];
		sph2cart(lat, lon, dep, &xx, &yy, &zz);
		double RADIUS = 6371;
		xx = xx/RADIUS;
		yy = yy/RADIUS;
		zz = zz/RADIUS;

	//cout  << xx << yy << zz << dvs << dep << lat << lon << endl;
	int size = 15;
	int prec = 6;
	myfile <<fixed
		<<setw(size)<<setprecision(prec) << xx 
		<<setw(size)<<setprecision(prec) << yy
		<<setw(size)<<setprecision(prec) << zz
		<<setw(size)<<setprecision(prec) << ave_dvs
		<<setw(size)<<setprecision(prec) << dep
		<<setw(size)<<setprecision(prec) << lat
		<<setw(size)<<setprecision(prec) << lon << endl;
		
		//<< "   " << yy << "   " << zz << "   " << dvs <<"   " <<dep <<"   "<< lat <<"   " << lon << endl;
	//cout <<setw(10)<<setprecision(5) << xx << "   " << yy << "   " << zz << endl;

	}
	}



	myfile.close();

}

/******************************************************************
 * This is a c script to output tomography model for new tomography
 *	since our model lat lon is parametrized in a uniq way, after exploring 
 *	into vtk unstructured format, I feel the best way to plot the model is 
 *	to interpolate the new tomo into 1degree*1degree 
 *	
 *	and this is what happend in this function
 *
 *	Input:
 *
 *	Output:
 *
 *
 *	DATE:	Aug 2016			Keywords:
 *	Reference:
******************************************************************/
int new_tomo::output_tomography_info3()
{

	cout << "======================== Output Tomography info =====================" << endl;
	this->output_tomo_file = "output_tomo." + this->MODEL;

	// define output file parameter
	ofstream myfile;
	myfile.open(this->output_tomo_file.c_str());

	// first output dimensions
	myfile << this->num_dep << endl;
	myfile << 179 << endl;
	myfile << 361 << endl;
	
	double dep;
	double lat;
	double dvs;
	double lon;

	// output model file
	int idep,ilat,ilon;
	for(idep = 0; idep < this->num_dep;idep++)
	{	
	for(lat = -89; lat <= 89 ; lat++ )
	for(lon = -180 ; lon <=180 ; lon++)
	{
		//normalize
		double xx,yy,zz;

		//void sph2cart(double lat, double lon, double dep, double* x, double* y, double* z)
			//lon = -180+ilon;
		//if( ilon < this->num_lon2[ilat])
		//{
			dep = this->dep[idep];

		dvs = this->find_dvs_with_dep_lat_lon(dep,lat,lon);
		//}
		//else 
		//{
			//lon = -180+ilon;
			//dvs = 0;
		//}

		//if( dep >= 100 && dep <=300 && lat >= 0 && lat <= 2 && lon >= 0  && lon <=2 )
		//if( dep == 2800 )
			//double do_it;
		//else
			//continue;



		//double dvs = this->dvs[idep][ilat][ilon];
		sph2cart(lat, lon, dep, &xx, &yy, &zz);
		double RADIUS = 6371;
		xx = xx/RADIUS;
		yy = yy/RADIUS;
		zz = zz/RADIUS;

	//cout  << xx << yy << zz << dvs << dep << lat << lon << endl;
	int size = 15;
	int prec = 6;
	myfile <<fixed
		<<setw(size)<<setprecision(prec) << xx 
		<<setw(size)<<setprecision(prec) << yy
		<<setw(size)<<setprecision(prec) << zz
		<<setw(size)<<setprecision(prec) << dvs 
		<<setw(size)<<setprecision(prec) << dep
		<<setw(size)<<setprecision(prec) << lat
		<<setw(size)<<setprecision(prec) << lon << endl;
		
		//<< "   " << yy << "   " << zz << "   " << dvs <<"   " <<dep <<"   "<< lat <<"   " << lon << endl;
	//cout <<setw(10)<<setprecision(5) << xx << "   " << yy << "   " << zz << endl;

	}
	}



	myfile.close();

	return 0;
}

/******************************************************************
 * This is a c script to output tomography model for STD
 *	
 *
 *	Input:
 *
 *	Output:
 *
 *
 *	DATE:	Aug 2016			Keywords:
 *	Reference:
******************************************************************/
void new_tomo::output_STD_tomography()
{

	cout << "======================== Output STD Tomography info =====================" << endl;
	this->output_tomo_file = "output_STD_tomo." + this->MODEL;

	// define output file parameter
	ofstream myfile;
	myfile.open(this->output_tomo_file.c_str());

	// first output dimensions
	myfile << this->num_dep << endl;
	myfile << 179 << endl;
	myfile << 361 << endl;
	
	double dep;
	double lat;
	double dvs_new;
	double dvs_old;
	double lon;

	// output model file
	int idep,ilat,ilon;
	for(idep = 0; idep < this->num_dep;idep++)
	{	
	for(lat = -89; lat <= 89 ; lat++ )
	for(lon = -180 ; lon <=180 ; lon++)
	{
		//normalize
		double xx,yy,zz;

		//void sph2cart(double lat, double lon, double dep, double* x, double* y, double* z)
			//lon = -180+ilon;
		//if( ilon < this->num_lon2[ilat])
		//{
			dep = this->dep[idep];


		int index_lat_min;
		int index_lon_min;
		index_lat_min = this->find_index_lat(lat);
		index_lon_min = this->find_index_lon(lon, index_lat_min);

		double STD;
		STD = this->my_cell[idep][index_lat_min][index_lon_min].STD;


		//double dvs = this->dvs[idep][ilat][ilon];
		sph2cart(lat, lon, dep, &xx, &yy, &zz);
		double RADIUS = 6371;
		xx = xx/RADIUS;
		yy = yy/RADIUS;
		zz = zz/RADIUS;

	//cout  << xx << yy << zz << dvs << dep << lat << lon << endl;
	int size = 15;
	int prec = 6;
	myfile <<fixed
		<<setw(size)<<setprecision(prec) << xx 
		<<setw(size)<<setprecision(prec) << yy
		<<setw(size)<<setprecision(prec) << zz
		<<setw(size)<<setprecision(prec) << STD
		<<setw(size)<<setprecision(prec) << dep
		<<setw(size)<<setprecision(prec) << lat
		<<setw(size)<<setprecision(prec) << lon << endl;
		
		//<< "   " << yy << "   " << zz << "   " << dvs <<"   " <<dep <<"   "<< lat <<"   " << lon << endl;
	//cout <<setw(10)<<setprecision(5) << xx << "   " << yy << "   " << zz << endl;

	}
	}


	myfile.close();

}
/******************************************************************
 * This is a c script to output tomography model for delta tomography
 *  delta tomo = updated_tomo - old tomo
 *	
 *
 *	Input:
 *
 *	Output:
 *
 *
 *	DATE:	Aug 2016			Keywords:
 *	Reference:
******************************************************************/
void new_tomo::output_delta_tomography(new_tomo* my_tomo)
{

	cout << "======================== Output delta Tomography info =====================" << endl;
	this->output_tomo_file = "output_delta_tomo." + this->MODEL;

	// define output file parameter
	ofstream myfile;
	myfile.open(this->output_tomo_file.c_str());

	// first output dimensions
	myfile << this->num_dep << endl;
	myfile << 179 << endl;
	myfile << 361 << endl;
	
	double dep;
	double lat;
	double dvs_new;
	double dvs_old;
	double lon;

	// output model file
	int idep,ilat,ilon;
	for(idep = 0; idep < this->num_dep;idep++)
	{	
	for(lat = -89; lat <= 89 ; lat++ )
	for(lon = -180 ; lon <=180 ; lon++)
	{
		//normalize
		double xx,yy,zz;

		//void sph2cart(double lat, double lon, double dep, double* x, double* y, double* z)
			//lon = -180+ilon;
		//if( ilon < this->num_lon2[ilat])
		//{
			dep = this->dep[idep];

		dvs_new = this->find_dvs_with_dep_lat_lon(dep,lat,lon);
		dvs_old = my_tomo->find_dvs_with_dep_lat_lon(dep, lat, lon);


		//double dvs = this->dvs[idep][ilat][ilon];
		sph2cart(lat, lon, dep, &xx, &yy, &zz);
		double RADIUS = 6371;
		xx = xx/RADIUS;
		yy = yy/RADIUS;
		zz = zz/RADIUS;

	//cout  << xx << yy << zz << dvs << dep << lat << lon << endl;
	int size = 15;
	int prec = 6;
	myfile <<fixed
		<<setw(size)<<setprecision(prec) << xx 
		<<setw(size)<<setprecision(prec) << yy
		<<setw(size)<<setprecision(prec) << zz
		<<setw(size)<<setprecision(prec) << dvs_new - dvs_old
		<<setw(size)<<setprecision(prec) << dep
		<<setw(size)<<setprecision(prec) << lat
		<<setw(size)<<setprecision(prec) << lon << endl;
		
		//<< "   " << yy << "   " << zz << "   " << dvs <<"   " <<dep <<"   "<< lat <<"   " << lon << endl;
	//cout <<setw(10)<<setprecision(5) << xx << "   " << yy << "   " << zz << endl;

	}
	}


	myfile.close();

}
/******************************************************************
 * This is a c script to output the ray path info in line
 * path info for plot/paraview
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
void new_tomo::output_record_path_line( big_new_record* my_big_record)
{

	string timeinfo_file;
	ofstream myfile;
	int width = 15;
	int prec = 5;

	int ista;
	for(ista = 0; ista < my_big_record->sta_num ; ista++)
	{

		timeinfo_file = "path_line."+ my_big_record->my_record[ista].EQ+ "."
			+my_big_record->my_record[ista].STA
			+"."+my_big_record->my_record[ista].PHASE + ".vtk";
		myfile.open(timeinfo_file.c_str());
		string flag;



		// find the point array along EQ STA geometry
		double current_dist = 0;
		double sta_lat,sta_lon, eq_lat,eq_lon,AZ;
		double current_lat, current_lon, current_dep;
		int idep,ilat,ilon;;
		double dep_tmp,lat_tmp, lon_tmp;
		double dvs;
		double xx,yy,zz;

		int x_dimension, y_dimension, z_dimension;
		x_dimension = my_big_record->my_record[ista].CP_num;
		y_dimension = 1;
		z_dimension = 1;
		int sum_points = x_dimension*y_dimension*z_dimension;
		//myfile << x_dimension << endl;
		//myfile << y_dimension << endl;
		//myfile << z_dimension << endl;

		flag = "# vtk DataFile Version 2.0";
		myfile << flag << endl;
		flag = "line";
		myfile << flag << endl;
		flag = "ASCII";
		myfile << flag << endl;
		flag = "DATASET POLYDATA";
		myfile << flag << endl;
		flag = "POINTS " + std::to_string(x_dimension) + " float" ;
		myfile << flag << endl;


		int count;
		for(count = 0; count < my_big_record->my_record[ista].CP_num ; count++)
		{
			idep = my_big_record->my_record[ista].CP_idep[count];
			ilat = my_big_record->my_record[ista].CP_ilat[count];
			ilon = my_big_record->my_record[ista].CP_ilon[count];


			dep_tmp = my_big_record->my_record[ista].CP_dep[count];
			lat_tmp = my_big_record->my_record[ista].CP_lat[count];
			lon_tmp = my_big_record->my_record[ista].CP_lon[count];

				sph2cart(lat_tmp, lon_tmp, dep_tmp, &xx, &yy, &zz);
				
				double RADIUS = 6371;
				xx /= RADIUS;
				yy /= RADIUS;
				zz /= RADIUS;

				int wid = 10;
				int prec = 4;

				myfile << fixed
					<< setw(wid) << setprecision(prec) << xx
					<< setw(wid) << setprecision(prec) << yy
					<< setw(wid) << setprecision(prec) << zz
					//<< setw(wid) << setprecision(prec) << idep
					//<< setw(wid) << setprecision(prec) << ilat
					//<< setw(wid) << setprecision(prec) << ilon
					<< endl;
		}
		flag = "LINES " + std::to_string(x_dimension -1) +" "+ std::to_string( 3* (x_dimension -1));
		myfile << flag << endl;
		for(count = 0; count < x_dimension -1 ; count++)
		{
			myfile << 2 << "  " << count << "  " << count +1 << endl;
		}

		myfile.close();
	}


}
/******************************************************************
 * This is a c script to output the ray path info in line
 * path info for plot/paraview
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
void new_tomo::output_record_path_line_in_one_file( big_new_record* my_big_record)
{
	cout <<"output big path file in a single file" << endl;

	string timeinfo_file;
	ofstream myfile;
	int width = 15;
	int prec = 5;
	int ista;
	string flag;
	int count;
	timeinfo_file = "big_path_line.vtk";

	myfile.open(timeinfo_file.c_str());


	int sum_data_points;				// all point number in path
	int sum_line_number;
	int sum_line_points;
	sum_line_number = 0;
	sum_line_points = 0;
	sum_data_points = 0;

	int delta_points_interp;			// we desample the path to make it easier to see
	delta_points_interp = 10;
	//cout << "station num is " << my_big_record->sta_num << endl;

	for(ista = 0; ista < my_big_record->sta_num ; ista++)
	{

		int x_dimension, y_dimension, z_dimension;
		x_dimension = my_big_record->my_record[ista].CP_num;
		x_dimension = floor(x_dimension / delta_points_interp) +1;
		//cout << "x_dimension is " << x_dimension << endl;
		sum_data_points += x_dimension;
		sum_line_number += x_dimension -1;
		sum_line_points += 3*(x_dimension -1);

	}
	cout << " big path info " << sum_data_points << " " << sum_line_number << " sum_line_points" <<  sum_line_points << endl;
		flag = "# vtk DataFile Version 2.0";
		myfile << flag << endl;
		flag = "line";
		myfile << flag << endl;
		flag = "ASCII";
		myfile << flag << endl;
		flag = "DATASET POLYDATA";
		myfile << flag << endl;
		flag = "POINTS " + std::to_string(sum_data_points) + " float" ;
		myfile << flag << endl;

	// add points into it
	for(ista = 0; ista < my_big_record->sta_num ; ista++)
	{

		string flag;

		// find the point array along EQ STA geometry
		double current_dist = 0;
		double sta_lat,sta_lon, eq_lat,eq_lon,AZ;
		double current_lat, current_lon, current_dep;
		int idep,ilat,ilon;;
		double dep_tmp,lat_tmp, lon_tmp;
		double dvs;
		double xx,yy,zz;

		int x_dimension, y_dimension, z_dimension;
		x_dimension = my_big_record->my_record[ista].CP_num;
		y_dimension = 1;
		z_dimension = 1;
		int sum_points = x_dimension*y_dimension*z_dimension;


		cout << "print path file "<< ista << endl;
		int count;
		for(count = 0; count < my_big_record->my_record[ista].CP_num ; count++)
		{

			if(count % delta_points_interp != 0)
				continue;

			idep = my_big_record->my_record[ista].CP_idep[count];
			ilat = my_big_record->my_record[ista].CP_ilat[count];
			ilon = my_big_record->my_record[ista].CP_ilon[count];


			dep_tmp = my_big_record->my_record[ista].CP_dep[count];
			lat_tmp = my_big_record->my_record[ista].CP_lat[count];
			lon_tmp = my_big_record->my_record[ista].CP_lon[count];

				sph2cart(lat_tmp, lon_tmp, dep_tmp, &xx, &yy, &zz);
				
				double RADIUS = 6371;
				xx /= RADIUS;
				yy /= RADIUS;
				zz /= RADIUS;

				int wid = 10;
				int prec = 4;

				myfile << fixed
					<< setw(wid) << setprecision(prec) << xx
					<< setw(wid) << setprecision(prec) << yy
					<< setw(wid) << setprecision(prec) << zz
					//<< setw(wid) << setprecision(prec) << idep
					//<< setw(wid) << setprecision(prec) << ilat
					//<< setw(wid) << setprecision(prec) << ilon
					<< endl;
		}

	}

	flag = "LINES " + std::to_string(sum_line_number) +" "+ std::to_string( sum_line_points);
	myfile << flag << endl;
	// add connectivity to it
	int point_count;
	point_count = 0;
	for(ista = 0; ista < my_big_record->sta_num ; ista++)
	{
		int x_dimension, y_dimension, z_dimension;
		x_dimension = my_big_record->my_record[ista].CP_num;
		x_dimension = floor(x_dimension / delta_points_interp) +1;
cout << "ista x_dimension " << ista <<"  "<<  x_dimension << endl;
		int kkk;
		kkk = point_count;
		for(count = point_count ; count < point_count +x_dimension -1 ; count++)
		{
			//if( (count-point_count ) % delta_points_interp != 0)
				//continue;
			myfile << 2 << "  " << kkk << "  " << kkk +1 << endl;
			kkk ++;
		}
		point_count += x_dimension;
	}

	myfile.close();
}
/******************************************************************
 * This is a c script to output cross section of EQ STA geometry
 * path info for plot/paraview
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
void new_tomo::output_record_path_crosssection( big_new_record* my_big_record)
{

	string timeinfo_file;
	ofstream myfile;
	int width = 15;
	int prec = 5;

	int ista;
	for(ista = 0; ista < my_big_record->sta_num ; ista++)
	{

		timeinfo_file = "cross_section."+ my_big_record->my_record[ista].EQ +"." +
			my_big_record->my_record[ista].STA+
			my_big_record->my_record[ista].PHASE + ".pathinfo";
		myfile.open(timeinfo_file.c_str());

		// find the point array along EQ STA geometry
		double current_dist = 0;
		double sta_lat,sta_lon, eq_lat,eq_lon,AZ;
		double current_lat, current_lon, current_dep;
		int idep,ilat,ilon;;
		double dep_tmp;
		double dvs;
		double xx,yy,zz;

		int x_dimension, y_dimension, z_dimension;
		x_dimension = (int) ( my_big_record->my_record[ista].DIST +2);
		y_dimension = this->num_dep;
		z_dimension = 1;
		int sum_points = x_dimension*y_dimension*z_dimension;
		myfile << x_dimension << endl;
		myfile << y_dimension << endl;
		myfile << z_dimension << endl;


		double distance_one_degree = 111;
		for(current_dist = 0; current_dist < x_dimension * distance_one_degree; current_dist = current_dist +distance_one_degree)
		{
			AZ = my_big_record->my_record[ista].AZ;
			eq_lat = my_big_record->my_record[ista].eq_lat;
			eq_lon = my_big_record->my_record[ista].eq_lon;

			point_AZ_dist_point(eq_lat, eq_lon, AZ, current_dist, &current_lat, &current_lon);
			ilat = find_index_lat(current_lat);
			ilon = find_index_lon(current_lon, ilat);

			for(idep = 0; idep < this->num_dep ;idep++)
			{
				
				dep_tmp = this->dep[idep];
				dvs = this->my_cell[idep][ilat][ilon].dvs ;

				sph2cart(current_lat, current_lon, dep_tmp, &xx, &yy, &zz);
				
				double RADIUS = 6371;
				xx /= RADIUS;
				yy /= RADIUS;
				zz /= RADIUS;

				int wid = 10;
				int prec = 4;

				myfile << fixed
					<< setw(wid) << setprecision(prec) << xx
					<< setw(wid) << setprecision(prec) << yy
					<< setw(wid) << setprecision(prec) << zz
					<< setw(wid) << setprecision(prec) << dvs
					<< setw(wid) << setprecision(prec) << dep_tmp
					<< setw(wid) << setprecision(prec) << current_lat
					<< setw(wid) << setprecision(prec) << current_lon
					<< endl;


			}


		}

		myfile.close();
	}


}

/******************************************************************
 * This is a c script to output timeinfo
 * 1. PREM time by taup_path given by eventinfo
 * 2. PREM time by code
 * 3. dt_PREM_taup_code
 * 4. observation time
 * 5. dt_obs_PREM_code = dt (T_obs - T_prem_code) = dt_obs_prem +PREM_taup - PREM_ code
 * 6. tomography correction for starting tomography
 * 7. dt_residual_after_tomo_correc = dt_obs_PREM_code - tomo_correction
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
void new_tomo::output_time_info( big_new_record* my_big_record)
{

	string timeinfo_file;
	//timeinfo_file = "timeinfo."+this->MODEL + ".iteration." +  to_string(this->current_iteration) + ".data";
	timeinfo_file = my_big_record->timeinfo_outfile;
	ofstream myfile;
	myfile.open(timeinfo_file.c_str());
	int width = 15;
	int prec = 5;

	int ista;
	double dt_PREM_taup_code;
	double dt_obs_PREM_code;
	double dt_residual_after_tomo_correc;

	for(ista = 0; ista < my_big_record->sta_num ; ista++)
	{

		dt_PREM_taup_code = my_big_record->my_record[ista].PREM - my_big_record->my_record[ista].code_PREM;
		dt_obs_PREM_code = my_big_record->my_record[ista].dt_obs_prem + dt_PREM_taup_code;
		dt_residual_after_tomo_correc = dt_obs_PREM_code - my_big_record->my_record[ista].dt_tomo_correction;
		myfile << fixed
			<< setw(width) << my_big_record->my_record[ista].EQ
			<< setw(width) << my_big_record->my_record[ista].STA
			<< setw(width) << my_big_record->my_record[ista].PHASE
			<< setw(width) << my_big_record->my_record[ista].COMP
			<< setw(width) << setprecision(prec) << my_big_record->my_record[ista].PREM
			<< setw(width) << setprecision(prec) << my_big_record->my_record[ista].code_PREM
			<< setw(width) << setprecision(prec) << dt_PREM_taup_code
			<< setw(width) << setprecision(prec) << my_big_record->my_record[ista].dt_obs_prem
			<< setw(width) << setprecision(prec) << dt_obs_PREM_code
			<< setw(width) << setprecision(prec) << my_big_record->my_record[ista].dt_tomo_correction
			<< setw(width) << setprecision(prec) << dt_residual_after_tomo_correc

			<< endl;


	}


}


/******************************************************************
 * This is a c script to 
 * update for phase with bottoming depth < K km
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
void new_tomo::S1_update(new_record* my_record)
{
	//cout << "===> Working on S1 update " << endl;
	double K = 2200;

	int ista;
	int count;
	double dep_tmp1;
	int dep_index_tmp1;
	if(this->current_layer_num == 1 )
	{
		this->current_layer_dep_min = 0;
		this->current_layer_dep_max = this->tomo_layer_array[this->current_layer_num-1];
	}
	else
	{
		this->current_layer_dep_min = this->tomo_layer_array[this->current_layer_num-2];
		this->current_layer_dep_max = this->tomo_layer_array[this->current_layer_num-1];
	}
	//cout << "current layer min max "<< this->current_layer_dep_min << "  " << this->current_layer_dep_max << endl;



	// make sure that bottoming depth is out of range goes to bad, current_iteration_coeff = 0
	double bottoming_depth = my_record->turning_depth;
	if( bottoming_depth < this->current_layer_dep_min || bottoming_depth > this->current_layer_dep_max )
		my_record->current_iteration_coeff = 0;
	else 
		my_record->current_iteration_coeff = 1;
	

	for(count = 0; count < my_record->CP_num  ; count ++)
	{
		dep_index_tmp1 = my_record->CP_idep[count];
		dep_tmp1 = my_record->CP_dep[count];
		my_record->current_CP_weight[dep_index_tmp1] = this->RMS_weight[dep_index_tmp1] ;
	}

}
//void new_tomo::S2_update(big_new_record* my_big_record)
//{
	//cout << "===> Working on S2 update " << endl;
	//double K = 2200;

	//int ista;
	//int count;
	//double dep_tmp1;
	//int dep_index_tmp1;
	//if(this->current_layer_num == 1 )
	//{
		//this->current_layer_dep_min = 0;
		//this->current_layer_dep_max = this->tomo_layer_array[this->current_layer_num-1];
	//}
	//else
	//{
		//this->current_layer_dep_min = this->tomo_layer_array[this->current_layer_num-2];
		//this->current_layer_dep_max = this->tomo_layer_array[this->current_layer_num-1];
	//}
	////cout << "current layer min max "<< this->current_layer_dep_min << "  " << this->current_layer_dep_max << endl;

	//for(ista = 0; ista < my_big_record->sta_num; ista++)
	//{

		//cout << "working on sta "<< ista << endl;

		//// make sure that bottoming depth is out of range goes to bad, current_iteration_coeff = 0
		//double bottoming_depth = my_big_record->my_record[ista].turning_depth;
		//if( bottoming_depth < this->current_layer_dep_min || bottoming_depth > this->current_layer_dep_max )
			//my_big_record->my_record[ista].current_iteration_coeff = 0;
		//else 
			//my_big_record->my_record[ista].current_iteration_coeff = 1;
		

		//// makesure lower most layer just update Dmin Dmax
		//for(count = 0; count < my_big_record->my_record[ista].CP_num  ; count ++)
		//{
			//dep_index_tmp1 = my_big_record->my_record[ista].CP_idep[count];
			//dep_tmp1 = my_big_record->my_record[ista].CP_dep[count];
			//my_big_record->my_record[ista].current_CP_weight[dep_index_tmp1] = this->RMS_weight[dep_index_tmp1] ;
			//// modify the lowermost layer
			//if(this->current_layer_dep_min == K && dep_tmp1 < K )
				//my_big_record->my_record[ista].current_CP_weight[dep_index_tmp1] = 0;
			//else if(this->current_layer_dep_min == K && dep_tmp1 > K )
				//my_big_record->my_record[ista].current_CP_weight[dep_index_tmp1] = this->RMS_weight[dep_index_tmp1] ;


		//}
	//}

//}
//void new_tomo::S3_update(big_new_record* my_big_record)
//{
	//cout << "===> Working on S3 update " << endl;
	//double K = 2200;

	//int ista;
	//int count;
	//double dep_tmp1;
	//int dep_index_tmp1;
	//if(this->current_layer_num == 1 )
	//{
		//this->current_layer_dep_min = 0;
		//this->current_layer_dep_max = this->tomo_layer_array[this->current_layer_num-1];
	//}
	//else
	//{
		//this->current_layer_dep_min = this->tomo_layer_array[this->current_layer_num-2];
		//this->current_layer_dep_max = this->tomo_layer_array[this->current_layer_num-1];
	//}
	////cout << "current layer min max "<< this->current_layer_dep_min << "  " << this->current_layer_dep_max << endl;

	//for(ista = 0; ista < my_big_record->sta_num; ista++)
	//{

		//cout << "working on sta "<< ista << endl;

		//// make sure that bottoming depth is out of range goes to bad, current_iteration_coeff = 0
		//double bottoming_depth = my_big_record->my_record[ista].turning_depth;
		//if(  bottoming_depth > this->current_layer_dep_max && this->current_layer_dep_min != K)
			//my_big_record->my_record[ista].current_iteration_coeff = 0;
		//else if (this->current_layer_dep_min == K && bottoming_depth < this->current_layer_dep_min )
			//my_big_record->my_record[ista].current_iteration_coeff = 0;
		//else 
			//my_big_record->my_record[ista].current_iteration_coeff = 1;
		

		//// makesure lower most layer just update Dmin Dmax
		//for(count = 0; count < my_big_record->my_record[ista].CP_num  ; count ++)
		//{
			//dep_index_tmp1 = my_big_record->my_record[ista].CP_idep[count];
			//dep_tmp1 = my_big_record->my_record[ista].CP_dep[count];
			//my_big_record->my_record[ista].current_CP_weight[dep_index_tmp1] = this->RMS_weight[dep_index_tmp1] ;
			//// modify the lowermost layer
			//if(this->current_layer_dep_min != K )
				//my_big_record->my_record[ista].current_CP_weight[dep_index_tmp1] = this->RMS_weight[dep_index_tmp1] ;
			//else if(this->current_layer_dep_min == K && dep_tmp1 < K )
				//my_big_record->my_record[ista].current_CP_weight[dep_index_tmp1] = 0 ;
			//else if(this->current_layer_dep_min == K && dep_tmp1 > K )
				//my_big_record->my_record[ista].current_CP_weight[dep_index_tmp1] = this->RMS_weight[dep_index_tmp1] ;

		//}
	//}

//}



//void new_tomo::S4_update(big_new_record* my_big_record)
//{
	//cout << "===> Working on S4 update " << endl;
	//double K = 2200;

	//int ista;
	//int count;
	//double dep_tmp1;
	//int dep_index_tmp1;
	//if(this->current_layer_num == 1 )
	//{
		//this->current_layer_dep_min = 0;
		//this->current_layer_dep_max = this->tomo_layer_array[this->current_layer_num-1];
	//}
	//else
	//{
		//this->current_layer_dep_min = this->tomo_layer_array[this->current_layer_num-2];
		//this->current_layer_dep_max = this->tomo_layer_array[this->current_layer_num-1];
	//}
	////cout << "current layer min max "<< this->current_layer_dep_min << "  " << this->current_layer_dep_max << endl;

	//for(ista = 0; ista < my_big_record->sta_num; ista++)
	//{

		//cout << "working on sta "<< ista << endl;

		//// make sure that bottoming depth is out of range goes to bad, current_iteration_coeff = 0
		//double bottoming_depth = my_big_record->my_record[ista].turning_depth;
		//if(  bottoming_depth > this->current_layer_dep_max || bottoming_depth < this->current_layer_dep_min )
			//my_big_record->my_record[ista].current_iteration_coeff = 0;
		//else 
			//my_big_record->my_record[ista].current_iteration_coeff = 1;
		

		//// makesure lower most layer just update Dmin Dmax
		//for(count = 0; count < my_big_record->my_record[ista].CP_num  ; count ++)
		//{
			//dep_index_tmp1 = my_big_record->my_record[ista].CP_idep[count];
			//dep_tmp1 = my_big_record->my_record[ista].CP_dep[count];
			////my_big_record->my_record[ista].current_CP_weight[dep_index_tmp1] = this->RMS_weight[dep_index_tmp1] ;
			//// modify the lowermost layer
			////if(this->current_layer_dep_min != K )
			//if(  dep_tmp1> this->current_layer_dep_max || dep_tmp1 < this->current_layer_dep_min )
				//my_big_record->my_record[ista].current_CP_weight[dep_index_tmp1] = 0 ;
			//else 
				//my_big_record->my_record[ista].current_CP_weight[dep_index_tmp1] = this->RMS_weight[dep_index_tmp1] ;

		//}
	//}

//}

//void new_tomo::S5_update(big_new_record* my_big_record)
//{
	//cout << "===> Working on S5 update " << endl;
	//double K = 2200;

	//int ista;
	//int count;
	//double dep_tmp1;
	//int dep_index_tmp1;
	//if(this->current_layer_num == 1 )
	//{
		//this->current_layer_dep_min = 0;
		//this->current_layer_dep_max = this->tomo_layer_array[this->current_layer_num-1];
	//}
	//else
	//{
		//this->current_layer_dep_min = this->tomo_layer_array[this->current_layer_num-2];
		//this->current_layer_dep_max = this->tomo_layer_array[this->current_layer_num-1];
	//}
	////cout << "current layer min max "<< this->current_layer_dep_min << "  " << this->current_layer_dep_max << endl;

	//for(ista = 0; ista < my_big_record->sta_num; ista++)
	//{

		//cout << "working on sta "<< ista << endl;

		//// make sure that bottoming depth is out of range goes to bad, current_iteration_coeff = 0
		//double bottoming_depth = my_big_record->my_record[ista].turning_depth;
		//if(  bottoming_depth < K  && this->current_layer_dep_min != K)
			//my_big_record->my_record[ista].current_iteration_coeff = 1;
		//else if(  bottoming_depth > K  && this->current_layer_dep_min == K)
			//my_big_record->my_record[ista].current_iteration_coeff = 1;
		//else 
			//my_big_record->my_record[ista].current_iteration_coeff = 0;

		

		//// makesure lower most layer just update Dmin Dmax
		//for(count = 0; count < my_big_record->my_record[ista].CP_num  ; count ++)
		//{
			//dep_index_tmp1 = my_big_record->my_record[ista].CP_idep[count];
			//dep_tmp1 = my_big_record->my_record[ista].CP_dep[count];
			//if(  dep_tmp1> this->current_layer_dep_max || dep_tmp1 < this->current_layer_dep_min )
				//my_big_record->my_record[ista].current_CP_weight[dep_index_tmp1] = 0 ;
			//else 
				//my_big_record->my_record[ista].current_CP_weight[dep_index_tmp1] = this->RMS_weight[dep_index_tmp1] ;

		//}
	//}

//}

//void new_tomo::S2_update(big_new_record* my_big_record)
//{
	//double K = 2200;

	//int ista;
	//int count;
	//double dep_tmp1;
	//int dep_index_tmp1;

	//for(ista = 0; ista < my_big_record->sta_num; ista++)
	//{

		//if(my_big_record->my_record[ista].turning_depth >= K )
		//{
			//my_big_record->my_record[ista].current_iteration_coeff = 1;
			//// modify shallow RMS weight = 0
			//for(count = 0; count < my_big_record->my_record[ista].CP_num  ; count ++)
			//{
				//dep_index_tmp1 = my_big_record->my_record[ista].CP_idep[count];
				//dep_tmp1 = my_big_record->my_record[ista].CP_dep[count];
				//if( dep_tmp1 >= K )
				//{
					//my_big_record->my_record[ista].current_CP_weight[dep_index_tmp1] = this->RMS_weight[dep_index_tmp1] ;
					////cout << "dep "<< dep_tmp1 << "RMS weight "<< this->RMS_weight[dep_index_tmp1]  << endl;
				//}
				//else
					//my_big_record->my_record[ista].current_CP_weight[dep_index_tmp1] = 0;

			//}

		//}
		//else 
			//my_big_record->my_record[ista].current_iteration_coeff = 0;
	//}

//}
//void new_tomo::S3_update(big_new_record* my_big_record)
//{
	//double K = 2200;

	//int ista;
	//int count;
	//double dep_tmp1;
	//int dep_index_tmp1;

	//for(ista = 0; ista < my_big_record->sta_num; ista++)
	//{

		//if(my_big_record->my_record[ista].turning_depth >= K )
		//{
			//my_big_record->my_record[ista].current_iteration_coeff = 1;
			//// modify shallow RMS weight = 0
			//dep_index_tmp1 = my_big_record->my_record[ista].CP_idep[count];
			//dep_tmp1 = my_big_record->my_record[ista].CP_dep[count];
			//for(count = 0; count < my_big_record->my_record[ista].CP_num  ; count ++)
			//{
				//my_big_record->my_record[ista].current_CP_weight[dep_index_tmp1] = this->RMS_weight[dep_index_tmp1] ;
			//}

		//}
		//else 
			//my_big_record->my_record[ista].current_iteration_coeff = 0;
	//}

//}

//void new_tomo::S4_update(big_new_record* my_big_record)
//{
	//double K = 2200;

	//int ista;
	//int count;
	//double dep_tmp1;
	//int dep_index_tmp1;

	//for(ista = 0; ista < my_big_record->sta_num; ista++)
	//{

			//// modify shallow RMS weight = 0
			//dep_index_tmp1 = my_big_record->my_record[ista].CP_idep[count];
			//dep_tmp1 = my_big_record->my_record[ista].CP_dep[count];
			//for(count = 0; count < my_big_record->my_record[ista].CP_num  ; count ++)
			//{
					//my_big_record->my_record[ista].current_CP_weight[dep_index_tmp1] = this->RMS_weight[dep_index_tmp1] ;
			//}
	//}

//}

void new_tomo::calculate_turning_depth(new_record* my_record)
{
	int ista;
	int count;

		// find turning depth for current record
		// my_record->
		my_record->turning_depth = 0;
		for(count = 0; count < my_record->CP_num ; count++)
		{
			////max_dep = my_record->CP_dep[count];
			//cout << "record CP_dep is "<< my_record->CP_dep[count] << endlg_record->my_record[ista].o;
			
			if(  my_record->CP_dep[count] > my_record->turning_depth )
				my_record->turning_depth = my_record->CP_dep[count];
		}

		//cout << " ista "<< ista << "turning dep " << my_big_record->my_record[ista].turning_depth << endl;


}

/******************************************************************
 * This is a c script for forward tomography main function
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
void new_tomo::forward_tomography_func(big_new_record* my_big_record)
{

	cout << "--> Begin forward tomography" << endl;


	int iteration;
	int layer_correction_num;
	//this->calculate_turning_depth(my_big_record);
	this->construct_RMS_profile();



	for(layer_correction_num = 1; layer_correction_num <= this->tomo_layer_number ; layer_correction_num ++ )
	{
		this->current_layer_num = layer_correction_num;
		cout << "working on layer " << layer_correction_num << endl;
		for(iteration = 1; iteration <= this->Iteration_M ; iteration ++)
		{
			cout <<"-- > working on  Iteration "<< iteration  << endl;
			this->forward_tomo_for_station(my_big_record);
			my_big_record->timeinfo_outfile = "timeinfo.layer."+ std::to_string(layer_correction_num) + ".iteration."+ to_string(iteration)+".info" ;
			//if( layer_correction_num == this->tomo_layer_number && iteration == 1)
			if( layer_correction_num == this->tomo_layer_number && iteration == 1)
			{
				this->output_coverage_hit_count();
				//this->output_cross_section( my_big_record );
			}
			this->output_time_info(my_big_record);
			this->update_tomo_for_current_iteration();


		}
	}



	//// ===================================================================
	////
	////	Processing S 1 3 2
	////
	//// ==================================================================

	//if(flag.compare("S_1_3_2") == 0)
	//{
		//this->S1_update(my_big_record);
		//for(iteration = 1; iteration <= this->Iteration_M ; iteration ++)
		//{
			//cout <<"-- > working on layer M Iteration "<< iteration  << endl;
			//this->forward_tomo_for_station(my_big_record);
			//my_big_record->timeinfo_outfile = "timeinfo.upper_layer."+to_string(iteration)+".flag."+flag+".info" ;
			//this->output_time_info(my_big_record);
			//this->update_tomo_for_current_iteration();
		//}
		//this->S3_update(my_big_record);
		//for(iteration = 1; iteration <= this->Iteration_MN ; iteration ++)
		//{
			//cout <<"-- > working on both MN Iteration "<< iteration  << endl;
			//this->forward_tomo_for_station(my_big_record);
			//my_big_record->timeinfo_outfile = "timeinfo.both_layer."+to_string(iteration)+".flag."+flag+".info" ;
			//this->output_time_info(my_big_record);
			//this->update_tomo_for_current_iteration();
		//}
		//this->S2_update(my_big_record);
		//for(iteration = 1; iteration <= this->Iteration_N ; iteration ++)
		//{
			//cout <<"-- > working on layer N Iteration "<< iteration  << endl;
			//this->forward_tomo_for_station(my_big_record);
			//my_big_record->timeinfo_outfile = "timeinfo.lower_layer."+to_string(iteration)+".flag."+flag+".info" ;
			//this->output_time_info(my_big_record);
			//this->update_tomo_for_current_iteration();
		//}
	//}

	//// ===================================================================
	////
	////	Processing S 4 3 2
	////
	//// ==================================================================

	//if(flag.compare("S_4_3_2") == 0)
	//{
		//this->S4_update(my_big_record);
		//for(iteration = 1; iteration <= this->Iteration_MN ; iteration ++)
		//{
			//cout <<"-- > working on layer both MN Iteration "<< iteration  << endl;
			//this->forward_tomo_for_station(my_big_record);
			//my_big_record->timeinfo_outfile = "timeinfo.both_layer."+to_string(iteration)+".flag."+flag+".info" ;
			//this->output_time_info(my_big_record);
			//this->update_tomo_for_current_iteration();
		//}
		//this->S3_update(my_big_record);
		//for(iteration = 1; iteration <= this->Iteration_MN ; iteration ++)
		//{
			//cout <<"-- > working on both MN Iteration "<< iteration  << endl;
			//this->forward_tomo_for_station(my_big_record);
			//my_big_record->timeinfo_outfile = "timeinfo.both_layer."+to_string(iteration)+".flag."+flag+".info" ;
			//this->output_time_info(my_big_record);
			//this->update_tomo_for_current_iteration();
		//}
		//this->S2_update(my_big_record);
		//for(iteration = 1; iteration <= this->Iteration_N ; iteration ++)
		//{
			//cout <<"-- > working on layer N Iteration "<< iteration  << endl;
			//this->forward_tomo_for_station(my_big_record);
			//my_big_record->timeinfo_outfile = "timeinfo.lower_layer."+to_string(iteration)+".flag."+flag+".info" ;
			//this->output_time_info(my_big_record);
			//this->update_tomo_for_current_iteration();
		//}
	//}

	//// ===================================================================
	////
	////	Processing S 3 2
	////
	//// ==================================================================

	//if(flag.compare("S_3_2") == 0)
	//{
		//this->S3_update(my_big_record);
		//for(iteration = 1; iteration <= this->Iteration_MN ; iteration ++)
		//{
			//cout <<"-- > working on both MN Iteration "<< iteration  << endl;
			//this->forward_tomo_for_station(my_big_record);
			//my_big_record->timeinfo_outfile = "timeinfo.both_layer."+to_string(iteration)+".flag."+flag+".info" ;
			//this->output_time_info(my_big_record);
			//this->update_tomo_for_current_iteration();
		//}
		//this->S2_update(my_big_record);
		//for(iteration = 1; iteration <= this->Iteration_N ; iteration ++)
		//{
			//cout <<"-- > working on layer N Iteration "<< iteration  << endl;
			//this->forward_tomo_for_station(my_big_record);
			//my_big_record->timeinfo_outfile = "timeinfo.lower_layer."+to_string(iteration)+".flag."+flag+".info" ;
			//this->output_time_info(my_big_record);
			//this->update_tomo_for_current_iteration();
		//}
	//}

	//// ===================================================================
	////
	////	Processing S 2
	////
	//// ==================================================================

	//if(flag.compare("S_2") == 0)
	//{
		//this->S2_update(my_big_record);
		//for(iteration = 1; iteration <= this->Iteration_N ; iteration ++)
		//{
			//cout <<"-- > working on layer N Iteration "<< iteration  << endl;
			//this->forward_tomo_for_station(my_big_record);
			//my_big_record->timeinfo_outfile = "timeinfo.lower_layer."+to_string(iteration)+".flag."+flag+".info" ;
			//this->output_time_info(my_big_record);
			//this->update_tomo_for_current_iteration();
		//}
	//}

	//for(iteration = 1; iteration <= this->tomo_iterations; iteration ++)
	//{
		//this->current_layer_num = iteration;

		//cout <<  " ======================================= " << endl;
		//cout <<  " ======================================= " << endl;
		//cout <<  " ======================================= " << endl;
		//cout << "		---> Working on Iteration:   " << iteration << endl;
		//this->current_iteration = iteration;

		//for(layer_correction_num = 1; layer_correction_num <= this->tomo_layer_number ; layer_correction_num ++ )
		//{
			//this->current_layer_num = layer_correction_num;

			////initiate for current run
			//// do station 
			//this->forward_tomo_for_station(my_big_record);

		//}

		//if(iteration == 1)
		//{
			//// output hit count/ coverage map
			//this->output_coverage_hit_count();
		//}


		//// for each iteration, we output timeinfo to evaluate current iteration
		//this->output_time_info(my_big_record);

		//// update current tomography for this iteration
		//this->update_tomo_for_current_iteration();

	//}



	// output record info


}


void new_tomo::output_coverage_hit_count()
{

	cout <<"--> Output hit count for tomo" << endl;

	string hit_count_file;
	hit_count_file = "hit_count."+this->MODEL+".data";
	ofstream myfile;
	myfile.open(hit_count_file.c_str());


	// first output dimensions
	
	double dep;
	double lat;
	double dvs;
	double lon;


	myfile << this->num_dep << endl;
	myfile << 179 << endl;
	myfile << 361 << endl;
	// output model file
	int idep,ilat,ilon;
	int index_lon, index_lat;
	int num_of_cell;
	for(idep = 0; idep < this->num_dep;idep++)
	{	
	for(lat = -89; lat <= 89 ; lat++ )
	for(lon = -180 ; lon <=180 ; lon++)
	{
		//normalize
		double xx,yy,zz;

		dep = this->dep[idep];
		index_lat = this->find_index_lat(lat);
		index_lon = this->find_index_lon(lon, index_lat);

		num_of_cell = this->my_cell[idep][index_lat][index_lon].sum_num_in_cell;




		//double dvs = this->dvs[idep][ilat][ilon];
		sph2cart(lat, lon, dep, &xx, &yy, &zz);
		double RADIUS = 6371;
		xx = xx/RADIUS;
		yy = yy/RADIUS;
		zz = zz/RADIUS;

	//cout  << xx << yy << zz << dvs << dep << lat << lon << endl;
	int size = 15;
	int prec = 6;
	myfile <<fixed
		<<setw(size)<<setprecision(prec) << xx 
		<<setw(size)<<setprecision(prec) << yy
		<<setw(size)<<setprecision(prec) << zz
		<<setw(size)<<setprecision(prec) << num_of_cell
		<<setw(size)<<setprecision(prec) << dep
		<<setw(size)<<setprecision(prec) << lat
		<<setw(size)<<setprecision(prec) << lon << endl;
		
		//<< "   " << yy << "   " << zz << "   " << dvs <<"   " <<dep <<"   "<< lat <<"   " << lon << endl;
	//cout <<setw(10)<<setprecision(5) << xx << "   " << yy << "   " << zz << endl;

	}
	}

	myfile.close();


}

/******************************************************************
 * This is a c script to update tomography model for current iteration
 * and clean the accumulated value in my_cell:
 * sum_dvs and sum_num for each cell
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

void new_tomo::update_tomo_for_current_iteration()
{
	cout << "--> Update tomo for current iteration " << endl;


	int count;
	int idep,ilat,ilon;
	double new_dvs;
	double delta_dvs ;
	double old_dvs;

	for(idep = 0; idep < this->num_dep ; idep++)
		for(ilat = 0; ilat < this->num_lat; ilat++)
			for(ilon = 0; ilon < this->num_lon2[ilat] ; ilon++)
			{

				if( this->my_cell[idep][ilat][ilon].sum_num_in_cell <= 0 )
					continue;

				//if( this->my_cell[idep][ilat][ilon].sum_num_in_cell != this->my_cell[idep][ilat][ilon].sum_num_in_cell)
					//cout << "ERROR this->my_cell[idep][ilat][ilon].sum_num_in_cell is NAN"<<endl;


				old_dvs = this->my_cell[idep][ilat][ilon].dvs;
				new_dvs = this->my_cell[idep][ilat][ilon].sum_dvs_in_cell / 
					this->my_cell[idep][ilat][ilon].sum_num_in_cell;


				delta_dvs = new_dvs - old_dvs;

				this->my_cell[idep][ilat][ilon].delta_dvs = delta_dvs;
				this->my_cell[idep][ilat][ilon].dvs = new_dvs;

				//if (new_dvs != new_dvs )
					//cout << "ERROR new_dvs is NAN" << endl;

				// initiate 
				this->my_cell[idep][ilat][ilon].sum_dvs_in_cell = 0;
				this->my_cell[idep][ilat][ilon].sum_num_in_cell = 0;
			}


}






void new_tomo::forward_tomo_for_station(big_new_record* my_big_record)
{

	int ista;

	int num_of_sta_go_into_current_layer;
	int tmp;
	num_of_sta_go_into_current_layer = 0;
	for(ista = 0; ista < my_big_record->sta_num; ista++)
	{
		//cout << "-- > working on station " << my_big_record->my_record[ista].STA << endl;
		tmp = this->forward_tomo_for_one_station(&my_big_record->my_record[ista]);
		num_of_sta_go_into_current_layer += tmp;
	}

	cout << "--> Working on tomo for current layer " << num_of_sta_go_into_current_layer << "/" << my_big_record->sta_num
		<< " station is processed" << endl;


}

int new_tomo::forward_tomo_for_one_station(new_record* my_record)
{

	int count;
	//double max_dep;
	//max_dep = 0;
	//// find turning depth for current record
	//my_record->turning_depth = 0;
	//for(count = 0; count < my_record->CP_num ; count++)
	//{
		//////max_dep = my_record->CP_dep[count];
		////cout << "record CP_dep is "<< my_record->CP_dep[count] << endl;
		
		//if(  my_record->CP_dep[count] > my_record->turning_depth )
			//my_record->turning_depth = my_record->CP_dep[count];
	//}
	


	// decide whether this record goes into current layer correction
	//if(this->current_layer_num == 1 )
	//{
		//this->current_layer_dep_min = 0;
		//this->current_layer_dep_max = this->tomo_layer_array[this->current_layer_num-1];
	//}
	//else
	//{
		//this->current_layer_dep_min = this->tomo_layer_array[this->current_layer_num-2];
		//this->current_layer_dep_max = this->tomo_layer_array[this->current_layer_num-1];
	//}

	//cout << "current bottoming depth is " << my_record->turning_depth << endl;
	//cout << "current layer min max is "<<  current_layer_dep_min
		//<< "   " << current_layer_dep_max << endl;


	//if( my_record->turning_depth >= this->current_layer_dep_min 
			//&& my_record->turning_depth <= this->current_layer_dep_max )
	//{
		//double we_continue_to_work_on_this_record;
		//we_continue_to_work_on_this_record = 0;
	//}
	//else
	//{
		//return 0;
	//}	

	//cout << "working on tomo for one sta for record " << my_record->STA << endl;



	// initiate current record CP memory
	//cout << " ============== initiate CP"<< endl;
	my_record->initiate_CP();

	// read in single record cross-point info 
	//cout << " ============== read  cross point"<< endl;
	my_record->read_cross_point_info(this);



	// calculate turning depth of current record
	this->calculate_turning_depth(my_record);

	// determine update flag
	//cout << " =================== update iteration scheme flag"<< endl;
	string flag = this->Iteration_Scheme;
		if(flag.compare("S_1") == 0)
			this->S1_update(my_record);
		//else if(flag.compare("S_2") == 0)
			//this->S2_update(my_big_record);
		//else if(flag.compare("S_3") == 0)
			//this->S3_update(my_big_record);
		//else if(flag.compare("S_4") == 0)
			//this->S4_update(my_big_record);

	// decide if current record will update current layer
	// if current_iteration_coeff = 0 we skip
	if(my_record->current_iteration_coeff == 0)
		return 0;

	// calculate tomography correction
	//cout << " ============== calculate tomography correction" << endl;
	my_record->calculate_tomo_correction(this);
	//cout << "tomo corr is done" << endl;
	

	// calculate travel time residual after tomography correction
	//cout << " ============== calculate dt residual" << endl;
	my_record->dt_residual_for_current_iteration = my_record->dt_obs_prem - my_record->dt_tomo_correction;



	// distribute dt residual to the whole depending on weight
	// and convert dt into dvs
	this->distribute_dt_residual_and_convert_to_dvs(my_record);
	

	// free space for new record CP array
	my_record->free_CP();


	return 1;
}



/******************************************************************
 * This is a c script to distribute the dt residual 
 * dt_residual = dt(obs - PREM) - dt(tomo - PREM)
 *
 * to each cell along its path and convert the dt residual 
 * in each cell into dvs value
 *
 * and then the dvs value is accumulated into my_cell.sum_dvs
 * and my_cell.sum_num ++
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


int new_tomo::distribute_dt_residual_and_convert_to_dvs(new_record* my_record)
{
	int count;

	//cout << " distribute time now" << endl;



	double dt_for_cell;
	dt_for_cell = 0;

	int idep,ilat,ilon;
	double dvs_new_tomo;
	double dl;
	double V_PREM;
	double dvs_old_tomo;
	//double code_PREM_time;
	//code_PREM_time = 0;


	// Here we introduce the core algorithm of the code
	// How to do the layer stirpping part
	// # 4 we divide the model into two layer,
	// first use anything that has bottoming depth in the first layer
	// to correct for first and then use the deep phase to correct
	// for deep layer
	
	// when it comes to deep layer, we assume the top layer is already correct
	// which would involve recalculate the weight for each cell in bottom layer
	// we use a temporary array to store this value 
	double weight_tmp[my_record->CP_num];
	// normalize dl and RMS
	double weight_RMS[my_record->CP_num];
	double weight_dl[my_record->CP_num];
	double weight_cell[my_record->CP_num];
	double SUM_weight;
	SUM_weight = 0;
	int dep_index_tmp1;
	double dep_tmp1;
	double SUM_RMS;
	double SUM_dl;
	double RMS_tmp;
	SUM_dl = 0;
	SUM_RMS = 0;
	// calculate SUM_RMS
	for(count = 0; count < my_record->CP_num  ; count ++)
	{
		dep_index_tmp1 = my_record->CP_idep[count];
		dep_tmp1 = my_record->CP_dep[count];
		//cout << "count "<< count << "current_CP_weight " << my_record->current_CP_weight[dep_index_tmp1]  << endl;
		SUM_RMS += my_record->current_CP_weight[dep_index_tmp1];
		SUM_dl  += my_record->CP_dl[count];
	}

	if(SUM_RMS == 0 ) 
		return 0;
	//cout << "SUM RMS dl " << SUM_RMS <<  "  " << SUM_dl << endl;

	for(count = 0; count < my_record->CP_num  ; count ++)
	{
		dep_index_tmp1 = my_record->CP_idep[count];
		dep_tmp1 = my_record->CP_dep[count];
		RMS_tmp = my_record->current_CP_weight[dep_index_tmp1];
		weight_RMS[count] = RMS_tmp / SUM_RMS * this->weight_ratio_path_length_RMS_tomo;
		weight_dl[count] = my_record->CP_dl[count] / SUM_dl;

		weight_cell[count] = weight_RMS[count] * weight_dl[count];
		SUM_weight += weight_cell[count];

		//cout << "count is "<< count << "all weight is "<< weight_cell[count] << "current RMS weight" << RMS_tmp << endl;

	}
	if(SUM_weight == 0)
		return 0;

	// define weight for each cell
	for(count = 0; count < my_record->CP_num  ; count++)
	{
		dep_tmp1 = my_record->CP_dep[count];
			weight_tmp[count] = weight_cell[count] / SUM_weight;
			//cout << "dep " << dep_tmp1 << "weight is "<< weight_tmp[count]<< endl;
	}




	double new_dvs_for_line_value[my_record->CP_num];
	int    new_dvs_for_line_num[my_record->CP_num];
	int    path_num_flag;
	int idep_tmp, ilat_tmp, ilon_tmp;


	path_num_flag = 0;
	//cout << "my record CP_ num is "<< my_record->CP_num << endl;
	idep_tmp = 0;
	ilat_tmp = 0;
	ilon_tmp = 0;
	for(count = 0; count < my_record->CP_num; count++)
	{
		//cout << "==================" << endl;
		//cout << "count is " << count << endl;
		//cout << "path num flag is " << path_num_flag << endl;


		dep_tmp1 = my_record->CP_dep[count];


		idep = my_record->CP_idep[count];
		ilat = my_record->CP_ilat[count];
		ilon = my_record->CP_ilon[count];

		if(idep_tmp == 0)
		{
			idep_tmp = idep;
			ilat_tmp = ilat;
			ilon_tmp = ilon;
		}
		//cout << "idep lat lon" << idep << "  " << ilat << " "  << ilon << endl;

		//dt_for_cell = my_record->dt_residual_for_current_iteration * my_record->CP_weight[count];
		dt_for_cell = my_record->dt_residual_for_current_iteration * weight_tmp[count];
		if(weight_tmp[count] == 0 ) 
		{
			//cout <<"weight is 0 continue" << endl;
			continue;
		}

		dl = my_record->CP_dl[count];
		V_PREM = my_record->CP_v_PREM[count];
		dvs_old_tomo = this->my_cell[idep][ilat][ilon].dvs;


		//code_PREM_time += dl / ( V_PREM * (1+dvs_old_tomo / 100) );
		//code_PREM_time += dl / ( V_PREM   );


		// the dvs is calculated using the following equation
		//dt_for_cell = 0;
		dvs_new_tomo = dl / ( dl / (1+dvs_old_tomo /100 ) + V_PREM * dt_for_cell ) -1 ;
		dvs_new_tomo = dvs_new_tomo * 100;
//cout << "we find a dvs new here"<< dvs_new_tomo << endl;

		if(dvs_new_tomo != dvs_new_tomo)
		{
			cout << dl << "  " << dvs_old_tomo <<"  "<< V_PREM <<"  " << dt_for_cell << " " << weight_tmp[count] << endl;
			cout << "dl problem count is " << count<<endl;
		}
		//if( count == 0)
		//{
			//idep_tmp = idep;
			//ilat_tmp = ilat;
			//ilon_tmp = ilon;
			//new_dvs_for_line_value[count] = dvs_new_tomo;
			//new_dvs_for_line_num[count] = path_num_flag;
			//continue;
		//}
		//else
		//{
			// only increase path_num_flag when cell index changes
			if(idep_tmp == idep && ilat_tmp == ilat && ilon_tmp == ilon)
			{
				double stay_in_same_cell_dont_change;
				stay_in_same_cell_dont_change = 0;
			}
			else
			{
		//cout << "count is " << count << endl;
		//cout << "path num flag is " << path_num_flag << endl;
				path_num_flag =  path_num_flag +1;
				idep_tmp = idep;
				ilat_tmp = ilat;
				ilon_tmp = ilon;
			}
			//cout << "idep lat lon" << idep_tmp << "  " << ilat_tmp << " "  << ilon_tmp << endl;

			//cout << "count is "<< count << endl;
			//cout << "path num flag is "<< path_num_flag << endl;
			new_dvs_for_line_value[count] = dvs_new_tomo;
			new_dvs_for_line_num[count] = path_num_flag;
		//}
		//this->my_cell[idep][ilat][ilon].sum_dvs_in_cell += dvs_new_tomo;
		//this->my_cell[idep][ilat][ilon].sum_num_in_cell += 1;

		//cout << "dt for cell is " << dt_for_cell 
			 ////<< "dl is "<< dl 
			////<< "V prem is " << V_PREM 
			////<< "dvs old tomo is " << dvs_old_tomo 
			//<< " sum num is " << this->my_cell[idep][ilat][ilon].sum_num_in_cell
			//<< "sum dvs is " << this->my_cell[idep][ilat][ilon].sum_dvs_in_cell
			//<< " old tomo is " << dvs_old_tomo 
			//<< "new tomo is  " << dvs_new_tomo << endl;
	}


	// for each line segment that path_num_flag defined, we go through them and calculate the 
	// average for path segment in current cell and update dvs and num_dvs
	int i_path_num_flag;
	double sum_dvs_current_cell;
	int sum_num_current_cell;
	double average_dvs_current_cell;
	int current_cell_count;
	for(i_path_num_flag = 0; i_path_num_flag < path_num_flag ; i_path_num_flag ++)
	{

		//cout << "working on i_path_num_flag " << i_path_num_flag << endl;
		sum_dvs_current_cell = 0;
		sum_num_current_cell = 0;
		for(count = 0; count < my_record->CP_num; count++)
		{
			dep_tmp1 = my_record->CP_dep[count];

			if(weight_tmp[count] == 0 ) 
				continue;
			//cout << "dep is "<< dep_tmp1 << "current layer minmax "<< this->current_layer_dep_min << "  " << this->current_layer_dep_max <<endl;
			idep = my_record->CP_idep[count];
			ilat = my_record->CP_ilat[count];
			ilon = my_record->CP_ilon[count];
			//cout << "             new count is "<< count << endl;
			if( new_dvs_for_line_num[count] == i_path_num_flag )
			{
			//cout << "new_dvs_for_line_num " << new_dvs_for_line_num[count] <<
				//" i_path_num_flag " <<  i_path_num_flag << endl;
				if (new_dvs_for_line_value[count]!=new_dvs_for_line_value[count])
					cout << "ERROR new_dvs_for_line_value is NAN"<<endl;
				sum_dvs_current_cell += new_dvs_for_line_value[count];
				sum_num_current_cell ++;
				current_cell_count = count;
			}
		}
		//cout << "ipath num flag "<< i_path_num_flag << endl;
		

		if(sum_num_current_cell == 0)
		{
			average_dvs_current_cell = 0;
			//cout << "ERROR sum_num_current_cell is 0 !!! " << endl;
			continue;
		}
		else
			average_dvs_current_cell = sum_dvs_current_cell / sum_num_current_cell;

		double STD;
		STD = 0;
		for(count = 0; count < my_record->CP_num; count++)
		{
			dep_tmp1 = my_record->CP_dep[count];
			if(weight_tmp[count] == 0 ) 
				continue;
			//cout << "dep is "<< dep_tmp1 << "current layer minmax "<< this->current_layer_dep_min << "  " << this->current_layer_dep_max <<endl;
			idep = my_record->CP_idep[count];
			ilat = my_record->CP_ilat[count];
			ilon = my_record->CP_ilon[count];
			//cout << "             new count is "<< count << endl;
			if( new_dvs_for_line_num[count] == i_path_num_flag )
			{
				STD += ( new_dvs_for_line_value[count] - average_dvs_current_cell ) * (new_dvs_for_line_value[count] - average_dvs_current_cell);
			}
		}

		STD = sqrt( STD / sum_num_current_cell);



		idep = my_record->CP_idep[current_cell_count];
		ilat = my_record->CP_ilat[current_cell_count];
		ilon = my_record->CP_ilon[current_cell_count];
		this->my_cell[idep][ilat][ilon].sum_dvs_in_cell += average_dvs_current_cell;
		this->my_cell[idep][ilat][ilon].sum_num_in_cell += 1;
		this->my_cell[idep][ilat][ilon].STD = STD;

	}
	

	//my_record->code_PREM = code_PREM_time;
	//cout << "code prem time is " << code_PREM_time << endl;
//

	return 0;

}


void new_tomo::output_vtk_format_model()
{




	cout <<  "Assign Number to each cell point" << endl;
	// assign number to each cell point and calculate the sum of all cells
	int index;
	index = 0;
	int index_tmp = 0;
	int idep,ilat,ilon;
	int idep_min, idep_max, ilat_min, ilat_max, ilon_min,ilon_max;
	for(idep = 0; idep< this->num_dep; idep++)
		for(ilat = 0; ilat < this->num_lat ; ilat++)
			for(ilon = 0; ilon < this->num_lon2[ilat]; ilon++)
			{
				this->my_cell[idep][ilat][ilon].vtk_index = index;
				index++;
			}
	
	int total_vtk_cell;
	total_vtk_cell = 0;
	for(idep = 0; idep< this->num_dep -1; idep++)
		for(ilat = 0; ilat < this->num_lat -1 ; ilat++)
			for(ilon = 0; ilon < this->num_lon2[ilat] -1; ilon++)
			{
				total_vtk_cell ++;
			}


	cout <<" loop through each cell and find the unstructed grid cell" << endl;
	// loop through each cell and find the unstructured grid cell array
	for(idep = 0; idep< this->num_dep; idep++)
		for(ilat = 0; ilat < this->num_lat ; ilat++)
			for(ilon = 0; ilon < this->num_lon2[ilat]; ilon++)
			{
				idep_min = idep;
				idep_max = idep +1;
				if(idep_max == this->num_dep )
					idep_max = idep;
				ilat_min = ilat;
				ilat_max = ilat +1;
				if(ilat_max == this->num_lat )
					ilat_max = 0;
				ilon_min = ilon;
				ilon_max = ilon+1;
				if(ilon_max == this->num_lon2[ilat] )
					ilon_max = 0;

				//cout << " "
					//<< "dep minmax " << idep_min << "  " << idep_max
					//<< "lat_minmax " << ilat_min << "  " << ilat_max
					//<< "lon minmax " << ilon_min << "  " << ilon_max
					//<< endl;


				index_tmp = 0;
				// point 0 
				this->my_cell[idep][ilat][ilon].vtk_index_array[index_tmp] =
					this->my_cell[idep_min][ilat_min][ilon_min].vtk_index;
				index_tmp++;
				// point 1
				this->my_cell[idep][ilat][ilon].vtk_index_array[index_tmp] =
					this->my_cell[idep_min][ilat_min][ilon_max].vtk_index;
				index_tmp++;
				// point 2
				this->my_cell[idep][ilat][ilon].vtk_index_array[index_tmp] =
					this->my_cell[idep_min][ilat_max][ilon_min].vtk_index;
				index_tmp++;
				// point 3
				this->my_cell[idep][ilat][ilon].vtk_index_array[index_tmp] =
					this->my_cell[idep_min][ilat_max][ilon_max].vtk_index;
				index_tmp++;
				// point 4
				this->my_cell[idep][ilat][ilon].vtk_index_array[index_tmp] =
					this->my_cell[idep_max][ilat_min][ilon_min].vtk_index;
				index_tmp++;
				// point 5
				this->my_cell[idep][ilat][ilon].vtk_index_array[index_tmp] =
					this->my_cell[idep_max][ilat_min][ilon_max].vtk_index;
				index_tmp++;
				// point 6
				this->my_cell[idep][ilat][ilon].vtk_index_array[index_tmp] =
					this->my_cell[idep_max][ilat_max][ilon_min].vtk_index;
				index_tmp++;
				// point 7
				this->my_cell[idep][ilat][ilon].vtk_index_array[index_tmp] =
					this->my_cell[idep_max][ilat_max][ilon_max].vtk_index;

			}







	string tomo_vtk;
	tomo_vtk = "tomo_vtk."+this->MODEL + ".vtk";

	ofstream myfile;
	myfile.open(tomo_vtk.c_str());

	string tmp;
	tmp = "# vtk DataFile Version 2.0";
	myfile << tmp<<endl;
	tmp = "unstructured tomo file";
	myfile << tmp << endl;

	tmp = "ASCII" ;
	myfile << tmp << endl;
	tmp = "DATASET UNSTRUCTURED_GRID";
	myfile << tmp << endl;
	tmp = "POINTS " +std::to_string(index) + "  float";
	myfile << tmp << endl;

	cout <<" output points coordinate" << endl;
	// output the points coordinate
	double index_array[10];
	int count;
	double lat_tmp, lon_tmp, dep_tmp;
	double xx,yy,zz,RADIUS;
	for(idep = 0; idep< this->num_dep; idep++)
		for(ilat = 0; ilat < this->num_lat ; ilat++)
			for(ilon = 0; ilon < this->num_lon2[ilat]; ilon++)
			{
				dep_tmp = this->dep[idep];
				lat_tmp = this->lat[ilat];
				lon_tmp = this->lon2[ilat][ilon];
				sph2cart(lat_tmp, lon_tmp, dep_tmp, &xx, &yy, &zz);
				RADIUS = 6371;
				xx = xx/RADIUS;
				yy = yy/RADIUS;
				zz = zz/RADIUS;
				int prec = 10;
				myfile << fixed << "  "
					<< setw(15) << setprecision(prec) << xx
					<< setw(15) << setprecision(prec) <<  yy
					<< setw(15) << setprecision(prec) <<  zz 
					<< setw(15) << setprecision(4) << dep_tmp
					<< setw(15) << setprecision(4) <<  lat_tmp
					<< setw(15) << setprecision(4) <<  lon_tmp
					<< endl;
				//this->my_cell[idep][ilat][ilon].vtk_index = index;
				//index++;
			}

	//int sum_list;
	//sum_list = 9*index;
	tmp = "CELLS " + std::to_string(total_vtk_cell) + " " + std::to_string(9* total_vtk_cell);
	myfile << tmp << endl;
	for(idep = 0; idep< this->num_dep -1; idep++)
		for(ilat = 0; ilat < this->num_lat -1 ; ilat++)
			for(ilon = 0; ilon < this->num_lon2[ilat] -1; ilon++)
			{
				myfile << 8  << "   " ;
				for(count = 0 ; count < 8; count ++)
				{
					index_tmp = this->my_cell[idep][ilat][ilon].vtk_index_array[count];
					myfile << index_tmp << "  ";
				}
					myfile << endl;
				//this->my_cell[idep][ilat][ilon].vtk_index = index;
				//index++;
			}



	tmp = "CELL_TYPES  " + std::to_string(total_vtk_cell) ;
	myfile << tmp << endl;
	for(idep = 0; idep< this->num_dep -1; idep++)
		for(ilat = 0; ilat < this->num_lat -1 ; ilat++)
			for(ilon = 0; ilon < this->num_lon2[ilat] -1; ilon++)
			{
				int ttt = 12;
				myfile << ttt  <<endl;
				//this->my_cell[idep][ilat][ilon].vtk_index = index;
				//index++;
			}



	tmp = "POINT_DATA    " + std::to_string(index);
	myfile << tmp << endl;
	tmp = "SCALARS scalars float 1"; 
	myfile << tmp << endl;
	tmp = "LOOKUP_TABLE default";
	myfile << tmp << endl;
	for(idep = 0; idep< this->num_dep; idep++)
		for(ilat = 0; ilat < this->num_lat ; ilat++)
			for(ilon = 0; ilon < this->num_lon2[ilat]; ilon++)
			{
				double dvs;
				dvs = this->my_cell[idep][ilat][ilon].dvs;
				//cout << "dvs is "<< dvs << endl;
				myfile << dvs <<endl;
				//this->my_cell[idep][ilat][ilon].vtk_index = index;
				//index++;
			}




}




/******************************************************************
 * This is a c script to output gradient tomography
 * for each point, we search for its strongest 3D gradient value and store it as its
 * gradient value
 *
 * The algorithm is modified from Chunpeng`s code.
 * There is a brief description:
 * 1. search for each location in tomography model 
 * 2. define a cube around the grid location. the size of the cube is set by user (1 ~ 5degree)
 * 3. all the other grid points that is within the cube is our sampling points
 * 4. calculate the gradient between grid point and each sampling point
 * 5. find the maximum gradient (depends on how we define it) and set it as the maximum gradient for current grid point
 *
 * gradient of grid point = (dvs_sampling_point - dvs_grid_point) / distance 
 *
 *
 *	Input:
 *	new_tomography*						my_tomo					tomo model struct
 *	double								cube_size_number		this is the size that we search for near the grid point
 *	
 *
 *
 *	Output:
 *	3D model with x y z and gradient dep lat lon
 *
 *	Input:
 *
 *
 *
 *	Output:
 *
 *
 *	DATE:	Aug 2016			Keywords:
 *	Reference:
******************************************************************/

#define GRADIENT_DEP_LAYER_DELTA 2
#define GRADIENT_LAT_LON_DELTA 0

void new_tomo::output_gradient_tomography()
{

	cout << "======================== Output gradient Tomography info =====================" << endl;
	this->output_tomo_file = "output_gradient." + this->MODEL;

	// define output file parameter
	ofstream myfile;
	myfile.open(this->output_tomo_file.c_str());

	// first output dimensions
	myfile << this->num_dep << endl;
	myfile << 179 << endl;
	myfile << 361 << endl;
	
	double dep;
	double lat;
	double dvs;
	double lon;

	// output model file
	int idep,ilat,ilon;
	int iidep,iilat,iilon;
	int iidep_min, iidep_max;
	double dep2, lat2, lon2;
	double dep2_min, dep2_max;
	double lat2_min, lat2_max;
	double lon2_min, lon2_max;
	double max_gradient;
	double distance;
	max_gradient = 0;
	double gradient_tmp;
	for(idep = 0; idep < this->num_dep;idep++)
	{	
	for(lat = -89; lat <= 89 ; lat++ )
	for(lon = -180 ; lon <=180 ; lon++)
	{
		//normalize
		double xx,yy,zz;

		dep = this->dep[idep];
		dvs = this->find_dvs_with_dep_lat_lon(dep,lat,lon);

		//double dvs = this->dvs[idep][ilat][ilon];
		sph2cart(lat, lon, dep, &xx, &yy, &zz);
		double RADIUS = 6371;
		xx = xx/RADIUS;
		yy = yy/RADIUS;
		zz = zz/RADIUS;



		// ===========================================
		// working on finding the gradient value of current point
		iidep_min = idep - GRADIENT_DEP_LAYER_DELTA;
		iidep_max = idep + GRADIENT_DEP_LAYER_DELTA;
		//iidep_max = idep;
		if(iidep_min < 0)
			iidep_min = 0;
		if(iidep_max > this->num_dep -1)
			iidep_max = this->num_dep -1;

		double line_fitting_data[20];
		int line_fitting_num;
		double depth_fitting_data[20];
		line_fitting_num = 2*GRADIENT_DEP_LAYER_DELTA +1;
		int ttt;

		max_gradient = 0;
		ttt = 0;
		for(iidep = iidep_min; iidep <= iidep_max ; iidep ++)
			for(lat2 = lat - GRADIENT_LAT_LON_DELTA; lat2 <= lat + GRADIENT_LAT_LON_DELTA ; lat2 ++)
				for(lon2 = lon - GRADIENT_LAT_LON_DELTA; lon2 <= lon + GRADIENT_LAT_LON_DELTA; lon2 ++)
				{
					//if(lon2 == lon && lat2 == lat)
						//continue;
					dep2 = this->dep[iidep];
					double dvs_tmp;
					dvs_tmp = find_dvs_with_dep_lat_lon(dep2, lat2, lon2);

					// simplify the gradient as dvs difference
					gradient_tmp =  dvs - dvs_tmp;
					if(gradient_tmp > max_gradient)
						max_gradient = gradient_tmp;
					line_fitting_data[ttt] = dvs_tmp;
					depth_fitting_data[ttt] = dep2;
					ttt++;
				}


		// calculate the slope of all data within range
		double dvs_ave;
		double dep_ave;
		double sum_SS_xx,sum_SS_yy,sum_SS_xy;
		double sum_dep_2, sum_dvs_2, sum_dep_dvs;;
		double sum_S_dep_dep, sum_S_dep_dvs, sum_S_dvs_dvs;
		dvs_ave = 0;
		dep_ave = 0;
		sum_dep_2 = 0;
		sum_dvs_2 = 0;
		sum_dep_dvs = 0;
		int count;
		for(count = 0; count < ttt ; count++)
		{
			if( lat == -12 && lon == -150 && dep == 2600)
				cout << "count "<< count << "depth" << depth_fitting_data[count] << "dvs "<< line_fitting_data[count] << endl;
			dep_ave += depth_fitting_data[count];
			dvs_ave += line_fitting_data[count];
			sum_dep_2 += depth_fitting_data[count] * depth_fitting_data[count];
			sum_dvs_2 += line_fitting_data[count] * line_fitting_data[count];
			sum_dep_dvs += depth_fitting_data[count]  * line_fitting_data[count];
		}
		dep_ave = dep_ave / line_fitting_num;
		dvs_ave = dvs_ave / line_fitting_num;

		sum_S_dep_dep = sum_dep_2 - line_fitting_num * dep_ave * dep_ave;
		sum_S_dep_dvs = sum_dep_dvs - line_fitting_num * dep_ave * dvs_ave;
		sum_S_dvs_dvs = sum_dvs_2 - line_fitting_num * dvs_ave * dvs_ave;

		double slope;
		slope = sum_S_dep_dvs / sum_S_dep_dep;
		slope = 1000 * slope;

			if( lat == -12 && lon == -150 && dep == 2600)
				cout << "slope is "<< slope << endl;



	//cout  << xx << yy << zz << dvs << dep << lat << lon << endl;
	int size = 15;
	int prec = 6;
	myfile <<fixed
		<<setw(size)<<setprecision(prec) << xx 
		<<setw(size)<<setprecision(prec) << yy
		<<setw(size)<<setprecision(prec) << zz
		<<setw(size)<<setprecision(prec) << slope
		<<setw(size)<<setprecision(prec) << dep
		<<setw(size)<<setprecision(prec) << lat
		<<setw(size)<<setprecision(prec) << lon << endl;
		
		//<< "   " << yy << "   " << zz << "   " << dvs <<"   " <<dep <<"   "<< lat <<"   " << lon << endl;
	//cout <<setw(10)<<setprecision(5) << xx << "   " << yy << "   " << zz << endl;

	}
	}

	myfile.close();

}



/******************************************************************
 * This is a c script to find the cross point
 * between two point A and B
 *
 * the corss point can be either dep/lat/lon cross point 
 * we search for all cross point and return the cross poin location (dep lat lon)
 * we assume there is only one cross point for each catagory, dep/lat/lon
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
void new_tomo::pointA_pointB_find_cross_pointC(double dep1, double lat1, double lon1,
		double dep2, double lat2, double lon2,
		int index_dep1,int index_lat1, int index_lon1,
		int index_dep2,int index_lat2, int index_lon2,
		double* mid_dep, double* mid_lat, double* mid_lon,
		double* delta_angle_d1_d2, double* radius)
{
	//double dep1, dep2, dep3;
	//double lat1, lat2, lat3;
	//double lon1, lon2, lon3;
	//int index_dep1,index_dep2;
	//int index_lat1, index_lat2;
	//int index_lon1, index_lon2;

	//cout << " "
		//<< " dep lat lon "<< dep1 << "  " << lat1 << "  " << lon1 
		//<< " dep lat lon "<< dep2 << "  " << lat2 << "  " << lon2
		//<< "index dep lat lon " << index_dep1 << " " << index_lat1 << "  " << index_lon1 
		//<< "index dep lat lon " << index_dep2 << " " << index_lat2 << "  " << index_lon2
		//<< endl;

	//string flag;
	// flag can be dep, lat, lon

	double value1, value2;
	double delta1, delta2;

	int mid_value;	// cross point boundary value for dep or lat or lon
	if(index_dep1 != index_dep2)
	{
		value1 = dep1;
		value2 = dep2;
		if(index_dep1 < index_dep2 )
			mid_value = this->dep[index_dep2];
		else
			mid_value = this->dep[index_dep1];
	}
	else if(index_lat1 != index_lat2 )
	{
		value1 = lat1;
		value2 = lat2;
		if(index_lat1 < index_lat2 )
		{
			mid_value = this->lat[index_lat2];
		}
		else
			mid_value = this->lat[index_lat1];
	}
	else if( index_lon1 != index_lon2 )
	{
		value1 = lon1;
		value2 = lon2;
		if(index_lon1 < index_lon2 )
			mid_value = this->lon2[index_lat1][index_lon2];
		else
			mid_value = this->lon2[index_lat1][index_lon1];
	}

	delta1 = abs(value1 - mid_value);
	delta2 = abs(value2 - mid_value);

	//cout << "middle value is "<< mid_value << endl;

	double dl;
	double delta_angle;
	double R1,R2;
	// calculate the dl between two points
	double delta_lat, delta_lon;
	double delta_lat_dist, delta_lon_dist;
	delta_lat = abs(lat1 - lat2);
	delta_lon = abs(lon1 - lon2);
	delta_lat_dist = sin(delta_lat /180*3.1415926)*(6371 - dep1);
	delta_lon_dist = sin(delta_lon /180*3.1415926)*(6371 - dep2);
	double delta_dist;
	//delta_dist = sqrt( delta_lat_dist * delta_lat_dist + delta_lon_dist * delta_lon_dist);
		//double dist_A_B(double A_lat, double A_lon, double B_lat, double B_lon) 
	delta_dist = dist_A_B(lat1, lon1, lat2, lon2);
	double mid_point_dist;
	if(delta1 + delta2 == 0)
		cout<< "ERROR delta1 + delta2 == 0!!" << endl;
	mid_point_dist = delta_dist * ( delta2 / (delta1 + delta2) );

	//cout << " delta dist is "<< delta_dist << endl;
	//cout << " delta1 delta2 " << delta1 << "  " << delta2 << endl;
	//cout << "  mid_point_dist " << mid_point_dist << endl;

	double AZ;
	AZ = az_A_B(lat1, lon1, lat2, lon2);
	//cout << " AZ " << AZ << endl;
	point_AZ_dist_point(lat1, lon1, AZ, mid_point_dist, mid_lat, mid_lon);
	*mid_dep = dep1 + (dep2-dep1) * delta1/(delta1+delta2);
	//double point_AZ_dist_point(double A_lat, double A_lon, double AZ, double dist, double* B_lat, double* B_lon)
	//double az_A_B(double A_lat, double A_lon, double B_lat, double B_lon)
//cout << "mid dep lat lon is "<< *mid_dep << " " << *mid_lat << " " << *mid_lon << endl;

	
	*delta_angle_d1_d2 = delta1 / (delta1 +delta2);
	*radius = 6371 - *mid_dep;

}




void new_tomo::output_cross_section(big_new_record* my_big_record )
{
	int ista;

	for(ista = 0; ista < my_big_record->sta_num;ista++)
	{
		cout << "---> Output cross section for record num " << ista << " /"<< my_big_record->sta_num << endl;

		this->output_cross_section_for_one_record( &my_big_record->my_record[ista]);
	}
}


void new_tomo::output_cross_section_for_one_record(new_record* my_sta)
{
	double A_lat = my_sta->eq_lat;
	double A_lon = my_sta->eq_lon;
	double B_lat = my_sta->sta_lat;
	double B_lon = my_sta->sta_lon;
	double x,y,z;
	int depth_num = this->num_dep;

	int wid = 15;
	int pre = 3;


	//get the mid point lat lon between two points
	double mid_lat, mid_lon;
	double dist_tmp = dist_A_B(A_lat, A_lon, B_lat, B_lon);
	double AZ;
	AZ = az_A_B(A_lat,  A_lon, B_lat, B_lon);
	point_AZ_dist_point(A_lat, A_lon, AZ, dist_tmp/2 , &mid_lat, &mid_lon);

	double grid_space = 1; 		//degree
	
	//dist is in km
	double dist_in_degree = dist_tmp/111;

	int npts_points = (int)(dist_in_degree/grid_space)+3;		// number of points between EQ and STA
	// we use cross-points to represent vertical slice
	//int npts_points = (int)(dist_in_degree/grid_space)+3;		// number of points between EQ and STA
	int MAX_NUM = 10000;
	double lat[MAX_NUM];
	double lon[MAX_NUM];
	double ang_dist[MAX_NUM];
	double dep[500];								// give a point every 30km

// total 96 points first is 30km last is 2880km
//printf("vertical slice between EQ STA distance %lf \n", dist_tmp);

	int i,j;
	ofstream myfile_gcp;
	string gcp_file;
	gcp_file = "GCP."+my_sta->EQ + "."+ my_sta->STA + "."+my_sta->PHASE + ".gcp";
	myfile_gcp.open(gcp_file.c_str());
	for(i = 0; i<npts_points; i++)
	{
		point_AZ_dist_point(A_lat, A_lon, AZ, i*grid_space*111,&lat[i], &lon[i]);
		ang_dist[i] = i*grid_space;
		myfile_gcp << setw(15) << setprecision(4) << fixed
			<< setw(wid) << setprecision(pre) << lat[i]
			<< setw(wid) << setprecision(pre) << lon[i]
			<< endl;

	}

	myfile_gcp.close();



	ofstream myfile_cross_section;
	string cross_file = "cross_section."+ my_sta->EQ + "."+ my_sta->STA + "." +my_sta->PHASE; 
	myfile_cross_section.open(cross_file.c_str());

	// output meta info for cross section file
	double theta_min, theta_max, radius_min, radius_max;
	theta_min = 0;
	theta_max = dist_in_degree;
	radius_min = 6371 - 2891;
	radius_max = 6371;
	myfile_cross_section << setw(20) << setprecision(3) ;
	myfile_cross_section << fixed
		<< theta_min  << "  "
		<< theta_max  <<  "  "
		<< radius_min  << "  "
		<< radius_max << "  "
		<< mid_lat << "  "
		<< mid_lon << "  "
		<< endl;

	// how far away in depth to print out a point
	double delta_depth = 50;

	for( i=0; i< npts_points; i++)
		for(j =0; j<depth_num; j++)
		{
			double lat_tmp = lat[i];
			double lon_tmp = lon[i];
			double dep_tmp = this->dep[j];
			double theta = ang_dist[i];
			double r = 6371 - dep_tmp;

			//find the dvs
			double dvs_tmp;
//printf("lat lon dep is %lf %lf %lf \n",lat_tmp, lon_tmp, dep_tmp);
			
			// hongyu 
			// for cross-section, we can either output dvs or hit count
			
			// 

			//dvs_tmp =  this->find_dvs_with_dep_lat_lon(dep_tmp, lat_tmp,lon_tmp);
			dvs_tmp =  this->find_hit_with_dep_lat_lon(dep_tmp, lat_tmp,lon_tmp);

			myfile_cross_section << fixed;
			myfile_cross_section 
				<< setw(wid) << setprecision(pre) << theta
				<< setw(wid) << setprecision(pre) << r
				<< setw(wid) << setprecision(pre) << dvs_tmp
				<< endl;
				
		}

	myfile_cross_section.close();


	// output taup_path here
	ofstream myfile_cross_path;
	string cross_path_file;
	cross_path_file = "cross_path."+my_sta->EQ + "."+ my_sta->STA + "."+my_sta->PHASE ;
	myfile_cross_path.open(cross_path_file.c_str());

	cout << "output cross path file for "<< cross_path_file << endl;
	my_sta->read_taup_path_info(this->taup_path_dir);
	for(i = 0 ; i< my_sta->taup_path_max_num ; i++)
	{
		myfile_cross_path << fixed
			<< setw(wid) << setprecision(pre) << my_sta->angle[i]
			<< setw(wid) << setprecision(pre) << my_sta->radius[i]
			<< endl;

	}


}


//// 
////
////
//// output cross section of hit count 
////
//void new_tomo::output_cross_section_hit_count(big_new_record* my_big_record )
//{
	//int ista;

	//for(ista = 0; ista < my_big_record->sta_num;ista++)
	//{
		//cout << "---> Output cross section for hit count  record num " << ista << " /"<< my_big_record->sta_num << endl;

		//this->output_cross_section_for_one_record_hit_count( &my_big_record->my_record[ista]);
	//}
//}


//void new_tomo::output_cross_section_for_one_record_hit_count(new_record* my_sta)
//{
	//double A_lat = my_sta->eq_lat;
	//double A_lon = my_sta->eq_lon;
	//double B_lat = my_sta->sta_lat;
	//double B_lon = my_sta->sta_lon;
	//double x,y,z;
	//int depth_num = this->num_dep;

	//int wid = 15;
	//int pre = 3;


	////get the mid point lat lon between two points
	//double mid_lat, mid_lon;
	//double dist_tmp = dist_A_B(A_lat, A_lon, B_lat, B_lon);
	//double AZ;
	//AZ = az_A_B(A_lat,  A_lon, B_lat, B_lon);
	//point_AZ_dist_point(A_lat, A_lon, AZ, dist_tmp/2 , &mid_lat, &mid_lon);

	//double grid_space = 1; 		//degree
	
	////dist is in km
	//double dist_in_degree = dist_tmp/111;

	//int npts_points = (int)(dist_in_degree/grid_space)+3;		// number of points between EQ and STA
	//// we use cross-points to represent vertical slice
	////int npts_points = (int)(dist_in_degree/grid_space)+3;		// number of points between EQ and STA
	//int MAX_NUM = 10000;
	//double lat[MAX_NUM];
	//double lon[MAX_NUM];
	//double ang_dist[MAX_NUM];
	//double dep[500];								// give a point every 30km

//// total 96 points first is 30km last is 2880km
////printf("vertical slice between EQ STA distance %lf \n", dist_tmp);



	//ofstream myfile_cross_section;
	//string cross_file = "cross_section."+ my_sta->EQ + "."+ my_sta->STA + "." +my_sta->PHASE; 
	//myfile_cross_section.open(cross_file.c_str());

	//// output meta info for cross section file
	//double theta_min, theta_max, radius_min, radius_max;
	//theta_min = 0;
	//theta_max = dist_in_degree;
	//radius_min = 6371 - 2891;
	//radius_max = 6371;
	//myfile_cross_section << setw(20) << setprecision(3) ;
	//myfile_cross_section << fixed
		//<< theta_min  << "  "
		//<< theta_max  <<  "  "
		//<< radius_min  << "  "
		//<< radius_max << "  "
		//<< mid_lat << "  "
		//<< mid_lon << "  "
		//<< endl;

	//// how far away in depth to print out a point
	//double delta_depth = 50;

	//for(count = 0; count < my_sta->CP_num; count++)
	//{
		//double dep_tmp1 = my_record->CP_dep[count];
		//double r = 6371 - dep_tmp1;
		//double theta = this-

		//int idep = my_record->CP_idep[count];
		//int ilat = my_record->CP_ilat[count];
		//int ilon = my_record->CP_ilon[count];
		
		//hit_count = this->my_cell[idep][ilat][ilon].sum_num_in_cell;
	//}

	//for( i=0; i< npts_points; i++)
		//for(j =0; j<depth_num; j++)
		//{
			//double lat_tmp = lat[i];
			//double lon_tmp = lon[i];
			//double dep_tmp = this->dep[j];
			//double theta = ang_dist[i];
			//double r = 6371 - dep_tmp;

			////find the dvs
			//double dvs_tmp;
			//dvs_tmp =  this->find_dvs_with_dep_lat_lon(dep_tmp, lat_tmp,lon_tmp);
			////dvs_tmp =  this->find_hit_with_dep_lat_lon(dep_tmp, lat_tmp,lon_tmp);

			//myfile_cross_section << fixed;
			//myfile_cross_section 
				//<< setw(wid) << setprecision(pre) << theta
				//<< setw(wid) << setprecision(pre) << r
				//<< setw(wid) << setprecision(pre) << dvs_tmp
				//<< endl;
				
		//}

	//myfile_cross_section.close();


//}

/******************************************************************
 * This is a c script to write the tomography (my_tomo) to netcdf format
 *
 *	Input:
 *		new_tomography* 	my_tomo			
 *
 *
 *
 *	Output:
 *		char*				netcdf_file
 *
 *
 *	DATE:				Keywords:
 *	Reference:
******************************************************************/

int new_tomo::CONVERT_write_to_netcdf( char* netcdf_file)
{
	ofstream myfile;
	myfile.open(netcdf_file);

	myfile<< "netcdf Tomography"<< endl;
	myfile<<"dimensions:"<<endl;
	myfile<<"depth = "<<this->num_dep<< endl;
	myfile<<"latitude = "<<this->num_lat<< endl;
	myfile<<"longitude = "<<this->num_lon<< endl;
	myfile<<"data:"<< endl;
	myfile << endl;
	myfile<<"depth = "<< endl;


	//output depth profile
	int idep;
	int line_num = 0;
	for(idep = 0 ; idep < this->num_dep; idep++)
	{
		myfile<< this->dep[idep] << " ";
		//fprintf(in,"%.0f, ",this->dep[idep]);
		line_num++;
		if(line_num == 17)
		{
					myfile<<endl;
			//fprintf(in,"\n");
			line_num = 0;
		}
	}


	myfile<< endl;
	myfile<< endl;
	myfile<<"latitude = ";

	//output lat profile
	int ilat;
	line_num = 0;
	for(ilat = 0 ; ilat < this->num_lat; ilat++)
	{
		//fprintf(in,"%.0f, ",this->lat[ilat]);
		myfile<< this->lat[ilat]<< " ";
		line_num++;
		if(line_num == 17)
		{
					myfile<<endl;
			//fprintf(in,"\n");
			line_num = 0;
		}
	}

	myfile<< endl;
	myfile<< endl;
	myfile<<"longitude = ";

	//output lat profile
	int ilon;
	line_num = 0;
	for(ilon = 0 ; ilon < this->num_lon; ilon++)
	{
		myfile<< this->lon[ilon]<< " ";
		//fprintf(in,"%.0f, ",this->lon[ilon]);
		line_num++;
		if(line_num == 17)
		{
					myfile<<endl;
			//fprintf(in,"\n");
			line_num = 0;
		}
	}


	myfile<< endl;
	myfile<< endl;
	myfile<<"vs = ";
	line_num = 0;
	int ivs;
	for(idep = 0; idep < this->num_dep; idep++)
		for(ilat = 0; ilat < this->num_lat ; ilat ++)
			for(ilon = 0; ilon < this->num_lon; ilon++)
			{
				//fprintf(in,"%8.5f, ",this->dvs[idep][ilat][ilon]);
				myfile<< this->my_cell[idep][ilat][ilon].dvs<< " ";
				line_num++;
				if(line_num == 17)
				{
					myfile<<endl;
					//fprintf(in,"\n");
					line_num = 0;
				}
			}

	return 0;
}
