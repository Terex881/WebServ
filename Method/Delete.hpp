#ifndef DELETE_HPP 
#define DELETE_HPP 

#include <dirent.h>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/event.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <exception>
#include <iostream>
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>
#include <iostream>
#include <map>
#include "../Config/DynamicStruct.hpp"


using std::string;
using std::map;
class Delete
{
	public:
		int flag_response;
		std::string response;
		std::string file_to_deleted;
		std::string	path_config;
		int delete_config;
		DynamicStruct	server;
		map<int, string> codeStatusMap;

		int cgi_;
		Delete();
		Delete(std::string file, std::string path, int delete_con, int cgi, DynamicStruct server);
		int	Can_Be_Deleted();
		int	Delete_File(size_t &Status_Code, map<int, string> &codeStatusMap);
		~Delete();
};

#endif