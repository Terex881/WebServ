#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include <unistd.h>
using	std::string;

class Cgi
{
	private :
		string file_path;
		int client_socket;
	public :
		string response;
		int content_len;
		Cgi(int socket_fd, string path);
		bool	execute_script();
};

#endif
