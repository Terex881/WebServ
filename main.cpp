#include "Server.hpp"
#include <iostream>
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>

#define MAX_CLIENTS 128

int main()
{
    int size = 2;
    Server* server[size];
    int fd[size];

    // Initialize servers
    server[0] = new Server("127.0.0.1", 6000);
    server[1] = new Server("127.0.0.1", 8080);

    // Initialize each server and get their listening socket fd
    fd[0] = server[0]->ft_server_init();
    fd[1] = server[1]->ft_server_init();

    // Create a kqueue
    int kq = kqueue();
    if (kq == -1)
    {
        std::cerr << "kqueue failed" << std::endl;
        exit(1);
    }

    // Set up events for each server listening socket
    struct kevent event;
    for (int i = 0; i < size; ++i)
    {
        EV_SET(&event, fd[i], EVFILT_READ, EV_ADD, 0, 0, NULL);
        if (kevent(kq, &event, 1, NULL, 0, NULL) == -1)
        {
            std::cerr << "kevent failed for server " << i << std::endl;
            exit(1);
        }
    }

    // Array to store client socket file descriptors
    int client_sockets[MAX_CLIENTS] = {0};

    while (true)
    {
        struct kevent events[MAX_CLIENTS];
        int n = kevent(kq, NULL, 0, events, MAX_CLIENTS, NULL); // Wait for events
        if (n < 0)
        {
            std::cerr << "kevent failed" << std::endl;
            exit(1);
        }

        for (int i = 0; i < n; ++i)
        {
            bool is_new_connection = false;

            // Check if the event corresponds to one of the server's listening sockets
            for (int j = 0; j < size; ++j)
            {
                if (events[i].ident == fd[j] && events[i].filter == EVFILT_READ)
                {
                    // Accept new client connection
                    struct sockaddr_in client_addr;
                    socklen_t addrlen = sizeof(client_addr);
                    int new_socket = accept(fd[j], (struct sockaddr*)&client_addr, &addrlen);
                    if (new_socket < 0)
                    {
                        std::cerr << "accept failed" << std::endl;
                        continue;
                    }

                    std::cout << "New client connected on server " << j << ": " << new_socket << std::endl;

                    // Add the new socket to the kqueue for future reading events
                    EV_SET(&event, new_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
                    kevent(kq, &event, 1, NULL, 0, NULL);

                    // Optionally, store the socket in an array for further management
                    for (int k = 0; k < MAX_CLIENTS; ++k)
                    {
                        if (client_sockets[k] == 0)
                        {
                            client_sockets[k] = new_socket;
                            break;
                        }
                    }

                    is_new_connection = true;
                    break;
                }
            }

            // If it's not a new connection, it's a client sending data
            if (!is_new_connection && events[i].filter == EVFILT_READ)
            {
                int client_socket = events[i].ident;
                char buffer[1024] = {0};
                int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
                if (bytes_received <= 0)
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
                    // Handle incoming message (echo or any other processing)
                    std::string msg(buffer);
                    std::cout << "Received from client: " << msg << std::endl;

                    // Send a response back
                    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 11\r\n\r\nHello world";
                    send(client_socket, response.c_str(), response.length(), 0);

                    // Close the client connection after serving
                    close(client_socket);
                    EV_SET(&event, client_socket, EVFILT_READ, EV_DELETE, 0, 0, NULL);
                    kevent(kq, &event, 1, NULL, 0, NULL);
                }
            }
        }
    }

    // Cleanup: Close all server and client connections
    for (int i = 0; i < size; ++i)
    {
        close(fd[i]);
    }

    return 0;
}
