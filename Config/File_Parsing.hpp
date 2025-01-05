
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

struct dt
{
	string key;
	string val;
};

struct location_data
{
	string root;
	string	path;
	string methods;
	string	directory_listing;
	string	cgi;
	string 	rturn;
};

class File_Parsing
{
	public :
		int	open_b;
		int	close_b;
		ifstream file;
		string conf_path;
		int	servers_count;
		DynamicStruct obj;
		DynamicStruct servers;
		vector<dt> host_port;

		DynamicStruct locations[1000];

		File_Parsing(void);
		File_Parsing(string conf_path);
		~File_Parsing(void);
		DynamicStruct	recursive_push(ifstream *file, string parent, int *open_bracket, int *close_bracket);
		void	Checking_Hierarchy(DynamicStruct &block, DynamicStruct *server, DynamicStruct *locations, const string &name = "");

		void	Struct_Call(DynamicStruct inner);
		void	get_host_name(void);
		location_data	get_location_val(DynamicStruct location);
};

#endif
