
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
	vector<string> methods;
	string	directory_listing;
	string 	rturn;
};

struct server_data
{
	string server_name;
	string	root;
	string client_max_body_size;
	string default_page;
};

class File_Parsing
{
	public:
		int	open_b;
		int	close_b;
		ifstream file;
		string conf_path;
		int	servers_count;
		int	port_count;
		DynamicStruct obj;
		static vector<dt> host_port;

		DynamicStruct servers[1000];
		DynamicStruct locations[1000];

		File_Parsing(void);
		File_Parsing(string conf_path);
		~File_Parsing(void);
		DynamicStruct	recursive_push(ifstream *file, string parent, int *open_bracket, int *close_bracket);
		void	Checking_Hierarchy(DynamicStruct *block, DynamicStruct *server, DynamicStruct *locations, const string &name = "");
		void	recursive_call(DynamicStruct &block, DynamicStruct *server, DynamicStruct *locations, const string &name = "");

		void	override_server(int index, DynamicStruct *server);
		void	Struct_Call(DynamicStruct inner);
		void	get_host_name(void);
		location_data	get_location_val(DynamicStruct location);
		server_data		get_server_val(DynamicStruct server);
		string			get_body_size();
		string			get_error_page(string statut_code, DynamicStruct server);

		void	getLocationByPortAndUrl(string port, string url, DynamicStruct &location,  DynamicStruct &server);

};

#endif
