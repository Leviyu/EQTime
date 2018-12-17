#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<hongyulib.h>
#include<sstream>


double string_to_double(string mystring)
{

	double result;

	result = atof(mystring.c_str());

	return result;
}


string double_to_string(double mydouble)
{
	string result;


	ostringstream hahaha;
	hahaha<< mydouble;
	string str = hahaha.str();


	return result;
}

string int_to_string(int mydouble)
{
	string result;


	ostringstream hahaha;
	hahaha<< mydouble;
	string str = hahaha.str();


	return result;
}

char* string_to_char(string mystring)
{
	char* mychar;
	mychar = new char[mystring.size()+1];
	copy(mystring.begin(), mystring.end(), mychar);
	mychar[mystring.size()] = '\0';
	return mychar;
}
