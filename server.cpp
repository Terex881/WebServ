#include "Server.hpp"
#include <ostream>

#define MAX_CLIENTS 128

void    Server::ft_server_init()
{
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

    int kq = kqueue();
    if (kq == -1)
    {
        std::cout << "kqueue failed" << std::endl;
        exit(1);
    }

    struct kevent event;
    EV_SET(&event, server_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    kevent(kq, &event, 1, NULL, 0, NULL);

    // Array to store client socket file descriptors
    int client_sockets[MAX_CLIENTS] = {0};

    while (true)
    {
        struct kevent events[MAX_CLIENTS];
        int n = kevent(kq, NULL, 0, events, MAX_CLIENTS, NULL); // Wait for events
        if (n < 0)
        {
            std::cout << "kevent failed" << std::endl;
            exit(1);
        }

        for (int i = 0; i < n; ++i)
        {
            if (events[i].ident == server_fd && events[i].filter == EVFILT_READ)
            {
                // Accept new client connection
                struct sockaddr_in client_addr;
                socklen_t addrlen = sizeof(client_addr);
                int new_socket = accept(server_fd, (struct sockaddr*)&client_addr, &addrlen);
                if (new_socket < 0)
                {
                    std::cout << "accept failed" << std::endl;
                    continue;
                }

                std::cout << "New client connected: " << new_socket << std::endl;

                // Add the new socket to the kqueue for future reading events
                EV_SET(&event, new_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
                kevent(kq, &event, 1, NULL, 0, NULL);

                // Optionally, store the socket in an array for further management
                for (int j = 0; j < MAX_CLIENTS; ++j)
                {
                    if (client_sockets[j] == 0)
                    {
                        client_sockets[j] = new_socket;
                        break;
                    }
                }
            }
            else if (events[i].filter == EVFILT_READ)
            {
                // Handle reading data from the client socket
                int client_socket = events[i].ident;
                char buffer[1024] = {0};
                int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
                if (bytes_received  <= 0)
                {
                    // Client has closed the connection or error occurred
                    std::cout << "Client " << client_socket << " disconnected or error occurred." << std::endl;
                    close(client_socket);
                    EV_SET(&event, client_socket, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                    kevent(kq, &event, 1, NULL, 0, NULL);

                    // Remove from the client list
                    for (int j = 0; j < MAX_CLIENTS; ++j)
                    {
                        if (client_sockets[j] == client_socket)
                        {
                            client_sockets[j] = 0;
                            break;
                        }
                    }
                }
                else
                {
                    // Print the received request (this is a simple echo)
                    std::string msg;
                    msg.assign(buffer, 1024);
                    std::cout << msg << std::endl;
                    // Send a simple HTTP response
                    std::string message = "HTTP/1.1 200 OK\r\n"
                                          "Content-Type: text/plain\r\n"
                                          "Content-Length: " + std::to_string(11) + "\r\n"
                                          "\r\n" +  // Blank line separating headers and body
                                          "Hello world";

                    int bytes_sent = send(client_socket, message.c_str(), message.length(), 0);
                    if (bytes_sent < 0)
                    {
                        std::cout << "send failed" << std::endl;
                    }

                    // Optionally close the connection after serving
                    close(client_socket);
                    EV_SET(&event, client_socket, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                    kevent(kq, &event, 1, NULL, 0, NULL);
                }
            }
        }
    }

    // Cleanup: Close server socket and client connections
    close(server_fd);

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

