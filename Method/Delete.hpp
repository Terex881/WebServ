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
class Delete
{
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