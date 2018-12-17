#include "hongyulibcpp.h"
/******************************************************************
 * This is a c script to acquire the taup_time using c
 *
 *	Input: 
 *	EQ_lat
 *	EQ_lon
 *	EQ_dep
 *	STA_lat
 *	STA_lon
 *	
 *	char* phase
 *	
 *
 *	Output:
 *	PREM_time  			which is the PREM time for this phase
 *
 * NOTE: model is prem
 * 		 we only return the first travel_time
 * 		 for multi-path using taup_time in csh to acquire more info
 *
 *
 *	DATE:Oct 21 2014				Keywords: taup_time
******************************************************************/

double taup_time(double EQ_lat, double EQ_lon, double EQ_dep, double STA_lat, double STA_lon, string phase)
{

	double PREM_time;

	string command;

	command = "taup_time -mod prem -ph "+ phase + " -evt " + std::to_string(EQ_lat) + " " + std::to_string(EQ_lon) +
		" -sta " + std::to_string(STA_lat) + " " + std::to_string(STA_lon) + " -h " + std::to_string(EQ_dep) + " | awk 'NR==6 {print $4}'";


	//cout << "command is " << command << endl;
	string new_phase;
	


	size_t found;
	found = phase.find("m");
	if(found != string::npos )
	{
		new_phase =  phase.substr(0, phase.size()-1);
		command = "taup_time -mod prem -ph "+ new_phase + " -evt " + std::to_string(EQ_lat) + " " + std::to_string(EQ_lon) +
			" -sta " + std::to_string(STA_lat) + " " + std::to_string(STA_lon) + " -h " + std::to_string(EQ_dep) 
			+ " | awk 'NR>5 {print $0}' |awk '$8>180 {print $4}' |awk 'NR==1 {print $0}' ";
	}
	else
	{
		new_phase = phase;
		command = "taup_time -mod prem -ph "+ new_phase + " -evt " + std::to_string(EQ_lat) + " " + std::to_string(EQ_lon) +
			" -sta " + std::to_string(STA_lat) + " " + std::to_string(STA_lon) + " -h " + std::to_string(EQ_dep) 
			+ " | awk 'NR>5 {print $0}' |awk '$8<180 {print $4}' |awk 'NR==1 {print $0}' ";
	}



	string out;
	out = exec(command);

	//cout << "command is " << command <<endl;

	//cout << "new phase is "<< new_phase << endl;
	//cout << " out is "<< out <<endl;

	PREM_time = string_to_double(out);
	//PREM_time =  stod(out);
	
	if(PREM_time <= 0 ) 
		cout << " PREM time for current PHASE does not exist " << endl;

	return PREM_time;
}


double taup_incident(double EQ_lat, double EQ_lon, double EQ_dep, double STA_lat, double STA_lon, string phase)
{

	double PREM_time;

	string command;

	command = "taup_time -mod prem -ph "+ phase + " -evt " + std::to_string(EQ_lat) + " " + std::to_string(EQ_lon) +
		" -sta " + std::to_string(STA_lat) + " " + std::to_string(STA_lon) + " -h " + std::to_string(EQ_dep) + " | awk 'NR==6 {print $7}'";


	//cout << "command is " << command << endl;
	string new_phase;
	


	size_t found;
	found = phase.find("m");
	if(found != string::npos )
	{
		new_phase =  phase.substr(0, phase.size()-1);
		command = "taup_time -mod prem -ph "+ new_phase + " -evt " + std::to_string(EQ_lat) + " " + std::to_string(EQ_lon) +
			" -sta " + std::to_string(STA_lat) + " " + std::to_string(STA_lon) + " -h " + std::to_string(EQ_dep) 
			+ " | awk 'NR>5 {print $0}' |awk '$8>180 {print $7}' |awk 'NR==1 {print $0}' ";
	}
	else
	{
		new_phase = phase;
		command = "taup_time -mod prem -ph "+ new_phase + " -evt " + std::to_string(EQ_lat) + " " + std::to_string(EQ_lon) +
			" -sta " + std::to_string(STA_lat) + " " + std::to_string(STA_lon) + " -h " + std::to_string(EQ_dep) 
			+ " | awk 'NR>5 {print $0}' |awk '$8<180 {print $7}' |awk 'NR==1 {print $0}' ";
	}



	string out;
	out = exec(command);

	//cout << "command is " << command <<endl;

	//cout << "new phase is "<< new_phase << endl;
	//cout << " out is "<< out <<endl;

	PREM_time = string_to_double(out);
	//PREM_time =  stod(out);

	return PREM_time;
}
