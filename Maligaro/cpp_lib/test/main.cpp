//###include "../01_cpp_lib/hongyulib.h"


#include <iostream>     // std::cout
#include <algorithm>    // std::unique, std::distance
#include <vector>       // std::vector

bool myfunction (int i, int j) {
  return (i==j);
}

using namespace std;
int main () {
  int myints[] = {10,20,20,20,30,30,20,20,10};           // 10 20 20 20 30 30 20 20 10
  string mystrings[] = {"me", "the_me", "me", "me","me", "the_me"};
//##std::vector<int> myvector (myints,myints+9);
//##vector<string> mystringvector ( mystrings, mystrings+5);
  vector<string> mystringvector(mystrings,mystrings+6);

  // using default comparison:
//##std::vector<int>::iterator it;
  vector<string>:: iterator it;
  //it = std::unique (myvector.begin(), myvector.end());   // 10 20 30 20 10 ?  ?  ?  ?
                                                         //                ^
  //it = unique( mystringvector.begin(), mystringvector.end());
  it = unique( mystringvector.begin(), mystringvector.end());
	//yvector.resize( std::distance(myvector.begin(),it) ); // 10 20 30 20 10
  mystringvector.resize(distance(mystringvector.begin() , it));

  // using predicate comparison:
//##std::unique (myvector.begin(), myvector.end(), myfunction);   // (no changes)

  // print out content:
  std::cout << "myvector contains:";
  for (it=mystringvector.begin(); it!=mystringvector.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}
