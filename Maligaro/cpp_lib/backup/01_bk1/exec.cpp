#include<cstdio>
#include<iostream>
#include<memory>
#include<stdexcept>
#include<string>
#include "hongyulib.h"


std::string exec(string command)
{

	const char *cmd = command.c_str();

	    char buffer[128];
	    std::string result = "";
	    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
	    if (!pipe) throw std::runtime_error("popen() failed!");
	    while (!feof(pipe.get())) 
		{
	       if (fgets(buffer, 128, pipe.get()) != NULL)
	            result += buffer;
	    }
	 return result;
}
