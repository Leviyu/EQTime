
#include "forward_tomography.h"


new_cell::new_cell()
{
	this->sum_num_in_cell = 0;
	this->sum_dvs_in_cell = 0;
	delta_dvs = 0;
	gradient = 0;
	BAZ = 0;
	num_BAZ = 0;
	//this->vtk_index_array = new int[10];
	this->vtk_index_array.resize(10);

}	

new_cell::~new_cell()
{


}
