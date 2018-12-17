#include "../02_forward_tomo/forward_tomography.h"
#include "../01_cpp_lib/hongyulibcpp.h"

using namespace std;
int main()
{
	string PHASE;
	cin >> PHASE ;
	cout << "============= BEGIN" << endl;
	string infile="INFILE";

	big_new_record my_big_record;
	my_big_record.PHASE = PHASE;
	// cout << " PHASE is "<< PHASE << endl;
	my_big_record.get_PHASE_min_max_dist();
	my_big_record.read_INFILE();
	my_big_record.initiate_big_record();
	my_big_record.virtual_station_main();

	cout << " ================ END of code " << endl;
	return 0;
}
