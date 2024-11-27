#ifndef DELETE_HPP 
#define DELETE_HPP 

#include "Server.hpp"
#include <dirent.h>

class Delete{

public:
	int flag_response;
	std::string response;
	std::string file_to_deleted;
	Delete();
	Delete(std::string file);
	int Can_Be_Deleted();
	void Delete_File();
	~Delete();
};

#endif