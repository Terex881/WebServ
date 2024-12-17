

#ifndef DYNAMICSTRUCT_HPP
#define DYNAMICSTRUCT_HPP

#include <map>
#include <vector>
#include <iostream>

using std::vector;
using std::map;
using std::string;

class DynamicStruct
{
	public :
		std::map<std::string, std::string> values;
		std::map<std::string, std::vector<DynamicStruct> > children;
};

#endif
