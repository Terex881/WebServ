#ifndef SERVER_HPP 
#define SERVER_HPP 

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/event.h>
#include <fcntl.h>

class Server{

public:
	std::string host;
	int port;
	Server();
	Server(std::string host, int port);
	void ft_server_init();
	~Server();
};

#endif