#ifndef SERVER_HPP 
#define SERVER_HPP 

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/event.h>
#include <fcntl.h>
#include <sys/stat.h>

class Server{

public:
	std::string host;
	std::string end;
	double port;
	Server();
	Server(std::string host, double port);
	int ft_server_init();
	~Server();
};

#endif