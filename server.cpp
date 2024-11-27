#include "Server.hpp"
#include "Delete.hpp"
#include <ostream>

#define MAX_CLIENTS 128

int    Server::ft_server_init()
{
    if (this->port < 1024 || this->port > 65535)
    {
        std::cout << "Error port number\n";
        exit(1);
    }

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        std::cout << "socket failed" << std::endl;
        exit(1);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(this->host.c_str()); // by default Binding to localhost
    server_addr.sin_port = htons(this->port); // by default Port 8080

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        std::cout << "bind failed" << std::endl;
        exit(1);
    }

    std::cout << "Server is listening on "<<this->host << ":"<<this->port<<std::endl;
    if (listen(server_fd, MAX_CLIENTS) == -1)
    {
        std::cout << "listen failed" << std::endl;
        exit(1);
    }

    return (server_fd);
}

Server::Server(std::string host, int port)
{
    this->host = host;
    this->port = port;
}

Server::Server()
{
    this->host = "127.0.0.1";
    this->port = 8080;
}

Server::~Server()
{
    std::cout << "good bye...\n";
}

