#ifndef SERVER_HPP 
#define SERVER_HPP 

#include "Client.hpp"
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

class Server{

public:
	std::string host;
	double port;
	Server();
	Server(std::string host, int port);
	int		ft_server_init();
	void	ft_start(int size, int *fd);
	std::string buffering_header(std::string msg, int first_time);
	~Server();


	std::map<int, Client> clientsMap;
};

#endif