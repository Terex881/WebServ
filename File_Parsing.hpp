
#ifndef FILE_PARSING_HPP
#define FILE_PARSING_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include "./DynamicStruct.hpp"

using std::ifstream;
using std::stringstream;
using std::cout;
using std::endl;

class File_Parsing
{
	public :
		ifstream file;
		DynamicStruct obj;
		DynamicStruct servers;
		DynamicStruct locations[1000];

		File_Parsing(void);
		~File_Parsing(void);
		DynamicStruct	recursive_push(ifstream *file, string parent, int *open_bracket, int *close_bracket);
		void	Checking_Hierarchy(const DynamicStruct &block, DynamicStruct *server, DynamicStruct *locations, const string &name = "");

		void	Struct_Call(DynamicStruct inner);
};

#endif
