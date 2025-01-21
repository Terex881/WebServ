#ifndef CGI_HPP
#define CGI_HPP

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/event.h>
// #include "../Client.hpp"
class Client;

using	std::string;

// class Cgi
// {
// 	private :
// 		string file_path;
// 		int client_socket;
// 	public :
// 		string response;
// 		int content_len;
// 		Cgi(int socket_fd, string path);
// 		bool	execute_script();
// };

class Cgi
{
	private :
		string file_path;
		int client_socket;
	public :
		string response;
		int content_len;
		Cgi();
		Cgi(int socket_fd, string path);
		void	execute_script(int client_socket, int kq, Client* data);
		void	handleProcessExit(pid_t pid, int client_socket, int kq, Client* data);
		void	handleTimeout(pid_t pid, int client_socket, int kq, Client* data);
};

#endif
