#ifndef DELETE_HPP 
#define DELETE_HPP 

#include "../Server.hpp"
#include <dirent.h>

class Delete{

public:
	int flag_response;
	std::string response;
	std::string file_to_deleted;
	std::string	path_config;
	int delete_config;
	int cgi_;
	Delete();
	Delete(std::string file, std::string path, int delete_con, int cgi);
	int Can_Be_Deleted();
	void Delete_File();
	~Delete();
};

#endif